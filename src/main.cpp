#include "testApp.h"
#include "ofAppGlutWindow.h"

//--------------------------------------------------------------
int main(){
	ofAppGlutWindow window; // create a window
	// set width, height, mode (OF_WINDOW or OF_FULLSCREEN)
    window.setGlutDisplayString("rgba double samples>=4");
	ofSetupOpenGL(&window, 1440, 700, OF_WINDOW);
	ofRunApp(new testApp()); // start the app
}
