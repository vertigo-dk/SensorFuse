#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

#define PORT 49161
#define NUM_MSG_STRINGS 20



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
    

    
    
    // Gates
    vector<ofVec3f> gates;
    
    
    //Gate properties - make them into objects?
    float oscX = 1.0;
    float oscY = 1.0;
    float oscZ = 1.0;
    float color = 1.0;
    
    
    string messageText = "hello";
    
    //Geo data
    ofVec3f gate1Geo = ofVec3f(-50,1,0);

    
  //  private:
        ofxOscReceiver	receiver;
    
        int				current_msg_string;
        string		msg_strings[NUM_MSG_STRINGS];
        float			timers[NUM_MSG_STRINGS];
    
        int				mouseX, mouseY;
        string			mouseButtonState;

    
};


