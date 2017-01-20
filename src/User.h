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
    User(World2D_ptr* world, ofVec2f position, ofVec2f velocity){
        this->particle = (*world)->makeParticle();
        this->particle->moveTo(position);
        this->particle->addVelocity(velocity);
        this->particle->setRadius(3);
        this->particle->disableCollision();
        this->startPosition = position;
    }
    
    void draw(){
        ofSetColor(ofColor::darkBlue);
        ofDrawCircle(particle->getPosition(), particle->getRadius());
        std::string info;
        info+=ofToString(particle->getPosition());
        ofDrawBitmapString(info, particle->getPosition());
    }
    
    bool hasTravelledForTooLongNow(){
        return startPosition.distance(this->particle->getPosition()) > maxDist;
    }
    
    Particle2D_ptr particle;
    ofVec2f startPosition;
    float maxDist = 30;
    
};


#endif /* User_h */
