#include "comms/sample/utilities/sdk/device_manager/interactions.h"
#include "comms/sample/utilities/commands_handler.h"

#include <iostream>

namespace dolbyio::comms::sample {

void device_interactions::enable() {
  devices_ = wait(sdk_->device_management().get_audio_devices());
  video_devices_ = wait(sdk_->device_management().get_video_devices());
  auto curr_input_device = wait(sdk_->device_management().get_current_audio_input_device());
  if (curr_input_device)
    std::cerr << "Current input device: " << curr_input_device->name() << std::endl;

  auto curr_output_device = wait(sdk_->device_management().get_current_audio_output_device());
  if (curr_output_device)
    std::cerr << "Current output device: " << curr_output_device->name() << std::endl;

  handlers_.push_back(
      wait(sdk_->device_management().add_event_handler([this](const dolbyio::comms::audio_device_added& e) {
        if (std::find(devices_.begin(), devices_.end(), e.device) == devices_.end())
          devices_.push_back(e.device);
      })));
  handlers_.push_back(
      wait(sdk_->device_management().add_event_handler([this](const dolbyio::comms::audio_device_removed& e) {
        devices_.erase(std::remove_if(devices_.begin(), devices_.end(),
                                      [&e](const auto& device) { return device == e.device_id; }),
                       devices_.end());
      })));
  handlers_.push_back(
      wait(sdk_->device_management().add_event_handler([this](const dolbyio::comms::audio_device_changed& e) {
        std::optional<dolbyio::comms::audio_device> new_current{};
        if (!e.device) {
          if (e.utilized_direction == dolbyio::comms::audio_device::input) {
            std::cerr << "Input audio device closed" << std::endl;
          }
          if (e.utilized_direction == dolbyio::comms::audio_device::output) {
            std::cerr << "Output audio device closed" << std::endl;
          }
        } else {
          for (const auto& dev : devices_) {
            if (dev == e.device) {
              new_current = dev;
              break;
            }
          }
          if (new_current) {
            if (e.utilized_direction == dolbyio::comms::audio_device::input) {
              std::cerr << "Input device switched to " << new_current->name() << std::endl;
            }
            if (e.utilized_direction == dolbyio::comms::audio_device::output) {
              std::cerr << "Output device switched to " << new_current->name() << std::endl;
            }
          }
        }
      })));
  handlers_.push_back(
      wait(sdk_->device_management().add_event_handler([](const dolbyio::comms::audio_device_timeout_failure&) {
        std::cerr << "Prolonged audio device problem, you may have no audio." << std::endl;
      })));
  handlers_.push_back(
      wait(sdk_->device_management().add_event_handler([](const dolbyio::comms::audio_volume_changed& e) {
        std::cerr << "New audio volume: " << e.volume.get_value() << " in direction: " << static_cast<int>(e.direction)
                  << std::endl;
      })));
  handlers_.push_back(
      wait(sdk_->device_management().add_event_handler([this](const dolbyio::comms::video_device_added& e) {
        if (std::find_if(video_devices_.begin(), video_devices_.end(), [device{e.device}](const auto& dev) {
              return device.unique_id == dev.unique_id;
            }) == video_devices_.end()) {
          std::cerr << "New video device added: " << e.device.display_name << std::endl;
          video_devices_.push_back(e.device);
        }
      })));
  handlers_.push_back(
      wait(sdk_->device_management().add_event_handler([this](const dolbyio::comms::video_device_removed& e) {
        video_devices_.erase(std::remove_if(video_devices_.begin(), video_devices_.end(),
                                            [&e](const auto& device) {
                                              if (device.unique_id == e.uid) {
                                                std::cerr << "Video device removed: " << device.display_name
                                                          << std::endl;
                                                return true;
                                              }
                                              return false;
                                            }),
                             video_devices_.end());
      })));
}

void device_interactions::disable() {
  for (auto& iter : handlers_)
    wait(iter->disconnect());
  handlers_.clear();
  devices_.clear();
}

void device_interactions::set_sdk(dolbyio::comms::sdk* sdk) {
  if (sdk_)
    disable();
  sdk_ = sdk;
  if (sdk_)
    enable();
}

void device_interactions::register_command_line_handlers(commands_handler&) {}

void device_interactions::register_interactive_commands(commands_handler& handler) {
  handler.add_interactive_command("g", "get audio devices", [this]() { get_audio_devices(); });
  handler.add_interactive_command("i", "set input audio device", [this]() { set_input_audio_device(); });
  handler.add_interactive_command("o", "set output audio device", [this]() { set_output_audio_device(); });
  handler.add_interactive_command("get-cameras", "get video devices", [this]() { get_video_devices(); });
  handler.add_interactive_command("set-input-volume", "Sets input volume", [this]() { set_input_volume(); });
  handler.add_interactive_command("set-output-volume", "Sets output volume", [this]() { set_output_volume(); });
}

void device_interactions::get_audio_devices() {
  devices_ = wait(sdk_->device_management().get_audio_devices());
  for (const auto& iter : devices_)
    std::cerr << iter.name() << std::endl;
}

void device_interactions::set_input_audio_device() {
  std::cout << "Enter input device name:" << std::endl;
  std::string msg;
  std::cin.ignore();
  std::getline(std::cin, msg);
  auto iter = std::find_if(devices_.begin(), devices_.end(), [&msg](const auto& device) {
    return device.name() == msg && (device.direction() & dolbyio::comms::audio_device::input);
  });
  if (iter != devices_.end())
    wait(sdk_->device_management().set_preferred_input_audio_device(*iter));
  else
    std::cerr << "Device not found\n";
}

void device_interactions::set_output_audio_device() {
  std::cout << "Enter output device name:" << std::endl;
  std::string msg;
  std::cin.ignore();
  std::getline(std::cin, msg);
  auto iter = std::find_if(devices_.begin(), devices_.end(), [&msg](const auto& device) {
    return device.name() == msg && (device.direction() & dolbyio::comms::audio_device::output);
  });
  if (iter != devices_.end())
    wait(sdk_->device_management().set_preferred_output_audio_device(*iter));
  else
    std::cerr << "Device not found\n";
}

void device_interactions::set_input_volume() {
  std::string vol;
  std::cerr << "Please enter the new input volume (0.0 ... 1.0)" << std::endl;
  std::cin >> vol;
  double vold = std::strtod(vol.c_str(), nullptr);
  std::cerr << "Setting input volume: " << vold << std::endl;

  wait(sdk_->device_management().set_input_volume(dolbyio::comms::linear_volume{vold}));
}

void device_interactions::set_output_volume() {
  std::string vol;
  std::cerr << "Please enter the new output volume (0.0 ... 1.0)" << std::endl;
  std::cin >> vol;
  double vold = std::strtod(vol.c_str(), nullptr);
  std::cerr << "Setting output volume: " << vold << std::endl;

  wait(sdk_->device_management().set_output_volume(dolbyio::comms::linear_volume{vold}));
}

void device_interactions::get_video_devices() {
  video_devices_ = wait(sdk_->device_management().get_video_devices());
  for (const auto& iter : video_devices_)
    std::cerr << iter.display_name << " " << iter.unique_id << std::endl;
}

};  // namespace dolbyio::comms::sample
