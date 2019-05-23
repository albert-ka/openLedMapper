//
//  LedStrip.hpp
//  TFG_v0_3
//
//  Created by Albert on 11/03/2019.
//

#ifndef LedStrip_hpp
#define LedStrip_hpp

#include "ofMain.h"

class LedStrip {
    
public:
    
    LedStrip();
    // TODO: destructor
    
    void setup(int _numLeds, int _startUniverse, int _firstChannel); // Here we are going to setup or LedStripe properties
    void setID(int _stripId);
    int getId();
    void move(int x, int y);
    bool mouseInside(int x,int y);
    void readPixels(ofPixels & screenPixels); // Here we are going to read the pixels and store it
    void drawNoFill(int _ledStripFlag); // Here we are going to draw the LedStripe in our program
    void drawColor(); // Here we are going to draw the LedStripe in our program
    void setPosition(ofVec2f _x1y1, ofVec2f _x2y2); // To update the position while moving through the GUI
    void setChUn(int _firstCh, int _firstUn);
    int getNumCh();
    
    std::pair<int,int> getFirstChFirstUn();
    std::pair<int,int> getLastChLastUn();
    std::pair<int,int> getNextStripChUn();
    
    struct ledPixel{
        int ledIndex;
        ofVec2f position;
        unsigned char ledColor[3];
    };
    
    struct ledStripChannels{
        int channel;
        unsigned char chValue; // 0 to 255, DMX range (8 bit)
        int chUniverse;
    };
    
    vector <ledStripChannels> getChannels();
    
private:
    
    ofVec2f x1y1;
    ofVec2f x2y2;
    int numLeds;
    int radius;
    float length;
    float stepX;
    float stepY;
    int stripID;
    int firstCh;
    int lastCh;
    int firstUn;
    int lastUn;
    int numCh;
    
    vector <ledPixel> myLedStripPixels; //Tamany numLeds
    vector <ledStripChannels> myLedStripChannels;
    
};


#endif /* LedStrip_hpp */
