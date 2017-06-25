/*
 * Copyright (C) 2016 The CyanogenMod Project
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
#include <hardware/hardware.h>
#include <hardware/power.h>

#include <stdbool.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <utils/Log.h>

#include "power.h"
#include "power-feature.h"
#include "utils.h"

static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static int boostpulse_fd = -1;
static int current_power_profile = 1;

static struct hw_module_methods_t power_module_methods = {
    .open = NULL,
};

static bool check_governor(void)
{
    struct stat s;
    int err = stat(INTERACTIVE_PATH0, &s);
    if (err != 0) return false;
    if (S_ISDIR(s.st_mode)) return true;
    return false;
}

static int is_profile_valid(int profile)
{
    if (profile >= 0 && profile < PROFILE_MAX)
	return profile;
    else
	return -1;
}

static void power_init(__attribute__((unused)) struct power_module *module)
{
    ALOGI("%s", __func__);
}

static int boostpulse_open()
{
    pthread_mutex_lock(&lock);
    if (boostpulse_fd < 0) {
        boostpulse_fd = open(INTERACTIVE_PATH0 "boostpulse", O_WRONLY);
    }
    pthread_mutex_unlock(&lock);

    return boostpulse_fd;
}

static void power_set_interactive(__attribute__((unused)) struct power_module *module, int on)
{
    if (is_profile_valid(current_power_profile) < 0) {
        ALOGD("%s: no power profile selected yet", __func__);
        return;
    }

    // break out early if governor is not interactive
    if (!check_governor()) return;

    if (!on) {
        sysfs_write_int(INTERACTIVE_PATH0 "hispeed_freq",
                        profiles0[current_power_profile].hispeed_freq);
        sysfs_write_int(INTERACTIVE_PATH0 "go_hispeed_load",
                        profiles0[current_power_profile].go_hispeed_load);
        sysfs_write_int(INTERACTIVE_PATH0 "timer_rate",
                        profiles0[current_power_profile].timer_rate);
        sysfs_write_str(INTERACTIVE_PATH0 "target_loads",
                        profiles0[current_power_profile].target_loads);

        sysfs_write_int(INTERACTIVE_PATH1 "hispeed_freq",
                        profiles1[current_power_profile].hispeed_freq);
        sysfs_write_int(INTERACTIVE_PATH1 "go_hispeed_load",
                        profiles1[current_power_profile].go_hispeed_load);
        sysfs_write_int(INTERACTIVE_PATH1 "timer_rate",
                        profiles1[current_power_profile].timer_rate);
        sysfs_write_str(INTERACTIVE_PATH1 "target_loads",
                        profiles1[current_power_profile].target_loads);
    } else {
        sysfs_write_int(INTERACTIVE_PATH0 "hispeed_freq",
                        profiles0[current_power_profile].hispeed_freq_off);
        sysfs_write_int(INTERACTIVE_PATH0 "timer_rate",
                        profiles0[current_power_profile].timer_rate_off);
        sysfs_write_int(INTERACTIVE_PATH0 "go_hispeed_load",
                        profiles0[current_power_profile].go_hispeed_load_off);
        sysfs_write_str(INTERACTIVE_PATH0 "target_loads",
                        profiles0[current_power_profile].target_loads_off);

        sysfs_write_int(INTERACTIVE_PATH1 "hispeed_freq",
                        profiles1[current_power_profile].hispeed_freq_off);
        sysfs_write_int(INTERACTIVE_PATH1 "timer_rate",
                        profiles1[current_power_profile].timer_rate_off);
        sysfs_write_int(INTERACTIVE_PATH1 "go_hispeed_load",
                        profiles1[current_power_profile].go_hispeed_load_off);
        sysfs_write_str(INTERACTIVE_PATH1 "target_loads",
                        profiles1[current_power_profile].target_loads_off);
    }
}

static void set_power_profile(int profile)
{
    if (is_profile_valid(profile) < 0) {
        ALOGE("%s: unknown profile: %d", __func__, profile);
        return;
    }

    // break out early if governor is not interactive
    if (!check_governor()) return;

    if (profile == current_power_profile)
        return;

    ALOGD("%s: setting profile %d", __func__, profile);

    sysfs_write_str(INTERACTIVE_PATH0 "above_hispeed_delay",
                    profiles0[profile].above_hispeed_delay);
    sysfs_write_int(INTERACTIVE_PATH0 "boost",
                    profiles0[profile].boost);
    sysfs_write_int(INTERACTIVE_PATH0 "boostpulse_duration",
                    profiles0[profile].boostpulse_duration);
    sysfs_write_int(INTERACTIVE_PATH0 "boostpulse_min_interval",
                    profiles0[profile].boostpulse_min_interval);
    sysfs_write_int(INTERACTIVE_PATH0 "go_hispeed_load",
                    profiles0[profile].go_hispeed_load);
    sysfs_write_int(INTERACTIVE_PATH0 "hispeed_freq",
                    profiles0[profile].hispeed_freq);
    sysfs_write_int(INTERACTIVE_PATH0 "io_is_busy",
                    profiles0[profile].io_is_busy);
    sysfs_write_int(INTERACTIVE_PATH0 "min_sample_time",
                    profiles0[profile].min_sample_time);
    sysfs_write_str(INTERACTIVE_PATH0 "target_loads",
                    profiles0[profile].target_loads);
    sysfs_write_int(INTERACTIVE_PATH0 "timer_rate",
                    profiles0[profile].timer_rate);
    sysfs_write_int(INTERACTIVE_PATH0 "timer_slack",
                    profiles0[profile].timer_slack);
    sysfs_write_int(CPUFREQ_PATH0 "scaling_max_freq",
                    profiles0[profile].scaling_max_freq);
    sysfs_write_int(CPUFREQ_PATH0 "scaling_min_freq",
                    profiles0[profile].scaling_min_freq);
    sysfs_write_int(KERNEL_HMP_PATH "down_threshold",
                    profiles0[profile].down_threshold);
    sysfs_write_int(KERNEL_HMP_PATH "up_threshold",
                    profiles0[profile].up_threshold);
    sysfs_write_int(KERNEL_HMP_PATH "task_fork_on_bigcluster",
                    profiles0[profile].task_fork_on_bigcluster);
    sysfs_write_int(GPUFREQ_PATH "max_freq",
                    profiles0[profile].max_freq);
    sysfs_write_int(GPUFREQ_PATH "min_freq",
                    profiles0[profile].min_freq);
    sysfs_write_int(GPUFREQ_PATH "polling_interval",
                    profiles0[profile].polling_interval);

    sysfs_write_str(INTERACTIVE_PATH1 "above_hispeed_delay",
                    profiles1[profile].above_hispeed_delay);
    sysfs_write_int(INTERACTIVE_PATH1 "boost",
                    profiles1[profile].boost);
    sysfs_write_int(INTERACTIVE_PATH1 "boostpulse_duration",
                    profiles1[profile].boostpulse_duration);
    sysfs_write_int(INTERACTIVE_PATH1 "boostpulse_min_interval",
                    profiles1[profile].boostpulse_min_interval);
    sysfs_write_int(INTERACTIVE_PATH1 "go_hispeed_load",
                    profiles1[profile].go_hispeed_load);
    sysfs_write_int(INTERACTIVE_PATH1 "hispeed_freq",
                    profiles1[profile].hispeed_freq);
    sysfs_write_int(INTERACTIVE_PATH1 "io_is_busy",
                    profiles1[profile].io_is_busy);
    sysfs_write_int(INTERACTIVE_PATH1 "min_sample_time",
                    profiles1[profile].min_sample_time);
    sysfs_write_str(INTERACTIVE_PATH1 "target_loads",
                    profiles1[profile].target_loads);
    sysfs_write_int(INTERACTIVE_PATH1 "timer_rate",
                    profiles1[profile].timer_rate);
    sysfs_write_int(INTERACTIVE_PATH1 "timer_slack",
                    profiles1[profile].timer_slack);
    sysfs_write_int(CPUFREQ_PATH1 "scaling_max_freq",
                    profiles1[profile].scaling_max_freq);
    sysfs_write_int(CPUFREQ_PATH1 "scaling_min_freq",
                    profiles1[profile].scaling_min_freq);
    sysfs_write_int(KERNEL_HMP_PATH "down_threshold",
                    profiles1[profile].down_threshold);
    sysfs_write_int(KERNEL_HMP_PATH "up_threshold",
                    profiles1[profile].up_threshold);
    sysfs_write_int(KERNEL_HMP_PATH "task_fork_on_bigcluster",
                    profiles1[profile].task_fork_on_bigcluster);
    sysfs_write_int(GPUFREQ_PATH "max_freq",
                    profiles1[profile].max_freq);
    sysfs_write_int(GPUFREQ_PATH "min_freq",
                    profiles1[profile].min_freq);
    sysfs_write_int(GPUFREQ_PATH "polling_interval",
                    profiles1[profile].polling_interval);

    current_power_profile = profile;
}

static void process_video_encode_hint(void *metadata)
{
    int on;

    if (!metadata)
        return;

    /* Break out early if governor is not interactive */
    if (!check_governor())
        return;

    on = !strncmp(metadata, STATE_ON, sizeof(STATE_ON));

    sysfs_write_int(INTERACTIVE_PATH0 "timer_rate", on ?
            VID_ENC_TIMER_RATE :
            profiles0[current_power_profile].timer_rate);
    sysfs_write_int(INTERACTIVE_PATH1 "timer_rate", on ?
            VID_ENC_TIMER_RATE :
            profiles1[current_power_profile].timer_rate);


    sysfs_write_int(INTERACTIVE_PATH0 "io_is_busy", on ?
            VID_ENC_IO_IS_BUSY :
            profiles0[current_power_profile].io_is_busy);
    sysfs_write_int(INTERACTIVE_PATH1 "io_is_busy", on ?
            VID_ENC_IO_IS_BUSY :
            profiles1[current_power_profile].io_is_busy);
}

static void power_hint(__attribute__((unused)) struct power_module *module,
                       power_hint_t hint, void *data)
{
    char buf[80];
    int len;

    switch (hint) {
    case POWER_HINT_INTERACTION:
    case POWER_HINT_LAUNCH:
    case POWER_HINT_CPU_BOOST:
        if (is_profile_valid(current_power_profile) < 0) {
            ALOGD("%s: no power profile selected yet", __func__);
            return;
        }

        if (!profiles0[current_power_profile].boostpulse_duration && !profiles1[current_power_profile].boostpulse_duration)
            return;

        // break out early if governor is not interactive
        if (!check_governor()) return;

        if (boostpulse_open() >= 0) {
            snprintf(buf, sizeof(buf), "%d", 1);
            len = write(boostpulse_fd, &buf, sizeof(buf));
            if (len < 0) {
                strerror_r(errno, buf, sizeof(buf));
                ALOGE("Error writing to boostpulse: %s\n", buf);

                pthread_mutex_lock(&lock);
                close(boostpulse_fd);
                boostpulse_fd = -1;
                pthread_mutex_unlock(&lock);
            }
        }
        break;
    case POWER_HINT_SET_PROFILE:
        pthread_mutex_lock(&lock);
        set_power_profile(*(int32_t *)data);
        pthread_mutex_unlock(&lock);
        break;
    case POWER_HINT_VIDEO_ENCODE:
        pthread_mutex_lock(&lock);
        process_video_encode_hint(data);
        pthread_mutex_unlock(&lock);
        break;
    default:
        break;
    }
}

static int get_number_of_profiles()
{
    return PROFILE_MAX;
}

static void set_feature(struct power_module *module, feature_t feature, int mode)
{
    set_device_specific_feature(module, feature, mode);
}

static int get_feature(struct power_module *module __unused, feature_t feature)
{
    if (feature == POWER_FEATURE_SUPPORTED_PROFILES) {
        return get_number_of_profiles();
    }
    return -1;
}

struct power_module HAL_MODULE_INFO_SYM = {
    .common = {
        .tag = HARDWARE_MODULE_TAG,
        .module_api_version = POWER_MODULE_API_VERSION_0_3,
        .hal_api_version = HARDWARE_HAL_API_VERSION,
        .id = POWER_HARDWARE_MODULE_ID,
        .name = "HI3650 Power HAL",
        .author = "Surdu Petru",
        .methods = &power_module_methods,
    },

    .init = power_init,
    .powerHint = power_hint,
    .setInteractive = power_set_interactive,
    .setFeature = set_feature,
    .getFeature = get_feature
};
