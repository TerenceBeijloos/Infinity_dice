package com.avans.infinitydice.bluetooth;

import android.app.Service;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCallback;
import android.content.Intent;
import android.os.IBinder;

public class ble_device_service extends Service {

    private BluetoothAdapter mBle_adapter;
    private BluetoothDevice mBle_device;
    private BluetoothGatt mBle_gatt;


    @Override
    public IBinder onBind(Intent intent) {
        // TODO: Return the communication channel to the service.
        throw new UnsupportedOperationException("Not yet implemented");
    }


    // Method that starts when startService is called.
    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {

        mBle_adapter = BluetoothAdapter.getDefaultAdapter();

        String data = intent.getStringExtra("ble_device_address");

        if(data != null) {
            mBle_device = mBle_adapter.getRemoteDevice(data);
            connect_to_device(mBle_device);
        }

        return START_STICKY;
    }


    // Makes a connection with the given device and calls the gatt callback.
    public void connect_to_device(BluetoothDevice device) {
        if (mBle_gatt == null) {
            mBle_gatt = device.connectGatt(this, false, mBle_gatt_callback);
        }
    }


    // Gatt callback that reports on connection changes, services, characteristics and more.
    private final BluetoothGattCallback mBle_gatt_callback = new BluetoothGattCallback() {
        @Override
        public void onConnectionStateChange(BluetoothGatt gatt, int status, int newState) {
            switch (newState) {
                case 0:
                    Intent mDisconnected = new Intent("disconnected");
                    sendBroadcast(mDisconnected);
                case 2:
                    Intent mConnected = new Intent("connected");
                    sendBroadcast(mConnected);
                default:
                    break;
            }
        }
    };
}
