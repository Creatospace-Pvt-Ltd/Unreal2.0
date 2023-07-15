#pragma once

/***************************************************************************
 * This program is licensed by the accompanying "license" file. This file is
 * distributed "AS IS" AND WITHOUT WARRANTY OF ANY KIND WHATSOEVER, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 *
 *                Copyright (C) 2022-2023 by Dolby Laboratories.
 ***************************************************************************/

#ifdef __APPLE__
#include "ui.h"

#include <objc/objc.h>

#include <dolbyio/comms/media_engine/media_engine.h>
#include <dolbyio/comms/media_engine/video_frame_macos.h>
#include <dolbyio/comms/sdk.h>

#include <dolbyio/comms/utils/vfs_event.h>

#include <unordered_map>

namespace dolbyio::comms::sample {

class macosui : public ui_interface {
 public:
  macosui(int argc, char** argv);
  ~macosui() override;

  // The main run call, on macOS this will offload all SDK interfactions to a helper thread and then
  // proceed to run the NSMainLoop. This NSMainLoop is required by macOS system libraries for
  // video rendering, camera start/stop, as well as receiving device notifications.
  void run(dolbyio::comms::sdk* sdk) override;

  // Runs the macOS specific UI configurations. This is mainly subscribing to video tracks and participant
  // events which are necessary for rendering.
  void ui_loop_on_helper_thread() override;

  // Run this macOS NsMainLoop
  void application_loop_on_main_thread() override;

  struct track_data {
    struct impl;
    virtual ~track_data() = default;
    virtual impl* get_impl() = 0;
  };

 private:
  void cleanup_before_exit();
  void update_participant(const dolbyio::comms::participant_info& participant);
  void setup_notification_handlers();

  // Only accessed from the UI loop thread:
  std::vector<dolbyio::comms::event_handler_id> ev_handlers_{};

  // Accessed from the SDK thread and the video decoder's thread:
  std::mutex video_tracks_lock_{};
  std::unordered_map<std::string /* stream ID */, std::shared_ptr<track_data>> video_tracks_{};

  // Only accessed from the SDK thread:
  std::unordered_map<std::string /* participant ID */, dolbyio::comms::participant_info> participants_{};

  std::shared_ptr<std::atomic<bool>> app_in_background_ = std::make_shared<std::atomic<bool>>(false);
  std::vector<id> notification_observers_;
};

using ui_impl = macosui;

}  // namespace dolbyio::comms::sample
#endif  // Apple
