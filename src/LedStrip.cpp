//
//  LedStrip.cpp
//  TFG_v0_3
//
//  Created by Albert on 11/03/2019.
//

#include "LedStrip.hpp"
#include <iostream>

// -- Constructor by default -- //

LedStrip::LedStrip(){
    
    ofVec2f initialPos1;
    ofVec2f initialPos2;
    initialPos1.set(500,50);
    initialPos2.set(800,50);
    /*int rand1 = (int)ofRandom(0,200);
    int rand2 = (int)ofRandom(0,200);
    initialPos1.set(500+rand1,50+rand2);
    initialPos2.set(800+rand1,50+rand2);
    */
    setup(initialPos1,initialPos2,20,0,0); //By default 200 pixels long strip with 20 LEDs and first channel = 0 and start universe = 0
    
}
void LedStrip::setup(ofVec2f _x1y1, ofVec2f _x2y2){
    
    x1y1 = _x1y1;
    x2y2 = _x2y2;
    length = x1y1.distance(x2y2); // By now unused, is it necessary?
    stepX = (x2y2[0]-x1y1[0])/(numLeds*2);
    stepY = (x2y2[1]-x1y1[1])/(numLeds*2);
    radius = round(sqrt(stepX*stepX+stepY*stepY)); //TODO: Radius automatic or adjusted through the GUI???
    numCh = numLeds*3; // By now R-G-B only Led Strips
    lastCh = 0; // TO DO: Read the last element of myLedStripChannels.channel
    lastUn = 0; // TO DO: Read the last element of myLedStripChannels.chUniverse
    
    //Here we allocate the number of LEDs of the strip
    
    for( int i = 0; i<numLeds; i++){
        ledPixel myLed;
        myLedStripPixels.push_back(myLed);
        myLedStripPixels.at(i).ledIndex = i+1;
    }
    
    //Here we allocate the number of channels of the whole strip
    
    for( int i = 0; i<numCh; i++){
        ledStripChannels myCh;
        myLedStripChannels.push_back(myCh);
    }
    
    //If we full all the "usable" channels of the universe, reset channel and increase universe
    //For example the usable channels of one universe go from 0.....509, 510 and 511 are not used (?)
    //so once we exceed the channel 509 we have to send to the next universe.
    
    int universe = 0;
    int channel = 0;
    
    for( int i = 0; i<numCh; i++){
        
        unsigned char zero = '0';
        
        if ( (((firstCh + channel) == 510) && universe == 0) || (channel == 510) ){
            channel = 0;
            universe++;
        }
        
        if (universe == 0 ){
            myLedStripChannels.at(i).channel = firstCh + channel;
            
        }else {
            myLedStripChannels.at(i).channel = channel;
            
        }
        
        myLedStripChannels.at(i).chValue = zero;
        myLedStripChannels.at(i).chUniverse = firstUn + universe;
        channel++;
        
    }
    
    //Here we assing the position of every LED, note the round inside ofVec2f due to we work with -float- increments but -int- pixels
    
    int counter = 0;
    for (int i=1; i<numLeds*2; i=i+2){
        myLedStripPixels.at(counter).position = ofVec2f(round(x1y1[0]+i*stepX) , round(x1y1[1]+i*stepY));
        myLedStripPixels.at(counter).ledIndex = counter+1; //Id of the pixels, first LED = 1
        counter++;
    }
    
    lastCh = myLedStripChannels.at(numCh-1).channel;
    lastUn = myLedStripChannels.at(numCh-1).chUniverse;
    
    
}
void LedStrip::setup(ofVec2f _x1y1, ofVec2f _x2y2, int _numLeds, int _firstCh, int _firstUn){

    x1y1 = _x1y1;
    x2y2 = _x2y2;
    numLeds = _numLeds;
    length = x1y1.distance(x2y2); // By now unused, is it necessary?
    stepX = (x2y2[0]-x1y1[0])/(numLeds*2);
    stepY = (x2y2[1]-x1y1[1])/(numLeds*2);
    radius = round(sqrt(stepX*stepX+stepY*stepY)); //TODO: Radius automatic or adjusted through the GUI???
    firstCh = _firstCh;
    firstUn = _firstUn;
    numCh = numLeds*3; // By now R-G-B only Led Strips
    lastCh = 0; // TO DO: Read the last element of myLedStripChannels.channel
    lastUn = 0; // TO DO: Read the last element of myLedStripChannels.chUniverse
    
    //Here we allocate the number of LEDs of the strip
    
    for( int i = 0; i<numLeds; i++){
        ledPixel myLed;
        myLedStripPixels.push_back(myLed);
        myLedStripPixels.at(i).ledIndex = i+1;
    }
    
    //Here we allocate the number of channels of the whole strip
    
    for( int i = 0; i<numCh; i++){
        ledStripChannels myCh;
        myLedStripChannels.push_back(myCh);
    }
    
    //If we full all the "usable" channels of the universe, reset channel and increase universe
    //For example the usable channels of one universe go from 0.....509, 510 and 511 are not used (?)
    //so once we exceed the channel 509 we have to send to the next universe.
    
    int universe = 0;
    int channel = 0;
    
    for( int i = 0; i<numCh; i++){

        unsigned char zero = '0';
        
        if ( (((firstCh + channel) == 510) && universe == 0) || (channel == 510) ){
            channel = 0;
            universe++;
        }
        
        if (universe == 0 ){
            myLedStripChannels.at(i).channel = firstCh + channel;

        }else {
            myLedStripChannels.at(i).channel = channel;
            
        }
        
        myLedStripChannels.at(i).chValue = zero;
        myLedStripChannels.at(i).chUniverse = firstUn + universe;
        channel++;
        
    }
    //Here we assing the position of every LED, note the round inside ofVec2f due to we work with -float- increments but -int- pixels
    
    int counter = 0;
    for (int i=1; i<numLeds*2; i=i+2){
        myLedStripPixels.at(counter).position = ofVec2f(round(x1y1[0]+i*stepX) , round(x1y1[1]+i*stepY));
        myLedStripPixels.at(counter).ledIndex = counter+1; //Id of the pixels, first LED = 1
        counter++;
    }
    
    lastCh = myLedStripChannels.at(numCh-1).channel;
    lastUn = myLedStripChannels.at(numCh-1).chUniverse;
    
    
}
void LedStrip::setPosition(ofVec2f _x1y1, ofVec2f _x2y2){
    
    //TODO: call to setup passing only position parameters
   
    
}

void LedStrip::readPixels(ofPixels & screenPixels){

    for (int i=0; i<numLeds; i++){
        
        int indexArray = (myLedStripPixels.at(i).position.y*screenPixels.getWidth()+myLedStripPixels.at(i).position.x)*3;
        
        myLedStripPixels.at(i).ledColor[0] = screenPixels[indexArray];
        myLedStripPixels.at(i).ledColor[1] = screenPixels[indexArray+1];
        myLedStripPixels.at(i).ledColor[2] = screenPixels[indexArray+2];
    }
    
    // TODO: Si el color es el mismo, no leer otra vez, por lo tanto "break"
}

int LedStrip::getNumCh(){
    
    return numCh;
    
}
std::pair<int,int> LedStrip::getFirstChFirstUn(){
    
    std::pair<int,int> firstLastCh;
    firstLastCh = std::make_pair (firstCh,firstUn);
    
    return firstLastCh;
}

std::pair<int,int> LedStrip::getLastChLastUn(){
    
    std::pair<int,int> firstLastUn;
    firstLastUn = std::make_pair (lastCh,lastUn);
    
    return firstLastUn;
}

std::pair<int,int> LedStrip::getNextStripChUn(){
    
    std::pair<int,int> nextStripChUn;
    nextStripChUn = std::make_pair (lastCh+1,lastUn);
    
    return nextStripChUn;
    
}


vector <LedStrip::ledStripChannels> LedStrip::getChannels(){
    
    for (int i=0; i<numLeds; i++){
        
        myLedStripChannels.at(0+i*3).chValue = myLedStripPixels.at(i).ledColor[0];
        myLedStripChannels.at(1+i*3).chValue = myLedStripPixels.at(i).ledColor[1];
        myLedStripChannels.at(2+i*3).chValue = myLedStripPixels.at(i).ledColor[2];
        
    }
    
    return myLedStripChannels;
}

void LedStrip::drawNoFill(){
    
    //color = _c;
    ofNoFill();
    //ofColor(255,0,0);
    
    for (int i=0; i<numLeds; i++){
        
        ofDrawCircle(myLedStripPixels.at(i).position.x,myLedStripPixels.at(i).position.y,radius);
        
    }
}
void LedStrip::drawColor(){
    
    ofFill();
    
    for (int i=0; i<numLeds; i++){
        
        int r =(int)myLedStripPixels.at(i).ledColor[0];
        int g =(int)myLedStripPixels.at(i).ledColor[1];
        int b =(int)myLedStripPixels.at(i).ledColor[2];
        
        ofFill();
        ofPath circle;
        circle.arc(myLedStripPixels.at(i).position.x,myLedStripPixels.at(i).position.y , radius, radius, 0, 360);
        circle.close();
        circle.setColor(ofColor(r,g,b));
        circle.draw();
            
    }
}
void LedStrip::update(){
    
    //TODO: call to setup? -> For example when moving the LedStrip or modifying the stratChannel or Universe.
  
        
        /*if (ofDist(ofGetMouseX(), ofGetMouseY(), x1y1.x, x1y1.y) <= radius) {
            // Mouse is inside of circle
            x1y1.set(ofGetMouseX(),ofGetMouseY());
            LedStrip::setup(x1y1,x2y2);
        }*/

    
   /* if (ofDist(mouseX, mouseY, x2y2.x, x2y2.y) <= radius) {
        // Mouse is inside of circle
        
    }*/
    
    
}
