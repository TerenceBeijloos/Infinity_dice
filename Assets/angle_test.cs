//using System.Collections;
//using System.Collections.Generic;
//using UnityEngine;

//public class angle_test : MonoBehaviour
//{
//    public Vector3 v3Start_orientation;
//    public Vector3 v3Orientation        = new Vector3(0,0,0);
//    public Vector3 v3Prev_orientation   = new Vector3(0,0,0);

//    public float xRotation = 0;
//    public float yRotation = 0;
//    public float zRotation = 0;

//    public int iGround_collisions = 0;

//    public float fForce_mult = 5;
//    private Rigidbody rb;
//    // Start is called before the first frame update
//    void Start()
//    {
        
//    }

//    private void Awake()
//    {
//        rb = GetComponent<Rigidbody>();
//        rb.maxAngularVelocity = float.MaxValue;
//        rb.angularVelocity = new Vector3(-10, -10, -50);
//    }

//    // Update is called once per frame
//    void FixedUpdate()
//    {
//        if (iGround_collisions == 1)
//        {
//            v3Start_orientation = this.transform.eulerAngles;
//        }
//        if (iGround_collisions > 0)
//        {
//            v3Prev_orientation  = v3Orientation;
//            v3Orientation       = this.transform.eulerAngles;
//            if(v3Prev_orientation != new Vector3(0, 0, 0))
//            {
//                store();
//            }


//        }
       

//    }

//    void OnCollisionEnter(Collision collisionInfo)
//    {

//        if (collisionInfo.collider.name == "ground")
//        {
//            iGround_collisions++;

//        }

//    }

//    void store()
//    {
//        StringBuilder sbOutput = new StringBuilder();
//        int[] iaOutput = new int[]{
//            (int)v3First_orientation.x,(int)v3First_orientation.y,(int)v3First_orientation.z,(int)(v3Result.x*ACCURACY),(int)(v3Result.y*ACCURACY),(int)(v3Result.z*ACCURACY),iOn_top_number
//        };
//    }

//    Vector3 get_difference(Vector3 vector1, Vector3 vector2)
//    {
//        float x = vector1.x - vector2.x;


//        return new Vector3();
//    }

//}
