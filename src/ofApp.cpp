#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    flowWidth = WINDOW_WIDTH / 4;
    flowHeight = WINDOW_HEIGHT / 4;
    tuioForce.setup(flowWidth, flowHeight, WINDOW_WIDTH, WINDOW_HEIGHT);
    
    loadXMLSettings();
    setupGui();
    
    ofEnableSmoothing();
    
    
    fluidSimulation.setup(flowWidth, flowHeight, WINDOW_WIDTH, WINDOW_HEIGHT);
    fluidSimulation.addObstacle(backgroundLogo.getTexture());
    
    particleFlow.setup(flowWidth, flowHeight, WINDOW_WIDTH, WINDOW_HEIGHT);
    
    puks.setup();
}

//--------------------------------------------------------------
void ofApp::loadXMLSettings()
{
    ofxXmlSettings xmlSettings;
    xmlSettings.load("settings.xml");
    
    fullscreen = xmlSettings.getValue("FULLSCREEN", false);
    ofSetFullscreen(fullscreen);
    
    debug = xmlSettings.getValue("DEBUG", false);
    
    string backgroundLogoPath = xmlSettings.getValue("BACKGROUND_LOGO", "logo.png");
    backgroundLogo.load(backgroundLogoPath);
    
    int logoR = xmlSettings.getValue("LOGO_COLOR:R", 255);
    int logoG = xmlSettings.getValue("LOGO_COLOR:G", 255);
    int logoB = xmlSettings.getValue("LOGO_COLOR:B", 255);
    
    logoColor.set(logoR, logoG, logoB);
}

//--------------------------------------------------------------
void ofApp::setupGui() {
    
    gui.setup("settings");
    gui.setDefaultBackgroundColor(ofColor(0, 0, 0, 127));
    gui.setDefaultFillColor(ofColor(160, 160, 160, 160));
    gui.add(guiFPS.set("average FPS", 0, 0, 60));
    gui.add(guiMinFPS.set("minimum FPS", 0, 0, 60));
    gui.add(doFullScreen.set("fullscreen (F)", false));
    doFullScreen.addListener(this, &ofApp::setFullScreen);
    gui.add(toggleGuiDraw.set("show gui (G)", false));
    gui.add(drawMode.set("draw mode", DRAW_COMPOSITE, DRAW_COMPOSITE, DRAW_VELDOTS));
    drawMode.addListener(this, &ofApp::drawModeSetName);
    gui.add(drawName.set("MODE", "draw name"));
    
    
    int guiColorSwitch = 0;
    ofColor guiHeaderColor[2];
    guiHeaderColor[0].set(160, 160, 80, 200);
    guiHeaderColor[1].set(80, 160, 160, 200);
    ofColor guiFillColor[2];
    guiFillColor[0].set(160, 160, 80, 200);
    guiFillColor[1].set(80, 160, 160, 200);
    
    gui.setDefaultHeaderBackgroundColor(guiHeaderColor[guiColorSwitch]);
    gui.setDefaultFillColor(guiFillColor[guiColorSwitch]);
    guiColorSwitch = 1 - guiColorSwitch;
    gui.add(fluidSimulation.parameters);
    
    gui.setDefaultHeaderBackgroundColor(guiHeaderColor[guiColorSwitch]);
    gui.setDefaultFillColor(guiFillColor[guiColorSwitch]);
    guiColorSwitch = 1 - guiColorSwitch;
    gui.add(particleFlow.parameters);
    
    gui.setDefaultHeaderBackgroundColor(guiHeaderColor[guiColorSwitch]);
    gui.setDefaultFillColor(guiFillColor[guiColorSwitch]);
    guiColorSwitch = 1 - guiColorSwitch;
    gui.add(tuioForce.parameters);
    
    gui.setDefaultHeaderBackgroundColor(guiHeaderColor[guiColorSwitch]);
    gui.setDefaultFillColor(guiFillColor[guiColorSwitch]);
    guiColorSwitch = 1 - guiColorSwitch;
    gui.add(velocityDots.parameters);
    
    // if the settings file is not present the parameters will not be set during this setup
    if (!ofFile("fluidSettings.xml"))
    {
        gui.saveToFile("fluidSettings.xml");
    }
    gui.loadFromFile("fluidSettings.xml");
    
    gui.minimizeAll();
    toggleGuiDraw = false;
    
}

//--------------------------------------------------------------
void ofApp::update()
{
    deltaTime = ofGetElapsedTimef() - lastTime;
    lastTime = ofGetElapsedTimef();
    
    
    for (auto puk : puks.getPuks())
    {
        tuioForce.move(puk.location, puk.lastLocation);
    }
    
    tuioForce.update(deltaTime);
    for (int i = 0; i < tuioForce.getNumForces(); ++i)
    {
        if (tuioForce.didChange(i))
        {
            switch (tuioForce.getType(i))
            {
                case FT_DENSITY:
                    fluidSimulation.addDensity(tuioForce.getTextureReference(i), tuioForce.getStrength(i));
                    break;
                case FT_VELOCITY:
                    fluidSimulation.addVelocity(tuioForce.getTextureReference(i), tuioForce.getStrength(i));
                    particleFlow.addFlowVelocity(tuioForce.getTextureReference(i), tuioForce.getStrength(i));
                    break;
                case FT_TEMPERATURE:
                    fluidSimulation.addTemperature(tuioForce.getTextureReference(i), tuioForce.getStrength(i));
                    break;
                case FT_PRESSURE:
                    fluidSimulation.addPressure(tuioForce.getTextureReference(i), tuioForce.getStrength(i));
                    break;
                case FT_OBSTACLE:
                    fluidSimulation.addTempObstacle(tuioForce.getTextureReference(i));
                default:
                    break;
            }
        }
    }
    
    fluidSimulation.update();
    
    if (particleFlow.isActive())
    {
        particleFlow.setSpeed(fluidSimulation.getSpeed());
        particleFlow.setCellSize(fluidSimulation.getCellSize());
        //        particleFlow.addFlowVelocity(opticalFlow.getOpticalFlow());
        particleFlow.addFluidVelocity(fluidSimulation.getVelocity());
        //		particleFlow.addDensity(fluidSimulation.getDensity());
        particleFlow.setObstacle(fluidSimulation.getObstacle());
    }
    particleFlow.update();
}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofClear(0,0);
    
    if (!toggleGuiDraw) {
        ofHideCursor();
        drawComposite();
    }
    else {
        ofShowCursor();
        switch(drawMode.get()) {
            case DRAW_COMPOSITE: drawComposite(); break;
            case DRAW_PARTICLES: drawParticles(); break;
            case DRAW_FLUID_DENSITY: drawFluidDensity(); break;
            case DRAW_VELDOTS: drawVelocityDots(); break;
        }
        drawGui();
    }
    
    if (debug) {
//        tuioClient.drawCursors();
    }
}

//--------------------------------------------------------------
void ofApp::drawComposite(int _x, int _y, int _width, int _height) {
    ofPushStyle();
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    fluidSimulation.draw(_x, _y, _width, _height);
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    if (particleFlow.isActive())
        particleFlow.draw(_x, _y, _width, _height);
    
    ofSetColor(logoColor);
    backgroundLogo.draw(0, 0);
    
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::drawParticles(int _x, int _y, int _width, int _height) {
    ofPushStyle();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    if (particleFlow.isActive())
        particleFlow.draw(_x, _y, _width, _height);
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::drawFluidDensity(int _x, int _y, int _width, int _height) {
    ofPushStyle();
    
    fluidSimulation.draw(_x, _y, _width, _height);
    
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::drawVelocityDots(int _x, int _y, int _width, int _height) {
    ofPushStyle();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    velocityDots.setVelocity(fluidSimulation.getVelocity());
    velocityDots.draw(_x, _y, _width, _height);
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::drawGui() {
    guiFPS = (int)(ofGetFrameRate() + 0.5);
    
    // calculate minimum fps
    deltaTimeDeque.push_back(deltaTime);
    
    while (deltaTimeDeque.size() > guiFPS.get())
        deltaTimeDeque.pop_front();
    
    float longestTime = 0;
    for (int i=0; i<deltaTimeDeque.size(); i++){
        if (deltaTimeDeque[i] > longestTime)
            longestTime = deltaTimeDeque[i];
    }
    
    guiMinFPS.set(1.0 / longestTime);
    
    
    ofPushStyle();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    gui.draw();
    
    // HACK TO COMPENSATE FOR DISSAPEARING MOUSE
    ofEnableBlendMode(OF_BLENDMODE_SUBTRACT);
    ofDrawCircle(ofGetMouseX(), ofGetMouseY(), ofGetWindowWidth() / 300.0);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofDrawCircle(ofGetMouseX(), ofGetMouseY(), ofGetWindowWidth() / 600.0);
    ofPopStyle();
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
        case 'F':
        case 'f':
            fullscreen = !fullscreen;
            ofSetFullscreen(fullscreen);
            break;
        case 'G':
        case 'g':
            toggleGuiDraw = !toggleGuiDraw;
            break;
        case '1':
            drawMode.set(DRAW_COMPOSITE);
            break;
        case '2':
            drawMode.set(DRAW_FLUID_DENSITY);
            break;
        case '3':
            drawMode.set(DRAW_PARTICLES);
            break;
        case '4':
            drawMode.set(DRAW_VELDOTS);
            break;
    }
}

//--------------------------------------------------------------
void ofApp::drawModeSetName(int &_value) {
    switch(_value) {
        case DRAW_COMPOSITE:		drawName.set("Composite"); break;
        case DRAW_PARTICLES:		drawName.set("Particles"); break;
        case DRAW_FLUID_DENSITY:	drawName.set("Fluid Density"); break;
        case DRAW_VELDOTS:			drawName.set("VelDots"); break;
    }
}
