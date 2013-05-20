#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
	ofBackground(20,186,204);
    ofEnableSmoothing();
    ofSetCircleResolution(100);
    
    circleOut.radius = (ofGetHeight() - 200)/2.0;
    circleInner.radius = circleOut.radius - 225;
    
    circleOut.xPos = ofGetWidth()/2.0;
    circleOut.yPos = ofGetHeight()/2.0;
    
    circleInner.xPos = circleOut.xPos;
    circleInner.yPos = circleOut.yPos;
    
    
    createApprox();
    generateNoisePoints();
    
    
    numSteps = 8;
    numPoints = 1040/numSteps;
    curStep = 0;
    //add generic clearing and generation of everything
    
    doTriangulation(numPoints);
    
    c1 = ofColor(10,10,10);
    c2 = ofColor(250,250,250);
    
    drawNodesAndEdges = false;
    
    capture = false;
    
}


//--------------------------------------------------------------
void testApp::generateNoisePoints(){
    
    ofPoint curPoint;
    
    for(int x=circleOut.xPos-circleOut.radius;x<circleOut.xPos+circleOut.radius;x++){
        for(int y=circleOut.yPos-circleOut.radius;y<circleOut.yPos+circleOut.radius;y++){
            
            if(circleOut.thisPath.inside(x,y) && !circleInner.thisPath.inside(x, y)){
                float a = x * .01 ;
                float b = y * .01 ;
                float c = ofRandom(1);
                float noise = ofNoise(a,b,c) * 255;
                
                if(noise>170){
                    curPoint.set(x,y);
                    noisePoints.push_back(noiseType());
                    noisePoints.back().point = curPoint;
                    noisePoints.back().used = false;
                    //noisePoints.point.push_back(curPoint);
                }
            }
        }
    }
    
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
        
        i+=0.1;
        
    }
    
    circleOut.thisPath.close();
    circleInner.thisPath.close();
    
}

//--------------------------------------------------------------
void testApp::doTriangulation(int numPoints){
    
    int numInside = 0;
    ofPoint curPoint;
    
    //resetting info
    dTriangle.triangleMesh.clear();
    dTriangle.triangles.clear();
    nodes.clear();
    
    int subtracted_amt = 0;
    
    if(curStep==0){ subtracted_amt=stepsToProcess[curStep];}
    else{
        subtracted_amt = stepsToProcess[curStep] - stepsToProcess[curStep-1];
    }
    
    while(numInside<subtracted_amt){
        
        float xRand = ofRandom(circleOut.thisPath.getBoundingBox().x,circleOut.thisPath.getBoundingBox().x+circleOut.thisPath.getBoundingBox().width);
        float yRand = ofRandom(circleOut.thisPath.getBoundingBox().y,circleOut.thisPath.getBoundingBox().y+circleOut.thisPath.getBoundingBox().height);
        
        if(circleOut.thisPath.inside(xRand,yRand) && !circleInner.thisPath.inside(xRand, yRand)){
            numInside++;
            curPoint.set(xRand,yRand);
            dTriangle.addPoint(xRand,yRand,1);
        }
        
    }
    
    int noiseCount = 0;
    
    while(noiseCount<200){
        
        int output = 0 + (rand() % (int)(noisePoints.size() - 0 + 1));
        if(noisePoints[output].used==false){
            dTriangle.addPoint(noisePoints[output].point.x,noisePoints[output].point.y,1);
            noisePoints[output].used = true;
        }
        
        noiseCount++;
    }
    
    dTriangle.triangulate();
    networkUpdated = false;
    
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
                nodes.back().fadeValue = ofRandom(0,1);
                nodes.back().nodeColor =  c;
                nodes.back().isOn = true;
                nodes.back().ID = curID;
                
                curID++;
                
                nodes.push_back(nodeType());
                nodes.back().thisPoint = tp2;
                nodes.back().fadeValue = ofRandom(0,1);
                nodes.back().nodeColor =  c;
                nodes.back().isOn = true;
                nodes.back().ID = curID;
                
                
                curID++;
                
                nodes.push_back(nodeType());
                nodes.back().thisPoint = tp3;
                nodes.back().fadeValue = ofRandom(0,1);
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
            
            
            float min = nodes[i].thisPoint.y;
            float max = nodes[indVal1].thisPoint.y;
            
            if(nodes[indVal1].thisPoint.y > max) max = nodes[indVal1].thisPoint.y;
            if(nodes[indVal2].thisPoint.y > max) max = nodes[indVal2].thisPoint.y;
            
            if(nodes[indVal1].thisPoint.y < min) min = nodes[indVal1].thisPoint.y;
            if(nodes[indVal2].thisPoint.y < min) min = nodes[indVal2].thisPoint.y;
            
            
            
            glBegin(GL_TRIANGLES);
            
            float percentT = ofMap(min,(ofGetHeight()/2.0)-circleOut.radius,ofGetHeight()/2.0 + circleOut.radius,0,100); //ofMap(min, 0, ofGetHeight(), 0, 100);
            float percentB = ofMap(max,(ofGetHeight()/2.0)-circleOut.radius,ofGetHeight()/2.0 + circleOut.radius,0,100); //ofMap(max, 0, ofGetHeight(), 0, 100.0);
            
            ofColor cT;
            ofColor cB;
            
            cT.r = ((float)(c1.r) - (float)(c2.r)) * (percentT/100.0) + (float)(c2.r);
            cB.r = ((float)(c1.r) - (float)(c2.r)) * (percentB/100.0) + (float)(c2.r);
            
            cT.g = ((float)(c1.g) - (float)(c2.g)) * (percentT/100.0) + (float)(c2.g);
            cB.g = ((float)(c1.g) - (float)(c2.g)) * (percentB/100.0) + (float)(c2.g);
            
            cT.b = ((float)(c1.b) - (float)(c2.b)) * (percentT/100.0) + (float)(c2.b);
            cB.b = ((float)(c1.b) - (float)(c2.b)) * (percentB/100.0) + (float)(c2.b);
            
            
             glColor3f( cB.r/255.0, (float)cB.g/255.0, (float)cB.b/255.0 );
             glVertex3f(nodes[i].thisPoint.x,nodes[i].thisPoint.y, 1);    // lower left vertex
             glColor3f( cT.r/255.0, cT.g/255.0,cT.b/255.0 );
             glVertex3f(nodes[indVal1].thisPoint.x,nodes[indVal1].thisPoint.y, 1);    // lower left vertex
             glVertex3f(nodes[indVal2].thisPoint.x,nodes[indVal2].thisPoint.y, 1);    // lower left vertex
             
            
            glEnd();
            
            
            if(drawNodesAndEdges){
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
                
                
                output.line(nodes[indVal1].thisPoint.x, nodes[indVal1].thisPoint.y, nodes[i].thisPoint.x, nodes[i].thisPoint.y);
                output.line(nodes[i].thisPoint.x,nodes[i].thisPoint.y,nodes[indVal2].thisPoint.x,nodes[indVal2].thisPoint.y);
                output.line(nodes[indVal2].thisPoint.x,nodes[indVal2].thisPoint.y,nodes[indVal1].thisPoint.x, nodes[indVal1].thisPoint.y);
                

                
                //main point
                /*
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
                 //ofCircle(nodes[i].thisPoint,2);
                 
                 
                 //connect pt1
                 ofNoFill();
                 ofSetColor(nodeColor,nodes[indVal1].fadeValue*255);
                 //ofCircle(nodes[indVal1].thisPoint,2);
                 
                 
                 
                 //connect pt2
                 
                 ofNoFill();
                 ofSetColor(nodeColor,nodes[indVal2].fadeValue*255.0);
                 //ofCircle(nodes[indVal2].thisPoint,2);
                 */
            }
            
            
        }
        
    }
    

    
}
//--------------------------------------------------------------
void testApp::drawCircle(){
    
    
    
}


//--------------------------------------------------------------
void testApp::draw(){
    
    if(capture){
        string fName = "test" + ofToString(curStep) + ".ps";
        output.beginEPS(fName);
        output.noFill();
        output.setColor(0x000000);
    }
    ofBackground(255,255,255);
    ofNoFill();
    ofSetLineWidth(1.0);
    drawNetwork();
    drawCircle();
    
    if(capture){
        output.endEPS();
        capture = !capture;
    }
    
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){
    
    
    if(key=='c'){
        if(drawNodesAndEdges){
            drawNodesAndEdges=false;
        }
        else{
            drawNodesAndEdges=true;
        }
    }
    
    if(key==' '){
        doTriangulation(numPoints);
    }
    
    if(key=='a'){
        if(curStep<8){
            curStep++;
            doTriangulation(numPoints);
        };
        
    }
    
}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
    capture = true;
    
    //numPoints = ofMap(mouseX, 0, ofGetWidth(), 10, 1250);
    // doTriangulation(numPoints);
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(){
    
}
