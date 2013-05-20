#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxDelaunay.h"
#include "ofxTriangle.h"
#include "ofxVectorGraphics.h"

class testApp : public ofBaseApp{
	
public:
    
       
    void setup();
    void update();
    void draw();
    
    void doTriangulation(int numPoints);
    void createApprox();
    void generateNoisePoints();
    void drawNetwork();
    void drawCircle();
    
    void keyPressed  (int key);
    void keyReleased (int key);
    
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased();
    
    typedef struct {
    
        float radius;
        float xPos;
        float yPos;
        ofPolyline thisPath;
    
    } circleType;
    
    typedef struct{
        
        ofPoint  thisPoint;
        ofColor nodeColor;
        float fadeValue;
        bool isOn;
        int ID;
        int connectID[2];
        bool isInside;
        float fadeAddAmt;
        
    } nodeType;
    
    typedef struct{
        ofPoint point;
        bool used = false;
    } noiseType;
    
    bool capture;
    
    //int stepsToProcess[8]  = {1,40,100,150,200,400,700,1000};
    int stepsToProcess[8]  = {1,20,50,150,200,400,700,1000};

    ofxVectorGraphics output;
    
    ofColor yellowColor, blueColor, pinkColor, whiteColor;
    vector <nodeType> nodes;
    vector <noiseType> noisePoints;
    
    ofColor c1,c2;
    
    int numPoints, numSteps, curStep;
    bool drawNodesAndEdges;
    
    circleType circleOut;
    circleType circleInner;

    vector <ofPoint> insidePoints;
    
    ofxDelaunay dTriangle;
    ofxTriangle triangles;
    int noisePointsDivider;
    
    bool networkUpdated;
    
};

#endif

