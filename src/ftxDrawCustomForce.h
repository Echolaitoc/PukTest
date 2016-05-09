//
//  ftxCustomForce.hpp
//  PukTest
//
//  Created by Tristan Weber on 09/05/16.
//
//

#include "ofMain.h"
#include "ftFbo.h"
#include "ftDrawForce.h"


namespace flowTools {
    
    class ftxDrawCustomForces {
        
    public:
        ftxDrawCustomForces();
        ~ftxDrawCustomForces();
        
        void setup(int _simulationWidth, int _simulationHeight, int _densityWidth = 0, int _densityHeight = 0) ;
        void update(float _deltaTime);
        void reset() { for (int i=0; i<numDrawForces; i++) drawForces[i].reset(); }
        
        int getNumForces() { return numDrawForces; }
        
        bool didChange(int _index);
        ftDrawForceType getType(int _index);
        ofTexture& getTextureReference(int _index);
        float getStrength(int _index);
        
        void move(ofVec2f normalizedPosition, ofVec2f normalizedLastPosition);
        
        ofParameterGroup parameters;
        
    private:
        ofParameter<bool> doResetDrawForces;
        void resetDrawForcesListner(bool& _value) { if (_value) { reset(); }; doResetDrawForces.set(false); }
        void mouseDragged(ofMouseEventArgs & mouse);
        
        int numDrawForces;
        ftDrawForce* drawForces;
        
        float deltaTime;
        
        int	simulationWidth;
        int	simulationHeight;
        int	densityWidth;
        int	densityHeight;
        
        
        ofVec2f lastNormalizedMouse;
        
    };
}
