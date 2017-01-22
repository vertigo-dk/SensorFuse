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
    
    GateSF(string address, ofVec2f position, vector<User>* users, World2D_ptr* world, ofParameter<float>* timingThreshold, ofxOscSender* sender){
        this->artnetAddress = address;
        sensor = Sensor(address);
        this->position = position;
        this->users = users;
        this->world = world;
        this->timingThreshold = timingThreshold;
        this->sender = sender;
    }
    
    void addNeighbours(std::vector<GateSF*> neighbours){
        this->neighbours = neighbours;
    }
    
    void draw(){
        if(ofGetElapsedTimef() - lastActivationTime < *timingThreshold){
            color = ofColor::darkRed;
        }else{
            color = ofColor::darkGray;
        }
        
        ofSetColor(color);
        ofSetLineWidth(3);
        ofDrawLine(position,ofVec2f(position.x,position.y+width));
    }
    
    void activate(){
        lastActivationTime = ofGetElapsedTimef();
                
        for(auto& n : neighbours){
            if(n->isActivated()){
                //create particle and add velociy
                float velocity =  std::abs(distanceToNeighbour/(n->lastActivationTime - this->lastActivationTime))/100;
                ofVec2f velocityVector = ofVec2f((this->position.x-n->position.x)*velocity,0);
                
                User user = User(world,ofVec2f(this->position.x,this->position.y+width/2),velocityVector);
                this->users->push_back(user);
                break;
            }
        }
        
        // SEND OSC
        ofxOscMessage m;
    }
    
    bool isActivated(){
        return ofGetElapsedTimef() - lastActivationTime < *timingThreshold;
    }
    

    //MEMBERS
    string artnetAddress = "";
    Sensor sensor; //laser sensor on the gate.
    
    int triggerVal = 0;     //Current Trigger Value
    
    ofxOscSender sender;
    
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
