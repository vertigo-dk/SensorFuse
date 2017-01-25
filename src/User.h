//
//  User.h
//  SensorFuse
//
//  Created by Frederik Tollund Juutilainen on 20/01/2017.
//
//

#ifndef User_h
#define User_h

#include "MsaPhysics2D.h"

using namespace msa::physics;

class User{
public:
    User(World2D_ptr* world, ofVec2f position, ofVec2f velocity, string userId){
        this->particle = (*world)->makeParticle();
        this->particle->moveTo(position);
        this->particle->addVelocity(velocity);
        this->particle->setRadius(0.1);
        this->particle->disableCollision();
        this->activationPosition = position;
        this->userId = userId;
        this->timeOfBirth = ofGetElapsedTimef();
    }
    
    void draw(){
        ofSetColor(ofColor::darkBlue);
        ofDrawCircle(particle->getPosition(), particle->getRadius());
        std::string info;
        info+=ofToString(userId);
        ofDrawBitmapString(info, particle->getPosition());
    }
    
    bool hasTravelledForTooLongNow(){
        return activationPosition.distance(this->particle->getPosition()) > maxDist;
    }
    
    void setVelocity(ofVec2f vel){
        this->particle->setVelocity(vel);
        activationPosition = this->particle->getPosition();
    }
    
    ofVec2f getPosition(){
        return this->particle->getPosition();
    }
    
    float getLifespan(){
        return timeOfBirth-ofGetElapsedTimef();
    }
    
    float getVelocity(){
        return particle->getVelocity().x;
    }
    
    bool isMovingRight(){
        return particle->getVelocity().x > 0;
    }
    
    string getId(){ return userId; }
private:
    Particle2D_ptr particle;
    ofVec2f activationPosition;
    float maxDist = 3.0;
    float timeOfBirth = 0;
    string userId = "ID NOT SET";
};

#endif /* User_h */
