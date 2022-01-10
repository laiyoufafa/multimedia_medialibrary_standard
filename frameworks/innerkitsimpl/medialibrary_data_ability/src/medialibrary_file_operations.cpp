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

#include "medialibrary_file_operations.h"
#include "media_log.h"

using namespace std;
using namespace OHOS::NativeRdb;

namespace OHOS {
namespace Media {
void UpdateDateModifiedForAlbum(const shared_ptr<RdbStore> &rdbStore, const string &albumPath)
{
    if (!albumPath.empty()) {
        int32_t count(0);
        vector<string> whereArgs = { albumPath };
        ValuesBucket valuesBucket;
        valuesBucket.PutLong(MEDIA_DATA_DB_DATE_MODIFIED,
            MediaLibraryDataAbilityUtils::GetAlbumDateModified(albumPath));

        int32_t updateResult = rdbStore->Update(count, MEDIALIBRARY_TABLE, valuesBucket,
                                                MEDIA_DATA_DB_FILE_PATH + " = ?", whereArgs);
        if (updateResult != E_OK) {
            MEDIA_ERR_LOG("Update failed for album");
        }
    }
}

int32_t MediaLibraryFileOperations::HandleCreateAsset(const ValuesBucket &values,
                                                      const shared_ptr<RdbStore> &rdbStore)
{
    string relativePath("");
    string path("");
    string displayName("");
    int32_t errCode = DATA_ABILITY_FAIL;
    int32_t mediaType = static_cast<int32_t>(MEDIA_TYPE_FILE);
    FileAsset fileAsset;
    MediaLibraryFileDb fileDbOprn;
    ValueObject valueObject;
    NativeAlbumAsset  nativeAlbumAsset;
    // Obtain file displayName
    if (values.GetObject(MEDIA_DATA_DB_NAME, valueObject)) {
        valueObject.GetString(displayName);
    }

    // Obtain relative path
    if (values.GetObject(MEDIA_DATA_DB_RELATIVE_PATH, valueObject)) {
        valueObject.GetString(relativePath);
        path = MEDIA_DATA_DB_Path + relativePath + displayName;
    }

    // Obtain mediatype
    if (values.GetObject(MEDIA_DATA_DB_MEDIA_TYPE, valueObject)) {
        valueObject.GetInt(mediaType);
        fileAsset.SetMediaType(static_cast<MediaType>(mediaType));
    }

    nativeAlbumAsset = MediaLibraryDataAbilityUtils::CreateAlbum(relativePath, rdbStore);
    if (nativeAlbumAsset.GetAlbumId() < 0) {
        MEDIA_ERR_LOG("fileAsset CreateAlbum faild error");
        return errCode;
    }

    nativeAlbumAsset = MediaLibraryDataAbilityUtils::GetAlbumAsset(to_string(nativeAlbumAsset.GetAlbumId()), rdbStore);
    if (MediaLibraryDataAbilityUtils::isFileExistInDb(path, rdbStore)) {
        if (fileAsset.IsFileExists(path)) {
            MEDIA_ERR_LOG("fileAsset File is Exists error");
            return errCode;
        } else {
            MEDIA_ERR_LOG("fileAsset File not Exists");
            int32_t deletedRows(FILE_OPERATION_ERR);
            vector<string> whereArgs = { path };

            int32_t deleteResult = rdbStore->Delete(deletedRows, MEDIALIBRARY_TABLE, "data = ?", whereArgs);
            if (deleteResult != E_OK) {
                MEDIA_ERR_LOG("Delete rows failed");
                return errCode;
            }
        }
    }
    errCode = fileAsset.CreateAsset(path);
    if ((errCode == DATA_ABILITY_SUCCESS) && (!displayName.empty()) && (displayName.at(0) != '.')) {
        // Fill basic file information into DB
        ValuesBucket updatedAssetInfo = UpdateBasicAssetDetails(mediaType, displayName, relativePath, path);
        updatedAssetInfo.PutInt(MEDIA_DATA_DB_BUCKET_ID, nativeAlbumAsset.GetAlbumId());
        updatedAssetInfo.PutString(MEDIA_DATA_DB_BUCKET_NAME, nativeAlbumAsset.GetAlbumName());
        // will return row id
        return fileDbOprn.Insert(updatedAssetInfo, rdbStore);
    }
    return errCode;
}

int32_t MediaLibraryFileOperations::HandleCloseAsset(string &srcPath, const ValuesBucket &values,
    const shared_ptr<RdbStore> &rdbStore)
{
    string fileName;

    if (!srcPath.empty() && ((fileName = MediaLibraryDataAbilityUtils::GetFileName(srcPath)).length() != 0) &&
        (fileName.at(0) != '.')) {
        string albumPath = MediaLibraryDataAbilityUtils::GetParentPath(srcPath);
        UpdateDateModifiedForAlbum(rdbStore, albumPath);
    }

    return DATA_ABILITY_SUCCESS;
}

int32_t MediaLibraryFileOperations::HandleModifyAsset(const string &rowNum, const string &srcPath,
    const ValuesBucket &values, const shared_ptr<RdbStore> &rdbStore)
{
    string dstFilePath;
    int32_t errCode = DATA_ABILITY_FAIL;
    ValueObject valueObject;
    string dstFileName;
    FileAsset fileAsset;
    MediaLibraryFileDb fileDbOprn;

    if (values.GetObject(MEDIA_DATA_DB_FILE_PATH, valueObject)) {
        valueObject.GetString(dstFilePath);
    }

    string destAlbumPath = MediaLibraryDataAbilityUtils::GetParentPath(dstFilePath);
    dstFileName = MediaLibraryDataAbilityUtils::GetFileName(dstFilePath);

    errCode = fileAsset.ModifyAsset(srcPath, dstFilePath);
    if (errCode == DATA_ABILITY_FAIL) {
        MEDIA_ERR_LOG("Failed to modify the file in the device");
        return errCode;
    }

    if (dstFileName == ".nomedia") {
        int32_t deletedRows(ALBUM_OPERATION_ERR);
        vector<string> whereArgs = {
            (destAlbumPath.back() != '/' ? (destAlbumPath + "/%") : (destAlbumPath + "%")), destAlbumPath
        };

        int32_t deleteResult = rdbStore->Delete(deletedRows, MEDIALIBRARY_TABLE,
            MEDIA_DATA_DB_FILE_PATH + " LIKE ? OR " + MEDIA_DATA_DB_FILE_PATH + " = ?", whereArgs);
        if (deleteResult != E_OK) {
            MEDIA_ERR_LOG("Delete rows for the hidden album failed");
        }

        whereArgs.clear();
        whereArgs.push_back(srcPath);
        deleteResult = rdbStore->Delete(deletedRows, MEDIALIBRARY_TABLE, MEDIA_DATA_DB_FILE_PATH + " = ?", whereArgs);
        if (deleteResult != E_OK) {
            MEDIA_ERR_LOG("Delete rows for the old path failed");
        }
        return errCode;
    }

    if ((!dstFileName.empty()) && (dstFileName.at(0) == '.')) {
        int32_t deletedRows(ALBUM_OPERATION_ERR);
        vector<string> whereArgs = { srcPath };

        int32_t deleteResult = rdbStore->Delete(deletedRows, MEDIALIBRARY_TABLE,
                                                MEDIA_DATA_DB_FILE_PATH + " = ?", whereArgs);
        if (deleteResult != E_OK) {
            MEDIA_ERR_LOG("Delete rows failed");
        }
        return errCode;
    }

    if (fileDbOprn.Modify(rowNum, dstFilePath, rdbStore) > 0) {
        UpdateDateModifiedForAlbum(rdbStore, destAlbumPath);

        string srcAlbumPath = MediaLibraryDataAbilityUtils::GetParentPath(srcPath);
        UpdateDateModifiedForAlbum(rdbStore, srcAlbumPath);
    }

    return errCode;
}

int32_t MediaLibraryFileOperations::HandleDeleteAsset(const string &rowNum, const string &srcPath,
    const shared_ptr<RdbStore> &rdbStore)
{
    int32_t errCode = DATA_ABILITY_FAIL;
    FileAsset fileAsset;
    MediaLibraryFileDb fileDbOprn;

    if (!srcPath.empty()) {
        errCode = fileAsset.DeleteAsset(srcPath);
    }

    if (errCode == DATA_ABILITY_SUCCESS) {
        if (fileDbOprn.Delete(rowNum, rdbStore) > 0) {
            string albumPath = MediaLibraryDataAbilityUtils::GetParentPath(srcPath);
            UpdateDateModifiedForAlbum(rdbStore, albumPath);
        }
    }

    return errCode;
}

int32_t MediaLibraryFileOperations::HandleFileOperation(const string &oprn, const ValuesBucket &values,
    const shared_ptr<RdbStore> &rdbStore)
{
    int32_t errCode = DATA_ABILITY_FAIL;

    if (oprn == MEDIA_FILEOPRN_CREATEASSET) {
        return HandleCreateAsset(values, rdbStore);
    }

    string actualUri;
    ValueObject valueObject;

    if (values.GetObject(MEDIA_DATA_DB_URI, valueObject)) {
        valueObject.GetString(actualUri);
    }

    string id = MediaLibraryDataAbilityUtils::GetIdFromUri(actualUri);
    string srcPath = MediaLibraryDataAbilityUtils::GetPathFromDb(id, rdbStore);
    CHECK_AND_RETURN_RET_LOG(!srcPath.empty(), DATA_ABILITY_FAIL, "Failed to obtain path from Database");

    if (oprn == MEDIA_FILEOPRN_MODIFYASSET) {
        errCode = HandleModifyAsset(id, srcPath, values, rdbStore);
    } else if (oprn == MEDIA_FILEOPRN_DELETEASSET) {
        errCode = HandleDeleteAsset(id, srcPath, rdbStore);
    } else if (oprn == MEDIA_FILEOPRN_CLOSEASSET) {
        errCode = HandleCloseAsset(srcPath, values, rdbStore);
    }

    return errCode;
}

ValuesBucket MediaLibraryFileOperations::UpdateBasicAssetDetails(int32_t mediaType,
                                                                 const string &fileName,
                                                                 const string &relPath,
                                                                 const string &path)
{
    ValuesBucket assetInfoBucket;
    assetInfoBucket.PutString(Media::MEDIA_DATA_DB_RELATIVE_PATH, relPath);
    assetInfoBucket.PutString(Media::MEDIA_DATA_DB_NAME, fileName);
    assetInfoBucket.PutString(Media::MEDIA_DATA_DB_TITLE, MediaLibraryDataAbilityUtils::GetFileTitle(fileName));

    struct stat statInfo {};
    if (stat(path.c_str(), &statInfo) == 0) {
        assetInfoBucket.PutLong(Media::MEDIA_DATA_DB_SIZE, statInfo.st_size);
        assetInfoBucket.PutLong(Media::MEDIA_DATA_DB_DATE_ADDED, statInfo.st_ctime);
    }

    assetInfoBucket.PutInt(Media::MEDIA_DATA_DB_MEDIA_TYPE, mediaType);
    assetInfoBucket.PutString(Media::MEDIA_DATA_DB_FILE_PATH, path);
    return assetInfoBucket;
}
} // namespace Media
} // namespace OHOS