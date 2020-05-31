package com.avans.infinitydice.ui.normal;


import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;


import com.avans.infinitydice.R;

public class normal_fragment extends Fragment {


    // Method called when fragment/view is created.
    public View onCreateView(@NonNull LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View root = inflater.inflate(R.layout.fragment_normal, container, false);
        return root;
    }
}