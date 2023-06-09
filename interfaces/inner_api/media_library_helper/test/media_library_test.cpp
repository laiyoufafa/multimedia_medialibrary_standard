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

#include <dirent.h>
#include <unistd.h>

#include "media_library.h"
#include "media_log.h"

using namespace std;
using namespace OHOS::Media;

namespace MediaLibTest {
    const int32_t SECOND_ARG_IDX = 2;
}

class MediaLibraryTest {
public:
    void TestGetAssets(const string &selection) const
    {
        unique_ptr<MediaLibrary> mediaLibrary = MediaLibrary::GetMediaLibraryInstance();
        vector<string> selectionArgs;

        MEDIA_DEBUG_LOG("Media assets:");
        vector<unique_ptr<MediaAsset>> mediaAssets
                            = mediaLibrary->GetMediaAssets(selection, selectionArgs);
        for (size_t i = 0; i < mediaAssets.size(); i++) {
            MEDIA_DEBUG_LOG("%{private}s", mediaAssets[i]->uri_.c_str());
        }

        MEDIA_DEBUG_LOG("Audio assets:");
        vector<unique_ptr<AudioAsset>> audioAssets
                            = mediaLibrary->GetAudioAssets(selection, selectionArgs);
        for (size_t i = 0; i < audioAssets.size(); i++) {
            MEDIA_DEBUG_LOG("%{private}s", audioAssets[i]->uri_.c_str());
        }

        MEDIA_DEBUG_LOG("Video assets:");
        vector<unique_ptr<VideoAsset>> videoAssets
                            = mediaLibrary->GetVideoAssets(selection, selectionArgs);
        for (size_t i = 0; i < videoAssets.size(); i++) {
            MEDIA_DEBUG_LOG("%{private}s", videoAssets[i]->uri_.c_str());
        }

        MEDIA_DEBUG_LOG("Image assets:");
        vector<unique_ptr<ImageAsset>> imageAssets
                            = mediaLibrary->GetImageAssets(selection, selectionArgs);
        for (size_t i = 0; i < imageAssets.size(); i++) {
            MEDIA_DEBUG_LOG("uri: %{private}s", imageAssets[i]->uri_.c_str());
            MEDIA_DEBUG_LOG("id: %{private}d", imageAssets[i]->id_);
            MEDIA_DEBUG_LOG("media type: %{private}d", imageAssets[i]->mediaType_);
            MEDIA_DEBUG_LOG("mime type: %{private}s", imageAssets[i]->mimeType_.c_str());
            MEDIA_DEBUG_LOG("name: %{private}s", imageAssets[i]->name_.c_str());
            MEDIA_DEBUG_LOG("date added: %{private}lld", imageAssets[i]->dateAdded_);
            MEDIA_DEBUG_LOG("date modified: %{private}lld", imageAssets[i]->dateModified_);
        }
    }

    void TestGetAlbums(const string &selection) const
    {
        unique_ptr<MediaLibrary> mediaLibrary = MediaLibrary::GetMediaLibraryInstance();
        vector<string> selectionArgs;
        int32_t requestType = (int32_t)MediaType::MEDIA_TYPE_VIDEO;
        vector<unique_ptr<AlbumAsset>> albumAssets
                        = mediaLibrary->GetAlbumAssets(selection, selectionArgs, requestType);
        for (size_t i = 0; i < albumAssets.size(); i++) {
            MEDIA_DEBUG_LOG("Album name:%{private}s", albumAssets[i]->albumName_.c_str());
            MEDIA_DEBUG_LOG("Image album assets:");
            for (size_t j = 0; j < albumAssets[i]->imageAssetList_.size(); j++) {
                MEDIA_DEBUG_LOG("uri: %{private}s", albumAssets[i]->imageAssetList_[j]->uri_.c_str());
                MEDIA_DEBUG_LOG("id: %{private}d", albumAssets[i]->imageAssetList_[j]->id_);
                MEDIA_DEBUG_LOG("width: %{private}d", albumAssets[i]->imageAssetList_[j]->width_);
                MEDIA_DEBUG_LOG("height: %{private}d", albumAssets[i]->imageAssetList_[j]->height_);
                MEDIA_DEBUG_LOG("media type: %{private}d",
                                albumAssets[i]->imageAssetList_[j]->mediaType_);
                MEDIA_DEBUG_LOG("mime type: %{private}s",
                                albumAssets[i]->imageAssetList_[j]->mimeType_.c_str());
                MEDIA_DEBUG_LOG("name: %{private}s", albumAssets[i]->imageAssetList_[j]->name_.c_str());
                MEDIA_DEBUG_LOG("added: %{private}lld",
                                albumAssets[i]->imageAssetList_[j]->dateAdded_);
                MEDIA_DEBUG_LOG("modified: %{private}lld",
                                albumAssets[i]->imageAssetList_[j]->dateModified_);
            }

            MEDIA_DEBUG_LOG("Video album assets:");
            for (size_t k = 0; k < albumAssets[i]->videoAssetList_.size(); k++) {
                MEDIA_DEBUG_LOG("uri: %{private}s", albumAssets[i]->videoAssetList_[k]->uri_.c_str());
                MEDIA_DEBUG_LOG("id: %{private}d", albumAssets[i]->videoAssetList_[k]->id_);
                MEDIA_DEBUG_LOG("width: %{private}d", albumAssets[i]->videoAssetList_[k]->width_);
                MEDIA_DEBUG_LOG("height: %{private}d", albumAssets[i]->videoAssetList_[k]->height_);
                MEDIA_DEBUG_LOG("duration: %{private}d", albumAssets[i]->videoAssetList_[k]->duration_);
                MEDIA_DEBUG_LOG("media type: %{private}d",
                                albumAssets[i]->videoAssetList_[k]->mediaType_);
                MEDIA_DEBUG_LOG("mime type: %{private}s",
                                albumAssets[i]->videoAssetList_[k]->mimeType_.c_str());
                MEDIA_DEBUG_LOG("name: %{private}s", albumAssets[i]->videoAssetList_[k]->name_.c_str());
                MEDIA_DEBUG_LOG("added: %{private}lld",
                                albumAssets[i]->videoAssetList_[k]->dateAdded_);
                MEDIA_DEBUG_LOG("modified: %{private}lld",
                                albumAssets[i]->videoAssetList_[k]->dateModified_);
            }
        }
    }

    void TestMediaLibAPI(const string &selection) const
    {
        MEDIA_INFO_LOG("TestMediaLibAPI start");
        TestGetAssets(selection);
        TestGetAlbums(selection);
        MEDIA_INFO_LOG("TestMediaLibAPI end");
    }
};

int32_t main(int32_t argc, const char *argv[])
{
    MEDIA_INFO_LOG("media lib test in");

    if (argv == nullptr) {
        MEDIA_ERR_LOG("argv is null");
        return 0;
    }

    MEDIA_DEBUG_LOG("argc=%d, argv[0]=%s", argc, argv[0]);
    string selection = "";
    if (argc == MediaLibTest::SECOND_ARG_IDX) {
        selection = argv[MediaLibTest::SECOND_ARG_IDX - 1];
    }
    MediaLibraryTest testObj;
    testObj.TestMediaLibAPI(selection);

    return 0;
}
