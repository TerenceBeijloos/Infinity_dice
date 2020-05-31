package com.avans.infinitydice.bluetooth;

import androidx.appcompat.app.AppCompatActivity;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCallback;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattService;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.text.method.ScrollingMovementMethod;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.avans.infinitydice.R;


import java.util.UUID;


public class ble_device_activity extends AppCompatActivity {

    // Declarations of user interfaces.
    private TextView mBle_connection_info;
    private Button mDisconnect_button;
    private Button mRead_data_button;
    private Button mWrite_data_button;
    private Button mClear_data_button;
    private Button mClear_text_button;

    // Declarations of Bluetooth classes.
    private BluetoothDevice mBle_device;
    private BluetoothGatt mBle_gatt;
    private BluetoothAdapter mBle_adapter;

    // Initialization of shared preferences.
    SharedPreferences mPrefs;

    // Declaration of strings and integers.
    String sStored_device_name;
    String sStored_device_address;
    int iStarted_from_scan;


    // Method called when activity is started.
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ble_device);

        // Adds button that on being clicked returns user to home screen.
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);

        // Finds ids of certain user interfaces.
        mBle_connection_info = findViewById(R.id.connection_info);
        mDisconnect_button = findViewById(R.id.disconnect_device);
        mRead_data_button = findViewById(R.id.read_data);
        mWrite_data_button = findViewById(R.id.write_data);
        mClear_data_button = findViewById(R.id.clear_data);
        mClear_text_button = findViewById(R.id.clear_text);

        // Interprets scrolling in the textview as scrolling keys.
        if(mBle_connection_info != null) {
            mBle_connection_info.setMovementMethod(new ScrollingMovementMethod());
        }

        // Stores default bluetooth adapter. Used to get remote device based on address.
        mBle_adapter = BluetoothAdapter.getDefaultAdapter();

        // Initialization of shared preferences.
        mPrefs = getSharedPreferences("device_data", 0);

        // Receives 'BluetoothDevice' from 'ble_scan_activity'.
        iStarted_from_scan = getIntent().getExtras().getInt("from_scan");

        // Stores intent value if a device was connected from the scan activity.
        if(iStarted_from_scan == 1) {
            mBle_device = getIntent().getExtras().getParcelable("ble_device");
        }

        if(mBle_device != null) {
            // Stores data of bluetooth device (permanently) if address equals address of dice.
            if(mBle_device.getName() != null && mBle_device.getAddress().equals("04:FF:00:00:FF:04")) {
                SharedPreferences.Editor mEdit_prefs = mPrefs.edit();
                mEdit_prefs.putString("key_name", mBle_device.getName());
                mEdit_prefs.putString("key_address", mBle_device.getAddress());
                mEdit_prefs.apply();
            }
        }

        // Stores the values from SharedPreferences in strings.
        sStored_device_name = mPrefs.getString("key_name", null);
        sStored_device_address = mPrefs.getString("key_address", null);

        // Makes a few buttons not visible when a device is not connected.
        mDisconnect_button.setVisibility(View.INVISIBLE);
        mRead_data_button.setVisibility(View.INVISIBLE);
        mWrite_data_button.setVisibility(View.INVISIBLE);
        mClear_data_button.setVisibility(View.INVISIBLE);
        mClear_text_button.setVisibility(View.INVISIBLE);

        // Starts connection with selected device.
        if(sStored_device_name != null && sStored_device_address.equals("04:FF:00:00:FF:04")) {
            connect_to_device(mBle_adapter.getRemoteDevice(sStored_device_address));
        } else {
            connect_to_device(mBle_device);
        }

        // Button click event to disconnect from device.
        mDisconnect_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                disconnect_device();
            }
        });
    }


    // Makes a connection with the given device and calls the gatt callback.
    private void connect_to_device(BluetoothDevice device) {
        if (mBle_gatt == null) {
            mBle_gatt = device.connectGatt(this, false, mBle_gatt_callback);
        }
    }


    // Disconnects with a connected device.
    private void disconnect_device() {
        SharedPreferences.Editor mEditor = mPrefs.edit();
        mEditor.remove("key_name");
        mEditor.remove("key_address");
        mEditor.apply();
        mBle_gatt.disconnect();

    }

    // Gatt callback that reports on connection changes, services, characteristics and more.
    private final BluetoothGattCallback mBle_gatt_callback = new BluetoothGattCallback() {
        @Override
        public void onConnectionStateChange(BluetoothGatt gatt, int status, int newState) {
            switch (newState) {
                case 0: // newState is 0 when a device is disconnected/not connected.
                    ble_device_activity.this.runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            mBle_connection_info.append("Verbinding verbroken.\n");
                            mDisconnect_button.setVisibility(View.INVISIBLE);
                            mRead_data_button.setVisibility(View.INVISIBLE);
                            mWrite_data_button.setVisibility(View.INVISIBLE);
                            mClear_data_button.setVisibility(View.INVISIBLE);
                            mClear_text_button.setVisibility(View.INVISIBLE);
                        }
                    });
                    break;

                case 2: // newState is 2 when a conneciton is made with a device.
                    ble_device_activity.this.runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            mBle_connection_info.append("Apparaat " + sStored_device_name + " verbonden.\n");
                            mDisconnect_button.setVisibility(View.VISIBLE);
                            mRead_data_button.setVisibility(View.VISIBLE);
                            mWrite_data_button.setVisibility(View.VISIBLE);
                            mClear_data_button.setVisibility(View.VISIBLE);
                            mClear_text_button.setVisibility(View.VISIBLE);
                        }
                    });

                    // Calls onServicesDiscovered method within this callback when the read data button is pressed.
                    mRead_data_button.setOnClickListener(new View.OnClickListener() {
                        @Override
                        public void onClick(View view) {
                            mBle_gatt.discoverServices();
                        }
                    });

                    // Calls write_gatt_char function within this class when the write data button is pressed.
                    mWrite_data_button.setOnClickListener(new View.OnClickListener() {
                        @Override
                        public void onClick(View view) {
                            write_gatt_char(mBle_gatt.getService(UUID.fromString("18424398-7CBC-11E9-8F9E-2A86E4085A59")));
                        }
                    });

                    // Calls clear_gatt_char function within this class when the clear data button is pressed.
                    mClear_data_button.setOnClickListener(new View.OnClickListener() {
                        @Override
                        public void onClick(View view) {
                            clear_gatt_char(mBle_gatt.getService(UUID.fromString("18424398-7CBC-11E9-8F9E-2A86E4085A59")));
                        }
                    });

                    // Clears all text information when clear text button is pressed.
                    mClear_text_button.setOnClickListener(new View.OnClickListener() {
                        @Override
                        public void onClick(View view) {
                            mBle_connection_info.setText("");
                        }
                    });
                    break;

                default:
                    ble_device_activity.this.runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            mBle_connection_info.append("Deze status is niet geldig.\n");
                            mDisconnect_button.setVisibility(View.INVISIBLE);
                            mRead_data_button.setVisibility(View.INVISIBLE);
                        }
                    });
                    break;
            }
        }

        // Is called when the read data button is pressed and services on the device are found.
        @Override
        public void onServicesDiscovered(BluetoothGatt gatt, int status) {
            ble_device_activity.this.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    mBle_connection_info.append("\nEr zijn services ontdekt.\n");
                }
            });

            // Calls function to report on services and characteristics.
            display_gatt_service_char(gatt.getService(UUID.fromString("18424398-7CBC-11E9-8F9E-2A86E4085A59")));
        }

        // Is called when the readCharacteristic function is executed and characteristic on the device are found.
        @Override
        public void onCharacteristicRead(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic, int status) {
            String sChar_read = characteristic.getStringValue(0);
            mBle_connection_info.append("Data gelezen: " + sChar_read + "\n");
        }

        // Is called when the writeCharacteristic function is executed.
        @Override
        public void onCharacteristicWrite(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic, int status) {
            String sChar_written = characteristic.getStringValue(0);
            mBle_connection_info.append("Data geschreven: " + sChar_written + "\n");
        }

};


    // This method displays data from services and characteristics that are found on the device.
    // Meant for debugging. Not necessarily used in end product.
    private void display_gatt_service_char(BluetoothGattService service) {

        // Checks if the given service is existing.
        if(service == null) {
            return;
        }

        // Displays the service uuid that is found.
        final String sUuid_service = service.getUuid().toString();
        ble_device_activity.this.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mBle_connection_info.append("\nService ontdekt: " + sUuid_service + "\n");
            }
        });

        // Displays the characteristic belonging to the service specified above.
        BluetoothGattCharacteristic characteristic = service.getCharacteristic(UUID.fromString("772AE377-B3D2-FF8E-1042-5481D1E03456"));
        mBle_gatt.readCharacteristic(characteristic); // onCharacteristicRead is called.
        final String sUuid_char = characteristic.getUuid().toString();
        ble_device_activity.this.runOnUiThread(new Runnable() {
            @Override
                public void run() {
                    mBle_connection_info.append("Characteristic ontdekt voor service: " + sUuid_char + "\n");
            }
        });

        // Makes scrolling through the textview possible.
        final int scrollAmount = mBle_connection_info.getLayout().getLineTop(mBle_connection_info.getLineCount()) - mBle_connection_info.getHeight();
        if(scrollAmount > 0) {
            mBle_connection_info.scrollTo(0, scrollAmount);
        }
    }


    // This method writes data to a specified characteristic.
    // This method is also meant for debugging. However, writing data is necessary in the end product.
    private void write_gatt_char(BluetoothGattService service) {
        BluetoothGattCharacteristic characteristic = service.getCharacteristic(UUID.fromString("772AE377-B3D2-FF8E-1042-5481D1E03456"));
        characteristic.setValue("020010020020010020");
        mBle_gatt.setCharacteristicNotification(characteristic, true);
        mBle_gatt.writeCharacteristic(characteristic); // onCharacteristicWrite is called.
    }

    // This method clears data of a specified characteristic.
    // This method is also meant for debugging. However, writing data is necessary in the end product.
    private void clear_gatt_char(BluetoothGattService service) {
        BluetoothGattCharacteristic characteristic = service.getCharacteristic(UUID.fromString("772AE377-B3D2-FF8E-1042-5481D1E03456"));
        characteristic.setValue("");
        mBle_gatt.setCharacteristicNotification(characteristic, true);
        mBle_gatt.writeCharacteristic(characteristic); // onCharacteristicWrite is called.
    }
}


