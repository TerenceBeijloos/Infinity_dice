package com.avans.infinitydice.admin;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

import com.avans.infinitydice.R;

public class admin_activity extends AppCompatActivity {

    // Method called when activity is started.
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_admin);

        // Adds button that on being clicked returns user to home screen.
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);
    }
}
