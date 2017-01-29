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
#include "SoundObject.h"
#include "User.h"
#include <cstdlib>
#include <limits>

#define TRIGGER_NO 0
#define TRIGGER_MAYBE 1
#define TRIGGER_YES 2

static int8_t userIdCount = 0;


class GateSF{
    
public:
    //CONSTRUCTORS
    GateSF(){
        
    }
    
    
    GateSF(int gateId, string address, ofVec2f position, vector<User>* users, World2D_ptr* world, ofParameterGroup* parameterGroup, ofxOscSender* sender, vector<SoundObject>* soundObjects){
        this->artnetAddress = address;
        sensor = Sensor(address, parameterGroup);
        this->position = position;
        this->users = users;
        this->world = world;
        this->soundObjects = soundObjects;
        this->timingThreshold = &parameterGroup->get("timingThreshold").cast<float>();
        this->sender = sender;
        this->gateId = gateId;
    }
    
    
    void draw(){
        if(sensor.getTrigger() > 0){
            color = ofColor::darkRed;
        }else{
            color = ofColor::darkGray;
        }
        
        ofSetColor(color);
        ofSetLineWidth(3);
        ofDrawLine(ofVec2f(position.x,position.y-width/2),ofVec2f(position.x,position.y+width/2));
    }
    
    int oldTrigger = 0;
    
    void update(){
        // Check value of sensor and activate if necessary
        if(oldTrigger == 0 && sensor.getTrigger() > 0){
            activate();
            
            // SEND OSC gate 1
            ofxOscMessage m;
            m.setAddress("/Gate/"+ofToString(gateId));
            m.addInt32Arg(1);
            sender->sendMessage(m);
        }else if(oldTrigger == 2 && sensor.getTrigger() == 0){
            // SEND OSC gate 0
            ofxOscMessage m;
            m.setAddress("/Gate/"+ofToString(gateId));
            m.addInt32Arg(0);
            sender->sendMessage(m);
        }
        oldTrigger = sensor.getTrigger();
    }
    
    void activate(){

        User* closestUser;
        float distClosest = 2.1;
        bool userClose = false;
        
        // check if close user?
        for(auto& u : *users){
            int dist = std::abs(this->position.x-u.getPosition().x);
            if(dist < distClosest){

                userClose = true;
                closestUser = &u; // set local pointer to close user
                distClosest = dist; // to check if closer;
                
            }
        }
        if(userClose){
            // if found: move existing
            closestUser->activate(gateId);
            
        } else {
            // if not: create new
            // find user id
            
            int userId = 1;
            bool newID = false;
            if(users->size()>0){
                while(!newID){
                    bool isOccupied = false;
                    for(int i = 0; i<users->size(); i++){
                        if(ofToInt(users->at(i).getId()) == userId){
                            isOccupied = true;
                        }
                        if(!isOccupied){
                            newID = true;
                        }else{
                            userId++;
                            
                        }
                    }
                }
            }
            
            
            User user = User(world,ofVec2f(this->position.x,this->position.y), ofToString(userId), gateId);
            int closestDist = std::numeric_limits<int>::max();
            SoundObject* closestSoundObject;
            for(auto& s : *soundObjects){
                int dist = user.getPosition().distance(s.getPosition());
                if(dist < closestDist){
                    closestSoundObject = &s;
                    closestDist = dist;
                }
            }
            
            user.attractions.push_back(closestSoundObject->createAttraction(user.getAttractionParticle_ptr()));
            
            // create the new USER
            this->users->push_back(user);
        }
        

    }
    
    
    Sensor sensor; //laser sensor on the gate.

private:
    //MEMBERS
    string artnetAddress = "";
    
    int triggerVal = 0;     //Current Trigger Value
    
    ofxOscSender* sender;
    
    // Stuff from PositionEstimator
    vector<GateSF*> neighbours;
    vector<User>* users;
    World2D_ptr* world;
    vector<SoundObject>* soundObjects;
    ofColor color = ofColor::darkGray;
    float lastActivationTime = -10;
    ofParameter<float>* timingThreshold;
    float distanceToNeighbour = 2.0;
    ofVec2f position;
    float width = 4.0;
    int gateId;
};


#endif /* GateSF_h */
