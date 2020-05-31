package com.avans.infinitydice.bluetooth;


import androidx.appcompat.app.AppCompatActivity;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothManager;
import android.bluetooth.le.BluetoothLeScanner;
import android.bluetooth.le.ScanCallback;
import android.bluetooth.le.ScanResult;
import android.content.Context;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.ProgressBar;
import android.widget.TextView;

import com.avans.infinitydice.R;

import java.util.ArrayList;
import java.util.HashSet;

public class ble_scan_activity extends AppCompatActivity {

    private static final int REQUEST_ENABLE_BT = 1;

    // Declarations of views, buttons and other user interfaces.
    private ListView mScan_results;
    private TextView mConnection_help;
    private Button mStart_scan_button;
    private Button mStop_scan_button;
    private ProgressBar mLoading_circle;

    // Declarations of Bluetooth classes.
    private BluetoothManager mBle_manager;
    private BluetoothAdapter mBle_adapter;
    private BluetoothLeScanner mBle_scanner;
    private BluetoothDevice mScan_device;

    // Declaration of variables that help with storing bluetooth device list data.
    private ArrayList<String> mDevice_list = new ArrayList<String>();
    private HashSet<String> mHash_set = new HashSet<String>();


    // Method called when activity is started.
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ble_scan);

        // Adds button that on being clicked returns user to home screen.
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);

        // Finds ids of certain user interfaces.
        mLoading_circle = findViewById(R.id.busy_scanning);
        mConnection_help = findViewById(R.id.connect_help);
        mStart_scan_button = findViewById(R.id.start_scan_button);
        mStop_scan_button = findViewById(R.id.stop_scan_button);
        mScan_results = findViewById(R.id.scan_results);

        // Makes the busy scanning progress bar and connection help text not visible.
        mLoading_circle.setVisibility(View.INVISIBLE);
        mConnection_help.setVisibility(View.INVISIBLE);

        // Starts new activity on list view item click and passes bluetooth device data to 'ble_device_activity'.
        mScan_results.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                Intent intent = new Intent(ble_scan_activity.this, ble_device_activity.class);
                intent.putExtra("ble_device", mScan_device);
                intent.putExtra("from_scan", 1);
                startActivity(intent);
            }
        });

        // Start scanning button. Starts event on clicking button.
        mStart_scan_button.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                start_scan();
            }
        });

        // Stop scanning button. Starts event on clicking button.
        mStop_scan_button.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                stop_scan();
            }
        });
        mStop_scan_button.setVisibility(View.INVISIBLE);

        // Store instance of BluetoothManager.
        mBle_manager = (BluetoothManager) getSystemService(Context.BLUETOOTH_SERVICE);

        // Get instance of BluetoothAdapter.
        mBle_adapter = mBle_manager.getAdapter();

        if(mBle_adapter != null && !mBle_adapter.isEnabled()) {
            Intent mEnable_intent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(mEnable_intent, REQUEST_ENABLE_BT);
        }

        mBle_scanner = mBle_adapter.getBluetoothLeScanner();

    }


    // Start scanning function.
    private void start_scan() {
        mLoading_circle.setVisibility(View.VISIBLE);
        mConnection_help.setVisibility(View.VISIBLE);
        mStart_scan_button.setVisibility(View.INVISIBLE);
        mStop_scan_button.setVisibility(View.VISIBLE);
        mScan_results.setAdapter(null);
        mDevice_list.clear();
        mHash_set.clear();
        AsyncTask.execute(new Runnable() {
            @Override
            public void run() {
                mBle_scanner.startScan(mBle_scan_callback);
            }
        });
    }


    // Stop scanning function.
    private void stop_scan() {
        mLoading_circle.setVisibility(View.INVISIBLE);
        mStop_scan_button.setVisibility(View.INVISIBLE);
        mStart_scan_button.setVisibility(View.VISIBLE);
        AsyncTask.execute(new Runnable() {
            @Override
            public void run() {
                mBle_scanner.stopScan(mBle_scan_callback);
            }
        });
    }


    // Scan callback to report scan results.
    private ScanCallback mBle_scan_callback = new ScanCallback() {
        @Override
        public void onScanResult(int callbackType, ScanResult result) {
            if(result.getDevice().getName() != null && !mHash_set.contains(result.getDevice().getAddress())) {
                mScan_device = result.getDevice(); // 'mScan_device' is sent to 'ble_device_activity'.
                mDevice_list.add(result.getDevice().getName() + "\n" + result.getDevice().getAddress());
                mHash_set.add(result.getDevice().getAddress()); // Prevents devices from showing multiple times in the list.
                mScan_results.setAdapter(new ArrayAdapter<String>(ble_scan_activity.this, R.layout.list_item, mDevice_list));
            }
        }
    };
}
