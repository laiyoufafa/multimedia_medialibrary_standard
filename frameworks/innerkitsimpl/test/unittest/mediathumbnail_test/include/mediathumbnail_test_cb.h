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

#ifndef FRAMEWORKS_INNERKITSIMPL_TEST_UNITTEST_MEDIATHUMBNAIL_TEST_INCLUDE_MEDIATHUMBNAIL_TEST_CB_H_
#define FRAMEWORKS_INNERKITSIMPL_TEST_UNITTEST_MEDIATHUMBNAIL_TEST_INCLUDE_MEDIATHUMBNAIL_TEST_CB_H_

#include "gtest/gtest.h"
#include "medialibrary_thumbnail.h"
#include "media_data_ability_const.h"
#include "medialibrary_data_ability.h"

namespace OHOS {
namespace Media {
class MediaThumbnailTestCB : public RdbOpenCallback {
public:
    int OnCreate(RdbStore &rdbStore) override;
    int OnUpgrade(RdbStore &rdbStore, int oldVersion, int newVersion) override;
};
} // namespace Media
} // namespace OHOS
#endif  // FRAMEWORKS_INNERKITSIMPL_TEST_UNITTEST_MEDIATHUMBNAIL_TEST_INCLUDE_MEDIATHUMBNAIL_TEST_CB_H_
