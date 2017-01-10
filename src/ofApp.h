#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "Sensor.h"
#include "GateSF.h"

#define PORT 49161
#define NUM_MSG_STRINGS 30
#define NUM_GATE_DISPLAY 100
#define DEBUG 1

#define SENDHOST "localhost"
#define SENDPORT 49161
#define LASTARTNETADDR 32

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
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
    
    //todo:
    void drawSensor(Sensor sensor);
    
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
    vector<string>  artnetAddrs{"000", "016", "032"};
    
    
    //const int lastArtNetAddr = 32;
    //Using arrays, since faster, easy to reference and number of gates is static.
    //Sensor sensors[LASTARTNETADDR];
    //GateSF gates[LASTARTNETADDR];
    //Sensor sensors[32];
    //GateSF gates[32];
    
    //last artnet address  + 1
    std::array<GateSF, 33> gates;
    std::array<Sensor,33> sensors;
    
    //display history of setting for debuging debouncing
    
    //x = sensorValue, y = triggerConfidence
    vector<ofVec2f> gateDisplay;
    Sensor testSensor;
    
    
    
    
};


