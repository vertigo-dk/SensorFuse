//
//  CountingVector.h
//  WAVE_SensorFuse
//
//  Created by Frederik Tollund Juutilainen on 24/01/2017.
//
//

#ifndef CountingVector_h
#define CountingVector_h

class CountingVector{
public:
    vector<User> vector;
    int counter = 0;
    
    void push_back(User user){
        this->vector.push_back(user);
        counter++;
    }
    
    int getCurrentCount(){
        if(counter > 128){
            counter = 0;
        }
        return counter;
    }
    
};


#endif /* CountingVector_h */
