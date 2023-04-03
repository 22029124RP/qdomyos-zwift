package org.cagnulen.qdomyoszwift;

import android.app.ActivityManager;
import android.app.Activity;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.provider.Settings;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;
import android.os.Looper;
import android.os.Handler;
import android.util.Log;
import com.garmin.android.connectiq.ConnectIQ;
import com.garmin.android.connectiq.ConnectIQAdbStrategy;
import com.garmin.android.connectiq.IQApp;
import com.garmin.android.connectiq.IQDevice;
import com.garmin.android.connectiq.exception.InvalidStateException;
import com.garmin.android.connectiq.exception.ServiceUnavailableException;
import android.content.BroadcastReceiver;
import android.content.ContextWrapper;
import android.content.IntentFilter;
import android.widget.Toast;

import org.jetbrains.annotations.Nullable;

import java.util.HashMap;
import java.util.List;

public class Garmin {

    private static IQDevice deviceCache;

    public static Boolean connectIqReady = false;
    private static Boolean connectIqInitializing = false;

    private static ConnectIQ connectIQ;
    private static Context context;

    private static final String TAG = "CIQManager: ";

    public static void init(Context c) {
        new Handler(Looper.getMainLooper()).post(new Runnable() {
            @Override
            public void run() {
            connectIQ = ConnectIQ.getInstance(c, ConnectIQ.IQConnectType.WIRELESS);

            // init a wrapped SDK with fix for "Cannot cast to Long" issue viz https://forums.garmin.com/forum/developers/connect-iq/connect-iq-bug-reports/158068-?p=1278464#post1278464
            context = initializeConnectIQWrapped(c, connectIQ, false, new ConnectIQ.ConnectIQListener() {

                @Override
                public void onInitializeError(ConnectIQ.IQSdkErrorStatus errStatus) {
                    Log.e(TAG, errStatus.toString());
                    connectIqReady = false;
                }

                @Override
                public void onSdkReady() {
                    connectIqInitializing = false;
                    connectIqReady = true;
                    Log.i(TAG, " onSdkReady");

                    registerWatchMessagesReceiver();
                    registerDeviceStatusReceiver();
                    isWatchAppAvailable();
                }

                @Override
                public void onSdkShutDown() {
                    connectIqInitializing = false;
                    connectIqReady = false;
                }
            });
            }
        });
    }

    @Nullable
    public static IQDevice getDevice() {
        return getDevice(connectIQ);
    }

    @Nullable
    private static IQDevice getDevice(ConnectIQ connectIQ) {
        try {
            List<IQDevice> devices = connectIQ.getConnectedDevices();
            if (devices != null && devices.size() > 0) {
                Log.v(TAG, "getDevice connected: " + devices.get(0).toString() );
                deviceCache = devices.get(0);
                return deviceCache;
            } else {
                return deviceCache;
            }
        } catch (InvalidStateException e) {
            Log.e(TAG, e.toString());
        } catch (ServiceUnavailableException e) {
            Log.e(TAG, e.toString());
        }
        return null;
    }

    // Should rewrite to use connectiq.getApplicationInfo() with callback (maybe wrap in RxJava)
    public static IQApp getApp() {
        return new IQApp("feec8674-2795-4e03-a283-0b69a0a291e3");
    }

    public void onOpenAppOnWatch(ConnectIQ.IQOpenApplicationListener listener) throws InvalidStateException, ServiceUnavailableException {
        if (getDevice() == null) { return; }

        if (getDevice().getFriendlyName().contains("vívoactive3")) {
            //Notifications.showCannotStartFromPhoneNotification(context);
        } else {
            connectIQ.openApplication(getDevice(), getApp(), listener);
        }
    }

    public void sendMessageToWatch(String message, ConnectIQ.IQSendMessageListener listener) throws InvalidStateException, ServiceUnavailableException {
        connectIQ.sendMessage(getDevice(), getApp(), message, listener);
    }

    private static Context initializeConnectIQWrapped(Context context, ConnectIQ connectIQ, boolean autoUI, ConnectIQ.ConnectIQListener listener) {
        if (connectIQ instanceof ConnectIQAdbStrategy) {
            connectIQ.initialize(context, autoUI, listener);
            return context;
        }
        Context wrappedContext = new ContextWrapper(context) {
            private HashMap<BroadcastReceiver, BroadcastReceiver> receiverToWrapper = new HashMap<>();

            @Override
            public Intent registerReceiver(final BroadcastReceiver receiver, IntentFilter filter) {
                BroadcastReceiver wrappedRecv = new IQMessageReceiverWrapper(receiver);
                synchronized (receiverToWrapper) {
                    receiverToWrapper.put(receiver, wrappedRecv);
                }
                return super.registerReceiver(wrappedRecv, filter);
            }

            @Override
            public void unregisterReceiver(BroadcastReceiver receiver) {
                BroadcastReceiver wrappedReceiver = null;
                synchronized (receiverToWrapper) {
                    wrappedReceiver = receiverToWrapper.get(receiver);
                    receiverToWrapper.remove(receiver);
                }
                if (wrappedReceiver != null) super.unregisterReceiver(wrappedReceiver);
            }
        };
        connectIQ.initialize(wrappedContext, autoUI, listener);
        return wrappedContext;
    }

    private static void isWatchAppAvailable() {
        try {
            connectIQ.getApplicationInfo("feec8674-2795-4e03-a283-0b69a0a291e3", getDevice(), new ConnectIQ.IQApplicationInfoListener() {

                @Override
                public void onApplicationInfoReceived(IQApp app) {

                }

                @Override
                public void onApplicationNotInstalled(String applicationId) {
                    if (getDevice() != null) {
                        Toast.makeText(context, "Sleep not installed on your Garmin watch", Toast.LENGTH_LONG).show();
                        Log.d(TAG, "Sleep watch app not installed.");
                    }
                }
            });
        } catch (InvalidStateException e) {
            Log.e(TAG, e.toString());
        } catch (ServiceUnavailableException e) {
            Log.e(TAG, e.toString());
        }
    }

    private static void registerDeviceStatusReceiver() {
        Log.d(TAG, "registerDeviceStatusReceiver");
        IQDevice device = getDevice();
        try {
            if (device != null) {
                connectIQ.registerForDeviceEvents(device, new ConnectIQ.IQDeviceEventListener() {
                    @Override
                    public void onDeviceStatusChanged(IQDevice device, IQDevice.IQDeviceStatus newStatus) {
                        Log.d(TAG, "Device status changed, now " + newStatus);
                    }
                });
            }
        } catch (InvalidStateException e) {
            e.printStackTrace();
        }
    }

    private static void registerWatchMessagesReceiver(){
        Log.d(TAG, "registerWatchMessageReceiver");
        IQDevice device = getDevice();
        try {
            if (device != null) {
                connectIQ.registerForAppEvents(device, getApp(), new ConnectIQ.IQApplicationEventListener() {
                    @Override
                    public void onMessageReceived(IQDevice device, IQApp app, List<Object> message, ConnectIQ.IQMessageStatus status) {
                        if (status == ConnectIQ.IQMessageStatus.SUCCESS) {
                            //MessageHandler.getInstance().handleMessageFromWatchUsingCIQ(message, status, context);
                            Log.d(TAG, "onMessageReceived, status: " + status.toString());
                        } else {
                            Log.d(TAG, "onMessageReceived error, status: " + status.toString());
                        }
                    }
                });
            } else {
                Log.d(TAG, "registerWatchMessagesReceiver: No device found.");
            }
        } catch (InvalidStateException e) {
            Log.e(TAG, e.toString());
        }
    }

    public void shutdown(Context applicationContext) {
        connectIqReady = false;
        unregisterApp(connectIQ);

        try {
            if (context != null) {
                Log.d(TAG,  "Shutting down with wrapped context");
                connectIQ.shutdown(context);
            } else {
                Log.d(TAG,  "Shutting down without wrapped context");
                connectIQ.shutdown(applicationContext);
            }
        } catch (InvalidStateException e) {
            // This is usually because the SDK was already shut down so no worries.
            Log.e(TAG, "This is usually because the SDK was already shut down so no worries.", e);
        } catch (IllegalArgumentException e) {
            Log.e(TAG, e.toString());
        } catch (RuntimeException e) {
            Log.e(TAG, e.toString());
        }
    }

    private void unregisterApp(ConnectIQ connectIQ) {
        try {
            if (connectIQ != null) {
                IQDevice device = getDevice();
                if (device != null) {
                    connectIQ.unregisterForApplicationEvents(device, getApp());
                    connectIQ.unregisterForDeviceEvents(device);
                }
            }
        } catch (InvalidStateException e) {
            Log.e(TAG, e.toString());
        } catch (IllegalArgumentException e) {
            Log.e(TAG, e.toString());
        } catch (RuntimeException e) {
            Log.e(TAG, e.toString());
        }
    }
}
