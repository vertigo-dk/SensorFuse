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

#define TRIGGER_NO 0
#define TRIGGER_MAYBE 1
#define TRIGGER_YES 2


class GateSF{
    
public:
    
    //CONSTRUCTORS
    GateSF(){
    }
    
    GateSF(string address){
        this->artnetAddress = address;
        sensor = Sensor(address);
    }
    
    //MEMBERS
    string artnetAddress;
    ofVec3d geolocation; //can be empty
    int GateID; //not sure if needed... matches the index of container.
    
    Sensor sensor; //laser sensor on the gate.
    
    //Current Trigger Value
    int triggerVal;
    
    
    //METHODS
    void setSensor(Sensor sen){
        this->sensor = sen;
    }
    
    void update(){
        triggerVal = sensor.getTrigger();
    }
        
};

#endif /* GateSF_h */
