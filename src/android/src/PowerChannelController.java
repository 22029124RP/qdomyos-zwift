/*
 * Copyright 2012 Dynastream Innovations Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */

package org.cagnulen.qdomyoszwift;

import android.os.RemoteException;
import android.util.Log;

import com.dsi.ant.channel.AntChannel;
import com.dsi.ant.channel.AntCommandFailedException;
import com.dsi.ant.channel.IAntChannelEventHandler;
import com.dsi.ant.message.ChannelId;
import com.dsi.ant.message.ChannelType;
import com.dsi.ant.message.EventCode;
import com.dsi.ant.message.fromant.AcknowledgedDataMessage;
import com.dsi.ant.message.fromant.ChannelEventMessage;
import com.dsi.ant.message.fromant.MessageFromAntType;
import com.dsi.ant.message.ipc.AntMessageParcel;

import java.util.Locale;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.TimeUnit;

public class PowerChannelController extends AntChannelController {
    public static final int POWER_SENSOR_ID = 0x9e3d4b66;

    // The device type and transmission type to be part of the channel ID message
    private static final int CHANNEL_POWER_DEVICE_TYPE = 0x0B;
    private static final int CHANNEL_POWER_TRANSMISSION_TYPE = 5;

    // The period and frequency values the channel will be configured to
    private static final int CHANNEL_POWER_PERIOD = 8182; // 1 Hz
    private static final int CHANNEL_POWER_FREQUENCY = 57;

    private static final String TAG = PowerChannelController.class.getSimpleName();

    int power = 0;
    int cadence = 0;

    public PowerChannelController(AntChannel antChannel) {
	    super(antChannel, POWER_SENSOR_ID, CHANNEL_POWER_DEVICE_TYPE, CHANNEL_POWER_TRANSMISSION_TYPE, CHANNEL_POWER_PERIOD, CHANNEL_POWER_FREQUENCY, ChannelType.BIDIRECTIONAL_MASTER, TAG, new ChannelEventCallback());
    }

    /**
     * Implements the Channel Event Handler Interface so that messages can be
     * received and channel death events can be handled.
     */
    public class ChannelEventCallback implements IAntChannelEventHandler {

        int cnt = 0;
        int eventCount = 0;
        int cumulativePower = 0;
        Timer carousalTimer = null;

        @Override
        public void onChannelDeath() {
            // Display channel death message when channel dies
            Log.e(TAG, "Channel Death");
        }

        @Override
        public void onReceiveMessage(MessageFromAntType messageType, AntMessageParcel antParcel) {
            Log.d(TAG, "Rx: " + antParcel);
            Log.d(TAG, "Message Type: " + messageType);
            byte[] payload = new byte[8];

            if(carousalTimer == null) {
               carousalTimer = new Timer(); // At this line a new Thread will be created
               carousalTimer.scheduleAtFixedRate(new TimerTask() {
                   @Override
                   public void run() {
                       Log.d(TAG, "Tx Unsollicited");
                       byte[] payload = new byte[8];
                       eventCount = (eventCount + 1) & 0xFF;
                       cumulativePower = (cumulativePower + power) & 0xFFFF;
                       payload[0] = (byte) 0x10;
                       payload[1] = (byte) eventCount;
                       payload[2] = (byte) 0xFF;
                       payload[3] = (byte) cadence;
                       payload[4] = (byte) ((cumulativePower) & 0xFF);
                       payload[5] = (byte) ((cumulativePower >> 8) & 0xFF);
                       payload[6] = (byte) ((power) & 0xFF);
                       payload[7] = (byte) ((power >> 8) & 0xFF);

                       if (mIsOpen) {
                           try {
                               // Setting the data to be broadcast on the next channel period
                               mAntChannel.setBroadcastData(payload);
                           } catch (RemoteException e) {
                               channelError(e);
                           }
                       }
                   }
               }, 0, 1000); // delay
           }

            // Switching on message type to handle different types of messages
            switch (messageType) {
                // If data message, construct from parcel and update channel data
                case BROADCAST_DATA:
                    // Rx Data
                    //updateData(new BroadcastDataMessage(antParcel).getPayload());
                    break;
                case ACKNOWLEDGED_DATA:
                    // Rx Data
                    //updateData(new AcknowledgedDataMessage(antParcel).getPayload());
                    payload = new AcknowledgedDataMessage(antParcel).getPayload();
                    Log.d(TAG, "AcknowledgedDataMessage: " + payload);

                    if ((payload[0] == 0) && (payload[1] == 1) && (payload[2] == (byte)0xAA)) {
                        payload[0] = (byte) 0x01;
                        payload[1] = (byte) 0xAC;
                        payload[2] = (byte) 0xFF;
                        payload[3] = (byte) 0xFF;
                        payload[4] = (byte) 0xFF;
                        payload[5] = (byte) 0xFF;
                        payload[6] = (byte) 0x00;
                        payload[7] = (byte) 0x00;
                        try {
                            // Setting the data to be broadcast on the next channel period
                            mAntChannel.setBroadcastData(payload);
                        } catch (RemoteException e) {
                            channelError(e);
                        }
                    } else {
                        eventCount = (eventCount + 1) & 0xFF;
                        cumulativePower = (cumulativePower + power) & 0xFFFF;
                        payload[0] = (byte) 0x10;
                        payload[1] = (byte) eventCount;
                        payload[2] = (byte) 0xFF;
                        payload[3] = (byte) cadence;
                        payload[4] = (byte) ((cumulativePower) & 0xFF);
                        payload[5] = (byte) ((cumulativePower >> 8) & 0xFF);
                        payload[6] = (byte) ((power) & 0xFF);
                        payload[7] = (byte) ((power >> 8) & 0xFF);

                        if (mIsOpen) {
                            try {
                                // Setting the data to be broadcast on the next channel period
                                mAntChannel.setBroadcastData(payload);
                            } catch (RemoteException e) {
                                channelError(e);
                            }
                        }
                    }
                    break;
                case CHANNEL_EVENT:
                    // Constructing channel event message from parcel
                    ChannelEventMessage eventMessage = new ChannelEventMessage(antParcel);
                    EventCode code = eventMessage.getEventCode();
                    Log.d(TAG, "Event Code: " + code);

                    // Switching on event code to handle the different types of channel events
                    switch (code) {
                        case TX:
                            cnt += 1;

                            if (cnt % 61 == 15) {
                                payload[0] = (byte) 0x50;
                                payload[1] = (byte) 0xFF;
                                payload[2] = (byte) 0xFF;
                                payload[3] = (byte) 0x01;
                                payload[4] = (byte) 0xFF;
                                payload[5] = (byte) 0x00;
                                payload[6] = (byte) 0x01;
                                payload[7] = (byte) 0x00;
                            } else if (cnt % 61 == 30) {
                                payload[0] = (byte) 0x51;
                                payload[1] = (byte) 0xFF;
                                payload[2] = (byte) 0xFF;
                                payload[3] = (byte) 0x01;
                                payload[4] = (byte) ((POWER_SENSOR_ID) & 0xFF);
                                payload[5] = (byte) ((POWER_SENSOR_ID >> 8) & 0xFF);
                                payload[6] = (byte) ((POWER_SENSOR_ID >> 16) & 0xFF);
                                payload[7] = (byte) ((POWER_SENSOR_ID >> 24) & 0xFF);
                            } else {
                                eventCount = (eventCount + 1) & 0xFF;
                                cumulativePower = (cumulativePower + power) & 0xFFFF;
                                payload[0] = (byte) 0x10;
                                payload[1] = (byte) eventCount;
                                payload[2] = (byte) 0xFF;
                                payload[3] = (byte) cadence;
                                payload[4] = (byte) ((cumulativePower) & 0xFF);
                                payload[5] = (byte) ((cumulativePower >> 8) & 0xFF);
                                payload[6] = (byte) ((power) & 0xFF);
                                payload[7] = (byte) ((power >> 8) & 0xFF);
                            }

                            if (mIsOpen) {
                                try {
                                    // Setting the data to be broadcast on the next channel period
                                    mAntChannel.setBroadcastData(payload);
                                } catch (RemoteException e) {
                                    channelError(e);
                                }
                            }
                            break;
                        case CHANNEL_COLLISION:
                            cnt += 1;
                            break;
                        case RX_SEARCH_TIMEOUT:
                            // TODO May want to keep searching
                            Log.e(TAG, "No Device Found");
                            break;
                        case CHANNEL_CLOSED:
                        case RX_FAIL:
                        case RX_FAIL_GO_TO_SEARCH:
                        case TRANSFER_RX_FAILED:
                        case TRANSFER_TX_COMPLETED:
                        case TRANSFER_TX_FAILED:
                        case TRANSFER_TX_START:
                        case UNKNOWN:
                            // TODO More complex communication will need to handle these events
                            break;
                    }
                    break;
                case ANT_VERSION:
                case BURST_TRANSFER_DATA:
                case CAPABILITIES:
                case CHANNEL_ID:
                case CHANNEL_RESPONSE:
                case CHANNEL_STATUS:
                case SERIAL_NUMBER:
                case OTHER:
                    // TODO More complex communication will need to handle these message types
                    break;
            }
        }
    }
}
