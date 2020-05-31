package com.avans.infinitydice.ui.admin;


import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AlertDialog;
import androidx.fragment.app.Fragment;

import com.avans.infinitydice.R;
import com.avans.infinitydice.admin.admin_activity;

public class admin_fragment extends Fragment {

    // Declaration of password input text box.
    private EditText mCheck_password;


    // Method called when fragment/view is created.
    public View onCreateView(@NonNull LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View root = inflater.inflate(R.layout.fragment_admin, container, false);

        // Finds id of password input text box.
        mCheck_password = root.findViewById(R.id.password_input);

        // Checks the password when the enter key on the keyboard is hit.
        mCheck_password.setOnKeyListener(new View.OnKeyListener() {
            @Override
            public boolean onKey(View v, int keyCode, KeyEvent event) {
                if(keyCode == KeyEvent.KEYCODE_ENTER ){
                    check_password();

                    // Hides keyboard when lost focus on password input text box.
                    InputMethodManager mInput_method_manager = (InputMethodManager) getActivity().getSystemService(Activity.INPUT_METHOD_SERVICE);
                    mInput_method_manager.hideSoftInputFromWindow(getActivity().getCurrentFocus().getWindowToken(), 0);
                    mCheck_password.clearFocus();
                    return true;
                } else {
                    return false;
                }
            }
        });
        return root;
    }


    // Function that compares the password in the password edit text box with the correct password.
    public void check_password() {
        if(mCheck_password.getText().toString().equals("0000")) {
            Intent intent = new Intent(getActivity(), admin_activity.class);
            startActivity(intent); // Starts new admin activity if password is correct.
        }
        else {
            final AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
            builder.setMessage("Verkeerd wachtwoord...");
            builder.setPositiveButton(R.string.ok, null);
            builder.show(); // A message is sent to the user that the password is incorrect.
        }
    }
}