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
    sender.setup( SENDHOST, SENDPORT );
    
    current_msg_string = 0;
    
    ofBackground( 30, 30, 130 );
    
    //fadetime on the message display
    fadeTime = 20.0f;
    
    //init Sensor objects in the artnetAddrs list with artnet name
    for(auto addressName : artnetAddrs){
        gates[ofToInt(addressName)] = GateSF(addressName);
    }
    
    gateDisplay.resize(NUM_GATE_DISPLAY);
}

//--------------------------------------------------------------
void ofApp::update(){
 
    //Cleanup/Hide old display strings
    for ( int i=0; i<NUM_MSG_STRINGS; i++ ){
        if ( timers[i] < ofGetElapsedTimef() - fadeTime )
            msg_strings[i] = "";
    }

    
    //PARSE OSC
    while( receiver.hasWaitingMessages() ){
        ofxOscMessage m;
        receiver.getNextMessage( &m );
        
        string msg_string;
        msg_string = m.getAddress(); //expect "/BeamBreak/[artnetaddr] [0-1]"
        msgTokens = ofSplitString(msg_string, "/", true); //ignore (leading) empty token = true
        
        
        if(msgTokens[0] == "BeamBreak"){
            
            //convert artnet string for easy array access
            int artnet = ofToInt(msgTokens[1]);
            //get value of BeamBreak, 0=false, 1=true
            int value = m.getArgAsInt32(0);
            long timeTriggered = ofGetElapsedTimeMillis();

            //add trigger value and timestamp to sensor@artnetAddr
            gates[artnet].sensor.add(value,timeTriggered);
            
            if(DEBUG){
                string tempstr = "obj:";
                tempstr += gates[artnet].sensor.toString();
                cout << tempstr << "\n";
                
            }
            msg_string += " value=";
            msg_string += ofToString(value);
            msg_string += " time=";
            msg_string += ofToString(timeTriggered);
        }
        
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
    buf = "listening for osc messages on port" + ofToString( PORT ) + "\n";
    buf += "press and release A to break/unbreak, send osc message [/BeamBreak/016 0-1]\n";
    buf += "press D to break: send osc message [/BeamBreak/016 0]\n";
    buf += "press F to unbreak: send osc message [/BeamBreak/016 1]\n";
    ofDrawBitmapString( buf, 10, 20 );
    //    ofDrawBitmapString( "press A to send osc message [/BeamBreak/016 0-1]", 10, 80 );
    
    for ( int i=0; i<NUM_MSG_STRINGS; i++ ){
        ofDrawBitmapString( msg_strings[i], 10, 90+15*i );
    }

    string str;
    
    //key for values
    ofPushMatrix();
    ofTranslate(300, 100);
    str = "BROKEN = 0";
    ofSetColor(ofColor::green);
    ofDrawBitmapString( str, 0, 0 );
    str = "UNBROKEN = 1";
    ofSetColor(ofColor::red);
    ofDrawBitmapString( str, 0, 10 );
    ofPopMatrix();
    
    //key for triggers
    ofPushMatrix();
    ofTranslate(500, 100);
    str = "YES = 2";
    ofSetColor(ofColor::green);
    ofDrawBitmapString( str, 0, 0 );
    str = "MAYBE = 1";
    ofSetColor(ofColor::yellow);
    ofDrawBitmapString( str, 0, 10 );
    str = "NO = 0";
    ofSetColor(ofColor::red);
    ofDrawBitmapString( str, 0, 20 );
    ofPopMatrix();
    
    //get test sensor
    testSensor = gates[16].sensor;
    
    //populate gateDisplay to show gate display over time
    if(!testSensor.isNoSense() ){

        //change refresh rate?
        //bool refresh = (ofGetElapsedTimeMillis() % 500 == 0);

        float curVal = testSensor.getCurrentValue();
        float trigVal = testSensor.getTrigger();
        //cout << "curVal:" << curVal << " trigval:" << trigVal << "\n";
        
        gateDisplay.insert( gateDisplay.begin(), ofVec2f(curVal , trigVal) );
    }
    
    if (gateDisplay.size() > NUM_GATE_DISPLAY){
        gateDisplay.resize(NUM_GATE_DISPLAY);
    }

    ofPushMatrix();
    ofTranslate(400, 70);
    ofSetLineWidth(4);
    int curDisplay = 0;
    int yOffset = 5;
    
    if(gateDisplay.size() != 0 && curDisplay < NUM_GATE_DISPLAY){
        
        for(int i = 0 ; i < gateDisplay.size() ; i++){
            
            ofVec2f curGateVal = gateDisplay[i];
            //beam is broken = 0,
            if(curGateVal.x == 0.0f){
                ofSetColor(ofColor::green);
            }else{
                ofSetColor(ofColor::red);
            }
            ofDrawLine(0,0+curDisplay*yOffset,50,0+curDisplay*yOffset);
            
            //TriggerVal 2 = triggered, 1 = maybe triggered, 0 = not triggered
            if(curGateVal.y == 2.0f){
                ofSetColor(ofColor::green);
            }else if(curGateVal.y == 1.0f){
                ofSetColor(ofColor::yellow);
            }else {
                ofSetColor(ofColor::red);
            }
            ofDrawLine(50,0+curDisplay*yOffset,100,0+curDisplay*yOffset);
            curDisplay++;
        }
    }
    ofPopMatrix();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if ( key =='a' || key == 'A' ){
        ofxOscMessage m;
        m.setAddress( "/BeamBreak/016" );
        m.addIntArg( 0 );
        sender.sendMessage( m );
    }

    
    if ( key =='d' || key == 'D' ){
        ofxOscMessage m;
        m.setAddress( "/BeamBreak/016" );
        m.addIntArg( 0 );
        sender.sendMessage( m );
    }

    if ( key =='f' || key == 'F' ){
        ofxOscMessage m;
        m.setAddress( "/BeamBreak/016" );
        m.addIntArg( 1 );
        sender.sendMessage( m );
    }
    
    
    
}



//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if ( key =='a' || key == 'A' ){
        ofxOscMessage m;
        m.setAddress( "/BeamBreak/016" );
        m.addIntArg( 1 );
        sender.sendMessage( m );
    }
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
