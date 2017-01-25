#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxGUI.h"
#include "ofxJSON.h"
#include "GateSF.h"
#include "MsaPhysics2D.h"

#define PORT 49161
#define NUM_MSG_STRINGS 30
#define NUM_GATE_DISPLAY 100
#define DEBUG 0

#define SENDHOST "localhost"
#define SENDPORT 49162
#define LASTARTNETADDR 32

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void setupGUI();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    ofTrueTypeFont		font;
    
    //amount of time until message fades
    float fadeTime;
    
    ofxOscReceiver	receiver;
    ofxOscSender sender;
    
    int				current_msg_string;
    string          msg_strings[NUM_MSG_STRINGS];
    float			timers[NUM_MSG_STRINGS];
    
    //OSC address split
    vector<string>  msgTokens;
    
    //list of artnetAddress - todo: load off xml file?
    //need to be ordered asc
    vector<string>  artnetAddrs;
    
    //const int lastArtNetAddr = 32;
    //Using arrays, since faster, easy to reference and number of gates is static.

    
    //last artnet address  + 1
//    std::array<GateSF,33> gates;
    std::map<int,GateSF> gates;
    
    //display history of setting for debuging debouncing
    //x = sensorValue, y = triggerConfidence
    vector<ofVec2f> gateDisplay;
    Sensor testSensor;
    
    // Stuff from positionEstimator
    World2D_ptr world;
    vector<User> users;
    
    // GUI
    ofxPanel gui;
    bool hideGui = false;
    ofParameter<float> timingThreshold;
    ofParameter<bool> drawGatesToggle;
    ofParameter<bool> drawUsersToggle;
};


