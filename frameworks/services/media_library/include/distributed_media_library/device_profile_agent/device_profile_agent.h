/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef DEVCIE_PROFILE_AGENT_H
#define DEVCIE_PROFILE_AGENT_H

#include "iprofile_event_callback.h"
#include "profile_change_notification.h"
#include "nlohmann/json.hpp"

namespace OHOS {
namespace Media {
struct MedialibrayDpInfo {
    std::string version;
};

void from_json(const nlohmann::json &jsonObject, MedialibrayDpInfo &medialibraryInfo);

class DeviceProfileAgent final : public DeviceProfile::IProfileEventCallback,
                                 public std::enable_shared_from_this<DeviceProfileAgent> {
public:
    DeviceProfileAgent();
    virtual ~DeviceProfileAgent();
    DeviceProfileAgent(const DeviceProfileAgent&) = delete;
    DeviceProfileAgent& operator=(const DeviceProfileAgent&) = delete;
    DeviceProfileAgent(const DeviceProfileAgent&&) = delete;
    DeviceProfileAgent& operator=(const DeviceProfileAgent&&) = delete;
    int32_t PutDeviceProfile(const std::string &version);
    int32_t GetDeviceProfile(const std::string& udid, std::string &version);
    int32_t SyncDeviceProfile(const std::string &deviceId);
    void OnSyncCompleted(const DeviceProfile::SyncResult& syncResults) override;
    void OnProfileChanged(const DeviceProfile::ProfileChangeNotification& changeNotification) override;
private:
    void SubScribeMediaLibaryVersionEvent();
    void UnSubScribeMediaLibaryVersionEvent();
};
} // namespace Media
} // namespace OHOS
#endif // DEVCIE_PROFILE_AGENT_H