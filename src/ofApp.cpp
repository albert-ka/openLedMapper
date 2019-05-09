#include "ofApp.h"
#include "ofxFastFboReader.h"
#include "ofxImGui.h"
//#include "imgui_demo.cpp"

ofFbo fbo;
ofxFastFboReader reader;
ofPixels screenPixels;

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetVerticalSync(true); // Disabling incleases framerate but appears tearing artifact
    ofBackground(0);
    
    fbo.allocate(1280, 800, GL_RGB); // TODO: Resolution of the screen, read from computer.
    ofSetWindowShape(1280,800); // SAME
    ofSetFrameRate(60); // ????
    
    //ofxSyphon: setup our directory
    dir.setup();
    //ofxSyphon: setup our client
    client.setup();
    
    //ofxSyphon: register for our directory's callbacks
    ofAddListener(dir.events.serverAnnounced, this, &ofApp::serverAnnounced);
    ofAddListener(dir.events.serverRetired, this, &ofApp::serverRetired);
    
    dirIdx = -1;
    
    //-- Led Mapper Part --//
    
    // *Artnet Setup* //
        
    for ( int i = 0; i<totalNumUn; i++){
        
        artnetController.emplace_back();
        artnetController.back().setup(ipAdress,i);
        myControllerUniverses.emplace_back();
    }
    
 
    // *Led Strips Setup* //
    
    //myLedStrips.push_back(LedStrip());
    //myLedStrips.push_back(LedStrip());
    //myLedStrips.push_back(LedStrip());
    //myLedStrips.push_back(LedStrip());

    
    //myLedStrips[0].setup(ofVec2f(200,50),ofVec2f(600,50),10,505,0);
    //myLedStrips[1].setup(ofVec2f(600,150),ofVec2f(1000,150),10,myLedStrips[0].getNextStripChUn().first,myLedStrips[0].getNextStripChUn().second);
    
    //myLedStrips[2].setup(ofVec2f(100,50),ofVec2f(100,700),30,0,4);
    
    
    
    
    
    

    // ----- ***** ImGui **** ------ //
    
    ofSetLogLevel(OF_LOG_VERBOSE); // Input and output of text
    
    //required call
    gui.setup();
    
    ImGui::GetIO().MouseDrawCursor = false;
    show_test_window = true;
    show_another_window = false;
    floatValue = 0.0f;
    
    //load your own ofImage
    imageButtonSource.load("of.png");
    imageButtonID = gui.loadImage(imageButtonSource);
    
    //or have the loading done for you if you don't need the ofImage reference
    //imageButtonID = gui.loadImage("of.png");
    
    //can also use ofPixels in same manner
    ofLoadImage(pixelsButtonSource, "of_upside_down.png");
    pixelsButtonID = gui.loadPixels(pixelsButtonSource);
    
    //and alt method
    //pixelsButtonID = gui.loadPixels("of_upside_down.png");
    
    //pass in your own texture reference if you want to keep it
    textureSourceID = gui.loadTexture(textureSource, "of_upside_down.png");
    
    //or just pass a path
    //textureSourceID = gui.loadTexture("of_upside_down.png");
    
    ofLogVerbose() << "textureSourceID: " << textureSourceID;
    
    ofDirectory dataDirectory(ofToDataPath("", true));
    
    files = dataDirectory.getFiles();
    for(size_t i=0; i<files.size(); i++)
    {
        fileNames.push_back(files[i].getFileName());
    }

    
}
//--------------------------------------------------------------
void ofApp::update()
{
    
    //Read texture from Syphon and store it in an FBO
    fbo.begin();
    client.bind();
    ofClear(0, 0, 0, 0);
    
        if(dir.isValidIndex(dirIdx)){
            texFromSyphon = client.getTexture();
            if (resizeMode){
                texFromSyphon.draw(0,0,1280,800);
            }else{
                texFromSyphon.draw(0,0);
            }
        };
    
    client.unbind();
    fbo.end();
    

    if (mode == 0)
    {
        reader.readToPixels(fbo, screenPixels);
    }
    if (mode == 1)
    {
        fbo.readToPixels(screenPixels);
    }
    
    reader.setAsync(setAsyncFBO); //Asynchronous reading from PBO = Faster? (I don't see any difference in the framerate)
    
    
    // -- TODO: Create funciton "Route to artnet" or something simillar -- //
    
            // ***** FRAMERATE DROP!!!! OPTIMIZE THIS PART!!! ***** //
    
    for(vector<LedStrip>::iterator itLS = myLedStrips.begin(); itLS != myLedStrips.end(); ++itLS){
        
        //Read pixels from buffer to LedStrip instances
        (*itLS).readPixels(screenPixels);
        (*itLS).update();
        
        for (int i = (*itLS).getFirstChFirstUn().second; i<=(*itLS).getLastChLastUn().second ; i++){
            
            for (int j = 0 ; j<(*itLS).getNumCh(); j++){
                
                myControllerUniverses[i].universe[(*itLS).getChannels()[j].channel] = (*itLS).getChannels()[j].chValue;
                
            }
        }
    }
    
    
    
    for(int i = 0; i<totalNumUn; i++){
        
    artnetController[i].sendArtnet(myControllerUniverses[i].universe,512); //

    }
}
//--------------------------------------------------------------
void ofApp::draw()
{
    //ofBackground(0, 0, 0);
    //ofColor(255, 255, 255, 255);
    //ofEnableAlphaBlending();
    
    if(dir.isValidIndex(dirIdx) && (drawMode == 0 || drawMode == 1)){
       ofSetColor(255, 255, 255);
        
        if (resizeMode){
            texFromSyphon.draw(0,0,1280,800);
        }else{
            texFromSyphon.draw(0,0);
        }
    }
    
    for(vector<LedStrip>::iterator itLS = myLedStrips.begin(); itLS != myLedStrips.end(); ++itLS){
    
        if(drawMode == 0){
        
            //itLS->.drawNoFill();
            (*itLS).drawNoFill();

        
        } else if (drawMode == 1){
        
     
            //(*itLS).drawColor();
            //(*itLS).drawNoFill();
            (*itLS).drawColor();
            (*itLS).drawNoFill();
        
        }else if (drawMode == 2){
        
            (*itLS).drawColor();
        
        }
        
    }
    
    //ofDrawBitmapStringHighlight("Press S key to cycle through all available Syphon servers.", ofPoint(20, 760));
    //ofDrawBitmapStringHighlight("Press '1', '2' or '3' to cycle through all the draw modes", ofPoint(20, 780));
    ofDrawBitmapStringHighlight("Framerate "+ofToString(ofGetFrameRate()), ofPoint(20, 20));
    ofDrawBitmapStringHighlight("Mouse X "+ofToString(ofGetMouseX()), ofPoint(20, 40));
    ofDrawBitmapStringHighlight("Mouse Y "+ofToString(ofGetMouseY()), ofPoint(20, 60));

    
    
    
    
    // ---- **** ImGUI **** ---- //
    
    gui.begin();
    gui.setTheme(new MyTheme());
    ImGui::StyleColorsDark();
    ImGui::SetNextWindowPos(ofVec2f(650,2),ImGuiSetCond_FirstUseEver); // ??? Doesn't work
        
    ImGui::Begin("Led Mapper");
    
        if (ImGui::Button("ADD NEW FIXTURE"))
        {
            myLedStrips.push_back(LedStrip());
        }
        ImGui::End();
    
    //required to call this at end
    gui.end();
    
   
    
}
//--------------------------------------------------------------//
void ofApp::keyPressed(int key)
{
   
    if (key == 's' || key == 'S'){
        
        //ofxSyphon press 's' key to move through all available Syphon servers
        if (dir.size() > 0)
        {
            dirIdx++;
            if(dirIdx > dir.size() - 1)
                dirIdx = 0;
            
            client.set(dir.getDescription(dirIdx));
            string serverName = client.getServerName();
            string appName = client.getApplicationName();
            
            if(serverName == ""){
                serverName = "null";
            }
            if(appName == ""){
                appName = "null";
            }
            ofSetWindowTitle(serverName + ":" + appName);
        }
        else
        {
            ofSetWindowTitle("No Server");
        }
    }
    
    if (key == '1'){
        drawMode = 0;
    } else if (key == '2'){
        drawMode = 1;
    }else if (key == '3'){
        drawMode = 2;
    }
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
        /*if(button == 0){
            mouseState = true;
        }*/
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
    
}

// -- ofxSyphon: these are our directory's callbacks -- //
void ofApp::serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg)
{
    for( auto& dir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Announced")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
    }
    dirIdx = 0;
}

void ofApp::serverUpdated(ofxSyphonServerDirectoryEventArgs &arg)
{
    for( auto& dir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Updated")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
    }
    dirIdx = 0;
}

void ofApp::serverRetired(ofxSyphonServerDirectoryEventArgs &arg)
{
    for( auto& dir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Retired")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
    }
    dirIdx = 0;
}
