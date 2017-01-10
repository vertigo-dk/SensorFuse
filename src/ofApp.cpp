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
        sensors[ofToInt(addressName)] = Sensor(addressName);
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
            sensors[artnet].add(value,timeTriggered);
            
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
    buf += "press and release A to send osc message [/BeamBreak/016 0-1]\n";
    buf += "press D to send osc message [/BeamBreak/016 0]\n";
    buf += "press F to send osc message [/BeamBreak/016 1]\n";
    ofDrawBitmapString( buf, 10, 20 );
    //    ofDrawBitmapString( "press A to send osc message [/BeamBreak/016 0-1]", 10, 80 );
    
    
    for ( int i=0; i<NUM_MSG_STRINGS; i++ ){
        ofDrawBitmapString( msg_strings[i], 10, 90+15*i );
    }
    
    
    //quick and dirty "print" with sensor@16

    //for quick and dirty drawing of gate
    testSensor = sensors[16];
    
    //populate gateDisplay to show gate display over time
    
    //refresh rate of
    //bool refresh = (ofGetElapsedTimeMillis() % 500 == 0);
    
    if(!testSensor.isNoSense() ){
        float curVal = testSensor.currentValue();
        float trigVal = testSensor.isTriggered();
        //cout << "curVal:" << curVal << " trigval:" << trigVal << "\n";
        
        //insert to begining of vector
        gateDisplay.insert( gateDisplay.begin(), ofVec2f(curVal , trigVal) );
        //gateDisplay.push_back(ofVec2f(1,1));
    }
    
    if (gateDisplay.size() > NUM_GATE_DISPLAY){
        gateDisplay.resize(NUM_GATE_DISPLAY);
    }

    ofPushMatrix();
    ofTranslate(400, 70);
    ofSetLineWidth(4);
    int curDisplay = 0;
    int yOffset = 5;
    
    //cout << "gateDisplay size:" << gateDisplay.size() << "\n";
    
    
    if(gateDisplay.size() != 0 && curDisplay < NUM_GATE_DISPLAY){
        
        for(int i = 0 ; i < gateDisplay.size() ; i++){
            
            ofVec2f curGateVal = gateDisplay[i];
            //beam is broken = 0, but for display we'll reverse the logic
            if(curGateVal.x == 1.0f){
                ofSetColor(ofColor::green);
            }else{
                ofSetColor(ofColor::red);
            }
            ofDrawLine(0,0+curDisplay*yOffset,50,0+curDisplay*yOffset);
            
            //get Trigger val
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

//todo
//--------------------------------------------------------------
void ofApp::drawSensor(Sensor sensor){
    
    
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
