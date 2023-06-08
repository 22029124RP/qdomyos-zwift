package org.cagnulen.qdomyoszwift;

import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.Queue;
import java.util.Set;

import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbDeviceConnection;
import android.hardware.usb.UsbEndpoint;
import android.hardware.usb.UsbInterface;
import android.hardware.usb.UsbManager;
import android.util.Log;

public class CSafeRowerUSBHID {

	static HidBridge hidBridge;
	static byte[] receiveData;
	static int lastReadLen = 0;

	public static void open(Context context) {
		 Log.d("QZ","CSafeRowerUSBHID open");
		 hidBridge = new HidBridge(context, 0x17A4, 0x0002);
		 boolean ret = hidBridge.OpenDevice();
		 Log.d("QZ","hidBridge.OpenDevice " + ret);
	}

public static void write (byte[] bytes) {
	Log.d("QZ","CSafeRowerUSBHID writing " + new String(bytes, StandardCharsets.UTF_8));
	try {
		hidBridge.WriteData(bytes);
		}
	catch (IOException e) {
		Log.d("QZ","CSafeRowerUSBHID writing exception " + e.toString());
		}
	}

public static int readLen() {
	return lastReadLen;
	}

public static byte[] read() {
	try {
		receiveData = hidBridge.GetReceivedDataFromQueue();
		lastReadLen = receiveData.length;
		Log.d("QZ","CSafeRowerUSBHID reading " + lastReadLen + new String(receiveData, StandardCharsets.UTF_8));
		}
	   catch (IOException e) {
		  Log.d("QZ","CSafeRowerUSBHID reading exception " + e.toString());
		}
	return receiveData;
	}
}
