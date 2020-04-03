package com.avans.infinitydice.ui.admin;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.annotation.Nullable;
import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.Observer;
import androidx.lifecycle.ViewModelProviders;

import com.avans.infinitydice.R;

public class admin_fragment extends Fragment {

    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        admin_view_model mAdmin_view_model = ViewModelProviders.of(this).get(admin_view_model.class);
        View root = inflater.inflate(R.layout.fragment_admin, container, false);
        final TextView textView = root.findViewById(R.id.text_admin);
        mAdmin_view_model.getText().observe(this, new Observer<String>() {
            @Override
            public void onChanged(@Nullable String s) {
                textView.setText(s);
            }
        });
        return root;
    }
}