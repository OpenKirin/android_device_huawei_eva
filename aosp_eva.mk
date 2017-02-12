#
# Copyright (C) 2017 OpenKirin
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# Inherit from the common Open Source product configuration
$(call inherit-product, $(SRC_TARGET_DIR)/product/core_64_bit.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base_telephony.mk)

PRODUCT_NAME := aosp_eva
PRODUCT_DEVICE := eva
PRODUCT_BRAND := Android
PRODUCT_MODEL := AOSP on Eva
PRODUCT_MANUFACTURER := HUAWEI
PRODUCT_RESTRICT_VENDOR_FILES := false

BOARD_VENDOR := huawei
PRODUCT_GMS_CLIENTID_BASE := android-huawei

$(call inherit-product, device/huawei/eva/device.mk)
$(call inherit-product-if-exists, vendor/huawei/eva/eva-vendor.mk)

PRODUCT_PACKAGES += \
    Launcher3
