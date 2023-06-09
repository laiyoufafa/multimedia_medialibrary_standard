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

#include "smart_album_asset.h"

#include "media_file_utils.h"
#include "media_lib_service_const.h"
#include "media_log.h"

using namespace std;

namespace OHOS {
namespace Media {
SmartAlbumAsset::SmartAlbumAsset()
{
    albumId_ = DEFAULT_ALBUM_ID;
    albumName_ = DEFAULT_ALBUM_NAME;
    albumUri_ = DEFAULT_ALBUM_URI;
    albumTag_ = DEFAULT_SMART_ALBUM_TAG;
    albumPrivateType_ = DEFAULT_SMART_ALBUM_PRIVATE_TYPE;
    albumCapacity_ = DEFAULT_SMART_ALBUM_ALBUMCAPACITY;
    categoryId_ = DEFAULT_SMART_ALBUM_CATEGORYID;
    categoryName_ = DEFAULT_SMART_ALBUM_CATEGORYNAME;
    coverUri_ = DEFAULT_COVERURI;
}

SmartAlbumAsset::~SmartAlbumAsset() = default;


void SmartAlbumAsset::SetAlbumId(const int32_t albumId)
{
    albumId_ = albumId;
}

void SmartAlbumAsset::SetAlbumName(const string albumName)
{
    albumName_ = albumName;
}

void SmartAlbumAsset::SetAlbumUri(const string albumUri)
{
    albumUri_ = albumUri;
}

void SmartAlbumAsset::SetAlbumTag(const string albumTag)
{
    albumTag_ = albumTag;
}

void SmartAlbumAsset::SetAlbumCapacity(const int32_t albumCapacity)
{
    albumCapacity_ = albumCapacity;
}

void SmartAlbumAsset::SetCategoryId(const int32_t categoryId)
{
    categoryId_ = categoryId;
}

void SmartAlbumAsset::SetCategoryName(const string categoryName)
{
    categoryName_ = categoryName;
}

void SmartAlbumAsset::SetCoverUri(const string coverUri)
{
    coverUri_ = coverUri;
}

void SmartAlbumAsset::SetAlbumPrivateType(const int32_t albumPrivateType)
{
    albumPrivateType_ = albumPrivateType;
}

int32_t SmartAlbumAsset::GetAlbumId() const
{
    return albumId_;
}

string SmartAlbumAsset::GetAlbumName() const
{
    return albumName_;
}

string SmartAlbumAsset::GetAlbumUri() const
{
    return albumUri_;
}

string SmartAlbumAsset::GetAlbumTag() const
{
    return albumTag_;
}

int32_t SmartAlbumAsset::GetAlbumCapacity() const
{
    return albumCapacity_;
}

int32_t SmartAlbumAsset::GetCategoryId() const
{
    return categoryId_;
}

string SmartAlbumAsset::GetCategoryName() const
{
    return categoryName_;
}

string SmartAlbumAsset::GetCoverUri() const
{
    return coverUri_;
}

int32_t SmartAlbumAsset::GetAlbumPrivateType() const
{
    return albumPrivateType_;
}
}  // namespace Media
}  // namespace OHOS
