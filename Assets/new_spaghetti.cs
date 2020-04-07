using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using System.IO;
using System.Text;
// using System.Windows.Forms;

public class new_spaghetti : MonoBehaviour
{
    public Vector3 v3Prev_orientation;
    public Vector3 v3First_orientation;
    public Vector3 v3Second_orientation;
    public Vector3 v3Result;
    public int iOn_top_number = 0;
    public Vector3 v3End_orientation;
    public bool bFirst_collision = false;
    public bool bAfter_first_collision = false;
    public int iWait_frames = 2;
    public int iFrame_counter = 0;
    public int iCollider_counter = 0;
    public int iNo_movement_counter = 0;
    public Vector3 v3Start_position;
    public Vector3 v3Restart_position;
    public Vector3 v3Start_angles;
    // Start is called before the first frame update
    void Start()
    {   
        StringBuilder sbOutput = new StringBuilder();
        string[] saOutput = new string[]{
            "xdif,","ydif,","zdif,","number","\n"
        };

        int iLength = saOutput.GetLength(0);
        for (int i = 0; i < iLength; i++)
        {
            sbOutput.Append(string.Join(",",saOutput[i]));
        }

        if(!File.Exists(FILE_PATH_NOT_ACCURATE_DATA)){
            File.WriteAllText(FILE_PATH_NOT_ACCURATE_DATA,sbOutput.ToString());
        }
        if(!File.Exists(FILE_PATH_ACCURATE_DATA)){
            File.WriteAllText(FILE_PATH_ACCURATE_DATA,sbOutput.ToString());
        }
        // this.renderer.material.color = iCollider_counter.blue;
        this.transform.eulerAngles = (new Vector3(0,0,0));
        v3Start_position = this.transform.position;
        v3Restart_position = this.transform.position;
        v3Restart_angles = this.transform.eulerAngles;
        v3Start_angles = this.transform.eulerAngles;
    }

    // Update is called once per frames
    void Update()
    {
        float fMargen = 0.01f;
        Vector3 v3Subtraction = v3Prev_orientation - this.transform.rotation.eulerAngles;
        v3Subtraction = new Vector3(((v3Subtraction.x*v3Subtraction.x)/2),((v3Subtraction.y*v3Subtraction.y)/2),((v3Subtraction.z*v3Subtraction.z)/2) );

        if (v3Subtraction.x < fMargen && v3Subtraction.y < fMargen && v3Subtraction.y < fMargen){
            iNo_movement_counter++;
        }else{
            iNo_movement_counter = 0; 
        }

        if (iNo_movement_counter > 100 && iCollider_counter > 0){
            v3End_orientation = this.transform.rotation.eulerAngles;
            iOn_top_number = face_up(v3End_orientation);
            strore_data();
            restart();
        }

        v3Prev_orientation = this.transform.rotation.eulerAngles;
    
        if(bFirst_collision){
            bFirst_collision = false;
            bAfter_first_collision = true;
            v3First_orientation = this.transform.rotation.eulerAngles;
        }else if(bAfter_first_collision){
            iFrame_counter++;
            if(iWait_frames == iFrame_counter){
                v3Second_orientation = this.transform.rotation.eulerAngles;
                v3Result = get_difference(v3First_orientation,v3Second_orientation);
                bAfter_first_collision = false;
            }
        }

    }

    void OnCollisionEnter(Collision collisionInfo)
    {

        if( collisionInfo.collider.name == "ground"){
            iCollider_counter++;
            if ( iCollider_counter == 1){
                bFirst_collision = true;
            }
        }
        
    }

    Vector3 to_180(Vector3 vector){
        vector.x = (vector.x > 180) ? vector.x - 360 : vector.x;
        vector.y = (vector.y > 180) ? vector.y - 360 : vector.y;
        vector.z = (vector.z > 180) ? vector.z - 360 : vector.z;
        return vector;
    } 
    Vector3 get_difference(Vector3 vector1, Vector3 vector2){
        vector1 = to_180(vector1);
        vector2 = to_180(vector2);
        Vector3 v3Result = (vector2-vector1);
        const int ITHRESH_HOLD = 180;

        if (v3Result.x > ITHRESH_HOLD)
        {
            v3Result.x = (360 - v3Result.x);
        }
        if (v3Result.y > ITHRESH_HOLD)
        {
            v3Result.y = (360 - v3Result.y);
        }
        if (v3Result.z > ITHRESH_HOLD)
        {
            v3Result.z = (360 - v3Result.z);
        }
        // Vector3 result = new Vector3(vector2.x - vector1.x,vector2.y - vector1.y,vector2.z - vector1.z
        return v3Result;
    }

    int face_up(Vector3 dice){
        GameObject six = GameObject.Find("six");
        GameObject five = GameObject.Find("five");
        GameObject four = GameObject.Find("four");
        GameObject three = GameObject.Find("three");
        GameObject two = GameObject.Find("two");
        GameObject one = GameObject.Find("one");
        const float fUp_value = 1.4f;

        if(six.transform.position.y > fUp_value){
            return 6;
        }
        if(five.transform.position.y > fUp_value){
            return 5;
        }
        if(four.transform.position.y > fUp_value){
            return 4;
        }
        if(three.transform.position.y > fUp_value){
            return 3;
        }
        if(two.transform.position.y > fUp_value){
            return 2;
        }
        if(one.transform.position.y > fUp_value){
            return 1;
        }

        return 0;
    }

    void restart(){
    v3Prev_orientation = new Vector3(0,0,0);
    v3First_orientation = new Vector3(0,0,0);
    v3Second_orientation = new Vector3(0,0,0);
    v3Result = new Vector3(0,0,0);
    v3End_orientation = new Vector3(0,0,0);

    // angle = 0;
    bFirst_collision = false;
    bAfter_first_collision = false;
    iWait_frames = 2;
    iFrame_counter = 0;
    iCollider_counter = 0;
    iNo_movement_counter = 0;

    if(iCurrent_angle_step < iAngle_steps){
        iCurrent_angle_step++;
        v3Restart_angles.x += (360/iAngle_steps);
        v3Restart_angles.y += (360/iAngle_steps);
        v3Restart_angles.z += (360/iAngle_steps);
    }else{
        iCurrent_angle_step = 1;
        bNext_position_up = true;
        v3Restart_angles = v3Start_angles;
    }

    if (bNext_position_up)
    {
        bNext_position_up = false;
        if ((v3Restart_position.y-v3Start_position.y) > (ISTEPS_UP*ISTEP_UP_SIZE))
        {
            bNext_position_forward = true;
            v3Restart_position.y = v3Start_position.y;
        }else{
            v3Restart_position.y += ISTEP_UP_SIZE;
        }
    }

    if(bNext_position_forward){
        bNext_position_forward = false;
        v3Restart_position.x += ISTEP_FORWARD_SIZE; 
        if(v3Restart_position.x > IMAX_FORWARD_POSITION){
            UnityEditor.EditorApplication.isPlaying = false;
        }
    }

    this.transform.position = v3Restart_position;
    this.transform.eulerAngles = v3Restart_angles;
    }

    private const int ISTEPS_UP = 2;
    private const int ISTEP_UP_SIZE = 3;
    private const int IMAX_FORWARD_POSITION = -5;
    private const int ISTEP_FORWARD_SIZE = 4;
    public const int iAngle_steps = 5;
    public int iCurrent_angle_step = 1;
    public bool bNext_position_up = false;
    public bool bNext_position_forward = false;
    public Vector3 v3Restart_angles;
    public int iAngle_update_counter = 0;

    private const string FILE_PATH_NOT_ACCURATE_DATA = @"F:\Avans\dice\not_accurate_data.csv";
    private const string FILE_PATH_ACCURATE_DATA = @"F:\Avans\dice\accurate_data.csv";

    void strore_accurate_data(){
        StringBuilder sbOutput = new StringBuilder();
        const UInt32 ACCURACY = 10000000;
        int[] iaOutput = new int[]{
            (int)(v3Result.x*ACCURACY),(int)(v3Result.y*ACCURACY),(int)(v3Result.z*ACCURACY),iOn_top_number
        };

        int iLength = iaOutput.GetLength(0);
        for ( int i = 0; i < iLength; i++){
            sbOutput.Append(string.Join(",", iaOutput[i])).Append(",");
        }

        File.AppendAllText(FILE_PATH_ACCURATE_DATA,sbOutput.ToString());
    }
    void strore_not_accurate_data(){
        StringBuilder sbOutput = new StringBuilder();
        const UInt32 ACCURACY = 10000;
        int[] iaOutput = new int[]{
            (int)(v3Result.x*ACCURACY),(int)(v3Result.y*ACCURACY),(int)(v3Result.z*ACCURACY),iOn_top_number
        };

        int iLength = iaOutput.GetLength(0);
        for ( int i = 0; i < iLength; i++){
            sbOutput.Append(string.Join(",", iaOutput[i])).Append(",");
        }

        File.AppendAllText(FILE_PATH_NOT_ACCURATE_DATA,sbOutput.ToString());
    }
    void strore_data(){
        strore_accurate_data();
        strore_not_accurate_data();
    }
}
