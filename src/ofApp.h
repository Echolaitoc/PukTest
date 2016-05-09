#pragma once

#include "ofMain.h"
#include "ofxTuio.h"
#include "ofxFlowTools.h"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

using namespace flowTools;

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
private:
    void keyPressed(int key);
    
    void tuioAdded(ofxTuioCursor & tuioCursor);
    void tuioRemoved(ofxTuioCursor & tuioCursor);
    void tuioUpdated(ofxTuioCursor & tuioCursor);
    
    ofxTuioClient tuioClient;
    ftFluidSimulation fluidSimulation;
    ftParticleFlow particleFlow;
    ofImage backgroundLogo;
    int flowWidth;
    int flowHeight;
    bool debug;
};
