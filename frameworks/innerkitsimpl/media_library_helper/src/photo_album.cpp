/*
 * Copyright (C) 2023 Huawei Device Co., Ltd.
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
#define MLOG_TAG "PhotoAlbum"

#include "photo_album.h"

#include "medialibrary_type_const.h"

using namespace std;

namespace OHOS {
namespace Media {
PhotoAlbum::PhotoAlbum()
{
    albumId_ = DEFAULT_ALBUM_ID;
    type_ = USER;
    subType_ = USER_GENERIC;
    count_ = DEFAULT_COUNT;
    resultNapiType_ = ResultNapiType::TYPE_USERFILE_MGR;
}

PhotoAlbum::~PhotoAlbum() = default;

void PhotoAlbum::SetAlbumId(const int32_t albumId)
{
    albumId_ = albumId;
}

int32_t PhotoAlbum::GetAlbumId() const
{
    return albumId_;
}

void PhotoAlbum::SetPhotoAlbumType(const PhotoAlbumType type)
{
    type_ = type;
}

PhotoAlbumType PhotoAlbum::GetPhotoAlbumType() const
{
    return type_;
}

void PhotoAlbum::SetPhotoAlbumSubType(const PhotoAlbumSubType subType)
{
    subType_ = subType;
}

PhotoAlbumSubType PhotoAlbum::GetPhotoAlbumSubType() const
{
    return subType_;
}

void PhotoAlbum::SetAlbumUri(const string &uri)
{
    uri_ = uri;
}

const string& PhotoAlbum::GetAlbumUri() const
{
    return uri_;
}

void PhotoAlbum::SetAlbumName(const string &albumName)
{
    albumName_ = albumName;
}

const string& PhotoAlbum::GetAlbumName() const
{
    return albumName_;
}

void PhotoAlbum::SetCoverUri(const string &coverUri)
{
    coverUri_ = coverUri;
}

const string& PhotoAlbum::GetCoverUri() const
{
    return coverUri_;
}

void PhotoAlbum::SetCount(const int32_t count)
{
    count_ = count;
}

int32_t PhotoAlbum::GetCount() const
{
    return count_;
}

void PhotoAlbum::SetRelativePath(const string &relativePath)
{
    relativePath_ = relativePath;
}

const string& PhotoAlbum::GetRelativePath() const
{
    return relativePath_;
}

void PhotoAlbum::SetTypeMask(const string &typeMask)
{
    typeMask_ = typeMask;
}

const string& PhotoAlbum::GetTypeMask() const
{
    return typeMask_;
}

void PhotoAlbum::SetResultNapiType(const ResultNapiType resultNapiType)
{
    resultNapiType_ = resultNapiType;
}

ResultNapiType PhotoAlbum::GetResultNapiType() const
{
    return resultNapiType_;
}

bool PhotoAlbum::CheckPhotoAlbumType(const PhotoAlbumType albumType)
{
    return (albumType == USER) || (albumType == SYSTEM);
}

bool PhotoAlbum::CheckPhotoAlbumSubType(const PhotoAlbumSubType albumSubType)
{
    PhotoAlbumSubType systemStart = VIDEO;
    PhotoAlbumSubType systemEnd = CAMERA;
    return (albumSubType == USER_GENERIC) || ((albumSubType >= systemStart) && (albumSubType <= systemEnd)) ||
        (albumSubType == ANY);
}
}  // namespace Media
}  // namespace OHOS