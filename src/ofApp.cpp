//
//  SensorFuse
//
//  Created by Keith Lim on 3/1/17.
//
// Models the interactive state of Gates and Sensors and attempts to interperate where Users are, and return that as OSC GPS co-ordinates


#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    // listen on the given port
    cout << "listening for osc messages on port " << PORT << "\n";
    receiver.setup( PORT );
    
    current_msg_string = 0;
    
    ofBackground( 30, 30, 130 );
    
    //fadetime on the message display
    fadeTime = 20.0f;
    
    //list of ArtNetAddress
    
    //contruct Sensor Array
    //for(vector<string>::iterator it = artnetAddrs.begin(); it != artnetAddrs.end(); ++it){
    
    
    //might be nice to check max value of artnetAddrs and make the array of that one
    
    for(int i=0; i<artnetAddrs.size(); i++){
        //init Sensor objects in the artnetAddrs list
        Sensor sensor = Sensor(artnetAddrs[i]);
        int artnet = ofToInt(artnetAddrs[i]);
        sensors[artnet] = sensor;
    }
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    
    // hide old messages
    for ( int i=0; i<NUM_MSG_STRINGS; i++ )
    {
        if ( timers[i] < ofGetElapsedTimef() - fadeTime )
            msg_strings[i] = "";
    }
    
    // check for waiting messages
    while( receiver.hasWaitingMessages() )
    {
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage( &m );
    

        string msg_string;
        msg_string = m.getAddress();
        //ignore empty tokens = true
        msgTokens = ofSplitString(msg_string, "/", true);

        
        if(msgTokens[0] == "BeamBreak"){
            
            //get artnet address from OSC Message
            int artnet = ofToInt(msgTokens[1]);
            
            //get value of BeamBreak, 0=false, 1=true
            int value = m.getArgAsInt32(0);
            
            long timeTriggered = ofGetElapsedTimeMillis();
            
            //add trigger value and timestamp to sensor@artnetAddr
            sensors[artnet].add(timeTriggered,value);
            
            if(DEBUG){
                string tempstr = "obj:";
                tempstr += sensors[artnet].toString();
                cout << tempstr << "\n";
            }

            msg_string += " value=";
            msg_string += ofToString(value);
            msg_string += " time=";
            msg_string += ofToString(timeTriggered);
            
        }
        
        {
        /*
         msg_string += msgTokens[0]; //first token is empty because of leading "/"
        msg_string += "|t1=";
        msg_string += msgTokens[1]; //expect "BeamBreak"
        msg_string += "|t2=";
        msg_string += msgTokens[2]; //expect artnet address, eg "016"
        msg_string += "|arg0=";
        msg_string += ofToString( m.getArgAsInt32( 0 ) ); //expect 0|1
        */}
        
        // add to the list of strings to display
        msg_strings[current_msg_string] = msg_string;
        timers[current_msg_string] = ofGetElapsedTimef();
        current_msg_string = ( current_msg_string + 1 ) % NUM_MSG_STRINGS;
        // clear the next line
        msg_strings[current_msg_string] = "";
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetColor(ofColor::white);

    string buf;
    buf = "listening for osc messages on port" + ofToString( PORT );
    ofDrawBitmapString( buf, 10, 20 );
    
    
    for ( int i=0; i<NUM_MSG_STRINGS; i++ ){
        ofDrawBitmapString( msg_strings[i], 10, 40+15*i );
    }


    //quick and dirty "print" with sensor@16
    //ofDrawBitmapString( sensors[16].toString(), 10, 200 );
    
    
    //quick and dirty drawing of gate
    Sensor sensor16 = sensors[16];
    ofSetLineWidth(4);
    
    if(sensor16.currentValue == 0){
        ofSetColor(ofColor::red);
    }else{
        ofSetColor(ofColor::green);
    }
    ofDrawLine(200,50,200,100);
    
    if(sensor16.isTriggered()){
        ofSetColor(ofColor::red);
    }else{
        ofSetColor(ofColor::green);
    }
    ofDrawLine(200,100,200,150);
 
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if ( key =='a' || key == 'A' ){
        
    }
    
}

//--------------------------------------------------------------
void ofApp::drawSensor(Sensor sensor){
    

}



//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
