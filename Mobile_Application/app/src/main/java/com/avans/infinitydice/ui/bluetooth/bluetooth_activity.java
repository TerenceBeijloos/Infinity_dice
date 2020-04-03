package com.avans.infinitydice.ui.bluetooth;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.Manifest;
import android.app.AlertDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCallback;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattService;
import android.bluetooth.BluetoothManager;
import android.bluetooth.le.BluetoothLeScanner;
import android.bluetooth.le.ScanCallback;
import android.bluetooth.le.ScanResult;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.AsyncTask;
import android.os.Bundle;
import android.text.method.ScrollingMovementMethod;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.avans.infinitydice.R;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;


public class bluetooth_activity extends AppCompatActivity {

    private static final int PERMISSION_REQUEST_COARSE_LOCATION = 1;
    private final static int REQUEST_ENABLE_BT = 1;

    BluetoothManager mBle_manager;
    BluetoothAdapter mBle_adapter;
    BluetoothLeScanner mBle_scanner;
    BluetoothGatt mBle_gatt;

    Button startScanningButton;
    Button stopScanningButton;
    Button connectToDevice;
    Button disconnectDevice;

    EditText deviceIndexInput;
    TextView mBle_results_view;

    ArrayList<BluetoothDevice> devicesDiscovered = new ArrayList<>();

    int deviceIndex = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_bluetooth);

        getSupportActionBar().setDisplayHomeAsUpEnabled(true);

        mBle_results_view = findViewById(R.id.ble_scan_results);
        if(mBle_results_view != null) {
            mBle_results_view.setMovementMethod(new ScrollingMovementMethod());
        }
        deviceIndexInput = findViewById(R.id.edit_text_index);
        deviceIndexInput.setText("0");


        startScanningButton = findViewById(R.id.start_scan_button);
        if(startScanningButton != null) {
            startScanningButton.setOnClickListener(new View.OnClickListener() {
                public void onClick(View v) {
                    startScanning();
                }
            });
        }
        stopScanningButton = findViewById(R.id.stop_scan_button);
        if(stopScanningButton != null) {
            stopScanningButton.setOnClickListener(new View.OnClickListener() {
                public void onClick(View v) {
                    stopScanning();
                }
            });
            stopScanningButton.setVisibility(View.INVISIBLE);
        }

        connectToDevice = findViewById(R.id.connect_device_button);
        connectToDevice.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                connectToDeviceSelected();
            }
        });
        disconnectDevice = findViewById(R.id.disconnect_device_button);
        disconnectDevice.setVisibility(View.INVISIBLE);
        disconnectDevice.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                disconnectDeviceSelected();
            }
        });


        mBle_manager = (BluetoothManager) getSystemService(Context.BLUETOOTH_SERVICE);
        if (mBle_manager != null) {
            mBle_adapter = mBle_manager.getAdapter();
        }
        mBle_scanner = mBle_adapter.getBluetoothLeScanner();

        if(mBle_adapter != null && !mBle_adapter.isEnabled()) {
            Intent mEnable_intent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(mEnable_intent, REQUEST_ENABLE_BT);
        }

        // Requesting permission to location data right after starting the application.
        if(this.checkSelfPermission(Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
            final AlertDialog.Builder builder = new AlertDialog.Builder(this);
            builder.setTitle("This app needs location access");
            builder.setMessage("Please grant location access so this app can detect peripherals.");
            builder.setPositiveButton(R.string.ok, null);
            builder.setOnDismissListener(new DialogInterface.OnDismissListener() {
                @Override
                public void onDismiss(DialogInterface dialog) {
                    requestPermissions(new String[]{Manifest.permission.ACCESS_COARSE_LOCATION}, PERMISSION_REQUEST_COARSE_LOCATION);
                }
            });
            builder.show();
        }

    }


    private ScanCallback mBle_scan_callback = new ScanCallback() {
        @Override
        public void onScanResult(int callbackType, ScanResult result) {
            if(result.getDevice().getName() != null) {
                mBle_results_view.append("Index: " + deviceIndex + ", Device Name: " + result.getDevice().getName() + " rssi: " + result.getRssi() + "\n");
                devicesDiscovered.add(result.getDevice());
                deviceIndex++;
            }

            final int scrollAmount = mBle_results_view.getLayout().getLineTop(mBle_results_view.getLineCount()) - mBle_results_view.getHeight();
            if(scrollAmount > 0) {
                mBle_results_view.scrollTo(0, scrollAmount);
            }
        }
    };

    // Function to check if location is shared with the application
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        switch (requestCode) {
            case PERMISSION_REQUEST_COARSE_LOCATION: {
                if(grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    System.out.println("coarse location permission granted");
                } else {
                    final AlertDialog.Builder builder = new AlertDialog.Builder(this);
                    builder.setTitle("Functionality limited");
                    builder.setMessage("Since location access has not been granted, this app will not be able to discover beacons when in the background.");
                    builder.setPositiveButton(android.R.string.ok, null);
                    builder.setOnDismissListener(new DialogInterface.OnDismissListener() {
                        @Override
                        public void onDismiss(DialogInterface dialog) {
                        }
                    });
                    builder.show();
                }
            }
        }
    }

    public void startScanning() {
        deviceIndex = 0;
        devicesDiscovered.clear();
        mBle_results_view.setText("");
        mBle_results_view.append("Started scanning\n");
        startScanningButton.setVisibility(View.INVISIBLE);
        stopScanningButton.setVisibility(View.VISIBLE);
        AsyncTask.execute(new Runnable() {
            @Override
            public void run() {
                mBle_scanner.startScan(mBle_scan_callback);
            }
        });
    }

    public void stopScanning() {
        mBle_results_view.append("Stopped scanning\n");
        startScanningButton.setVisibility(View.VISIBLE);
        stopScanningButton.setVisibility(View.INVISIBLE);
        AsyncTask.execute(new Runnable() {
            @Override
            public void run() {
                mBle_scanner.stopScan(mBle_scan_callback);
            }
        });
    }

    private final BluetoothGattCallback mBle_gatt_callback = new BluetoothGattCallback() {
        @Override
        public void onCharacteristicChanged(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic) {
            bluetooth_activity.this.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    mBle_results_view.append("device read or wrote to\n");
                }
            });
        }

        @Override
        public void onConnectionStateChange(BluetoothGatt gatt, int status, int newState) {
            switch(newState) {
                case 0:
                    bluetooth_activity.this.runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            mBle_results_view.append("device disconnected\n");
                            connectToDevice.setVisibility(View.VISIBLE);
                            disconnectDevice.setVisibility(View.INVISIBLE);
                        }
                    });
                    break;
                case 2:
                    bluetooth_activity.this.runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            mBle_results_view.append("device connected\n");
                            connectToDevice.setVisibility(View.INVISIBLE);
                            disconnectDevice.setVisibility(View.VISIBLE);
                        }
                    });

                    mBle_gatt.discoverServices();
                    break;
                default:
                    bluetooth_activity.this.runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            mBle_results_view.append("we encountered an unknown state, uh oh\n");
                        }
                    });
                    break;
            }
        }

        @Override
        public void onServicesDiscovered(BluetoothGatt gatt, int status) {
            bluetooth_activity.this.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    mBle_results_view.append("device services have been discovered\n");
                }
            });
            displayGattServices(mBle_gatt.getServices());
        }

        @Override
        public void onCharacteristicRead(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic, int status) {
            if(status == BluetoothGatt.GATT_SUCCESS) {
                broadcastUpdate(characteristic);
            }
        }
    };

    private void broadcastUpdate(BluetoothGattCharacteristic characteristic) {
        System.out.println(characteristic.getUuid());
    }

    public void connectToDeviceSelected() {
        mBle_results_view.append("Trying to connect to device at index: " + deviceIndexInput.getText() + "\n");
        int deviceSelected = Integer.parseInt(deviceIndexInput.getText().toString());
        if(deviceSelected >= deviceIndex) {
            mBle_results_view.append("That device was not found.\n");
        } else {
            mBle_gatt = devicesDiscovered.get(deviceSelected).connectGatt(this, false, mBle_gatt_callback);
        }
    }

    public void disconnectDeviceSelected() {
        mBle_results_view.append("Disconnecting from device\n");
        mBle_gatt.disconnect();
    }

    private void displayGattServices(List<BluetoothGattService> gattServices) {
        if(gattServices == null) return;

        for(BluetoothGattService gattService : gattServices) {
            final String uuid = gattService.getUuid().toString();
            bluetooth_activity.this.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    mBle_results_view.append("Service discovered: "+uuid+"\n");
                }
            });
            new ArrayList<HashMap<String, String>>();
            List<BluetoothGattCharacteristic> gattCharacteristics = gattService.getCharacteristics();

            for(BluetoothGattCharacteristic gattCharacteristic : gattCharacteristics) {
                final String charUuid = gattCharacteristic.getUuid().toString();
                bluetooth_activity.this.runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        mBle_results_view.append("Characteristic discovered for service: "+charUuid+"\n");
                    }
                });
            }
        }
    }
}
