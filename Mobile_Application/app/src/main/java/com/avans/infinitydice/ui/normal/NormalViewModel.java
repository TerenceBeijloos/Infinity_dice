package com.avans.infinitydice.ui.normal;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

public class NormalViewModel extends ViewModel {

    private MutableLiveData<String> mText;

    public NormalViewModel() {
        mText = new MutableLiveData<>();
        mText.setValue("This is the normal mode page.");
    }

    public LiveData<String> getText() {
        return mText;
    }
}