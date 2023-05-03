/*
 * Copyright (C) 2021-2022 Huawei Device Co., Ltd.
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

#include "medialibrary_data_manager_utils.h"

#include "media_file_utils.h"
#include "media_log.h"
#include "medialibrary_common_utils.h"
#include "medialibrary_db_const.h"
#include "photo_album_column.h"

using namespace std;

namespace OHOS {
namespace Media {
bool MediaLibraryDataManagerUtils::IsNumber(const string &str)
{
    if (str.empty()) {
        MEDIA_ERR_LOG("IsNumber input is empty ");
        return false;
    }

    for (char const &c : str) {
        if (isdigit(c) == 0) {
            return false;
        }
    }
    return true;
}

std::string MediaLibraryDataManagerUtils::GetFileTitle(const std::string &displayName)
{
    std::string title;
    if (!displayName.empty()) {
        std::string::size_type pos = displayName.find_last_of('.');
        if (pos == displayName.length()) {
            return displayName;
        }
        title = displayName.substr(0, pos);
        MEDIA_DEBUG_LOG("title substr = %{private}s", title.c_str());
    }
    return title;
}

string MediaLibraryDataManagerUtils::GetOperationType(const string &uri)
{
    string oprn;
    size_t found = uri.rfind('/');
    if (found != string::npos) {
        oprn = uri.substr(found + 1);
    }

    return oprn;
}

string MediaLibraryDataManagerUtils::GetIdFromUri(const string &uri)
{
    string rowNum = "-1";

    size_t pos = uri.rfind('/');
    if (pos != std::string::npos) {
        rowNum = uri.substr(pos + 1);
    }

    return rowNum;
}

string MediaLibraryDataManagerUtils::GetNetworkIdFromUri(const string &uri)
{
    string networkId;
    if (uri.empty()) {
        return networkId;
    }
    size_t pos = uri.find(MEDIALIBRARY_DATA_ABILITY_PREFIX);
    if (pos == string::npos) {
        return networkId;
    }
    string tempUri = uri.substr(MEDIALIBRARY_DATA_ABILITY_PREFIX.length());
    if (tempUri.empty()) {
        return networkId;
    }
    pos = tempUri.find_first_of('/');
    if (pos == 0 || pos == string::npos) {
        return networkId;
    }
    networkId = tempUri.substr(0, pos);

    return networkId;
}

string MediaLibraryDataManagerUtils::GetDisPlayNameFromPath(const std::string &path)
{
    string displayName;
    size_t lastSlashPosition = path.rfind("/");
    if (lastSlashPosition != string::npos) {
        displayName = path.substr(lastSlashPosition + 1);
    }
    return displayName;
}

void MediaLibraryDataManagerUtils::SplitKeyValue(const string &keyValue, string &key, string &value)
{
    string::size_type pos = keyValue.find('=');
    if (string::npos != pos) {
        key = keyValue.substr(0, pos);
        value = keyValue.substr(pos + 1);
    }
}

void MediaLibraryDataManagerUtils::SplitKeys(const string &query, vector<string> &keys)
{
    string::size_type pos1 = 0;
    string::size_type pos2 = query.find('&');
    while (string::npos != pos2) {
        keys.push_back(query.substr(pos1, pos2 - pos1));
        pos1 = pos2 + 1;
        pos2 = query.find('&', pos1);
    }
    if (pos1 != query.length()) {
        keys.push_back(query.substr(pos1));
    }
}

string MediaLibraryDataManagerUtils::ObtionCondition(string &strQueryCondition, const vector<string> &whereArgs)
{
    for (string args : whereArgs) {
        size_t pos = strQueryCondition.find('?');
        if (pos != string::npos) {
            strQueryCondition.replace(pos, 1, "'" + args + "'");
        }
    }
    return strQueryCondition;
}

void MediaLibraryDataManagerUtils::RemoveTypeValueFromUri(std::string &uri)
{
    size_t typeIndex = uri.find('#');
    if (typeIndex != std::string::npos) {
        uri = uri.substr(0, typeIndex);
    }
}

std::string MediaLibraryDataManagerUtils::GetTypeUriByUri(std::string &uri)
{
    string typeUri;
    if (uri.find(PhotoColumn::PHOTO_URI_PREFIX) != string::npos) {
        typeUri = PhotoColumn::PHOTO_URI_PREFIX;
    } else if (uri.find(PhotoAlbumColumns::ALBUM_URI_PREFIX) != string::npos) {
        typeUri = PhotoAlbumColumns::ALBUM_URI_PREFIX ;
    }
    return typeUri;
}
} // namespace Media
} // namespace OHOS
