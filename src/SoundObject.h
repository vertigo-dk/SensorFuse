//
//  SoundObjects.h
//  WAVE_SensorFuse
//
//  Created by Frederik Tollund Juutilainen on 26/01/2017.
//
//

#ifndef SoundObjects_h
#define SoundObjects_h

#include "MsaPhysics2D.h"
#include <limits>

using namespace msa::physics;

class SoundObject {
public:
    SoundObject(World2D_ptr* world, ofVec2f initPos){
        this->world = world;
        this->particle = (*world)->makeParticle();
        this->particle->setRadius(0.3);
        this->particle->moveTo(initPos);
        this->particle->disableCollision();
        this->particle->addVelocity(ofVec2f(-0.1,0.0));
    }
    
    void draw(){
        ofSetColor(ofColor::crimson);
        ofDrawSphere(particle->getPosition(),particle->getRadius());
    }
    
    void createAttraction(Particle2D_ptr* attractor){
        Attraction2D_ptr attraction = (*world)->makeAttraction(this->particle, (*attractor), attractionStrength);
        // set constrains for attraction
        attraction->setMaxDistance(maxDistAttraction);
        attraction->setMinDistance(minDistAttraction);
        attractions.push_back(attraction);
    }
    
//private:
    Particle2D_ptr particle;
    float attractionStrength = 0.3;
    float distThreshold = 30.0;
    float maxDistAttraction = 20.0f;
    float minDistAttraction = 2.0f;
    World2D_ptr* world;
    vector<Attraction2D_ptr> attractions;
    std::string getPositionString(){
        stringstream stream;
        stream << fixed << setprecision(0) << this->particle->getPosition().x << ", " << this->particle->getPosition().y;
        return stream.str();
    }
};


#endif /* SoundObjects_h */
