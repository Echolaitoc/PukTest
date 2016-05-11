//
//  pukDetector.h
//  PukTest
//
//  Created by Tristan Weber on 09/05/16.
//
//
#pragma once

#include "ofxTuio.h"

class pukDetector
{
public:
    struct tuioContainer {
        void setLastLocation() {
            lastLocation.set(location);
        }
        
        ofVec2f getDelta() {
            return location - lastLocation;
        }
        
        long sid;
        ofVec2f location;
        ofVec2f lastLocation;
    };
    
    struct puk {
        vector<tuioContainer> tuioPoints;
        ofVec2f location;
        ofVec2f lastLocation;
    };
    
    pukDetector();
    ~pukDetector();
    void setup();
    void update();
    vector<puk> getPuks();
    
private:
    void tuioAdded(ofxTuioCursor & tuioCursor);
    void tuioRemoved(ofxTuioCursor & tuioCursor);
    void tuioUpdated(ofxTuioCursor & tuioCursor);
    int getTuioPointIndex(int sid);
    
    ofxTuioClient tuioClient;
    vector<tuioContainer> tuioPoints;
    vector<puk> puks;
};
