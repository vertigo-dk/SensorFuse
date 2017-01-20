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
#include "User.h"

#define TRIGGER_NO 0
#define TRIGGER_MAYBE 1
#define TRIGGER_YES 2


class GateSF{
    
public:
    //CONSTRUCTORS
    GateSF(){
        
    }
    
    GateSF(string address, ofVec2f position, vector<User>* users, World2D_ptr* world, ofParameter<float>* timingThreshold){
        this->artnetAddress = address;
        sensor = Sensor(address);
        this->position = position;
        this->users = users;
        this->world = world;
        this->timingThreshold = timingThreshold;
    }
    
    void addNeighbours(std::vector<GateSF*> neighbours){
        this->neighbours = neighbours;
    }
    
    
    void update(){
        triggerVal = sensor.getTrigger();
    }
    
    //MEMBERS
    string artnetAddress;
    Sensor sensor; //laser sensor on the gate.
    
    int triggerVal = 0;     //Current Trigger Value
    
    // Stuff from PositionEstimator
    vector<GateSF*> neighbours;
    vector<User>* users;
    World2D_ptr* world;
    ofColor color = ofColor::darkGray;
    float lastActivationTime = 0;
    ofParameter<float>* timingThreshold;
    float distanceToNeighbour = 2.0;
    ofVec2f position;
    float width = 100;
    int index = 0;
};

#endif /* GateSF_h */
