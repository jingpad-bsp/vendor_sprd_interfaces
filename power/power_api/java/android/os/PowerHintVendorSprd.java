/*
 * Copyright (C) 2007 The Android Open Source Project
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

package android.os;

// Update version when Power HAL upgrade
import vendor.sprd.hardware.power.V4_0.PowerHint;

// For provide the same API to user.
// Update the class when update PowerHint in types.hal
public final class PowerHintVendorSprd {
    public static final int POWER_HINT_VSYNC = PowerHint.VSYNC;
    public static final int POWER_HINT_INTERACTION= PowerHint.INTERACTION;
    public static final int POWER_HINT_VIDEO_ENCODE = PowerHint.VIDEO_ENCODE;
    public static final int POWER_HINT_VIDEO_DECODE = PowerHint.VIDEO_DECODE;
    public static final int POWER_HINT_LOW_POWER = PowerHint.LOW_POWER;
    public static final int POWER_HINT_SUSTAINED_PERFORMANCE = PowerHint.SUSTAINED_PERFORMANCE;
    public static final int POWER_HINT_VR_MODE = PowerHint.VR_MODE;
    public static final int POWER_HINT_LAUNCH = PowerHint.LAUNCH;

    public static final int POWER_HINT_VENDOR_BENCHMARK = PowerHint.VENDOR_BENCHMARK;
    public static final int POWER_HINT_VENDOR_VIDEO_SYNC = PowerHint.VENDOR_VIDEO_SYNC;
    public static final int POWER_HINT_VENDOR_VIDEO_60FPS = PowerHint.VENDOR_VIDEO_60FPS;
    public static final int POWER_HINT_VENDOR_VIDEO_1080P = PowerHint.VENDOR_VIDEO_1080P;
    public static final int POWER_HINT_VENDOR_MP4_PLAYBACK = PowerHint.VENDOR_MP4_PLAYBACK;
    public static final int POWER_HINT_VENDOR_SCREENOFF_MP3_PLAYBACK = PowerHint.VENDOR_SCREENOFF_MP3_PLAYBACK;
    public static final int POWER_HINT_VENDOR_CAMERA_PERFORMANCE = PowerHint.VENDOR_CAMERA_PERFORMANCE;
    public static final int POWER_HINT_VENDOR_CAMERA_LOW_POWER = PowerHint.VENDOR_CAMERA_LOW_POWER;
    public static final int POWER_HINT_VENDOR_CAMERA_HIGH_PERFORMANCE = PowerHint.VENDOR_CAMERA_HIGH_PERFORMANCE;
    public static final int POWER_HINT_VENDOR_PERFORMANCE_GTS = PowerHint.VENDOR_PERFORMANCE_GTS;
    public static final int POWER_HINT_VENDOR_PERFORMANCE_CTS = PowerHint.VENDOR_PERFORMANCE_CTS;
    public static final int POWER_HINT_VENDOR_PERFORMANCE = PowerHint.VENDOR_PERFORMANCE;
    public static final int POWER_HINT_VENDOR_PERFORMANCE_MAX = PowerHint.VENDOR_PERFORMANCE_MAX;
    public static final int POWER_HINT_VENDOR_RADIO_CALL = PowerHint.VENDOR_RADIO_CALL;
    public static final int POWER_HINT_VENDOR_RADIO_NVIOT = PowerHint.VENDOR_RADIO_NVIOT;
    public static final int POWER_HINT_VENDOR_BT_DOWNLOAD = PowerHint.VENDOR_BT_DOWNLOAD;
    public static final int POWER_HINT_VENDOR_GTS_MEMORY = PowerHint.VENDOR_GTS_MEMORY;
    public static final int POWER_HINT_VENDOR_GTS_MEMORY_PSS = PowerHint.VENDOR_GTS_MEMORY_PSS;
    public static final int POWER_HINT_VENDOR_GTS_VM_FAULT_AROUND = PowerHint.VENDOR_GTS_VM_FAULT_AROUND;
    public static final int POWER_HINT_VENDOR_TEMP_CTRL = PowerHint.VENDOR_TEMP_CTRL;
    public static final int POWER_HINT_VENDOR_SCREEN_ROTATION = PowerHint.VENDOR_SCREEN_ROTATION;
    public static final int POWER_HINT_VENDOR_AUDIO_PLAYBACK = PowerHint.VENDOR_AUDIO_PLAYBACK;
    public static final int POWER_HINT_VENDOR_VIDEO_1 = PowerHint.VENDOR_VIDEO_1;
    public static final int POWER_HINT_VENDOR_VIDEO_2 = PowerHint.VENDOR_VIDEO_2;
    public static final int POWER_HINT_VENDOR_VIDEO_3 = PowerHint.VENDOR_VIDEO_3;
    public static final int POWER_HINT_VENDOR_VIDEO_4 = PowerHint.VENDOR_VIDEO_4;
    public static final int POWER_HINT_VENDOR_DDR = PowerHint.VENDOR_DDR;
    public static final int POWER_HINT_VENDOR_CAMERA_LOW_POWER_1 = PowerHint.VENDOR_CAMERA_LOW_POWER_1;

    public static final int POWER_HINT_VENDOR_INTERACTION_OTHER = PowerHint.VENDOR_INTERACTION_OTHER;
    public static final int POWER_HINT_VENDOR_INTERACTION_TOUCH = PowerHint.VENDOR_INTERACTION_TOUCH;
    public static final int POWER_HINT_VENDOR_INTERACTION_LAUNCH = PowerHint.VENDOR_INTERACTION_LAUNCH;
    public static final int POWER_HINT_VENDOR_INTERACTION_FLING = PowerHint.VENDOR_INTERACTION_FLING;
    public static final int POWER_HINT_VENDOR_INTERACTION_FLING_1 = PowerHint.VENDOR_INTERACTION_FLING_1;
    public static final int POWER_HINT_VENDOR_INTERACTION_BUTTON = PowerHint.VENDOR_INTERACTION_BUTTON;
    public static final int POWER_HINT_VENDOR_INTERACTION_WAKEUP = PowerHint.VENDOR_INTERACTION_WAKEUP;

    public static final int POWER_HINT_VENDOR_MODE_NORMAL = PowerHint.VENDOR_MODE_NORMAL;
    public static final int POWER_HINT_VENDOR_MODE_LOW_POWER = PowerHint.VENDOR_MODE_LOW_POWER;
    public static final int POWER_HINT_VENDOR_MODE_POWER_SAVE = PowerHint.VENDOR_MODE_POWER_SAVE;
    public static final int POWER_HINT_VENDOR_MODE_ULTRA_POWER_SAVE = PowerHint.VENDOR_MODE_ULTRA_POWER_SAVE;
    public static final int POWER_HINT_VENDOR_MODE_PERFORMANCE = PowerHint.VENDOR_MODE_PERFORMANCE;
    //public static final int VENDOR_MODE_MAX = PowerHint.VENDOR_MODE_MAX;

    public static final int POWER_HINT_VENDOR_SCREEN_ON = PowerHint.VENDOR_SCREEN_ON;
    public static final int POWER_HINT_VENDOR_SCREEN_OFF = PowerHint.VENDOR_SCREEN_OFF;
};
