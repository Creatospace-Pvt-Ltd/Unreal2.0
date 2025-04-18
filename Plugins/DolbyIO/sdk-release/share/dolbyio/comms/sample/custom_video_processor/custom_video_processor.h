/***************************************************************************
 * This program is licensed by the accompanying "license" file. This file is
 * distributed "AS IS" AND WITHOUT WARRANTY OF ANY KIND WHATSOEVER, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 *
 *                Copyright (C) 2022-2023 by Dolby Laboratories.
 ***************************************************************************/

#include <dolbyio/comms/media_engine/media_engine.h>

#include <condition_variable>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

class custom_video_processor final : public dolbyio::comms::video_source,
                                     public dolbyio::comms::video_sink,
                                     public dolbyio::comms::video_frame_handler,
                                     public std::enable_shared_from_this<custom_video_processor> {
 public:
  static constexpr unsigned int max_camera_frame_size = 1280 * 720;

  custom_video_processor();
  ~custom_video_processor() override;

  // video_sink interface
  void handle_frame(std::unique_ptr<dolbyio::comms::video_frame>) override;

  // video_source interface
  void set_sink(const std::shared_ptr<video_sink>& sink, const config& config) override;

  // video_frame_handler interface
  std::shared_ptr<video_sink> sink() override { return shared_from_this(); }
  std::shared_ptr<video_source> source() override { return shared_from_this(); }

 private:
  void frame_queue_loop();
  void process_frame_in_place(dolbyio::comms::video_frame& frame);

  std::shared_ptr<dolbyio::comms::video_sink> sdk_video_sink_{};
  std::vector<std::unique_ptr<dolbyio::comms::video_frame>> frame_queue_{};
  std::mutex frame_queue_lock_;
  std::mutex sink_lock_;
  std::condition_variable frame_queue_cond_;
  std::thread frame_queue_thread_;
  bool exit_{false};

#if defined(__APPLE__)
  uint8_t pixel_conversion_buffers_[2][max_camera_frame_size / 2];
#endif
};
