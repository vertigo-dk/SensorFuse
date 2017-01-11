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
#define DEBOUNCELOWER 100
#define DEBOUNCEUPPER 200

#define DEBUG 0

#define TRIGGER_NO 0
#define TRIGGER_MAYBE 1
#define TRIGGER_YES 2
#define UNBROKEN 1
#define BROKEN 0

class Sensor{
public:
    //CONSTUCTORS
    Sensor(){ //empty constructor for array init
        values.push_back(1);
        timestamps.push_back(0);
        add(0,1);
        add(1,2);
    }
    
    Sensor(string address){
        this->artNetAddress = address;
        //initial dummy values
        values.push_back(1);
        timestamps.push_back(0);
        add(0,1);
        add(1,2);
    }
    
    //MEMEBERS
    string artNetAddress;
    
    //Most recent first
    vector<int> values; //0=beam broken, 1=beam unbroken
    vector<long> timestamps; //in millisec
    
    
    //FUNCTIONS
    
    //values and timestamps vectors ordered by most recent first
    //only add on a rising or falling edge...
    void add(int value , long time){
        if(values.size() > 0){
            if(values[0] == value){
                //i.e ignore timestamp if values match
                //only possible from keyboard input presumably.
                //otherwise, update timestamps, but this will re-trigger
                //timestamps[0] = time;
            }else{
                values.insert(values.begin(), value);
                timestamps.insert(timestamps.begin(), time);
            }
        }else{
            cout << "need to set initial dummy sensor value";
            abort();
        }
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
    
    
    
    //inspect timestamps and values with debounce thresholds and return true if a person is in a gate.
    
    // four cases. three results. two thresholds.
    //
    // return 0 = not triggered, 1 = maybe/mid triggered, 2 = triggered
    
    int getTrigger(){
        
        
        //only do the comparison if we have more than three values
        if(values.size() >= 4 && timestamps.size() >= 4){
            
            double curTime = ofGetElapsedTimeMillis();
            
            //beam unbroken
            if(values[0] == UNBROKEN){
                //environment and leg debounce
                if(curTime - timestamps[0] < DEBOUNCEUPPER){
                    //check for env
                    if(timestamps[0]-timestamps[1] < DEBOUNCELOWER){
                        //definitely ENV debounce
                        return TRIGGER_NO;
                    }else{
                        
                        if(timestamps[1] - timestamps[2] < DEBOUNCEUPPER &&
                           timestamps[2] - timestamps[3] < DEBOUNCEUPPER){
                            return TRIGGER_YES;
                        }else{
                            return TRIGGER_MAYBE; //or last known
                        }
                    }
                }
                else{ //curTime - timestamps[0] > DEBOUNCEUPPER)
                    return TRIGGER_NO;
                }
            }
            //beam broken
            else if(values[0] == BROKEN){
                //might be mid leg break or env
                if(curTime - timestamps[0] < DEBOUNCEUPPER){
                    //looks like ENV
                    if(curTime - timestamps[0] < DEBOUNCELOWER){
                       return TRIGGER_NO;
                    }else{
                        //might be a leg
                        return TRIGGER_MAYBE;
                    }
                }
                
                //user standing still in gate
                if(curTime - timestamps[0] > DEBOUNCEUPPER){
                    if(DEBUG) cout << "trigger: standing at gate\n";
                    return TRIGGER_YES;
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
    
    
    int getCurrentValue(){
        return values[0];
    }
    


    
};



#endif /* Sensor_h */
