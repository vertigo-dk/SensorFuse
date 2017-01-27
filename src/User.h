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
        this->particle->addVelocity(velocity*1/ofGetFrameRate());
        this->particle->setRadius(0.1);
        this->particle->disableCollision();
        this->activationPosition = position;
        this->userId = userId;
        this->timeOfBirth = ofGetElapsedTimef();
        this->velocity = velocity.x;
        
        // Attractor particle for sound objects
        this->attractorParticle = (*world)->makeParticle();
        this->attractorParticle->moveTo(particle->getPosition());
        this->attractorParticle->setRadius(0.1);
        this->attractorParticle->makeFixed();
    }
    
    void draw(){
        ofSetColor(ofColor::darkBlue);
        ofDrawCircle(particle->getPosition(), particle->getRadius());
        std::string info;
        info+=ofToString(userId);
        ofDrawBitmapString(info, particle->getPosition());
    }
    
    void update(){
        // set fixed particle to free particles position
        attractorParticle->moveTo(particle->getPosition());
    }
    
    bool hasTravelledForTooLongNow(){
        return activationPosition.distance(this->particle->getPosition()) > maxDist;
    }
    
    void setVelocity(ofVec2f vel){
        this->particle->setVelocity(vel*1/ofGetFrameRate());
        activationPosition = this->particle->getPosition();
        velocity = vel.x;
    }
    
    ofVec2f getPosition(){
        return this->particle->getPosition();
    }
    
    float getLifespan(){
        return ofGetElapsedTimef()-timeOfBirth;
    }
    
    float getVelocity(){
        return velocity;
    }
    
    bool isMovingRight(){
        return particle->getVelocity().x > 0;
    }
    
    Particle2D_ptr* getAttractionParticle_ptr(){
        return &this->attractorParticle;
    }
    
    string getId(){ return userId; }
private:
    Particle2D_ptr particle;
    Particle2D_ptr attractorParticle;

    ofVec2f activationPosition;
    float maxDist = 3.0;
    float timeOfBirth = 0;
    string userId = "ID NOT SET";
    float velocity = 0;
};

#endif /* User_h */
