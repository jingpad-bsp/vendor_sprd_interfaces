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

import android.util.Log;
import android.content.Context;
import android.os.RemoteException;
import android.os.Handler;
import android.hidl.base.V1_0.IBase;

// Update version when Power HAL upgrade
import vendor.sprd.hardware.power.V4_0.IPower;

/**
 * Using this class to access Power HAL HIDL service.
 */
public final class PowerHALManager implements HwBinder.DeathRecipient {
    private static final String TAG = "PowerHALManager";

    /**
     * Duration times(ms) limit for all POWER_HINT_VENDOR subtype.
     * @hide
     */
    public static final int POWER_HINT_VENDOR_TIMEOUT_MIN = 500;
    /** {@hide} */
    public static final int POWER_HINT_VENDOR_TIMEOUT_MAX = 5000;

    private Context mContext;
    private IPower mService;
    private Handler mHandler;

    /** {@hide} */
    public PowerHALManager(Context context, Handler handler) {
        mContext = context;
        mHandler = handler;

        try {
            mService = IPower.getService();
        } catch (Exception e) {
            Log.e(TAG, "Get Power HIDL HAL service failed!!!");
            e.printStackTrace();
        }
    }

    /** {@hide} */
    public PowerHintScene createPowerHintScene(String tag, int sceneId, String sceneName) {
        if (mService == null) {
            Log.e(TAG, "Power HAL service isn't working");
            return null;
        }

        try {
            mService.linkToDeath(this, 0);
        } catch (Exception e) {
            Log.e(TAG, "linkToDeath() failed");
            e.printStackTrace();
        }

        return new PowerHintScene(tag, sceneId, sceneName);
    }

    /** {@hide} */
    public final int getSceneIdByName(String sceneName) {
        int sceneId = 0;
        if (sceneName == null || mService == null) {
            Log.e(TAG, "getSceneIdByName(null) or mService is null!!!");
            return 0;
        }

        try {
            sceneId = mService.getSceneIdByName(sceneName);
        } catch (RemoteException e) {
        }

        return sceneId;
    }

    public void enable() {
        if (mService == null) {
            Log.e(TAG, "mService is null!!!");
            return;
        }

        try {
            mService.controlPowerHint(1);
        } catch (RemoteException e) {
        }
    }

    public void disable() {
        if (mService == null) {
            Log.e(TAG, "mService is null!!!");
            return;
        }

        try {
            mService.controlPowerHint(0);
        } catch (RemoteException e) {
        }
    }

    @Override
    /** {@hidle} */
    public void serviceDied(long cookie) {
        Log.e(TAG, "Power HAL service has died");
        mService = null;
    }

    /** {@hide} */
    public final class PowerHintScene {
        private int mSceneId;
        private String mSceneName;
        private String mTag;
        private IBase mToken;
        private int mCount;

        private final Runnable mReleaser = new Runnable() {
            public void run() {
                release();
            }
        };

        private PowerHintScene(String tag, int sceneId, String sceneName) {
            mTag = tag;
            mSceneId= sceneId;
            mSceneName = sceneName;
            mCount = 0;
            mToken = new IBase.Stub() {
            };
        }

        @Override
        protected void finalize() throws Throwable {
            synchronized (this) {
                if (mCount > 0) {
                    try {
                        if (mHandler != null)
                            mHandler.removeCallbacks(mReleaser);
                        if (mService == null) {
                            try {
                                mService = IPower.getService();
                            } catch (Exception e) {
                                Log.e(TAG, "Get Power HIDL HAL service failed!!!");
                                e.printStackTrace();
                            }

                            if (mService == null)
                                return;

                            try {
                                mService.linkToDeath(PowerHALManager.this, 0);
                            } catch (Exception e) {
                                Log.e(TAG, "linkToDeath() failed");
                                e.printStackTrace();
                            }
                        }

                        if (mSceneId != 0) {
                            mService.releasePowerHintBySceneId(mToken, mSceneId);
                        } else {
                            mService.releasePowerHintBySceneName(mToken, mSceneName);
                        }
                    } catch (RemoteException e) {
                    }
                }
            }
        }

        public void acquire() {
            synchronized (this) {
                acquireLocked();
            }
        }

        public void acquire(long timeout) {
            if (mHandler == null) {
                Log.e(TAG, "mHandler == null, Unable to support timed release!!!");
                return;
            }
            synchronized (this) {
                acquireLocked();
                if (timeout < POWER_HINT_VENDOR_TIMEOUT_MIN) {
                    timeout = POWER_HINT_VENDOR_TIMEOUT_MIN;
                } else if (timeout > POWER_HINT_VENDOR_TIMEOUT_MAX) {
                    timeout = POWER_HINT_VENDOR_TIMEOUT_MAX;
                }
                mHandler.postDelayed(mReleaser, timeout);
            }
        }

        private void acquireLocked() {
            if (mCount++ == 0) {
                if (mHandler != null)
                    mHandler.removeCallbacks(mReleaser);
                try {
                    if (mService == null) {
                        return;
                    }
                    if (mSceneId != 0) {
                        mService.acquirePowerHintBySceneId(mToken, mTag, mSceneId);
                    } else {
                        mService.acquirePowerHintBySceneName(mToken, mTag, mSceneName);
                    }
                } catch (RemoteException e) {
                }
            }
        }

        public void release() {
            synchronized (this) {
                if (--mCount == 0) {
                    if (mHandler != null)
                        mHandler.removeCallbacks(mReleaser);
                    try {
                        if (mService == null) {
                            return;
                        }

                        if (mSceneId != 0) {
                            mService.releasePowerHintBySceneId(mToken, mSceneId);
                        } else {
                            mService.releasePowerHintBySceneName(mToken, mSceneName);
                        }
                    } catch (RemoteException e) {
                    }
                }
            }
        }
    }
}
