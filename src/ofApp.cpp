#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    // listen on the given port
    cout << "listening for osc messages on port " << PORT << "\n";
    receiver.setup( PORT );
    
    current_msg_string = 0;
    
    ofBackground( 30, 30, 130 );
    
    fadeTime = 5.0f;
    
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
        msgTokens = ofSplitString(msg_string, "/");

        
        if(msgTokens[1] == "BeamBreak"){
            
            //get artnet address from OSC Message
            int artnet = ofToInt(msgTokens[2]);
            
            //get value of BeamBreak, 0=false, 1=true
            int value = m.getArgAsInt32(0);
            
            //add trigger value and timestamp to sensor@artnetAddr
            sensors[artnet].add(ofGetElapsedTimeMillis(),value);
            
            if(DEBUG){
                string tempstr = "obj:";
                tempstr += sensors[artnet].toString();
                cout << tempstr << "\n";
            }
        }
        
        {/*
        msg_string += "t0=";
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
    
    string buf;
    buf = "listening for osc messages on port" + ofToString( PORT );
    ofDrawBitmapString( buf, 10, 20 );
    
    // draw mouse state
//    buf = "mouse: " + ofToString( mouseX, 4) +  " " + ofToString( mouseY, 4 );
//    ofDrawBitmapString( buf, 430, 20 );
//    ofDrawBitmapString( mouseButtonState, 580, 20 );
    
    for ( int i=0; i<NUM_MSG_STRINGS; i++ ){
        ofDrawBitmapString( msg_strings[i], 10, 40+15*i );
    }


    //quick and dirty print
    
    //ofDrawBitmapString( sensors[16].toString(), 10, 200 );
    

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if ( key =='a' || key == 'A' ){
        
    }
    
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
