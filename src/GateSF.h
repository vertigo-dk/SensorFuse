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

#include "ofMain.h"
#include "Sensor.h"
#include "User.h"
#include "CountingVector.h"
#include <cstdlib>

#define TRIGGER_NO 0
#define TRIGGER_MAYBE 1
#define TRIGGER_YES 2

static int8_t userIdCount = 0;


class GateSF{
    
public:
    //CONSTRUCTORS
    GateSF(){
        
    }
    
    
    GateSF(string address, ofVec2f position, CountingVector* users, World2D_ptr* world, ofParameter<float>* timingThreshold, ofxOscSender* sender){
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
        if(sensor.getTrigger() > 0){
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
                float velocity =  (distanceToNeighbour/std::abs(n->lastActivationTime - this->lastActivationTime));
                cout << std::abs(n->lastActivationTime - this->lastActivationTime) << endl;

                ofVec2f velocityVector = ofVec2f((this->position.x-n->position.x),0).normalize() * velocity *1/ofGetFrameRate() ;
                createOrMoveUser(velocityVector);
                break;
            }
        }
        
        // SEND OSC
        ofxOscMessage m;
        m.setAddress("/Gate/");
        m.addInt32Arg(index);
        sender->sendMessage(m);
    }
    
    int oldTrigger = 0;
    
    void update(){
        // Check value of sensor and activate if necessary
        if(oldTrigger == 0 && sensor.getTrigger() > 0){
            activate();
        }
        oldTrigger = sensor.getTrigger();
    }
    
    void createOrMoveUser(ofVec2f velocityVector){
        User* closeUser;
        bool userClose = false;
        bool movingRight = velocityVector.x > 0;
        
        // check for existing user in this position
        for(auto& u : users->vector){
            int dist = std::abs(this->position.x-u.getPosition().x);
            if(dist < 16.0){
                // check for same direction
                if(movingRight == u.isMovingRight()){
                    userClose = true;
                    closeUser = &u; // set local pointer to close user
                    if(movingRight){
                        // if user is moving right, deactivate gate to left
                        neighbours.front()->lastActivationTime = -10;
                    }else{
                        // vice-versa
                        neighbours.back()->lastActivationTime = -10;
                    }
                    break; // out of loop
                }
            }
        }
        
        if(userClose){
            // if found: move existing
            closeUser->setVelocity(velocityVector);
        }else{
            // if not: create new
            User user = User(world,ofVec2f(this->position.x,this->position.y+width/2),velocityVector, ofToString(this->users->getCurrentCount()));
            this->users->add(user);
        }
    }
    
    bool isActivated(){
        return ofGetElapsedTimef() - lastActivationTime < *timingThreshold;
    }
    
    //MEMBERS
    string artnetAddress = "";
    Sensor sensor; //laser sensor on the gate.
    
    int triggerVal = 0;     //Current Trigger Value
    
    ofxOscSender* sender;
    
    // Stuff from PositionEstimator
    vector<GateSF*> neighbours;
    CountingVector* users;
    World2D_ptr* world;
    ofColor color = ofColor::darkGray;
    float lastActivationTime = -10;
    ofParameter<float>* timingThreshold;
    float distanceToNeighbour = 2.0;
    ofVec2f position;
    float width = 4.0;
    int index = 0;
};


#endif /* GateSF_h */
