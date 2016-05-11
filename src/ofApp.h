#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxGui.h"
#include "ofxFlowTools.h"
#include "ftxDrawCustomForce.h"
#include "pukDetector.h"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

#define USE_PROGRAMMABLE_GL

using namespace flowTools;

enum drawModeEnum{
    DRAW_COMPOSITE = 0,
    DRAW_FLUID_DENSITY,
    DRAW_PARTICLES,
    DRAW_VELDOTS
};

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
private:
    
    void loadXMLSettings();
    void setupGui();
    void keyPressed(int key);
    
    void drawGui();
    void setFullScreen(bool& _value) { ofSetFullscreen(_value);}
    void drawModeSetName(int& _value);
    void drawComposite() { drawComposite(0, 0, ofGetWindowWidth(), ofGetWindowHeight()); }
    void drawComposite(int _x, int _y, int _width, int _height);
    void drawFluidDensity() { drawFluidDensity(0, 0, ofGetWindowWidth(), ofGetWindowHeight()); }
    void drawFluidDensity(int _x, int _y, int _width, int _height);
    void drawParticles() { drawParticles(0, 0, ofGetWindowWidth(), ofGetWindowHeight()); }
    void drawParticles(int _x, int _y, int _width, int _height);
    void drawVelocityDots() { drawVelocityDots(0, 0, ofGetWindowWidth(), ofGetWindowHeight()); }
    void drawVelocityDots(int _x, int _y, int _width, int _height);
    
    ofxPanel gui;
    ofParameter<bool> toggleGuiDraw;
    ofParameter<float> guiFPS;
    ofParameter<float> guiMinFPS;
    deque<float> deltaTimeDeque;
    ofParameter<bool> doFullScreen;
    ofParameter<int> drawMode;
    ofParameter<string> drawName;
    pukDetector puks;
    ftFluidSimulation fluidSimulation;
    ftParticleFlow particleFlow;
    ftVelocitySpheres velocityDots;
    ftxDrawCustomForces tuioForce;
    ofImage backgroundLogo;
    ofColor logoColor;
    float lastTime;
    float deltaTime;
    int flowWidth;
    int flowHeight;
    bool debug;
    bool fullscreen;
};
