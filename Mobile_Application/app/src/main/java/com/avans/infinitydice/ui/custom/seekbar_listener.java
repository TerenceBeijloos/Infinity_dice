package com.avans.infinitydice.ui.custom;


import android.widget.SeekBar;


import com.avans.infinitydice.R;


public class seekbar_listener extends custom_fragment implements SeekBar.OnSeekBarChangeListener {


    private static final int MAX_PERC = 100;
    private int[] mProgress_array = {20,10,20,20,10,20};
    private String sPerc_data;
    private StringBuilder sDataString = new StringBuilder("020010020020010020");
    private int iRemaining_percentage;


    // Is called when the progress of one or multiple seekbars is changed by the user.
    @Override
    public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
        int iBar_index = find_bar_index(seekBar.getId());
        int iStored_progress = mProgress_array[iBar_index];

        custom_fragment.get_custom_instance().updateData(progress + "%", iBar_index);

        // Prevents the sum of the seekbars from going above 100%.
        if(progress > iStored_progress) {
            int iRem_perc = remaining();

            if(iRem_perc == 0) {
                seekBar.setProgress(iStored_progress);
                return;
            } else {
                mProgress_array[iBar_index] = Math.min(iStored_progress + iRem_perc, progress);
            }
        } else if(progress <= iStored_progress) {
            mProgress_array[iBar_index] = progress;
        }

        iRemaining_percentage = remaining();
    }


    // Function that checks the remaining percentage.
    // This is 100% minus the sum of the percentages of all sliders.
    private int remaining() {
        int remaining = MAX_PERC;

        for(int i = 0; i < 6; i++) {
            remaining -= mProgress_array[i];
        }

        if(remaining >= 100) {
            remaining = 100;
        } else if(remaining <= 0) {
            remaining = 0;
        }

        return remaining;
    }


    // Function that assigns the seekbars with an id.
    private int find_bar_index(int id) {
        switch (id) {
            case R.id.slider_chance_one:
                return 0;
            case R.id.slider_chance_two:
                return 1;
            case R.id.slider_chance_three:
                return 2;
            case R.id.slider_chance_four:
                return 3;
            case R.id.slider_chance_five:
                return 4;
            case R.id.slider_chance_six:
                return 5;
            default:
                throw new IllegalStateException(
                        "There should be a Seekbar with this id(" + id + ")!");
        }
    }


    // Is called when the user starts touching a slider.
    @Override
    public void onStartTrackingTouch(SeekBar seekBar) {
    }


    // Is called when the user stops touching a slider.
    // The code in this function builds the data string.
    @Override
    public void onStopTrackingTouch(SeekBar seekBar) {

        sDataString.setLength(0);

        for(int i = 0; i < 6; i++) {
            sPerc_data = String.valueOf(mProgress_array[i]);
            if(mProgress_array[i] >= 0 && mProgress_array[i] <= 9) {
                sDataString.append(sPerc_data.replace(sPerc_data, "00" + mProgress_array[i]));
            } else if(mProgress_array[i] >= 10 && mProgress_array[i] <= 99) {
                sDataString.append(sPerc_data.replace(sPerc_data, "0" + mProgress_array[i]));
            } else if(mProgress_array[i] == 100) {
                sDataString.append(mProgress_array[i]);
            }
        }
    }


    // Function that returns the complete data string that needs to be sent to the dice.
    public String get_data_string() {
        return this.sDataString.toString();
    }


    // Function that returns the remaining percentage from remaining().
    public int get_remaining_percentage() {
        return this.iRemaining_percentage;
    }
}
