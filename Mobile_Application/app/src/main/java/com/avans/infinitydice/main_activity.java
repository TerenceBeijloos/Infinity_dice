package com.avans.infinitydice;

import android.Manifest;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.TextView;
import android.widget.Toast;

import com.avans.infinitydice.bluetooth.ble_device_activity;
import com.avans.infinitydice.bluetooth.ble_device_service;
import com.avans.infinitydice.bluetooth.ble_scan_activity;
import com.google.android.material.bottomnavigation.BottomNavigationView;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.navigation.NavController;
import androidx.navigation.Navigation;
import androidx.navigation.ui.AppBarConfiguration;
import androidx.navigation.ui.NavigationUI;



public class main_activity extends AppCompatActivity {

    private static final int PERMISSION_REQUEST_COARSE_LOCATION = 1;

    // Declaration of stored device data strings.
    String sStored_device_name;
    String sStored_device_address;

    // Declaration of user interfaces.
    TextView mBle_device_info;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Finds id belonging to the textview.
        mBle_device_info = findViewById(R.id.ble_device_info);

        // Checks BLE support on the mobile device.
        if(!getPackageManager().hasSystemFeature(PackageManager.FEATURE_BLUETOOTH_LE)) {
            Toast.makeText(this, R.string.ble_no_support, Toast.LENGTH_LONG).show();
            finish();
        }

        // Asks the user for location permission so the app can detect peripherals.
        if(this.checkSelfPermission(Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
            final AlertDialog.Builder builder = new AlertDialog.Builder(this);
            builder.setTitle("Deze app heeft toegang tot uw locatie nodig.");
            builder.setMessage("Geef a.u.b. toegang tot uw locatie, zodat de app externe apparaten kan detecteren.");
            builder.setPositiveButton(R.string.ok, null);
            builder.setOnDismissListener(new DialogInterface.OnDismissListener() {
                @Override
                public void onDismiss(DialogInterface dialog) {
                    requestPermissions(new String[]{Manifest.permission.ACCESS_COARSE_LOCATION}, PERMISSION_REQUEST_COARSE_LOCATION);
                }
            });
            builder.show();

        }

        // Setup of the bottom menu bar.
        BottomNavigationView navView = findViewById(R.id.nav_view);
        // Passing each menu ID as a set of Ids because each
        // menu should be considered as top level destinations.
        AppBarConfiguration appBarConfiguration = new AppBarConfiguration.Builder(
                R.id.navigation_custom, R.id.navigation_normal, R.id.navigation_admin)
                .build();
        NavController navController = Navigation.findNavController(this, R.id.nav_host_fragment);
        NavigationUI.setupActionBarWithNavController(this, navController, appBarConfiguration);
        NavigationUI.setupWithNavController(navView, navController);


        // SharedPrefences is used to store data (permanently) when the app closes.
        SharedPreferences mPrefs = getSharedPreferences("device_data", 0);

        // Stores the values from SharedPreferences in strings.
        sStored_device_name = mPrefs.getString("key_name", null);
        sStored_device_address = mPrefs.getString("key_address", null);

        // Checks whether the bluetooth service should start, depending on whether the dice is already known to the app.
        if(sStored_device_address != null) {
            Intent mService_intent = new Intent(main_activity.this, ble_device_service.class);
            mService_intent.putExtra("ble_device_address", sStored_device_address);
            startService(mService_intent);
        } else {
            mBle_device_info.setText("");
            mBle_device_info.append(getResources().getString(R.string.not_connected));
        }

        // Registers the broadcast receiver to receive messages from 'ble_device_service'.
        IntentFilter filter = new IntentFilter("connected");
        filter.addAction("disconnected");
        this.registerReceiver(mBroadcast_receiver, filter);
    }


    // Sends information to the user to let the user know if a device is connected or disconnected.
    private BroadcastReceiver mBroadcast_receiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if(action.equals("connected")) {
                mBle_device_info.setText("");
                mBle_device_info.append("Verbonden met: " + sStored_device_name + "(" + sStored_device_address + ")");
            } else if(action.equals("disconnected")) {
                mBle_device_info.setText("");
                mBle_device_info.append("Niet verbonden.");
            }
        }
    };


    // Specifies the menu options in the action bar.
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.top_nav_menu, menu);
        return super.onCreateOptionsMenu(menu);
    }


    // Starts the bluetooth scanning activity on clicking the bluetooth icon menu button.
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch(item.getItemId()) {
            case R.id.bluetooth_button: // Starts scanning activity or device activity depending on whether a device is known.
                if(sStored_device_address != null) {
                    Intent mActivity_intent = new Intent(main_activity.this, ble_device_activity.class);
                    mActivity_intent.putExtra("from_scan", 0);
                    startActivity(mActivity_intent);
                } else {
                    startActivity(new Intent(main_activity.this, ble_scan_activity.class));
                }
                return true;
            default:
                return super.onOptionsItemSelected(item);
        }
    }


    // Returns information on whether the device was granted permission to location data.
    @Override
    public void onRequestPermissionsResult(int requestCode, String permissions[], int[] grantResults) {
        switch(requestCode) {
            case PERMISSION_REQUEST_COARSE_LOCATION: {
                if (grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    System.out.println("coarse location permission granted");
                } else {
                    final AlertDialog.Builder builder = new AlertDialog.Builder(this);
                    builder.setTitle("Functionality limited");
                    builder.setMessage("Since location access has not been granted, this app will not be able to discover beacons when in the background.");
                    builder.setPositiveButton(R.string.ok, null);
                    builder.setOnDismissListener(new DialogInterface.OnDismissListener() {
                        @Override
                        public void onDismiss(DialogInterface dialog) {
                        }
                    });
                    builder.show();
                }
                return;
            }
        }
    }
}
