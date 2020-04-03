package com.avans.infinitydice.ui.settings;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

import com.avans.infinitydice.R;

public class settings_activity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_settings);

        getSupportActionBar().setDisplayHomeAsUpEnabled(true);
    }
}
