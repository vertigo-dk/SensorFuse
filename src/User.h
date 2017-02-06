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
    User(World2D_ptr* world, ofVec2f position, string userId, int lastActivationGateId){
        this->particle = (*world)->makeParticle();
        this->particle->moveTo(position);
        this->particle->setRadius(0.1);
        this->particle->disableCollision();
        this->activationPosition = position.x;
        this->userId = userId;
        this->timeOfBirth = ofGetElapsedTimef();
        
        // Attractor particle for sound objects
        this->attractorParticle = (*world)->makeParticle();
        this->attractorParticle->moveTo(particle->getPosition());
        this->attractorParticle->setRadius(0.1);
        this->attractorParticle->makeFixed();
        
        this->lastActivationTime = ofGetElapsedTimef();
        this->lastActivationGateId = lastActivationGateId;
    }
    
    ~User(){
        
    }
    
    void killParticles(){
        // Doing this in the constructor gave issues
        attractions.clear();
        if(hasAttractedSoundObject){this->attractedSoundObject->setOccupied(false);}
        this->attractorParticle->kill();
        this->particle->kill();
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
        return abs(this->particle->getPosition().x-activationPosition) > 5. ;// abs(velocity*3.);
    }
    
    float getTimeSinceLastActivation(){
        return ofGetElapsedTimef()-lastActivationTime;
    }
    
    void setVelocity(ofVec2f vel){
        this->particle->setVelocity(vel*1/ofGetFrameRate());
        activationPosition = this->particle->getPosition().x;
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
    
    void setLastActivationGateId(int id){ lastActivationGateId = id; }
    void setLastActivationTime(float lastActivationTime){ this->lastActivationTime = lastActivationTime; }
    
    bool isMovingRight(){
        return particle->getVelocity().x > 0;
    }
    
    Particle2D_ptr* getAttractionParticle_ptr(){
        return &this->attractorParticle;
    }
    
    void setAttractedSoundObject(SoundObject* attractedSoundObject){
        this->hasAttractedSoundObject = true;
        this->attractedSoundObject = attractedSoundObject;
        this->attractedSoundObject->setOccupied(true);
    }
    
    void activate(int gateId){
        
        if(gateId == lastActivationGateId){
            this->lastActivationTime = ofGetElapsedTimef();
            return;
        }
        
        float distanceTraveled = (gateId - lastActivationGateId)*2.;
        
        float distanceToTravel = 0;
        if(distanceTraveled > 0){
            distanceToTravel= ((gateId+1)*2.0+4.) - this->particle->getPosition().x;
        }else{
            distanceToTravel = ((gateId-1.)*2.0+4.) - this->particle->getPosition().x;
        }
        float deltaTime = ofGetElapsedTimef()-lastActivationTime;
        
        velocity = (float)distanceTraveled/deltaTime;
        velocity *= abs(distanceToTravel) / 2.;
                
        this->particle->setVelocity(ofVec2f(velocity, 0.)/ofGetFrameRate());
        
        this->activationPosition = this->particle->getPosition().x;
        
        this->lastActivationTime = ofGetElapsedTimef();
        this->lastActivationGateId = gateId;
    }
    
    string getId(){ return userId; }
    vector<Attraction2D_ptr> attractions;
    
private:
    Particle2D_ptr particle;
    Particle2D_ptr attractorParticle;
    
    SoundObject* attractedSoundObject;
    bool hasAttractedSoundObject = false;
    
    float activationPosition;
    float maxDist = 4.5;
    float timeOfBirth = 0;
    string userId = "ID NOT SET";
    float velocity = 0;
    int lastActivationGateId;
    float lastActivationTime;
};

#endif /* User_h */
