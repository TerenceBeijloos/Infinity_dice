package com.avans.infinitydice.ui.custom;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.SeekBar;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AlertDialog;
import androidx.fragment.app.Fragment;

import com.avans.infinitydice.R;

public class custom_fragment extends Fragment {

    // Declaration of class instance.
    private static custom_fragment mCustom_instance;

    // Method called when fragment/view is created.
    public View onCreateView(@NonNull LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View root = inflater.inflate(R.layout.fragment_custom, container, false);
        mCustom_instance = this;

        // Definition of seekbar_listener class instance.
        final seekbar_listener mInstance = new seekbar_listener();

        // Finds ids of seekbars/sliders.
        SeekBar mBar_one = root.findViewById(R.id.slider_chance_one);
        SeekBar mBar_two = root.findViewById(R.id.slider_chance_two);
        SeekBar mBar_three = root.findViewById(R.id.slider_chance_three);
        SeekBar mBar_four = root.findViewById(R.id.slider_chance_four);
        SeekBar mBar_five = root.findViewById(R.id.slider_chance_five);
        SeekBar mBar_six = root.findViewById(R.id.slider_chance_six);

        // Finds ids of other user interfaces.
        Button mSend_data_button = root.findViewById(R.id.send_data_button);

        // Sets the seekbar listener to receive notifications of changes to progress levels.
        mBar_one.setOnSeekBarChangeListener(mInstance);
        mBar_two.setOnSeekBarChangeListener(mInstance);
        mBar_three.setOnSeekBarChangeListener(mInstance);
        mBar_four.setOnSeekBarChangeListener(mInstance);
        mBar_five.setOnSeekBarChangeListener(mInstance);
        mBar_six.setOnSeekBarChangeListener(mInstance);

        // Initializes seekbar progress levels.
        mBar_one.setProgress(20);
        mBar_two.setProgress(10);
        mBar_three.setProgress(20);
        mBar_four.setProgress(20);
        mBar_five.setProgress(10);
        mBar_six.setProgress(20);

        // Displays data in a textview on clicking send data button.
        mSend_data_button.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                int iRem_perc = mInstance.get_remaining_percentage();
                if(iRem_perc == 0) {
                    show_data_string(mInstance.get_data_string());
                } else {
                    final AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
                    builder.setMessage("Het totale percentage moet gelijk zijn aan 100.");
                    builder.setPositiveButton(R.string.ok, null);
                    builder.show();
                }
            }
        });
        return root;
    }


    // This method updates the progress percentage of every seekbar.
    public void updateData(String perc, int id) {
        TextView mPercentage_one = this.getActivity().findViewById(R.id.slider_one_percentage);
        TextView mPercentage_two = this.getActivity().findViewById(R.id.slider_two_percentage);
        TextView mPercentage_three = this.getActivity().findViewById(R.id.slider_three_percentage);
        TextView mPercentage_four = this.getActivity().findViewById(R.id.slider_four_percentage);
        TextView mPercentage_five = this.getActivity().findViewById(R.id.slider_five_percentage);
        TextView mPercentage_six = this.getActivity().findViewById(R.id.slider_six_percentage);

        if(id == 0 && mPercentage_one != null) {
            mPercentage_one.setText(perc);
        } else if(id == 1 && mPercentage_two != null) {
            mPercentage_two.setText(perc);
        } else if(id == 2 && mPercentage_three != null) {
            mPercentage_three.setText(perc);
        } else if(id == 3 && mPercentage_four != null) {
            mPercentage_four.setText(perc);
        } else if(id == 4 && mPercentage_five != null) {
            mPercentage_five.setText(perc);
        } else if(id == 5 && mPercentage_six != null) {
            mPercentage_six.setText(perc);
        }
    }


    // This method displays the data string that needs to be sent to the dice.
    public void show_data_string(String data) {
        TextView mData_string = this.getActivity().findViewById(R.id.data_string);

        if(mData_string != null) {
            mData_string.setText(data);
        }
    }


    // This method returns an instance of this class.
    // Used in seekbar_listener.
    public static custom_fragment get_custom_instance() {
        return mCustom_instance;
    }
}

