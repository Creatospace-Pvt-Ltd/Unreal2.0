/***************************************************************************
 * This program is licensed by the accompanying "license" file. This file is
 * distributed "AS IS" AND WITHOUT WARRANTY OF ANY KIND WHATSOEVER, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 *
 *                Copyright (C) 2022-2023 by Dolby Laboratories.
 ***************************************************************************/

// This file is compiled with C++17 standard: CMake build scripts set the
// C++17 standard, and the proper flag is passed to the compiler of the
// Objective-C++ source file. The CMAKE_OBJCXX_STANDARD value is populated from
// the CMAKE_CXX_STANDARD variable. Unfortunately, the clangd language server,
// when indexing the code, always uses C++14 standard for objective-c++ files. A
// quick workaorund of redefining the macro allows at least for parsing the
// std:: types correctly, and it appears that the whole code indexes correctly:
#if defined(IAPI_CLANGD_INDEXER) && IAPI_CLANGD_INDEXER
#if __cplusplus < 201703L
#undef __cplusplus
#define __cplusplus 201703L
#endif  // C++ < 17
#endif  // Clangd indexer

#include "macos_ui.h"

#include <iostream>

#import <Cocoa/Cocoa.h>

namespace dolbyio::comms::sample {

// This class is used for rendering participant video streams. There is a single
// instance of this class created for each participant video track received by the
// SDK. The constructor of this class will create the UI window
struct macosui::track_data::impl : public macosui::track_data, public dolbyio::comms::video_sink {
  impl* get_impl() override { return this; }
  impl(const std::string& peer_id,
       const std::string& title,
       bool is_screenshare,
       const std::shared_ptr<std::atomic<bool>>& app_in_background)
      : peer_id_(peer_id), title_(title), is_screenshare_(is_screenshare), app_in_background_(app_in_background) {
    if (is_screenshare_)
      do_create_window();
    auto mainDisplayId = CGMainDisplayID();
    display_width_ = CGDisplayPixelsWide(mainDisplayId);
    display_height_ = CGDisplayPixelsHigh(mainDisplayId);
  }

  void do_create_window() {
    __block std::string title_copy{};
    if (is_screenshare_)
      title_copy.append("Screenshare from ");
    title_copy.append(title_);
    dispatch_barrier_async(dispatch_get_main_queue(), ^{
      if (window_)
        return;
      NSString* win_name = [NSString stringWithUTF8String:title_copy.c_str()];
      if (win_name == nil) {
        win_name = @"(invalid string)";
      }
      window_ = [[NSWindow alloc] initWithContentRect:NSMakeRect(0, 0, 500, 500)
                                            styleMask:NSWindowStyleMaskTitled
                                              backing:NSBackingStoreBuffered
                                                defer:NO];
      [window_ cascadeTopLeftFromPoint:NSMakePoint(20, 20)];
      [window_ setTitle:win_name];
      [window_ makeKeyAndOrderFront:nil];

      view_ = [[NSView alloc] initWithFrame:NSMakeRect(0, 0, 500, 500)];
      video_layer_ = [CALayer layer];
      [view_ setLayer:video_layer_];
      [view_ setWantsLayer:YES];
      view_.layer.backgroundColor = [[NSColor yellowColor] CGColor];

      [window_.contentView addSubview:view_];

      // FIXME: What if there's a set_frame in progress now? We'd reset that frame.
      if (cur_frame_)
        set_frame(cur_frame_);
    });
  }

  void do_destroy_window() {
    dispatch_barrier_async_and_wait(dispatch_get_main_queue(), ^{
      if (window_) {
        [window_ setReleasedWhenClosed:NO];
        [window_ close];
        window_ = nil;
        view_ = nil;
        video_layer_ = nil;
      }
    });
  }

  ~impl() override { do_destroy_window(); }

  void set_visible(bool visible) {
    if (visible)
      do_create_window();
    else
      do_destroy_window();
  }

  void set_title(const std::string& title) {
    title_ = title;
    __block std::string title_copy{};
    if (is_screenshare_)
      title_copy.append("Screenshare from ");
    title_copy.append(title_);

    dispatch_async(dispatch_get_main_queue(), ^{
      if (!window_)
        return;

      NSString* win_title = [NSString stringWithUTF8String:title_copy.c_str()];
      if (win_title == nil) {
        win_title = @"(invalid string)";
      }
      [window_ setTitle:win_title];
    });
  }

  // This method will actually draw the IOSurface extracted from the pixel buffer
  // onto the respective window.
  void set_frame(const std::shared_ptr<dolbyio::comms::video_frame>& f) {
    std::shared_ptr<dolbyio::comms::video_frame> frame(f);
    dispatch_async(dispatch_get_main_queue(), ^{
      if (frame->get_native_frame()) {
        auto buf = frame->get_native_frame()->get_buffer();
        auto iosurface = CVPixelBufferGetIOSurface(buf);
        if (iosurface) {
          set_size_on_main_thread(frame->width(), frame->height());

          if (video_layer_)
            video_layer_.contents = (__bridge id)(iosurface);
          cur_frame_ = frame;
        }
      } else if (frame->get_i420_frame()) {
        // VP8 frames not supported for now! :[
      }
    });
  }

  // Updates the size of the window based on the width/height of the incoming
  // video frame. This method is invoked on the mainQueue.
  void set_size_on_main_thread(int width, int height) {
    if (!view_ || !window_)
      return;

    // Max view window is max size of the main display
    if (width > display_width_ || height > display_height_) {
        width = display_width_;
        height = display_height_;
      }

    if (width == view_.frame.size.width && height == view_.frame.size.height)
      return;

    NSRect new_rect{};
    new_rect.size.height = height;
    new_rect.size.width = width;
    view_.frame = new_rect;

    new_rect = window_.frame;
    new_rect.size.height = height;
    new_rect.size.width = width;
    [window_ setFrame:new_rect display:YES animate:NO];
  }

  const std::string& peer_id() const { return peer_id_; }

  // This is invoked by the SDK whenever a video frame has been decoded and is ready to be passed to a sink.
  // This implementation receives the video frame, finds the appropriate track_data implementation and invokes
  // its set_frame function which will perform the drawing of the frame.
  void handle_frame(std::unique_ptr<dolbyio::comms::video_frame> frame) override {
    if (app_in_background_->load())
      return;

    // Safe to access without lock: video_tracks_ may be mutated, but the track in question will not be removed while
    // it's running. If we're in handle_frame(), the track stays until we return:
    set_frame(std::move(frame));
  }

 private:
  const std::string peer_id_;
  std::string title_;
  const bool is_screenshare_;
  std::shared_ptr<std::atomic<bool>> app_in_background_;
  std::shared_ptr<dolbyio::comms::video_frame> cur_frame_{};
  NSWindow* window_ = nil;
  NSView* view_ = nil;
  CALayer* video_layer_ = nil;
  int display_height_{0};
  int display_width_{0};
};

macosui::macosui(int argc, char** argv) : ui_interface(argc, argv) {}

macosui::~macosui() {
  auto notification_center = [NSNotificationCenter defaultCenter];
  for (const auto& observer : notification_observers_)
    [notification_center removeObserver:observer];
}

void macosui::setup_notification_handlers() {
  auto notification_center = [NSNotificationCenter defaultCenter];
  notification_observers_.push_back([notification_center
      addObserverForName:NSApplicationDidResignActiveNotification
                  object:nil
                   queue:nil
              usingBlock:^(NSNotification*) {
                if (!sdk_params().background_render) {
                  *app_in_background_ = true;
                  [[NSOperationQueue mainQueue] cancelAllOperations];
                  std::cerr << "MacOS UI App has moved to background, no longer rendering video frames!\n";
                }
              }]);
  notification_observers_.push_back([notification_center
      addObserverForName:NSApplicationDidBecomeActiveNotification
                  object:nil
                   queue:nil
              usingBlock:^(NSNotification*) {
                if (!sdk_params().background_render) {
                  *app_in_background_ = false;
                  std::cerr << "MacOS UI App is becoming active, video rendering now enabled!\n";
                }
              }]);
}

// The helper ui function which is invoked on the offloaded thread, so that main can run the
// macOS NSMainLoop. This function sets up macOS sample specific handlers and then invokes
// the base class main ui loop helper function.
void macosui::ui_loop_on_helper_thread() {
  try {
    setup_notification_handlers();
    if (sdk_params().display_video) {
      ev_handlers_.push_back(
          wait(sdk_->conference().add_event_handler([this](const dolbyio::comms::video_track_removed& e) {
            std::shared_ptr<track_data> track{};
            {
              std::lock_guard<std::mutex> l(video_tracks_lock_);
              auto it = video_tracks_.find(e.track.stream_id);
              if (it != video_tracks_.end()) {
                track = std::move(it->second);
                video_tracks_.erase(it);
              }
            }
            // The track is now deleted, without holding the video_tracks_lock_, but still blocking with a
            // synchronisation with the main dispatch queue. TODO: make it async, non-blocking.
          })));
      ev_handlers_.push_back(
          wait(sdk_->conference().add_event_handler([this](const dolbyio::comms::video_track_added& e) {
            const dolbyio::comms::video_track& track = e.track;
            if (track.remote) {
              std::string title = track.peer_id;
              auto it = participants_.find(track.peer_id);
              if (it != participants_.end())
                title = it->second.info.name.value_or(track.peer_id);
              auto shared_track_data =
                  std::make_shared<track_data::impl>(track.peer_id, title, track.is_screenshare, app_in_background_);

              {
                std::lock_guard<std::mutex> l(video_tracks_lock_);
                video_tracks_.insert(std::make_pair(track.stream_id, shared_track_data));
              }
              sdk_->video().remote().set_video_sink(track, shared_track_data).on_error([](std::exception_ptr&& e) {
                try {
                  std::rethrow_exception(e);
                } catch (const std::exception& ee) {
                  std::cerr << "Unable to add video sink for a new track: " << ee.what() << std::endl;
                }
              });
            }
          })));
      ev_handlers_.push_back(wait(sdk_->conference().add_event_handler(
          [this](const dolbyio::comms::participant_added& e) { update_participant(e.participant); })));
      ev_handlers_.push_back(wait(sdk_->conference().add_event_handler(
          [this](const dolbyio::comms::participant_updated& e) { update_participant(e.participant); })));
      ev_handlers_.push_back(
          wait(sdk_->conference().add_event_handler([this](const dolbyio::comms::conference_status_updated& e) {
            if (e.is_ended()) {
              // Safe to check without a lock: the mutations of the list only happen on this thread:
              if (!video_tracks_.empty()) {
                std::lock_guard<std::mutex> l(video_tracks_lock_);
                video_tracks_.clear();
              }
              participants_.clear();
            }
          })));
      ev_handlers_.push_back(utils::vfs_event::add_event_handler(*sdk_, [this](const utils::vfs_event& e) {
        std::lock_guard<std::mutex> l(video_tracks_lock_);
        for (const auto& id : e.new_disabled) {
          auto it = video_tracks_.find(std::get<0>(id.second));
          if (it != video_tracks_.end()) {
            it->second->get_impl()->set_visible(false);
          }
        }
        for (const auto& id : e.new_enabled) {
          auto it = video_tracks_.find(std::get<0>(id.second));
          if (it != video_tracks_.end()) {
            it->second->get_impl()->set_visible(true);
          }
        }
      }));
    }

    // Wait for the main loop to start running:
    __block std::promise<void> prom{};
    auto future = prom.get_future();

    dispatch_async(dispatch_get_main_queue(), ^{
      prom.set_value();
    });

    future.get();

    ui_interface::ui_loop_on_helper_thread();
  } catch (const std::exception& e) {
    std::cerr << "Failure: " << e.what() << std::endl;
  }

  // base's ui loop exitted: we're not in the conference any more:
  cleanup_before_exit();
}

void macosui::application_loop_on_main_thread() {
  [NSApp run];
}

void macosui::run(dolbyio::comms::sdk* sdk) {
  @autoreleasepool {
    [NSApplication sharedApplication];
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
    [NSApp activateIgnoringOtherApps:YES];

    ui_interface::run(sdk);
  }
}

void macosui::cleanup_before_exit() {
  try {
    for (auto& eh : ev_handlers_) {
      wait(eh->disconnect());
    }
    ev_handlers_.clear();
    video_tracks_.clear();  // safe to clear without mutex: conference is left, and event handlers are detached. FIXME:
                            // Each track's destructor syncs to the main thread!
  } catch (const std::exception& e) {
    std::cerr << "Failure: " << e.what() << std::endl;
  }

  // The RTCVideoCapturer posts notification about start/stop video to main queue and then invokes the start/stop
  // mechanism on kCaptureSessionQueue from the mainqueue. Thus at the time of stoppage the mainqueue must exist.
  // Henceforth we ensure video capture is stopped prior to destroying the mainqueue.
  wait(sdk_->video().local().stop());

  dispatch_async(dispatch_get_main_queue(), ^{
    [NSApp stop:nil];

    // After stopping the application thread, the [NSApp run] still will not return until the loop handles some UI
    // event. Producing a fake, application-defined event for this purpose:
    NSEvent* ev = [NSEvent otherEventWithType:NSEventTypeApplicationDefined
                                     location:NSPoint{0, 0}
                                modifierFlags:0
                                    timestamp:0
                                 windowNumber:0
                                      context:nil
                                      subtype:0
                                        data1:0
                                        data2:0];
    [NSApp postEvent:ev atStart:FALSE];
  });
}

void macosui::update_participant(const dolbyio::comms::participant_info& participant) {
  participants_.insert_or_assign(participant.user_id, participant);

  // safe to access the video tracks without lock: the video_tracks_ collection is only mutated on the SDK thread, and
  // we're on the SDK thread here:
  for (auto& track : video_tracks_) {
    if (track.second->get_impl()->peer_id() == participant.user_id) {
      track.second->get_impl()->set_title(participant.info.name.value_or(participant.user_id));
      return;
    }
  }
}

}  // namespace dolbyio::comms::sample
