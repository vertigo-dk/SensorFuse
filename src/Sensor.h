//
//  Sensor.h
//  SensorFuse
//
//  Created by Keith Lim on 5/1/17.
//
// Sensor has value of 1 if laser beam is unbroken. 0 if broken.

#ifndef Sensor_h
#define Sensor_h

//thresholds in millisecs
//bigger numbers for now for testing
#define DEBOUNCEENV 30
#define DEBOUNCELEGS 80
#define DEBOUNCESTAY 100


class Sensor{
public:
    //CONSTUCTORS
    Sensor(){ //empty constructor for array init
    }
    
    Sensor(string address){
        this->artNetAddress = address;
    }
    
    //MEMEBERS
    string artNetAddress;
    
    //For now, pair both vectors by index.
    vector<long> timestamps; //in millisec
    vector<int> values; //0=false, 1=true
    int currentValue; //current state of the laser, should be the same as values.last()
    
    
    //FUNCTIONS
    void add(long time, int value){
        timestamps.push_back(time);
        values.push_back(value);
        //cout << "added:" << value << "@" << time <<"\n";
        currentValue = value;
    }
    
    string toString(){
        vector<long>::iterator itTime = timestamps.begin();
        vector<int>::iterator itVal = values.begin();
        
        string str = "{";
        for( ; itTime != timestamps.end() ; ++itTime){
            str += ofToString(*itVal);
            ++itVal;
            str += " @ ";
            str += ofToString(*itTime);
            str += " , ";
        }
        str += "}/n";
        return str;
    }
    
    

    //inspect timestamps and values with debounce thresholds and return true if a person is in a gate.
    //also do cleanup on the fly to keep things efficent?
    
    bool isTriggered(){
        //starting from the end of the vector, get the next two values
        vector<long>::reverse_iterator ritTime = timestamps.rbegin();
        vector<long>::reverse_iterator ritTimeNext = timestamps.rbegin();
        ++ritTimeNext;
        vector<long>::reverse_iterator ritTimeNextNext = timestamps.rbegin();
        ++ritTimeNextNext;
        if(ritTimeNextNext != timestamps.rend()){
            ++ritTimeNextNext;
        }else{
            //should catch when we have less than three values
            return false;
        }
        
    
        vector<int>::reverse_iterator ritValue = values.rbegin();
        vector<int>::reverse_iterator ritValueNext = values.rbegin();
        ++ritValueNext;
        vector<int>::reverse_iterator ritValueNextNext = values.rbegin();
        ++ritValueNextNext;
        if(ritValueNextNext != values.rend()) ++ritValueNextNext;
        
        //only do the comparison if we have more than three values
        
        
        
        //this for loop is more for clean up than anything else...
        //for (; ritValueNextNext != values.rend(); ++ritValueNextNext){
        if(ritValue != values.rend() && ritValueNext != values.rend()){
            
            double curTime = ofGetElapsedTimeMillis();
            
            if( *ritValue == 0){
                //user standing still in gate
                if(curTime - *ritTime > DEBOUNCESTAY){
                    return true;
                }
                //might be mid debounce
                if(curTime - *ritTime < DEBOUNCEENV){
                    if(*ritTime - *ritTimeNext < DEBOUNCEENV){
                        //very env debouncy
                        return false;
                    }else{
                        //not sure, return true for completion / stop flickering
                        return true;
                    }
                }
            }else if(*ritValue == 1){
                //do we have to check next values or can we presume alternating 0s and 1s?
                if(curTime - *ritTime < DEBOUNCESTAY){
                    return true;
                }else{
                    //not sure...
                    return false;
                }
                
                
            }else{
                //shouldn't happen.
                cout << "invalid sensor value";
                abort();
            }
            
            
        }
    }
    
    bool isEmpty(){
        return (artNetAddress == "");
    }
    
};

#endif /* Sensor_h */
