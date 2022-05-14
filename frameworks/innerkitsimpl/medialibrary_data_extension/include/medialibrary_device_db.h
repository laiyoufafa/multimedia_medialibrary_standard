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

#ifndef OHOS_MEDIALIBRARY_DEVICE_DB_H
#define OHOS_MEDIALIBRARY_DEVICE_DB_H

#include <string>
#include "media_data_ability_const.h"
#include "rdb_errno.h"
#include "rdb_helper.h"
#include "sys/stat.h"
#include "datashare_values_bucket.h"

namespace OHOS {
namespace Media {
using namespace OHOS::NativeRdb;

class MediaLibraryDeviceDb {
public:
    MediaLibraryDeviceDb() = default;
    ~MediaLibraryDeviceDb() = default;

    int64_t InsertDeviceInfo(const ValuesBucket &values, const std::shared_ptr<RdbStore> &rdbStore);
    int32_t DeleteDeviceInfo(const std::string &deviceId, const std::shared_ptr<RdbStore> &rdbStore);
    int32_t UpdateDeviceInfo(const ValuesBucket &values, const std::shared_ptr<RdbStore> &rdbStore);
};
} // namespace Media
} // namespace OHOS
#endif // OHOS_MEDIALIBRARY_DEVICE_DB_H
