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
    
    // setup gui
    setupGUI();
    
    // setup world for physics
    world = World2D::create();
    world->setGravity(ofVec2f(0,0));
    world->setWorldSize(ofVec2f(0,0), ofVec2f(ofGetWidth(),ofGetHeight()));
    world->enableCollision();
    world->setDrag(1);
    
    ofBackground( 30, 30, 130 );
    
    //fadetime on the message display
    fadeTime = 20.0f;
    
    //init Sensor objects in the artnetAddrs list with artnet name
    for(int i = 0; i < 40; i++){
        // Create gate
        ofVec2f position = ofVec2f((20.0*i)+20, ofGetHeight()/2);
        gates[i] = GateSF(ofToString(i),position,&users,&world, &timingThreshold, &sender);
        gates[i].index = i;        
    }
    
    // Add pointers to neighbours
    for(int i = 0; i < gates.size(); i++){
        std::vector<GateSF*> neighbours;
        // special cases for outer gates:
        if(i == 0){
            neighbours.push_back(&gates.at(1));
        }else if(i == gates.size()-1){
            neighbours.push_back(&gates.at(gates.size()-2));
        }else{
            // TWO NEIGHBOURS
            neighbours.push_back(&gates.at(i-1));
            neighbours.push_back(&gates.at(i+1));
        }
        gates.at(i).addNeighbours(neighbours);
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
    
    // MSA update for physics simulation
    world->update();
    
    // Delete dead users
    vector<User>::iterator it = users.begin();
    while(it != users.end()) {
        if((*it).hasTravelledForTooLongNow()) {
            it = users.erase(it);
        }
        else ++it;
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
            
            //add trigger value and timestamp to sensor@artnetAddr
            //            gates[artnet].sensor.add(value,timeTriggered);
            if(value == 1){
                gates[artnet].activate(); // activate gate
            }
            
            
            if(DEBUG){
                string tempstr = "obj:";
                tempstr += gates[artnet].sensor.toString();
                cout << tempstr << "\n";
                
            }
            msg_string += " value=";
            msg_string += ofToString(value);
            msg_string += " time=";
            msg_string += ofToString(ofGetElapsedTimeMillis());
        }
        
        // add to the list of strings to display
        msg_strings[current_msg_string] = msg_string;
        timers[current_msg_string] = ofGetElapsedTimef();
        current_msg_string = ( current_msg_string + 1 ) % NUM_MSG_STRINGS;
        // clear the next line
        msg_strings[current_msg_string] = "";
    }
    
    for(auto& u : users){
        // send user position
        ofxOscMessage m;
        m.setAddress("/User/");
        float normPos = u.particle->getPosition().x/(gates.size()*20.0);
        m.addFloatArg(normPos);
        
        sender.sendMessage(m);
        // Gate position for activated gate should be set in GateSF class.
        
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(ofColor::dimGray);
    ofPushMatrix();
    ofTranslate(0, 0);
    
    if(drawGatesToggle){
        for(auto& g : gates){
            g.second.draw();
        }
    }
    
    if(drawUsersToggle){
        for(auto& u : users){
            u.draw();
        }
    }
    
    ofPopMatrix();
    
    std::string info;
    info+="no. of users:\n";
    info+=ofToString(users.size());
    ofSetColor(ofColor::darkRed);
    ofDrawBitmapStringHighlight(info, 25+gui.getWidth(), 25);
    
    // GUI
    gui.draw();
    
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
void ofApp::setupGUI(){
    gui.setup();
    gui.add(timingThreshold.set("timing threshold", 2.5,0.5,5.0));
    gui.add(drawGatesToggle.set("draw gates", true));
    gui.add(drawUsersToggle.set("draw users", true));
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
