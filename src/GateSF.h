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
    
    void update(){
        triggerVal = sensor.getTrigger();
    }
    
    //MEMBERS
    string artnetAddress;
    int GateID; //not sure if needed... matches the index of container.
    
    Sensor sensor; //laser sensor on the gate.
    
    //Current Trigger Value
    int triggerVal = 0;
};

#endif /* GateSF_h */
