package com.avans.infinitydice.ui.custom;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

public class CustomViewModel extends ViewModel {

    private MutableLiveData<String> mText;

    public CustomViewModel() {
        mText = new MutableLiveData<>();
        mText.setValue("This is the custom mode page.");
    }

    public LiveData<String> getText() {
        return mText;
    }
}