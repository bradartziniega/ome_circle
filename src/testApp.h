#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxDelaunay.h"
#include "ofxTriangle.h"

class testApp : public ofBaseApp{
	
public:
    
       
    void setup();
    void update();
    void draw();
    
    void doTriangulation(int numPoints);
    void createApprox();
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
    
    ofColor yellowColor, blueColor, pinkColor, whiteColor;
    vector <nodeType> nodes;

    ofColor c1,c2;
    
    int numPoints;
    bool drawNodesAndEdges;
    
    circleType circleOut;
    circleType circleInner;

    vector <ofPoint> insidePoints;
    
    ofxDelaunay dTriangle;
    ofxTriangle triangles;
    
    bool networkUpdated;
    
};

#endif

