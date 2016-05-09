#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofEnableSmoothing();
    
    tuioClient.start(3333);
    
    flowWidth = WINDOW_WIDTH / 4;
    flowHeight = WINDOW_HEIGHT / 4;
    
    fluidSimulation.setup(flowWidth, flowHeight, WINDOW_WIDTH, WINDOW_HEIGHT);
    particleFlow.setup(flowWidth, flowHeight, WINDOW_WIDTH, WINDOW_HEIGHT);
    
    debug = false;
    
    ofAddListener(tuioClient.cursorAdded,this,&ofApp::tuioAdded);
    ofAddListener(tuioClient.cursorRemoved,this,&ofApp::tuioRemoved);
    ofAddListener(tuioClient.cursorUpdated,this,&ofApp::tuioUpdated);
}

//--------------------------------------------------------------
void ofApp::update()
{
    tuioClient.getMessage();
}

//--------------------------------------------------------------
void ofApp::draw()
{
    if (debug) {
        tuioClient.drawCursors();
        return;
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    switch (key)
    {
        case 'D':
        case 'd':
            debug = !debug;
            break;
    }
}

//--------------------------------------------------------------

void ofApp::tuioAdded(ofxTuioCursor &tuioCursor)
{
    ofPoint loc = ofPoint(tuioCursor.getX()*ofGetWidth(),tuioCursor.getY()*ofGetHeight());
    cout << "Point n" << tuioCursor.getSessionId() << " add at " << loc << endl;
}

void ofApp::tuioUpdated(ofxTuioCursor &tuioCursor)
{
    ofPoint loc = ofPoint(tuioCursor.getX()*ofGetWidth(),tuioCursor.getY()*ofGetHeight());
    cout << "Point n" << tuioCursor.getSessionId() << " updated at " << loc << endl;
}

void ofApp::tuioRemoved(ofxTuioCursor &tuioCursor)
{
    ofPoint loc = ofPoint(tuioCursor.getX()*ofGetWidth(),tuioCursor.getY()*ofGetHeight());
    cout << "Point n" << tuioCursor.getSessionId() << " remove at " << loc << endl;
}
