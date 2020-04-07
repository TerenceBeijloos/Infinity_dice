using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class new_spaghetti : MonoBehaviour
{
    public Vector3 v3Prev_orientation;
    public Vector3 v3First_orientation;
    public Vector3 v3Second_orientation;
    public Vector3 v3Result;
    public int angle = 0;
    public Vector3 v3End_orientation;
    public bool bFirst_collision = false;
    public bool bAfter_first_collision = false;
    public int iWait_frames = 2;
    public int iFrame_counter = 0;
    public int iCollider_counter = 0;
    public int iNo_movement_counter = 0;

    public Vector3 v3Start_position;
    // Start is called before the first frame update
    void Start()
    {
        // this.renderer.material.color = iCollider_counter.blue;
        this.transform.eulerAngles = (new Vector3(0,0,90));
        v3Start_position = this.transform.position;
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

        if (iNo_movement_counter > 60 && iCollider_counter > 1){
            v3End_orientation = this.transform.rotation.eulerAngles;
            angle = face_up(v3End_orientation);
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

    void strore_data(){

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

        // Vector3 result = new Vector3(vector2.x - vector1.x,vector2.y - vector1.y,vector2.z - vector1.z
        return (vector2-vector1);
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

    angle = 0;
    bFirst_collision = false;
    bAfter_first_collision = false;
    iWait_frames = 2;
    iFrame_counter = 0;
    iCollider_counter = 0;
    iNo_movement_counter = 0;

    this.transform.position = v3Start_position;
    }
}
