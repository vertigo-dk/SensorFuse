#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxGUI.h"
#include "ofxJSON.h"
#include "GateSF.h"
#include "SoundObject.h"
#include "MsaPhysics2D.h"

#define PORT 49161
#define NUM_MSG_STRINGS 30
#define NUM_GATE_DISPLAY 100
#define DEBUG 0
#define NUMBER_OF_SOUNDOBJECTS 6

#define SENDHOST "localhost"
#define SENDPORT_VISUAL 49162
#define SENDPORT_AUDIO 49163
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
    ofxOscSender senderVisual;
    ofxOscSender senderAudio;
    
    int				current_msg_string;
    string          msg_strings[NUM_MSG_STRINGS];
    float			timers[NUM_MSG_STRINGS];
    
    //OSC address split
    vector<string>  msgTokens;
    
    //list of artnetAddress - todo: load off xml file?
    //need to be ordered asc
    vector<string>  artnetAddrs;
    
    std::map<int,GateSF> gates;
    
    //display history of setting for debuging debouncing
    //x = sensorValue, y = triggerConfidence
    vector<ofVec2f> gateDisplay;

    // Sound objects moving around
    vector<SoundObject> soundObjects;
    Particle2D_ptr idleParticle;

    // Stuff from positionEstimator
    World2D_ptr world;
    vector<User> users;
    
    // GUI
    ofParameterGroup guiParameters;
    ofxPanel gui;
    bool hideGui = false;
    ofParameter<int> timingThreshold;
    ofParameter<float> distanceThreshold;
    ofParameter<int> debounceLower;
    ofParameter<int> debounceHigher;
    ofParameter<bool> drawGatesToggle;
    ofParameter<bool> drawUsersToggle;
};


