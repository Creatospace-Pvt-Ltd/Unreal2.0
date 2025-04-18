#pragma once

/***************************************************************************
 * This program is licensed by the accompanying "license" file. This file is
 * distributed "AS IS" AND WITHOUT WARRANTY OF ANY KIND WHATSOEVER, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 *
 *                Copyright (C) 2022 by Dolby Laboratories.
 ***************************************************************************/

#include <dolbyio/comms/exports.h>

#include <dolbyio/comms/participant_info.h>

#include <optional>
#include <string>

namespace dolbyio::comms {

/**
 * @brief Emitted when a participant successfully joins the conference.
 * @ingroup conference_service
 * @ingroup events
 */
struct DLB_COMMS_EXPORT participant_added {
  participant_added(const participant_info& info, const std::string& conf_id)
      : participant(info), conference_id(conf_id) {}
  participant_added(participant_added&&) = delete;
  participant_added(const participant_added&) = delete;
  participant_added& operator=(participant_added&&) = delete;
  participant_added& operator=(const participant_added&) = delete;

  const participant_info& participant; /**< Information about the participant. */
  const std::string& conference_id;    /**< Conference ID. */
};

/**
 * @brief Emitted when a participant changes status.
 * @ingroup conference_service
 * @ingroup events
 */
struct DLB_COMMS_EXPORT participant_updated {
  participant_updated(const participant_info& info, const std::string& conf_id)
      : participant(info), conference_id(conf_id) {}
  participant_updated(participant_updated&&) = delete;
  participant_updated(const participant_updated&) = delete;
  participant_updated& operator=(participant_updated&&) = delete;
  participant_updated& operator=(const participant_updated&) = delete;

  const participant_info& participant; /**< Updated information about the participant. */
  const std::string& conference_id;    /**< Conference ID. */
};

};  // namespace dolbyio::comms
