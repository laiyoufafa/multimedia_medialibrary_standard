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

#ifndef MTP_NATIVE_TEST_H
#define MTP_NATIVE_TEST_H

#include "gtest/gtest.h"
#include "iservice_registry.h"
#include "media_log.h"
#include "media_volume.h"
#include "media_library_manager.h"
#include "system_ability_definition.h"
#include "datashare_helper.h"
#include "mtp_medialibrary_manager.h"
#include "mtp_operation_context.h"
#include "mtp_storage_manager.h"

namespace OHOS {
    namespace Media {
        class MtpNativeTest : public testing::Test {
        public:
            static void SetUpTestCase (void);
            static void TearDownTestCase (void);
            void SetUp ();
            void TearDown ();
        };
    } // namespace Media
} // namespace OHOS
#endif // MTP_NATIVE_TEST_H