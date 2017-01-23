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
        this->particle->setRadius(3);
        this->particle->disableCollision();
        this->activationPosition = position;
        this->userId = userId;
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
    
    void addVelocity(ofVec2f vel){
        this->particle->addVelocity(vel);
        activationPosition = this->particle->getPosition();
    }
    
    ofVec2f getPosition(){
        return this->particle->getPosition();
    }
    
    bool isMovingRight(){
        return particle->getVelocity().x > 0;
    }
private:
    Particle2D_ptr particle;
    ofVec2f activationPosition;
    float maxDist = 30;
    string userId = "ID NOT SET";
    
};


#endif /* User_h */
