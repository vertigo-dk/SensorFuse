//
//  SensorFuse
//
//  Created by Keith Lim on 3/1/17.
//  small and reasonable edits by Frederik Juutilainen on several dates in january
//
// Models the interactive state of Gates and Sensors and attempts to interperate where Users are, and return that as OSC GPS co-ordinates

#include "ofApp.h"

#define NUM_GATES 40
#define SPACING_ENDS 4
#define SPACING_SIDE 5
#define INSTALLATION_LENGTH (NUM_GATES-1)*2
#define INSTALLATION_WIDTH 4

//--------------------------------------------------------------
void ofApp::setup(){
    // listen on the given port
    cout << "listening for osc messages on port " << PORT << "\n";
    receiver.setup( PORT );
    senderVisual.setup( SENDHOST, SENDPORT_VISUAL );
    senderAudio.setup( SENDHOST, SENDPORT_AUDIO );
    
    current_msg_string = 0;
    
    // setup gui
    setupGUI();
    
    // setup world for physics
    world = World2D::create();
    world->setGravity(ofVec2f(0,0));
    world->setWorldSize(ofVec2f(0,0), ofVec2f(INSTALLATION_LENGTH+SPACING_ENDS*2, INSTALLATION_WIDTH+SPACING_SIDE*2));
    world->enableCollision();
    world->setDrag(1);
    
    //fadetime on the message display
    fadeTime = 20.0f;
    
    // Artnet
    ofxJSONElement json;
    json.open("artnetAddresses.json");
    
    for(int i = 0; i < json.size(); i++){
        artnetAddrs.push_back(json[i].asString());
    }
    
    int i = 0;
    
    //init Sensor objects in the artnetAddrs list with artnet name
    for(auto& address : artnetAddrs){
        // Create gate
        ofVec2f position = ofVec2f((2.0*i)+SPACING_ENDS, INSTALLATION_WIDTH/2+SPACING_SIDE);
        gates[ofToInt(address)] = GateSF(i, address,position,&users,&world, &guiParameters, &senderVisual, &soundObjects);
        i++;
    }
    
    // Add soundobjects
    for(int i = 0; i < NUMBER_OF_SOUNDOBJECTS; i++){
        ofVec2f initPos = ofVec2f(ofRandom(0,INSTALLATION_LENGTH+SPACING_ENDS*2), ofRandom(0, INSTALLATION_WIDTH+SPACING_SIDE*2));
        soundObjects.push_back(SoundObject(&world, initPos));
    }
    
    // Create small amount of repulsion to other sound objects
    for (int i = 0; i<soundObjects.size(); i++) {
        for (int j = i+1; j<soundObjects.size(); j++) {
            soundObjects.at(i).repelOtherSoundObject(&soundObjects.at(j));
        }
    }
    
    gateDisplay.resize(NUM_GATE_DISPLAY);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    // MSA update for physics simulation
    world->update();
    
    // Keep Some speed in SoundObjects
    // estimate some kind of energy measure -> average velocity on x
    float avgVelocity;
    for(auto & soundObject : soundObjects){
        avgVelocity += soundObject.getVelocity().length();
    }
    avgVelocity /= soundObjects.size();
    
    float targetAvgVelocity = 1.5;
    float pFactor = 0.05; // how fast does it change
    
    float deltaVelocity = targetAvgVelocity-avgVelocity;
    float changeFactor = 0;
    
    if(deltaVelocity<0) changeFactor = 1-(abs(deltaVelocity)*pFactor);
    if(deltaVelocity>0) changeFactor = 1+(abs(deltaVelocity)*pFactor);
    
    // Update all users
    for(auto & soundObject : soundObjects){
        soundObject.update(changeFactor, targetAvgVelocity);
    }
    
    // Delete dead users
    vector<User>::iterator it = users.begin();
    while(it != users.end()) {
        (*it).update();
        if((*it).hasTravelledForTooLongNow() || (*it).getTimeSinceLastActivation() > 4.) {
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
            long timeTriggered = ofGetElapsedTimeMillis();
            
            //add trigger value and timestamp to sensor@artnetAddr
            // check if entry exists in map
            map<int,GateSF>::iterator i = gates.find(artnet);
            if (i == gates.end()) { /* Not found */ }
            else { gates[artnet].sensor.add(value,timeTriggered); }
            
            
            
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
    
    for(auto& g : gates){
        g.second.update();
    }
    
    for(int i = 0; i < soundObjects.size(); i++){
        ofxOscMessage m;
        m.setAddress("/soundObject/" + ofToString(i));
        ofVec2f pos = soundObjects.at(i).getPosition();
        m.addFloatArg(pos.x - SPACING_ENDS);
        m.addFloatArg(pos.y - (SPACING_SIDE+INSTALLATION_WIDTH/2));
        senderVisual.sendMessage(m);
        senderAudio.sendMessage(m);
    }
    
    for(auto& u : users){
        // send user position
        ofxOscMessage m;
        m.setAddress("/User/"+u.getId());
        m.addFloatArg(u.getPosition().x - SPACING_ENDS); // position
        m.addFloatArg(u.getLifespan());
        m.addFloatArg(u.getVelocity());
        senderVisual.sendMessage(m);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(ofColor::dimGray);
    ofPushMatrix();
    {
        ofTranslate(ofGetWidth()/2, gui.getHeight()+10+(ofGetHeight()-gui.getHeight()-10)/2);
        ofScale(10,10);
        ofTranslate(-INSTALLATION_LENGTH/2-SPACING_ENDS, -INSTALLATION_WIDTH/2-SPACING_SIDE);
        
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
        
        ofNoFill();
        ofSetColor(ofColor::antiqueWhite);
        ofDrawRectangle(0, 0, INSTALLATION_LENGTH+SPACING_ENDS*2, INSTALLATION_WIDTH+SPACING_SIDE*2); // Draw borders of world;
        
        for(auto& s : soundObjects){
            s.draw();
        }
    }
    ofPopMatrix();
    
    std::string info;
    info+="no. of users:\n";
    info+=ofToString(users.size());
    ofSetColor(ofColor::darkRed);
    ofDrawBitmapStringHighlight(info, 25+gui.getWidth(), 25);
    
    // GUI
    if(!hideGui){
        gui.draw();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'g' || key == 'G'){
        hideGui = !hideGui;
    }
    
    // Activate gate sensors based on key
    if(key-48 > 0 && key-48 < gates.size()){
        long timeTriggered = ofGetElapsedTimeMillis();
        string address = artnetAddrs.at(key-48);
        gates[ofToInt(address)].sensor.add(0, timeTriggered);
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    // Deactivate gate sensors based on key
    if(key-48 > 0 && key-48 < gates.size()){
        long timeTriggered = ofGetElapsedTimeMillis();
        string address = artnetAddrs.at(key-48);
        gates[ofToInt(address)].sensor.add(1, timeTriggered);
    }
}

//--------------------------------------------------------------
void ofApp::setupGUI(){
    guiParameters.setName("GUI");
    guiParameters.add(timingThreshold.set("timing threshold (s)", 2.5,0.5,5.0));
    guiParameters.add(distanceThreshold.set("dist threshold (m)", 1.9,0.5,4.5));
    guiParameters.add(debounceLower.set("debounce lower (ms)",100,20,400));
    guiParameters.add(debounceHigher.set("debounce higher (ms)",200,40,700));
    guiParameters.add(drawGatesToggle.set("draw gates", true));
    guiParameters.add(drawUsersToggle.set("draw users", true));
    gui.setup(guiParameters);
    gui.loadFromFile("settings.xml");
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
