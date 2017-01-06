//
//  GateSF.h
//  SensorFuse
//
//  Created by Keith Lim on 6/1/17.
//
//
//  "GateSF", standing for Sensor Fuse Gate, represents the user interaction of the gates. Storing the cleaned data from "Sensor", it's used by the visualisation program to get the state of gates

#ifndef GateSF_h
#define GateSF_h

#include "Sensor.h"
#include "ofVec3d.h"


class GateSF{
    
public:
    
    //CONSTRUCTORS
    GateSF(){
    }
    
    //passed a sensor by reference, make the gate wrapper around it
    GateSF(Sensor sensor){
    }
    
    GateSF(string address){
        this->artnetAddress = address;
    }

    GateSF(string address, ofVec3d geo){
        this->artnetAddress = address;
        this->geolocation = geo;
    }
    
    //MEMBERS
    string artnetAddress;
    ofVec3d geolocation; //can be empty
    
    //confience of triggered?
    //    0= off | 1=probably triggered | 2=definately triggered
    int triggered;
    
    
    //METHODS
    
};

#endif /* GateSF_h */
