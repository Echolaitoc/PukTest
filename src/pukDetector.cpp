//
//  pukDetector.cpp
//  PukTest
//
//  Created by Tristan Weber on 09/05/16.
//
//

#include "pukDetector.h"

pukDetector::pukDetector()
{
    ofAddListener(tuioClient.cursorAdded, this, &pukDetector::tuioAdded);
    ofAddListener(tuioClient.cursorRemoved, this, &pukDetector::tuioRemoved);
    ofAddListener(tuioClient.cursorUpdated, this, &pukDetector::tuioUpdated);
}

pukDetector::~pukDetector()
{
    ofRemoveListener(tuioClient.cursorAdded, this, &pukDetector::tuioAdded);
    ofRemoveListener(tuioClient.cursorRemoved, this, &pukDetector::tuioRemoved);
    ofRemoveListener(tuioClient.cursorUpdated, this, &pukDetector::tuioUpdated);
}

void pukDetector::setup()
{
    tuioClient.start(3333);
}

void pukDetector::update()
{
    tuioClient.getMessage();
}

//--------------------------------------------------------------

void pukDetector::tuioAdded(ofxTuioCursor &tuioCursor)
{
    tuioPoints.push_back(tuioContainer());
    tuioPoints.back().sid = tuioCursor.getSessionId();
    tuioPoints.back().location.set(tuioCursor.getX(), tuioCursor.getY());
    tuioPoints.back().setLastLocation();
}

void pukDetector::tuioUpdated(ofxTuioCursor &tuioCursor)
{
    //    ofPoint loc = ofPoint(tuioCursor.getX()*ofGetWidth(),tuioCursor.getY()*ofGetHeight());
    int cursorIndex = getTuioPointIndex(tuioCursor.getSessionId());
    if (cursorIndex < 0)
    {
        return;
    }
    tuioPoints.at(cursorIndex).setLastLocation();
    tuioPoints.at(cursorIndex).location.set(tuioCursor.getX(), tuioCursor.getY());
}

void pukDetector::tuioRemoved(ofxTuioCursor &tuioCursor)
{
    int cursorIndex = getTuioPointIndex(tuioCursor.getSessionId());
    if (cursorIndex >= 0)
    {
        tuioPoints.erase(tuioPoints.begin() + cursorIndex);
    }
}

int pukDetector::getTuioPointIndex(int sid)
{
    int cursorIndex = -1;
    for (int i = 0; i < tuioPoints.size(); ++i)
    {
        if (tuioPoints[i].sid == sid)
        {
            cursorIndex = i;
            break;
        }
    }
    return cursorIndex;
}
