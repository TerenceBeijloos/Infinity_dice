//using System.Collections;
//using System.Collections.Generic;
//using UnityEngine;

//public class test : MonoBehaviour
//{
//    public float fSpeed;
//    public float fAngular_speed;
//    public Rigidbody rb;

//    void Start()
//    {
//        rb = GetComponent<Rigidbody>();
//        rb.maxAngularVelocity = float.MaxValue;
//        rb.angularVelocity = new Vector3(0, 0, -30);
//    }

//    void FixedUpdate()
//    {
//        fSpeed = rb.velocity.magnitude;
//        fAngular_speed = rb.angularVelocity.magnitude;

//        //rb.AddTorque(Vector3.back);
        
//    }
//}
