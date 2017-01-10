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
#define DEBOUNCEENV 100
#define DEBOUNCELEGS 200
#define DEBOUNCESTAY 700


class Sensor{
public:
    //CONSTUCTORS
    Sensor(){ //empty constructor for array init
        values.push_back(0);
        timestamps.push_back(0);
        add(1,1);
        add(0,2);
    }
    
    Sensor(string address){
        this->artNetAddress = address;
        //initial dummy values
        values.push_back(0);
        timestamps.push_back(0);
        add(1,1);
        add(0,2);
    }
    
    //MEMEBERS
    string artNetAddress;
    
    //Most recent first
    vector<int> values; //0=beam broken, 1=beam unbroken
    vector<long> timestamps; //in millisec
    //    int currentValue; //current state of the laser, should be the same as values.last()
    
    
    //FUNCTIONS
    
    //values and timestamps vectors ordered by most recent first
    void add(int value , long time){
        values.insert(values.begin(), value);
        timestamps.insert(timestamps.begin(), time);
        
        //cout << "added:" << value << "@" << time <<"\n";
        //        currentValue = value;
        
        if(values.size() > 4 && timestamps.size() > 4){
            //we keep the last 4 values, delete the rest.
            values.resize(4);
            timestamps.resize(4);
        }
        
    }
    
    string toString(){
        vector<int>::iterator itVal = values.begin();
        vector<long>::iterator itTime = timestamps.begin();
        
        string str = "{";
        for( ; itTime != timestamps.end() ; ++itTime){
            str += ofToString(*itVal);
            ++itVal;
            str += " @ ";
            str += ofToString(*itTime);
            str += " , ";
        }
        str += "}\n";
        return str;
    }
    
    int currentValue(){
        return values[0];
    }
    
    
    
    //inspect timestamps and values with debounce thresholds and return true if a person is in a gate.
    
    // four cases. three results. two thresholds.
    //
    // return 0 = not triggered, 1 = maybe/mid triggered, 2 = triggered
    
    int isTriggered(){
        
        //only do the comparison if we have more than three values
        if(values.size() > 3 && timestamps.size() > 3){
            
            double curTime = ofGetElapsedTimeMillis();
            
            //beam broken
            if( values[0] == 0){
                //user standing still in gate
                if(curTime - timestamps[0] > DEBOUNCELEGS){
                    return 2;
                }
                //might be mid leg break or env
                if(curTime - timestamps[0] < DEBOUNCELEGS){
                    //looks like a leg
                    if(timestamps[0] - timestamps[1] > DEBOUNCELEGS){
                        return 1;
                        //looks like env
                    }else if(timestamps[0] - timestamps[1] > DEBOUNCEENV){
                        return 0;
                        //mabye ignore?
                    }else{
                        return 0;
                    }
                }
                //beam unbroken
            }else if(values[0] == 1){
                //do we have to check next values or can we presume alternating 0s and 1s?
                if(curTime - timestamps[0] < DEBOUNCESTAY){
                    return 2;
                }else{
                    //not sure...
                    return 1;
                }
            }else{
                cout << "invalid sensor value";  //shouldn't happen.
                abort();
            }
            
        }//end timestamps.size() > 2
        else{
            return 0; //not triggered
        }
    }
    
    bool isNoAddr(){
        return (artNetAddress == "");
    }
    
    bool isNoSense(){
        return (values.empty());
    }
    
    //
    bool triggerLogic(){
        
    }

    
};



#endif /* Sensor_h */
