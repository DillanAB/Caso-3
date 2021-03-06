#include "Observer.hpp"
#include "RoutingPoint.hpp"
#include <math.h>
#ifndef ROUTER_HEADER
#define ROUTER_HEADER
using namespace std;

double xInitialize, yInitialize, xEnd, yEnd, workPos, control, sortingControl= 0;
int controlFrame = 1;

void assignCoordinates(PathPoint *pPathPoint, double pHight, double pWidth);
void endPoint(PathPoint *pPathPoint, double pAngle, double pHight, double pWidth);
vector<vector<Figure>> createFrame(PathPoint *pPathPoint, Figure pFigure, int pTotalFrame, Frame 
pClassFrame, double pathX1, double pathY1, double pHight, double pWidth, int xFinal, int yFinal);
void cleanPoint(PathPoint *pPathPoint);
void sortingFrames(vector<vector<Figure>> pVectorFrame);
void finalSorting(vector<vector<Figure>> pVectorFrame);

class Router: public Subject, public Observer{
private:
   double * heightPtr;
   double * widthPtr;
   double angle;
   int framesAmount;
   PathPoint * pathPoint;
public:

   Router(PathPoint * pPathPoint, double pAngle, int pFrames, double * pHeight, double * pWidth){
      pathPoint = pPathPoint;
      heightPtr = pHeight;
      widthPtr = pWidth;
      angle = pAngle;
      framesAmount = pFrames;
   }
    
    /*

   Calculates the distance from the current point to the end, following an assigned angle
   Solution is O(n) because the N is traversed only once

   */
   PathPoint* routingFunction(PathPoint*pPathPoint , double pHight, double pWidth, double pAngle, int pFrame){
      double distance = 0;
      assignCoordinates(pPathPoint, pHight, pWidth);
      double result, pathX1, pathY1, xFinal, yFinal, costoRuta, position;
      double xInicial = pPathPoint->getXInicial();
      double yInicial = pPathPoint->getYInicial();
      int frameRes;
      Frame classFrame;
      Frame array[pFrame];
         
      for(int i = 0; i < pPathPoint->getFiguras().size(); i++)
      { 
         endPoint(pPathPoint, pAngle, pHight, pWidth);
         pathX1 = xInitialize; pathY1 = yInitialize; xFinal = xEnd; yFinal = yEnd; position = workPos;
            
         //Calculate the distance between the start and end point
         //Raiz ((xFinal - pathX1)?? + (yFinal-pathY1)??)
         distance = sqrt(pow(xFinal-pathX1,2) + pow(yFinal-pathY1,2)); 
         costoRuta = distance/pFrame; //
         createFrame(pPathPoint,pPathPoint->getFiguras()[i],pFrame,classFrame,pathX1,pathY1,pHight,pWidth, xFinal, yFinal);
      }
      control = 0;
      frameOrder.frames.size();
      return pPathPoint;
   }

   void update(void * pPointer){
    double width = *widthPtr;
    double height = *widthPtr;

    routingFunction(pathPoint, height, width, angle, framesAmount);
    sortingFrames(pathPoint->getVectorVector());
    cleanPoint(pathPoint);
    bool last = *(bool*)pPointer;

    //When all the points are passed, generation is called to make the svg
    if(last){
        for(int i = 0; i < frameOrder.frames.size();i++){
            vector<Figure> figures = frameOrder.frames[i];
            notify(&figures); 
        }
        frameOrder.frames.clear();
    }
    }
   void detach(){}
};
/*
    Create a square and take the x's and y's out of each corner. 
    This square works as a limit to later generate the figures

*/
void assignCoordinates(PathPoint *pPathPoint, double pHight, double pWidth){

    double pathX1 = pPathPoint->getXInicial();double pathY1 = pPathPoint->getYInicial();   
    int xIncrease, yIncrease = 0;
    double pathX2 = pPathPoint->getX2();double pathY2 = pPathPoint->getY2();
    double pathX3 = pPathPoint->getX3();double pathY3 = pPathPoint->getY3();
    double pathX4 = pPathPoint->getX4();double pathY4 = pPathPoint->getY4();
 
    if(pHight <= 2000){
        xIncrease = 100; yIncrease = 70;
    }if(pHight > 2000 || pWidth > 2000){
        xIncrease = 250 ;yIncrease = 150;
    }
    if( ((pathY1 - yIncrease) <= 0) && ((pathX1 + xIncrease) > pWidth)){ //Right upper corner
        pathX2 = pathX1 - xIncrease; pathY2 = pathY1;
        pathX3 = pathX1; pathY3 = pathY1;
        pathX4 = pathX1; pathY4 = pathY1 + yIncrease;
        pathX1 = pathX1 - xIncrease; pathY1 = pathY1 + yIncrease;   
    }
    else if(((pathX1 + xIncrease) < pWidth) && ((pathY1 - yIncrease) <= 0)){//Upper left corner
        pathX2 = pathX1; pathY2 = pathY1;
        pathX3 = pathX1+xIncrease; pathY3 = pathY1;
        pathX4 = pathX1 + xIncrease; pathY4 = pathY1 + yIncrease;
        pathX1 = pathX1; pathY1 = pathY1 + yIncrease;
    }
    else if((pathX1 + xIncrease) >= pWidth){ //Lower right corner
        pathX2 = pathX1 - xIncrease; pathY2 = pathY1 - yIncrease;
        pathX3 = pathX1; pathY3 = pathY1 - yIncrease;
        pathX4 = pathX1; pathY4 = pathY1;
        pathX1 = pathX1 - xIncrease; pathY1 = pathY1;
    }
    else{ //Lower left corner
        pathX2 = pathX1;  pathY2 = pathY1 - yIncrease;
        pathX3 = pathX1 + xIncrease; pathY3 = pathY1 - yIncrease;
        pathX4 = pathX1 + xIncrease; pathY4 = pathY1;
    }
    pPathPoint->setXYInitial(pathX1,pathY1);
    pPathPoint->setxySecond(pathX2,pathY2);
    pPathPoint->setxyThird(pathX3,pathY3);
    pPathPoint->setxyFourth(pathX4,pathY4);
    pPathPoint->setxyIncrease(xIncrease,yIncrease);
}

//To give the endpoints of the routing, it is based on the angle
void endPoint(PathPoint *pPathPoint, double pAngle, double pHight, double pWidth){
    
    double result;
    double pathX1,pathY1,xFinal,yFinal,position = 0;   
    pathX1 = pPathPoint->getXInicial();
    pathY1 = pPathPoint->getYInicial();

    if(pAngle <= 45){
        pathX1 = pPathPoint->getX3(); pathY1 = pPathPoint->getY3();
        result = (pathY1 -(pAngle * pathY1)/45) ;
        yFinal = result; xFinal = pWidth; position = 1;
    }
    else if (pAngle > 45 && pAngle <= 90){
        pathX1 = pPathPoint->getX2(); pathY1 = pPathPoint->getY2();
        result = (pWidth - ((pAngle-45)*(pWidth - pathX1))/45);
        xFinal = result; yFinal = 0; position = 2;
    }
    else if( pAngle > 90 && pAngle <= 135){
        pathX1 = pPathPoint->getX2(); pathY1 = pPathPoint->getY2();
        result = (pathX1 - (((pAngle - 90) * pathX1)/45));
        xFinal = result; yFinal = 0;position = 3;
    }
    else if(pAngle > 135 && pAngle <= 180){
        result = ((pAngle - 135) * pathY1)/45;
        yFinal = result; xFinal = 0;position = 4;
    }
    else if(pAngle > 180 && pAngle <= 225){
        result = pathY1 + (((pAngle - 180)* (pHight - pathY1))/45);
        yFinal = result; xFinal = 0;position = 5;
    }
    else if(pAngle > 225 && pAngle <= 270){ 
        result = ((pAngle - 225) * pathX1)/45;
        xFinal = result; yFinal = pHight;position = 6;
    }
    else if(pAngle > 270 && pAngle <= 315){
        pathX1 = pPathPoint->getX4(); pathY1 = pPathPoint->getY4();
        result = pathX1 + (((pAngle - 270) * (pWidth - pathX1))/45);
        xFinal = result; yFinal = pHight;position = 7;
    }
    else if(pAngle > 315){
        pathX1 = pPathPoint->getX4(); pathY1 = pPathPoint->getY4();
        result = pHight - ((pAngle - 315) * (pHight - pathY1))/45;
        yFinal = result; xFinal = pWidth;position = 8;
    }
    xInitialize = pathX1; yInitialize = pathY1; xEnd = xFinal; yEnd = yFinal; workPos = position;
}
/*
    This function calculates the movements that the figure must make to move 
    according to the given angle and the end point.
*/
vector<vector<Figure>> createFrame(PathPoint *pPathPoint, Figure pFigure, int pTotalFrame, Frame 
pClassFrame, double pathX1, double pathY1, double pHight, double pWidth, int xFinal, int yFinal){

    vector<Figure> figuras;
    vector<Figure> figuras_;
    double curve1, curve2, curve3, curve4, line1, line2, horizontal1, vertical1,pMove1, pMove2 ;
    double xInicial = pPathPoint->getXInicial(); double yInicial = pPathPoint->getYInicial();

    for(int j = 0; j < pTotalFrame; j++){
        int num = 10+rand()%(61-10); 
        pClassFrame.name = j;
        pPathPoint->setArray(j,pClassFrame);

        if(workPos == 1){              
            pMove1 = xInicial + ((pWidth - pathX1) / (pTotalFrame+1)) * (j+1); pMove2 = yInicial - ((pathY1 / (pTotalFrame+1)) * (j+1));
        }
        else if(workPos == 2){
            pMove2 = yInicial - ((pathY1 / pTotalFrame) * (j+1)); pMove1 = xInicial + (((pathX1 - xFinal)/pTotalFrame) * (j+1));
        }
        else if(workPos ==  3 || workPos == 4){
            pMove2 = yInicial - (((pathY1-yFinal)/pTotalFrame) * (j+1)); pMove1 = xInicial - ((pathX1 /pTotalFrame) * (j+1));
        }
        else if(workPos ==  5 || workPos == 6){
            pMove2 = yInicial + (((yFinal-pathY1)/pTotalFrame) * (j+1)); pMove1 = xInicial - (((pathX1 - xFinal)/pTotalFrame) * (j+1));            
        }
        else if(workPos ==  7 || workPos == 8){
            pMove2 = yInicial + (((yFinal-pathY1)/pTotalFrame) * (j+1));pMove1 = xInicial + (((xFinal - pathX1)/pTotalFrame) * (j+1));
        }
        pFigure.route[0] = pMove1; pFigure.route[1] = pMove2; 

        if((pFigure.type == "Q") || (pFigure.type == "C")|| (pFigure.type == "A")|| (pFigure.type == "S")){ 
            curve1 = (((pMove1 + pPathPoint->getxIncrease())-pMove1) / 2) + pMove1; curve2 = pMove2 - pPathPoint->getyIncrease();
            curve3 = pMove1 + pPathPoint->getxIncrease(); curve4 = pMove2 - num; 
            pFigure.type = "Q";
            pFigure.route[2] = curve1; pFigure.route[3] = curve2; 
            pFigure.route[4] = curve3; pFigure.route[5] = curve4;
            pPathPoint->setArrayPos(j,pFigure);
        }
        else if(pFigure.type == "L"){
            line1 = pMove1 + pPathPoint->getxIncrease(); line2 = (pMove2 - pPathPoint->getyIncrease()) + num;
            pFigure.route[2] = line1; pFigure.route[3] = line2;
        }
        else if(pFigure.type == "H"){
            pMove1 = pMove1 + num; pMove2 = pMove2 - num;
            pFigure.route[0] = pMove1; pFigure.route[1] = pMove2 - pPathPoint->getyIncrease(); 
            horizontal1 = pMove1 + pPathPoint->getxIncrease(); pFigure.route[2] = horizontal1;
        }
        else if(pFigure.type == "V"){
            pMove1 = pMove1 + pPathPoint->getxIncrease(); vertical1 = pMove2 - (pPathPoint->getyIncrease() - num);
            pFigure.route[0] = pMove1; pFigure.route[2] = vertical1;
        }
        if (control < pTotalFrame){
            figuras.push_back(pFigure); //This is a vector that has figures inside
            pPathPoint->setVectorVectorFiguras(figuras); //Put the vector into another vector
            figuras.clear();
        }
        else{
        	pPathPoint->setVectorVectorFigurasFigura(pFigure,j);
        }
        if(sortingControl < pTotalFrame){
            frameOrder.frames.push_back(figuras_);
        }
        control ++;
        sortingControl ++;
        
    }
    return pPathPoint->getVectorVector();
}

/*
This function sorts the shapes by frame.
It saves them in a structure that will later be used for the generation part
*/
void sortingFrames(vector<vector<Figure>> pVectorFrame){
    for(int a = 0; a < pVectorFrame.size(); a++){
        for(int j = 0; j < pVectorFrame[a].size(); j++ ){
            frameOrder.frames[a].push_back(pVectorFrame[a][j]);
        }
    }
}

void cleanPoint(PathPoint *pPathPoint){
	pPathPoint->cleanFiguras();
	pPathPoint->cleanFrames();
	pPathPoint->cleanrouting();
}
#endif
