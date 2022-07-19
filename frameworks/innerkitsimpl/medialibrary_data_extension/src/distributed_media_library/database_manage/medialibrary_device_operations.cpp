/*
 * Copyright (C) 2021 Huawei Device Co., Ltd.
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

#include "medialibrary_device_operations.h"
#include "media_log.h"
#include "result_set_utils.h"

using namespace std;
using namespace OHOS::NativeRdb;

namespace OHOS {
namespace Media {
static int64_t CurrentTimeMillis()
{
    auto now = std::chrono::system_clock::now();
    auto millisecs = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
    return static_cast<int64_t>(millisecs.count());
}

bool MediaLibraryDeviceOperations::InsertDeviceInfo(const std::shared_ptr<NativeRdb::RdbStore> &rdbStore,
    const OHOS::Media::MediaLibraryDeviceInfo &deviceInfo, const std::string &bundleName)
{
    if (rdbStore == nullptr) {
        MEDIA_ERR_LOG("rdbstore is nullptr");
        return false;
    }
    unique_ptr<AbsSharedResultSet> queryResultSet;
    std::vector<std::string> columns;
    AbsRdbPredicates mediaLibAbsPredDevice(DEVICE_TABLE);
    std::string strQueryCondition =
        DEVICE_DB_DEVICEID + "= '" + deviceInfo.deviceUdid + "'";
    mediaLibAbsPredDevice.SetWhereClause(strQueryCondition);
    queryResultSet = rdbStore->Query(mediaLibAbsPredDevice, columns);

    auto count = 0;
    auto ret = queryResultSet->GetRowCount(count);
    MEDIA_INFO_LOG("MediaLibraryDeviceOperations::InsertDeviceInfo ret = %{public}d, count = %{public}d", ret, count);
    if (ret == NativeRdb::E_OK) {
        if (count > 0) {
            // 更新数据库
            ValuesBucket valuesBucket;
            valuesBucket.PutString(DEVICE_DB_DEVICEID, deviceInfo.deviceUdid);
            valuesBucket.PutString(DEVICE_DB_NETWORK_ID, deviceInfo.deviceId);
            valuesBucket.PutInt(DEVICE_DB_SYNC_STATUS, 0);
            valuesBucket.PutLong(DEVICE_DB_DATE_MODIFIED, 0);
            MEDIA_INFO_LOG("MediaLibraryDeviceOperations::InsertDeviceInfo UpdateDeviceInfo");
            return MediaLibraryDeviceDb::UpdateDeviceInfo(valuesBucket, rdbStore) == E_SUCCESS;
        } else {
            // 插入数据库
            int64_t now = CurrentTimeMillis();
            ValuesBucket valuesBucket;
            valuesBucket.PutString(DEVICE_DB_DEVICEID, deviceInfo.deviceUdid);
            valuesBucket.PutString(DEVICE_DB_NETWORK_ID, deviceInfo.deviceId);
            valuesBucket.PutString(DEVICE_DB_NAME, deviceInfo.deviceName);
            valuesBucket.PutString(DEVICE_DB_IP, std::string());
            valuesBucket.PutString(DEVICE_DB_SELF_ID, deviceInfo.selfId);
            valuesBucket.PutInt(DEVICE_DB_TYPE, (int32_t) deviceInfo.deviceTypeId);
            valuesBucket.PutString(DEVICE_DB_PREPATH, std::string());
            valuesBucket.PutLong(DEVICE_DB_DATE_ADDED, now);
            MEDIA_INFO_LOG("MediaLibraryDeviceOperations::InsertDeviceInfo InsertDeviceInfo");
            return MediaLibraryDeviceDb::InsertDeviceInfo(valuesBucket, rdbStore) >= 0;
        }
    }
    return false;
}

bool MediaLibraryDeviceOperations::UpdateDeviceInfo(const std::shared_ptr<NativeRdb::RdbStore> &rdbStore,
    const OHOS::Media::MediaLibraryDeviceInfo &deviceInfo, const std::string &bundleName)
{
    unique_ptr<AbsSharedResultSet> queryResultSet;
    std::vector<std::string> columns;
    AbsRdbPredicates mediaLibAbsPredDevice(DEVICE_TABLE);
    std::string strQueryCondition =
        DEVICE_DB_DEVICEID + " = '" + deviceInfo.deviceUdid + "'";
    MEDIA_INFO_LOG("MediaLibraryDeviceOperations::UpdateDeviceInfo deviceId = %{private}s",
        deviceInfo.deviceUdid.c_str());
    mediaLibAbsPredDevice.SetWhereClause(strQueryCondition);
    queryResultSet = rdbStore->Query(mediaLibAbsPredDevice, columns);

    auto count = 0;
    auto ret = queryResultSet->GetRowCount(count);
    if (ret == NativeRdb::E_OK) {
        if (count > 0) {
            int64_t now = CurrentTimeMillis();
            // 更新数据库
            ValuesBucket valuesBucket;
            valuesBucket.PutString(DEVICE_DB_DEVICEID, deviceInfo.deviceUdid);
            valuesBucket.PutString(DEVICE_DB_NETWORK_ID, "");
            valuesBucket.PutString(DEVICE_DB_SELF_ID, deviceInfo.selfId);
            valuesBucket.PutLong(DEVICE_DB_DATE_MODIFIED, now);
            MEDIA_INFO_LOG("MediaLibraryDeviceOperations::UpdateDeviceInfo UpdateDeviceInfo");
            return MediaLibraryDeviceDb::UpdateDeviceInfo(valuesBucket, rdbStore) == E_SUCCESS;
        }
    }
    return false;
}

bool MediaLibraryDeviceOperations::DeleteDeviceInfo(const std::shared_ptr<NativeRdb::RdbStore> &rdbStore,
                                                    const std::string &deviceId)
{
    return MediaLibraryDeviceDb::DeleteDeviceInfo(deviceId, rdbStore);
}

bool MediaLibraryDeviceOperations::UpdateSyncStatus(const std::shared_ptr<NativeRdb::RdbStore> &rdbStore,
    const std::string &deviceId, int32_t syncStatus, const std::string &bundleName)
{
    unique_ptr<AbsSharedResultSet> queryResultSet;
    std::vector<std::string> columns;
    AbsRdbPredicates mediaLibAbsPredDevice(DEVICE_TABLE);

    std::string strQueryCondition = DEVICE_DB_DEVICEID + " = '" + deviceId + "'";
    mediaLibAbsPredDevice.SetWhereClause(strQueryCondition);
    queryResultSet = rdbStore->Query(mediaLibAbsPredDevice, columns);

    auto count = 0;
    auto ret = queryResultSet->GetRowCount(count);
    if (ret == NativeRdb::E_OK) {
        if (count > 0) {
            // 更新数据库
            ValuesBucket valuesBucket;
            valuesBucket.PutString(DEVICE_DB_DEVICEID, deviceId);
            valuesBucket.PutInt(DEVICE_DB_SYNC_STATUS, syncStatus);
            MEDIA_INFO_LOG("MediaLibraryDeviceOperations::UpdateSyncStatus");
            return MediaLibraryDeviceDb::UpdateDeviceInfo(valuesBucket, rdbStore) == E_SUCCESS;
        }
    }
    return false;
}

bool MediaLibraryDeviceOperations::GetSyncStatusById(const std::shared_ptr<NativeRdb::RdbStore> &rdbStore,
                                                     const std::string &deviceId,
                                                     int32_t &syncStatus, const std::string &bundleName)
{
    unique_ptr<AbsSharedResultSet> queryResultSet;
    std::vector<std::string> columns;
    AbsRdbPredicates mediaLibAbsPredDevice(DEVICE_TABLE);

    std::string strQueryCondition = DEVICE_DB_DEVICEID + " = '" + deviceId + "'";
    mediaLibAbsPredDevice.SetWhereClause(strQueryCondition);
    queryResultSet = rdbStore->Query(mediaLibAbsPredDevice, columns);
    if (queryResultSet == nullptr) {
        return false;
    }

    if (queryResultSet->GoToNextRow() == NativeRdb::E_OK) {
        int32_t columnIndexId;
        queryResultSet->GetColumnIndex(DEVICE_DB_SYNC_STATUS, columnIndexId);
        queryResultSet->GetInt(columnIndexId, syncStatus);
    }
    MEDIA_INFO_LOG("MediaLibraryDeviceOperations::GetSyncStatusById syncStatus = %{public}d", syncStatus);
    return true;
}

bool MediaLibraryDeviceOperations::QueryDeviceTable(const std::shared_ptr<NativeRdb::RdbStore> &rdbStore,
                                                    std::map<std::string, std::set<int>> &excludeMap)
{
    const int SHORT_UDID_LEN = 8;
    unique_ptr<AbsSharedResultSet> queryResultSet;
    std::vector<std::string> columns;
    AbsRdbPredicates mediaLibAbsPredDevice(DEVICE_TABLE);
    queryResultSet = rdbStore->Query(mediaLibAbsPredDevice, columns);
    if (queryResultSet == nullptr) {
        MEDIA_ERR_LOG("MediaLibraryDeviceOperations::QueryDeviceTable fail");
        return false;
    }

    if (queryResultSet->GoToNextRow() == NativeRdb::E_OK) {
        int32_t columnIndexId;
        std::string selfId;
        queryResultSet->GetColumnIndex(DEVICE_DB_SELF_ID, columnIndexId);
        queryResultSet->GetString(columnIndexId, selfId);
        if (selfId.length() > SHORT_UDID_LEN) {
            std::string shortUdid = selfId.substr(0, SHORT_UDID_LEN);
            std::string randNumber = selfId.substr(SHORT_UDID_LEN);
            if (randNumber.length() > 0) {
                auto &data = excludeMap[shortUdid];
                data.insert(atoi(randNumber.c_str()));
            }
        }
    }
    return true;
}

bool MediaLibraryDeviceOperations::GetAllDeviceDatas(
    const std::shared_ptr<NativeRdb::RdbStore> &rdbStore,
    vector<MediaLibraryDeviceInfo> &outDeviceList)
{
    shared_ptr<AbsSharedResultSet> queryResultSet;
    std::vector<std::string> columns;
    AbsRdbPredicates mediaLibAbsPredDevice(DEVICE_TABLE);
    queryResultSet = rdbStore->Query(mediaLibAbsPredDevice, columns);
    if (queryResultSet == nullptr) {
        MEDIA_ERR_LOG("MediaLibraryDeviceOperations::GetAllDeviceDatas fail");
        return false;
    }

    while (queryResultSet->GoToNextRow() == NativeRdb::E_OK) {
        MediaLibraryDeviceInfo deviceInfo;
        deviceInfo.deviceId = get<string>(ResultSetUtils::GetValFromColumn(DEVICE_DB_NETWORK_ID, queryResultSet,
            TYPE_STRING));
        deviceInfo.deviceName = get<string>(ResultSetUtils::GetValFromColumn(DEVICE_DB_NAME, queryResultSet,
            TYPE_STRING));
        deviceInfo.deviceTypeId =(DistributedHardware::DmDeviceType)(get<int32_t>(
            ResultSetUtils::GetValFromColumn(DEVICE_DB_TYPE, queryResultSet, TYPE_INT32)));
        deviceInfo.deviceUdid = get<string>(ResultSetUtils::GetValFromColumn(DEVICE_DB_DEVICEID, queryResultSet,
            TYPE_STRING));
        deviceInfo.selfId = get<string>(ResultSetUtils::GetValFromColumn(DEVICE_DB_SELF_ID, queryResultSet,
            TYPE_STRING));
        outDeviceList.push_back(deviceInfo);
    }
    return true;
}
} // namespace Media
} // namespace OHOS
