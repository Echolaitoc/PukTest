//
//  ftxCustomForce.cpp
//  PukTest
//
//  Created by Tristan Weber on 09/05/16.
//
//

#include "ftxDrawCustomForce.h"

namespace flowTools {
    
    ftxDrawCustomForces::ftxDrawCustomForces()
    : numDrawForces(3)
    , drawForces(NULL)
    , deltaTime(0.0f)
    , simulationWidth(1)
    , simulationHeight(1)
    , densityWidth(1)
    , densityHeight(1)
    {
//        ofAddListener(ofEvents().mouseDragged, this, &ftxDrawCustomForces::mouseDragged);
    }
    
    ftxDrawCustomForces::~ftxDrawCustomForces()
    {
//        ofRemoveListener(ofEvents().mouseDragged, this, &ftxDrawCustomForces::mouseDragged);
    }
    
    void ftxDrawCustomForces::setup(int _simulationWidth, int _simulationHeight, int _densityWidth, int _densityHeight)
    {
        simulationWidth = _simulationWidth;
        simulationHeight = _simulationHeight;
        densityWidth = (!_densityWidth)? simulationWidth : _densityWidth;
        densityHeight = (!_densityHeight)? simulationHeight: _densityHeight;
        
        numDrawForces = 3;
        drawForces = new ftDrawForce[numDrawForces];
        drawForces[0].setup(densityWidth, densityHeight, FT_DENSITY, true);
        drawForces[0].setName("draw full res");
        drawForces[1].setup(simulationWidth, simulationHeight, FT_VELOCITY, true);
        drawForces[1].setName("draw flow res 1");
        drawForces[2].setup(simulationWidth, simulationHeight, FT_TEMPERATURE, true);
        drawForces[2].setName("draw flow res 2");
        
        parameters.setName("tuio");
        parameters.add(doResetDrawForces.set("reset", false));
        doResetDrawForces.addListener(this, &ftxDrawCustomForces::resetDrawForcesListner);
        for (int i = 0; i < numDrawForces; ++i)
        {
            parameters.add(drawForces[i].parameters);
        }
    }
    
    void ftxDrawCustomForces::update(float _deltaTime)
    {
        deltaTime = _deltaTime;
        
        for (int i = 0; i < numDrawForces; ++i)
        {
            drawForces[i].update();
        }
    }
    
    //--------------------------------------------------------------
    bool ftxDrawCustomForces::didChange(int _index)
    {
        if (_index < 0 || _index >= numDrawForces)
        {
            ofLogWarning("ftxDrawCustomForces::getDrawForceType: index out of range");
            return false;
        }
        else
        {
            return drawForces[_index].didChange();
        }
    }
    
    //--------------------------------------------------------------
    ftDrawForceType ftxDrawCustomForces::getType(int _index)
    {
        if (_index < 0 || _index >= numDrawForces)
        {
            ofLogWarning("ftxDrawCustomForces::getDrawForceType: index out of range");
            return FT_NONE;
        }
        else
        {
            return drawForces[_index].getType();
        }
    }
    
    //--------------------------------------------------------------
    ofTexture& ftxDrawCustomForces::getTextureReference(int _index) {
        if (_index < 0 || _index >= numDrawForces)
        {
            ofLogError("ftxDrawCustomForces::getTexture: index out of range");
        }
        else
        {
            return drawForces[_index].getTexture();
        }
        
    }
    
    //--------------------------------------------------------------
    float ftxDrawCustomForces::getStrength(int _index)
    {
        if (_index < 0 || _index >= numDrawForces)
        {
            ofLogWarning("ftxDrawCustomForces::getStrength: index out of range");
            return 0;
        }
        else
        {
            if (drawForces[_index].getIsTemporary())
            {
                return drawForces[_index].getStrength();
            }
            else
            {
                return drawForces[_index].getStrength() * deltaTime;;
            }
        }
    }
    
    //--------------------------------------------------------------
//    void ftxDrawCustomForces::mouseDragged( ofMouseEventArgs& mouse ) {
//        ofVec2f normalizedMouse;
//        
//        normalizedMouse.set(mouse.x / (float)ofGetWindowWidth(), mouse.y / (float)ofGetWindowHeight());
//        
//        ofVec2f velocity = normalizedMouse - lastNormalizedMouse;
//        
//        for (int i = 0; i < numDrawForces; ++i)
//        {
//            if (drawForces[i].getType() == FT_VELOCITY)
//            {
//                drawForces[i].setForce(velocity);
//            }
//            drawForces[i].applyForce(normalizedMouse);
//        }
//        lastNormalizedMouse.set(normalizedMouse);
//        
//    }
    
    //--------------------------------------------------------------
    void ftxDrawCustomForces::move( ofVec2f normalizedPosition, ofVec2f normalizedLastPosition )
    {
        ofVec2f velocity = normalizedPosition - normalizedLastPosition;
        
        for (int i = 0; i < numDrawForces; ++i)
        {
            if (drawForces[i].getType() == FT_VELOCITY)
            {
                drawForces[i].setForce(velocity * 2);
            }
            drawForces[i].applyForce(normalizedPosition);
        }
//        lastNormalizedPosition.set(normalizedPosition);
        
    }
}