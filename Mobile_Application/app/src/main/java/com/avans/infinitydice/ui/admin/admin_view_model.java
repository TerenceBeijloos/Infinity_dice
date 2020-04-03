package com.avans.infinitydice.ui.admin;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

public class admin_view_model extends ViewModel {

    private MutableLiveData<String> mText;

    public admin_view_model() {
        mText = new MutableLiveData<>();
        mText.setValue("This is the admin mode page.");
    }

    LiveData<String> getText() {
        return mText;
    }
}