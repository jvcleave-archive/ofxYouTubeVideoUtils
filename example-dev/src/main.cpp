#include "ofMain.h"
#include "ofApp.h"
//#include "ofGLProgrammableRenderer.h"

#ifdef TARGET_OSX
#define __func__ __FUNCTION__
#endif

int main()
{
	ofSetLogLevel(OF_LOG_VERBOSE);
	//ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
	ofSetupOpenGL(1920, 1080, OF_WINDOW);
	ofRunApp( new ofApp());
}