package com.avans.infinitydice.ui.custom;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

public class custom_view_model extends ViewModel {

    private MutableLiveData<String> mText;

    public custom_view_model() {
        mText = new MutableLiveData<>();
        mText.setValue("This is the custom mode page.");
    }

    LiveData<String> getText() {
        return mText;
    }
}