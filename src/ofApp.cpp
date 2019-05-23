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
    setupArtnet(ipAdress,totalNumUn);

    // ----- ***** ImGui **** ------ //
    
    ofSetLogLevel(OF_LOG_VERBOSE); // Input and output of text
    
    //required call
    gui.setup();
    ImGui::GetIO().MouseDrawCursor = false;
    

    
}
//--------------------------------------------------------------
void ofApp::update()
{
    
    //Read texture from Syphon and store it into an FBO
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

    if (mode == 0){
        reader.readToPixels(fbo, screenPixels);
    }
    if (mode == 1){
        fbo.readToPixels(screenPixels);
    }
    reader.setAsync(setAsyncFBO); //Asynchronous reading from PBO = Faster? (I don't see any difference in the framerate)
    
    // -- TODO: Create function "Route to artnet" or something simillar? -- //
    
            // ***** FRAMERATE DROP!!!! OPTIMIZE THIS PART!!! ***** //
    
    for(vector<LedStrip>::iterator itLS = myLedStrips.begin(); itLS != myLedStrips.end(); ++itLS){
        //Read pixels from buffer to LedStrip instances
        (*itLS).readPixels(screenPixels);
        for (int i = (*itLS).getFirstChFirstUn().second; i<=(*itLS).getLastChLastUn().second ; i++){
            for (int j = 0 ; j<(*itLS).getNumCh(); j++){
                myControllerUniverses[i].universe[(*itLS).getChannels()[j].channel] = (*itLS).getChannels()[j].chValue;
            }
        }
    }
    
    if (artnetSetup){
        for(int i = 0; i<totalNumUn; i++){
            artnetController[i].sendArtnet(myControllerUniverses[i].universe,512); //
        }
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
            (*itLS).drawNoFill(ledStripFlag);
        } else if (drawMode == 1){
            (*itLS).drawColor();
            (*itLS).drawNoFill(ledStripFlag);
        }else if (drawMode == 2){
            (*itLS).drawColor();
        }
    }
    
    ofDrawBitmapStringHighlight("Framerate "+ofToString((int)ofGetFrameRate()), ofPoint(20, 20));
    //ofDrawBitmapStringHighlight("Mouse X "+ofToString(ofGetMouseX()), ofPoint(20, 40));
    //ofDrawBitmapStringHighlight("Mouse Y "+ofToString(ofGetMouseY()), ofPoint(20, 60));

    // ---- **** ImGUI **** ---- //
    
    ImGuiWindowFlags window_flags = 0;
    //window_flags |= ImGuiWindowFlags_NoMove;
    //window_flags |= ImGuiWindowFlags_NoResize;
    
    gui.begin();
    gui.setTheme(new MyTheme());
    ImGui::StyleColorsDark();
    ImGui::SetWindowPos(ofVec2f(1180,5),ImGuiSetCond_FirstUseEver); // ??? Doesn't work
    ImGui::Begin("Led Mapper", NULL, window_flags);
    ImGui::Text("Controller IP");
    ImGui::InputText("",ipAdress, 15);
    ImGui::Text("Num of Universes");
    ImGui::InputInt(" ",&totalNumUn);
    
        if(ImGui::Button("Save controller")){
            setupArtnet(ipAdress,totalNumUn); //Doesn't store the IP?
            artnetSetup = true;
        };
    
    ImGui::Text("Drawing Modes:");
    ImGui::RadioButton("1", &drawMode, 0); ImGui::SameLine();
    ImGui::RadioButton("2", &drawMode, 1); ImGui::SameLine();
    ImGui::RadioButton("3", &drawMode, 2);
    
    ImGui::PushID(1);
    ImGui::PushStyleColor(ImGuiCol_Button, ofColor(100));
    if(ImGui::Button("Swap SYPHON")) swapSyphon();
    ImGui::PopStyleColor(1);
    ImGui::PopID();
    ImGui::Checkbox("Syphon fullscreen", &resizeMode);
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    
    //Abans ficar les coses que toquen, si ja hem afegit una tira, despres
    
    ImGui::Text("Nex Fixture config:");
    ImGui::Text("Num LEDs");
    ImGui::InputInt("  ",&auxNumLeds);
    ImGui::Text("Start Universe");
    ImGui::InputInt("   ",&auxStartUn);
    ImGui::Text("Start Channel");
    ImGui::InputInt("    ",&auxStartCh);
    
    if (ImGui::Button("Add Nex Fixture")){
        myLedStrips.push_back(LedStrip());
        updateStripsId();
        myLedStrips.back().setup(auxNumLeds,auxStartUn,auxStartCh);
        auxStartUn = myLedStrips.back().getLastChLastUn().second;
        auxStartCh = myLedStrips.back().getLastChLastUn().first+1;
        //cout << myLedStrips.size();
    }
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Text("Edit Fixture:");
    
    if(ledStripFlag == -1){
         ImGui::TextColored(ImVec4(1.0f,0.0f,0.0f,1.0f), "No fixture selected!");
    }else {
        ImGui::Text("Some things to do here...");
    }
    
    //TO-DO: Cool delete button in red, dilema with the ledStripFlag, gets updated to -1 when pressing the button, it never erases anything.
    //ImGui::PushID(2);
    //ImGui::PushStyleColor(ImGuiCol_Button, ofColor(200,50,0));
    //if (ImGui::Button("Delete Fixture")){
    //    if ((myLedStrips.size() > 0) && (ledStripFlag != -1))
    //        myLedStrips.erase(myLedStrips.begin() + ledStripFlag);
    //}
    //ImGui::PopStyleColor(2);
    //ImGui::PopID();
    
    ImGui::End();
    //required to call this at end
    gui.end();
    
   
    
}
//--------------------------------------------------------------//
void ofApp::setupArtnet(char ipAdress[15],int totalNumUn){
    
    artnetController.clear();
    
    for ( int i = 0; i<totalNumUn; i++){
        artnetController.emplace_back();
        artnetController.back().setup(ipAdress,i);
        myControllerUniverses.emplace_back();
    }
    
}
void ofApp::updateStripsId(){
    
      for(vector<LedStrip>::iterator itID = myLedStrips.begin(); itID != myLedStrips.end(); ++itID){
          (*itID).setID(itID-myLedStrips.begin());
      }

}
//--------------------------------------------------------------//
void ofApp::keyPressed(int key)
{
    if ( key == OF_KEY_BACKSPACE || key == OF_KEY_DEL){
        if ((myLedStrips.size() > 0) && (ledStripFlag != -1)){
        myLedStrips.erase(myLedStrips.begin() + ledStripFlag);
            updateStripsId();
            if ( (myLedStrips.size() > 0)){
            auxStartUn = myLedStrips.back().getLastChLastUn().second;
            auxStartCh = myLedStrips.back().getLastChLastUn().first+1;
            } else {
                auxStartUn = 0;
                auxStartCh = 0;
            }
        }
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
     /*for(vector<LedStrip>::iterator itLS = myLedStrips.begin(); itLS != myLedStrips.end(); ++itLS){
         (*itLS).move(x,y);
     }*/
    //BUG: When we have more than two strips the ID of the first gets overwritten?
    if(ledStripFlag!= -1) myLedStrips[ledStripFlag].move(x,y);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
        /*if(button == 0){
            mouseState = true;
        }*/
    //BUG: When we have more than two strips the ID of the first gets overwritten
    for(vector<LedStrip>::iterator itLS = myLedStrips.begin(); itLS != myLedStrips.end(); ++itLS){
        if(!idAssignDone){
            if((*itLS).mouseInside(x,y) ){ //Very frequent problems detecting mouseInside ????
                cout << "MOUSE INSIDE" << endl;
                ledStripFlag = (*itLS).getId();
                idAssignDone = true;
                } else{
                    ledStripFlag = -1;
                }
        }
    cout << ledStripFlag << endl;
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
    idAssignDone = false;
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
void ofApp::swapSyphon(){
    
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
