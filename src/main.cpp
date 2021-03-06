#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    
    ofGLFWWindowSettings windowSettings;
#ifdef USE_PROGRAMMABLE_GL
    windowSettings.setGLVersion(4, 1);
#endif
    windowSettings.width = WINDOW_WIDTH;
    windowSettings.height = WINDOW_HEIGHT;
    windowSettings.windowMode = OF_WINDOW;
    
    ofCreateWindow(windowSettings);
	ofRunApp(new ofApp());

}
