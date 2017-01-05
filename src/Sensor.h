//
//  Sensor.h
//  SensorFuse
//
//  Created by Keith Lim on 5/1/17.
//
//

#ifndef Sensor_h
#define Sensor_h


class Sensor{
public:
    //constuctors
    Sensor(){ //empty constructor for array init
    }
    
    Sensor(string address){
        this->artNetAddress = address;
    }
   
    
    //members
    string artNetAddress;
    
    //For now, pair both vectors by index.
    vector<long> timestamps; //in millisec
    vector<int> values; //0=false, 1=true
    
    
    //functions
    void add(long time, int value){
        timestamps.push_back(time);
        values.push_back(value);
    }
    
    string toString(){
        string str = "{";
        for(auto& i : values){
            str += ofToString(values[i]);
            str += " @ ";
            str += ofToString(timestamps[i]);
            str += " , ";
        }
        str += "}/n";
        return str;
    }


    
};

#endif /* Sensor_h */
