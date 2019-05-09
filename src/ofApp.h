#pragma once
#include "ofxSyphon.h"
#include "ofMain.h"
#include "LedStrip.hpp"
#include "ofxArtnet.h"
#include "ofxImGui.h"
#include "MyTheme.h"

class ofApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg);
    void serverUpdated(ofxSyphonServerDirectoryEventArgs &args);
    void serverRetired(ofxSyphonServerDirectoryEventArgs &arg);
    
    ofxSyphonServerDirectory dir;
    ofxSyphonClient client;
    
    int dirIdx;
    bool setAsyncFBO = false;
    int mode = 0; // ofxFastFbo mode, 0 -> PBO // 1 -> readPixels
    int drawMode = 0; //LedMapper drawing modes
    bool resizeMode = true; //For Adobe After Effects plugin at 1024x640. It streches the Syphon source to full resolution.
    bool mouseState;
    
    vector <LedStrip> myLedStrips;
    
    //LedStrip myLedStrip;

    //unsigned char universe[512];
    
    ofColor col;
    ofTexture texFromSyphon;
    
    // -- Artnet -- //
    
    //ofxArtnetSender artnet;
    int totalNumUn = 8;
    char ipAdress[15] = "192.168.2.3";
    //std::vector<shared_ptr<ofxArtnetSender>> artnetController;
    std::vector<ofxArtnetSender> artnetController;
    
    //PROBLEMS! Vector de arrays unsigned char[512] ... why not??
    
    //vector<vector<unsigned char>> universe;
    
    struct controllerUniverse{
        unsigned char universe[512];
    };
    
    std::vector<controllerUniverse> myControllerUniverses;
    
    
    //std::array<unsigned char, 512> universe;
    
    //using universe = std::array<unsigned char, 512>;
    //std::vector <universe> myControllerUniverses;
    
    //std::vector<std::array<double, 4>> blabla;
    
    
    
    // ------ **** ImGUi **** ------ //
    
    ofxImGui::Gui gui;
    ImVec4 backgroundColor;
    bool show_test_window;
    bool show_another_window;
    
    float floatValue;
    ofImage imageButtonSource;
    GLuint imageButtonID;
    
    ofPixels pixelsButtonSource;
    GLuint pixelsButtonID;
    
    ofTexture textureSource;
    GLuint textureSourceID;
    
    std::vector<std::string> fileNames;
    std::vector<ofFile> files;
    
    
};




