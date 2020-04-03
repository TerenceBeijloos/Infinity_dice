package com.avans.infinitydice.ui.normal;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

public class normal_view_model extends ViewModel {

    private MutableLiveData<String> mText;

    public normal_view_model() {
        mText = new MutableLiveData<>();
        mText.setValue("This is the normal mode page.");
    }

    LiveData<String> getText() {
        return mText;
    }
}