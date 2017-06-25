// Overload this file in your device specific config if you need
// to add extra camera parameters.
// A typical file would look like this:
/*
 * Copyright (C) 2014 The CyanogenMod Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define CAMERA_PARAMETERS_EXTRA_C \
const char CameraParameters::HW_ZSL_MODE[] = "hw-zsl-mode"; \
const char CameraParameters::HW_VIDEO_RECORDER_ZSL_MODE[] = "hw_video_recorder_zsl_mode"; \
const char CameraParameters::HW_HIGH_RES_VIDEO_ZSL_MODE[] = "hw_high_res_video_zsl_mode"; \
const char CameraParameters::BEAUTY_HDR_REAR[] = "beauty_hdr_rear"; \
const char CameraParameters::BEAUTY_HDR_FRONT[] = "beauty_hdr_front"; \
const char CameraParameters::SMART_HDR_FRONT[] = "smart_hdr_front"; \
const char CameraParameters::HW_DM_TYPE_HDR[] = "hw_dm_type_hdr"; \
const char CameraParameters::HW_EXIF_HDR[] = "hw-exif-hdr"; \
const char CameraParameters::MAKEUP_FRONT[] = "makeup_front"; \
const char CameraParameters::MAKEUP_REAR[] = "makeup_rear";

#define CAMERA_PARAMETERS_EXTRA_H \
    static const char HW_ZSL_MODE[]; \
    static const char HW_VIDEO_RECORDER_ZSL_MODE[]; \
    static const char HW_HIGH_RES_VIDEO_ZSL_MODE[]; \
    static const char BEAUTY_HDR_REAR[]; \
    static const char BEAUTY_HDR_FRONT[]; \
    static const char SMART_HDR_FRONT[]; \
    static const char HW_DM_TYPE_HDR[]; \
    static const char HW_EXIF_HDR[]; \
    static const char MAKEUP_FRONT[]; \
    static const char MAKEUP_REAR[];

