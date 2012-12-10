#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
	ofBackground(20,186,204);
    ofEnableSmoothing();
    ofSetCircleResolution(100);
    
    circleOut.radius = (ofGetHeight() - 200)/2.0;
    circleInner.radius = circleOut.radius - 150;
    
    circleOut.xPos = ofGetWidth()/2.0;
    circleOut.yPos = ofGetHeight()/2.0;
    
    circleInner.xPos = circleOut.xPos;
    circleInner.yPos = circleOut.yPos;
    
    
    createApprox();
    
    numPoints = 500;
    
    //add generic clearing and generation of everything
    
    seedPoints(numPoints);
    
    dTriangle.triangulate();
    
    networkUpdated = false;
    
    yellowColor = ofColor(255,239,122);
    blueColor = ofColor(20,186,204);
    pinkColor = ofColor(255,0,138);
    whiteColor = ofColor(255,255,255);
    
}

//--------------------------------------------------------------
void testApp::createApprox(){
    
    float i=0;
    while (i< TWO_PI){
        
        float x_outer = circleOut.xPos + cos(i)*circleOut.radius;
        float y_outer = circleOut.yPos + sin(i)*circleOut.radius;
        
        float x_inner = circleInner.xPos + cos(i)*circleInner.radius;
        float y_inner = circleInner.yPos + sin(i)*circleInner.radius;
        
        circleOut.thisPath.addVertex(ofVec2f(x_outer,y_outer));
        circleInner.thisPath.addVertex(ofVec2f(x_inner,y_inner));
        
       // dTriangle.addPoint(x_outer,y_outer,1);
       // dTriangle.addPoint(x_inner,y_inner,1);
        
        i+=0.1;
        
    }
    
    circleOut.thisPath.close();
    circleInner.thisPath.close();
    
}

//--------------------------------------------------------------
void testApp::seedPoints(int numPoints){
    
    int numInside = 0;
    
    ofPoint curPoint;
    
    while(numInside<numPoints){
        
        
        float xRand = ofRandom(circleOut.thisPath.getBoundingBox().x,circleOut.thisPath.getBoundingBox().x+circleOut.thisPath.getBoundingBox().width);
        float yRand = ofRandom(circleOut.thisPath.getBoundingBox().y,circleOut.thisPath.getBoundingBox().y+circleOut.thisPath.getBoundingBox().height);
        
        if(circleOut.thisPath.inside(xRand,yRand) && !circleInner.thisPath.inside(xRand, yRand)){
            
            numInside++;
            curPoint.set(xRand,yRand);
            insidePoints.push_back(curPoint);
            dTriangle.addPoint(xRand,yRand,1);
            
        }
        
    }
    
    
    
}

//--------------------------------------------------------------
void testApp::update(){
    
    
    int curID = 0;
    
    if(!networkUpdated){
        
        for(int i=0;i<dTriangle.triangles.size();i++){
            
            ofPoint tp1 = ofPoint(dTriangle.vertices[dTriangle.triangles[i].p1].x,dTriangle.vertices[dTriangle.triangles[i].p1].y,1);
            ofPoint tp2 = ofPoint(dTriangle.vertices[dTriangle.triangles[i].p2].x,dTriangle.vertices[dTriangle.triangles[i].p2].y,1);
            ofPoint tp3 = ofPoint(dTriangle.vertices[dTriangle.triangles[i].p3].x,dTriangle.vertices[dTriangle.triangles[i].p3].y,1);
            
            float xAv = (float)(tp1.x + tp2.x + tp3.x)/3.0;
            float yAv = (float)(tp1.y + tp2.y + tp3.y)/3.0;
            
            ofPoint cP = ofPoint(xAv,yAv);
            
            
            if(!circleInner.thisPath.inside(cP.x,cP.y) && circleOut.thisPath.inside(cP.x, cP.y)){
                
                ofColor c = ofColor(0,0,0);
                
                //if has hole
                nodes.push_back(nodeType());
                nodes.back().thisPoint = tp1;
                nodes.back().fadeValue = 1.0;
                nodes.back().nodeColor =  c;
                nodes.back().isOn = true;
                nodes.back().ID = curID;
                
                curID++;
                
                nodes.push_back(nodeType());
                nodes.back().thisPoint = tp2;
                nodes.back().fadeValue = 1.0;
                nodes.back().nodeColor =  c;
                nodes.back().isOn = true;
                nodes.back().ID = curID;
                
                
                curID++;
                
                nodes.push_back(nodeType());
                nodes.back().thisPoint = tp3;
                nodes.back().fadeValue = 1.0;
                nodes.back().nodeColor =  c;
                nodes.back().isOn = true;
                nodes.back().ID = curID;
                
                nodes[curID-2].connectID[0] = curID;
                nodes[curID-2].connectID[1] = curID-1;
                
                nodes[curID-1].connectID[0] = curID-2;
                nodes[curID-1].connectID[1] = curID;
                
                nodes[curID].connectID[0] = curID-2;
                nodes[curID].connectID[1] = curID-1;
                
                curID++;
                
            }
        }
        
        networkUpdated = true;
        
    }
    
	
}

//--------------------------------------------------------------
void testApp::drawNetwork(){
    
    
    
    
    
    for(int i=0;i<nodes.size();i++){
        
        
        if(nodes[i].isOn){
            
            int indVal1 = nodes[i].connectID[0];
            int indVal2 = nodes[i].connectID[1];
            
            glBegin(GL_LINE_STRIP);
            
            glColor4f(nodes[i].nodeColor.r/255.0,nodes[i].nodeColor.g/255.0,nodes[i].nodeColor.b/255.0,nodes[indVal1].fadeValue);
            glVertex3f(nodes[indVal1].thisPoint.x, nodes[indVal1].thisPoint.y, nodes[indVal1].thisPoint.z);
            
            glColor4f(nodes[i].nodeColor.r/255.0,nodes[i].nodeColor.g/255.0,nodes[i].nodeColor.b/255.0,nodes[indVal1].fadeValue);
            glVertex3f(nodes[i].thisPoint.x, nodes[i].thisPoint.y, nodes[i].thisPoint.z);
            
            glEnd();
            
            glBegin(GL_LINE_STRIP);
            
            glColor4f(nodes[i].nodeColor.r/255.0,nodes[i].nodeColor.g/255.0,nodes[i].nodeColor.b/255.0,nodes[indVal1].fadeValue);
            glVertex3f(nodes[indVal2].thisPoint.x, nodes[indVal2].thisPoint.y, nodes[indVal2].thisPoint.z);
            
            glColor4f(nodes[i].nodeColor.r/255.0,nodes[i].nodeColor.g/255.0,nodes[i].nodeColor.b/255.0,nodes[indVal1].fadeValue);
            glVertex3f(nodes[i].thisPoint.x, nodes[i].thisPoint.y, nodes[i].thisPoint.z);
            
            glEnd();
            
            //main point
            
            ofColor nodeColor = ofColor(0,0,0);
            
            bool fillNodes = true;
            
            if(fillNodes){
                
                ofFill();
                ofSetColor(nodes[i].nodeColor.r,nodes[i].nodeColor.g,nodes[i].nodeColor.b,nodes[i].fadeValue*255.0);
                ofCircle(nodes[i].thisPoint,2);
                
                ofFill();
                ofSetColor(nodes[indVal1].nodeColor.r,nodes[indVal1].nodeColor.g,nodes[indVal1].nodeColor.b,nodes[indVal1].fadeValue*255.0);
                ofCircle(nodes[indVal1].thisPoint,2);
                
                ofFill();
                ofSetColor(nodes[indVal2].nodeColor.r,nodes[indVal2].nodeColor.g,nodes[indVal2].nodeColor.b,nodes[indVal2].fadeValue*255.0);
                ofCircle(nodes[indVal2].thisPoint,2);

            }
            

            
            
            ofNoFill();
            ofSetColor(nodeColor,nodes[i].fadeValue*255);
            ofCircle(nodes[i].thisPoint,2);
            
            
             //connect pt1
                        ofNoFill();
            ofSetColor(nodeColor,nodes[indVal1].fadeValue*255);
            ofCircle(nodes[indVal1].thisPoint,2);
            
            
             
             //connect pt2
                    
            ofNoFill();
            ofSetColor(nodeColor,nodes[indVal2].fadeValue*255.0);
            ofCircle(nodes[indVal2].thisPoint,2);
            
            
            
        }
        
    }
    
    
}
//--------------------------------------------------------------
void testApp::drawCircle(){
    
    
    
}


//--------------------------------------------------------------
void testApp::draw(){
    
    
    ofBackground(255,255,255);
    
    ofNoFill();
    ofSetLineWidth(1.0);
    /*
     circleOut.thisPath.draw();
     circleInner.thisPath.draw();
     //ofEllipse(circleOut.xPos, circleOut.yPos, circleOut.radius, circleOut.radius);
     //ofEllipse(circleInner.xPos, circleInner.yPos, circleInner.radius, circleInner.radius);
     
     
     
     for(int i=0;i<numPoints;i++){
     ofNoFill();
     ofSetLineWidth(1.0);
     ofSetColor(255, 233,80);
     ofEllipse(insidePoints[i].x, insidePoints[i].y, 6, 6);
     }
     */
    drawNetwork();
    
    drawCircle();
    
    
    
    
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){
    
    
    
    
}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    //xValues[0] = (x-ofGetWidth()/2);
    //yValues[0] = (y-ofGetHeight()/2);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(){
    
}
