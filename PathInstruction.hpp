#include <iostream>
#include <cmath>
#include <vector>
#include <array>
#define Y_SIZE 1000
#define X_SIZE 1000
#define NEAR_SIZE 10
#define DECREASING 0
#define INCREASING 1

using namespace std;

class PathInstruction{
protected:
    bool inverted;
    bool absolute;
    string color;
    float initialX;
    float finalX;
    float initialY;
    float finalY;
public:
    //PathInstruction();
    virtual bool isInRange(float pXPosition, float pYPosition) = 0;
    virtual void adjustSize(float pXPosition, float pYPosition) = 0;
    virtual void convertToAbs() = 0;
    virtual string convertToString() = 0;
    void printValues(){
        cout<<"Ix: "<<initialX<<"Iy: "<<initialY<<endl;
    }
    void setAbsolute(bool pBoolAbs){
        absolute = pBoolAbs;
    }
    void setInitialPoint(float pInitialX, float pInitialY){
        initialX = pInitialX;
        initialY = pInitialY;
    }
    void invertInitFinalPoint(){
        inverted = true;
        float auxValue;
        auxValue = initialX;
        initialX = finalX;
        finalX = auxValue;
        
        auxValue = initialY;
        initialY = finalY;
        finalY = auxValue;
    }   
    float getInitialX(){
        return initialX;
    }
    float getInitialY(){
        return initialY;
    }
    float getFinalX(){
        return finalX;
    }
    float getFinalY(){
        return finalY;
    }
    bool getAbsolute(){
        return absolute;
    }
    bool isPositive(){
        return initialX<finalX;
    }
};

class HorizontalLine: public PathInstruction{
public:
  HorizontalLine(float pFinalX){
    finalX = pFinalX;
  }
  HorizontalLine(float pInitialX, float pFinalX, float pYPosition, string pColor){
    color    = pColor;
    initialX = pInitialX;
    finalX   = pFinalX;
    initialY = finalY = pYPosition;
  }

  bool isInRange(float pXPosition, float pYPosition){
    bool inXRange = (initialX<=(pXPosition+NEAR_SIZE)&&finalX>=(pXPosition-NEAR_SIZE));
    bool inYRange = (initialY>(pYPosition-NEAR_SIZE)&&initialY<(pYPosition+NEAR_SIZE));
    return (inXRange&&inYRange);
  }

  void adjustSize(float pXPosition, float pYPosition){
    float minX  = pXPosition-NEAR_SIZE;
    float maxX  = pXPosition+NEAR_SIZE;

    initialX = (initialX < minX)  ? minX:initialX;
    finalX = (finalX > maxX) ? maxX:finalX;
    return;
  }

  void convertToAbs(){
    finalY = initialY;
    finalX += initialX;
  }

    string convertToString(){
      string strInstruction = "H ";
      strInstruction += to_string(finalX) + " ";
      return strInstruction;
    }
};

class VerticalLine: public PathInstruction{
public:
    VerticalLine(float pFinalY){
        finalY = pFinalY;
    }
    VerticalLine(float pInitialY, float pFinalY, float pXPosition, string pColor){
        color    = pColor;
        initialY = pInitialY;
        finalY   = pFinalY;
        initialX = finalX = pXPosition;
    }

    bool isInRange(float pXPosition, float pYPosition){
        bool inYRange = (initialY<=(pYPosition+NEAR_SIZE)&&finalY>=(pYPosition-NEAR_SIZE));
        bool inXRange = (initialX<=(pXPosition+NEAR_SIZE)&&initialX>=(pXPosition-NEAR_SIZE));
        return (inYRange&&inXRange);
    }
    
    void adjustSize(float pXPosition, float pYPosition){
        float minY  = pYPosition-NEAR_SIZE;
        float maxY  = pYPosition+NEAR_SIZE;

        initialY = (initialY < minY)  ? minY:initialY;
        finalY = (finalY > maxY) ? maxY:finalY;
        return;
    }

    void convertToAbs(){
        finalX = initialX;
        finalY += initialY;
    }
    string convertToString(){
      string strInstruction = "V ";
      strInstruction += to_string(finalY) + " ";
      return strInstruction;
    }
};

class Line: public PathInstruction{
private:
    unsigned short type;
public:
    Line(float pFinalX, float pFinalY){
        finalX = pFinalX;
        finalY = pFinalY;
        type = (initialY<finalY) ? INCREASING:DECREASING;
    }
    Line(float pInitialX, float pFinalX, float pFirstY, float pFinalY){
        initialX = pInitialX;
        finalX   = pFinalX;
        initialY = pFirstY;
        finalY   = pFinalY;
        type = (initialY<finalY) ? INCREASING:DECREASING;
    }
    bool isInRange(float pXPosition, float pYPosition){
        bool inXRange, inYRange;
        if(INCREASING){
            inXRange = (initialX<=(pXPosition+NEAR_SIZE)&&finalX>=(pXPosition-NEAR_SIZE));
            inYRange = (initialY>=(pYPosition-NEAR_SIZE)&&finalY>=(pYPosition+NEAR_SIZE));
        }
        else{
            inXRange = (initialX<=(pXPosition+NEAR_SIZE)&&finalX>=(pXPosition-NEAR_SIZE));
            inYRange = (initialY<=(pYPosition+NEAR_SIZE)&&finalY>=(pXPosition-NEAR_SIZE));
        }
        return (inXRange&&inYRange);
    }
    void adjustSize(float pXPosition, float pYPosition){
        float minX  = pXPosition-NEAR_SIZE;
        float maxX  = pXPosition+NEAR_SIZE;
        float minY  = pYPosition-NEAR_SIZE;
        float maxY  = pYPosition+NEAR_SIZE;

        initialX = (initialX<minX) ? minX:initialX;
        finalX   = (finalX>maxX) ? maxX:finalX;
        if(INCREASING){
            initialY = (initialY>maxY) ? maxY:initialY;
            finalY   = (finalY<minY) ? minY:finalY; 
        }
        else{
            initialY = (initialY<minY) ? minY:initialY;
            finalY   = (finalY>maxY) ? maxY:finalY; 
        }
        return;
    }

    void convertToAbs(){
        finalY += initialY;
        finalX += initialX;
    }
    string convertToString(){
      string strInstruction = "L ";
      strInstruction += to_string(initialX) + " " + to_string(initialY) + " ";
      strInstruction += to_string(finalX) + " " + to_string(finalY) + " ";
      return strInstruction;
    }
};

class QuadraticCurve: public PathInstruction{
private:
    bool doubleCtrPoint;
    float xControlPoint1;
    float yControlPoint1;
    float xControlPoint2;
    float yControlPoint2;

public:
    QuadraticCurve(float pFinalX, float pFinalY, float pXPoint1, float pYPoint1){
        finalX = pFinalX;
        finalY = pFinalY;
        xControlPoint1 = pXPoint1;
        yControlPoint1 = pYPoint1;
        doubleCtrPoint = false;
    }

    QuadraticCurve(float pFinalX, float pFinalY, float pXPoint1, float pYPoint1, float pXPoint2, float pYPoint2){
        finalX = pFinalX;
        finalY = pFinalY;
        xControlPoint1 = pXPoint1;
        yControlPoint1 = pYPoint1;
        xControlPoint2 = pXPoint2;
        yControlPoint2 = pYPoint2;
        doubleCtrPoint = true;
    }

    bool isInRange(float pXPosition, float pYPosition){
        bool inXRange = (initialX<=(pXPosition+NEAR_SIZE)&&finalX>=(pXPosition-NEAR_SIZE));
        bool inYRange = (initialY<=(pYPosition+NEAR_SIZE)&&finalY>=(pYPosition-NEAR_SIZE));
        bool inCtr1Range = (yControlPoint1<=(pYPosition+NEAR_SIZE)&&yControlPoint1>=(pYPosition-NEAR_SIZE));
        bool inCtr2Range = (yControlPoint2<=(pYPosition+NEAR_SIZE)&&yControlPoint2>=(pYPosition-NEAR_SIZE));
        return (inXRange&&(inCtr1Range||inCtr2Range));
    }

    void adjustSize(float pXPosition, float pYPosition){
        float minX  = pXPosition-NEAR_SIZE;
        float maxX  = pXPosition+NEAR_SIZE;
        float minY  = pYPosition-NEAR_SIZE;
        float maxY  = pYPosition+NEAR_SIZE;

        initialX = (initialX < minX)  ? minX:initialX;
        finalX = (finalX > maxX) ? maxX:finalX;
        initialY = (initialY < minY)  ? minY:initialY;
        finalY = (finalY > maxY) ? maxY:finalY;

        float xInflectionDistance = xControlPoint1-pXPosition;
        xControlPoint1 += (xInflectionDistance>NEAR_SIZE) ? xInflectionDistance:0;

        float yInflectionDistance = yControlPoint1-pYPosition;
        yControlPoint1 += (yInflectionDistance>NEAR_SIZE) ? yInflectionDistance:0;
    }

    void setColor(string pColor){
        color = pColor;
    }

    void convertToAbs(){
        finalY += initialY;
        finalX += initialX;
        xControlPoint1 += initialX;
        yControlPoint1 += initialY;

        if(doubleCtrPoint){
            xControlPoint2 += initialX;
            yControlPoint2 += initialY;
        }
    }
    string convertToString(){
      string strInstruction = "Q ";
      strInstruction += to_string(initialX) + " " + to_string(initialY) + " ";
      strInstruction += to_string(xControlPoint1) + " " + to_string(yControlPoint1) + " ";
      if (doubleCtrPoint)
        strInstruction += to_string(xControlPoint2) + " " + to_string(yControlPoint2) + " ";
      
      strInstruction += to_string(finalX) + " " + to_string(finalY) + " ";
      return strInstruction;
    }
};

class SmoothQuadratic: public PathInstruction{
private:
    bool doubleCtrPoint;
    float xControlPoint1;
    float yControlPoint1;
    float xControlPoint2;
    float yControlPoint2;

public:
    SmoothQuadratic(float pFinalX, float pFinalY, float pXPoint1, float pYPoint1){
        finalX = pFinalX;
        finalY = pFinalY;
        xControlPoint1 = pXPoint1;
        yControlPoint1 = pYPoint1;
        doubleCtrPoint = false;
    }

    SmoothQuadratic(float pFinalX, float pFinalY, float pXPoint1, float pYPoint1, float pXPoint2, float pYPoint2){
        finalX = pFinalX;
        finalY = pFinalY;
        xControlPoint1 = pXPoint1;
        yControlPoint1 = pYPoint1;
        xControlPoint2 = pXPoint2;
        yControlPoint2 = pYPoint2;
        doubleCtrPoint = true;
    }

    bool isInRange(float pXPosition, float pYPosition){
        bool inXRange = (initialX<=(pXPosition+NEAR_SIZE)&&finalX>=(pXPosition-NEAR_SIZE));
        bool inYRange = (initialY<=(pYPosition+NEAR_SIZE)&&finalY>=(pYPosition-NEAR_SIZE));
        bool inCtr1Range = (yControlPoint1<=(pYPosition+NEAR_SIZE)&&yControlPoint1>=(pYPosition-NEAR_SIZE));
        bool inCtr2Range = (yControlPoint2<=(pYPosition+NEAR_SIZE)&&yControlPoint2>=(pYPosition-NEAR_SIZE));
        return (inXRange&&(inCtr1Range||inCtr2Range));
    }

    void adjustSize(float pXPosition, float pYPosition){
        float minX  = pXPosition-NEAR_SIZE;
        float maxX  = pXPosition+NEAR_SIZE;
        float minY  = pYPosition-NEAR_SIZE;
        float maxY  = pYPosition+NEAR_SIZE;

        initialX = (initialX < minX)  ? minX:initialX;
        finalX = (finalX > maxX) ? maxX:finalX;
        initialY = (initialY < minY)  ? minY:initialY;
        finalY = (finalY > maxY) ? maxY:finalY;

        float xInflectionDistance = xControlPoint1-pXPosition;
        xControlPoint1 += (xInflectionDistance>NEAR_SIZE) ? xInflectionDistance:0;

        float yInflectionDistance = yControlPoint1-pYPosition;
        yControlPoint1 += (yInflectionDistance>NEAR_SIZE) ? yInflectionDistance:0;
    }

    void setColor(string pColor){
        color = pColor;
    }

    void convertToAbs(){
        finalY += initialY;
        finalX += initialX;
        xControlPoint1 += initialX;
        yControlPoint1 += initialY;

        if(doubleCtrPoint){
            xControlPoint2 += initialX;
            yControlPoint2 += initialY;
        }
    }
    string convertToString(){
      string strInstruction = "T ";
      strInstruction += to_string(initialX) + " " + to_string(initialY) + " ";
      strInstruction += to_string(xControlPoint1) + " " + to_string(yControlPoint1) + " ";
      if (doubleCtrPoint)
        strInstruction += to_string(xControlPoint2) + " " + to_string(yControlPoint2) + " ";
      
      strInstruction += to_string(finalX) + " " + to_string(finalY) + " ";
      return strInstruction;
    }
};

class Movement: public PathInstruction{
public:
  Movement(float pFinalX, float pFinalY){
    finalX = pFinalX;
    finalY = pFinalY;
  }

  bool isInRange(float pXPosition, float pYPosition){
    bool inXRange = (abs(finalX-pXPosition)<=NEAR_SIZE);
    bool inYRange = (abs(finalY-pYPosition)<=NEAR_SIZE);
    return (inXRange&&inYRange);
  }

  void adjustSize(float pXPosition, float pYPosition){
    return;
  }

  void convertToAbs(){
    finalY += initialY;
    finalX += initialX;
  }
  string convertToString(){
    string strInstruction = "M " + to_string(initialX) + " " + to_string(initialY);
    return strInstruction;
  }
};

class Curveto: public PathInstruction{
private:
    bool doubleCtrPoint;
    float xControlPoint1;
    float yControlPoint1;
    float xControlPoint2;
    float yControlPoint2;
public:
    Curveto(float pFinalX, float pFinalY, float pXPoint1, float pYPoint1){
        finalX = pFinalX;
        finalY = pFinalY;
        xControlPoint1 = pXPoint1;
        yControlPoint1 = pYPoint1;
    }
     Curveto(float pFinalX, float pFinalY, float pXPoint1, float pYPoint1, float pXPoint2, float pYPoint2){
        finalX = pFinalX;
        finalY = pFinalY;
        xControlPoint1 = pXPoint1;
        yControlPoint1 = pYPoint1;
        xControlPoint2 = pXPoint2;
        yControlPoint2 = pYPoint2;
    }

    void setControlPoint1(float pX, float pY){
        xControlPoint1 = pX;
        yControlPoint1 = pY;
    }
    void setControlPoint2(float pX, float pY){
        xControlPoint2 = pX;
        yControlPoint2 = pY;
    }

    bool isInRange(float pXPosition, float pYPosition){
        bool inXRange = (initialX<=(pXPosition+NEAR_SIZE)&&finalX>=(pXPosition-NEAR_SIZE));
        bool inYRange = (initialY<=(pYPosition+NEAR_SIZE)&&finalY>=(pYPosition-NEAR_SIZE));
        bool inCtr1Range = (yControlPoint1<=(pYPosition+NEAR_SIZE)&&yControlPoint1>=(pYPosition-NEAR_SIZE));
        bool inCtr2Range = (yControlPoint2<=(pYPosition+NEAR_SIZE)&&yControlPoint2>=(pYPosition-NEAR_SIZE));
        return (inXRange&&(inCtr1Range||inCtr2Range));
    }

    void adjustSize(float pXPosition, float pYPosition){
        float minX  = pXPosition-NEAR_SIZE;
        float maxX  = pXPosition+NEAR_SIZE;
        float minY  = pYPosition-NEAR_SIZE;
        float maxY  = pYPosition+NEAR_SIZE;

        initialX = (initialX < minX)  ? minX:initialX;
        finalX = (finalX > maxX) ? maxX:finalX;
        initialY = (initialY < minY)  ? minY:initialY;
        finalY = (finalY > maxY) ? maxY:finalY;

        float xInflectionDistance = xControlPoint1-pXPosition;
        xControlPoint1 += (xInflectionDistance>NEAR_SIZE) ? xInflectionDistance:0;

        float yInflectionDistance = yControlPoint1-pYPosition;
        yControlPoint1 += (yInflectionDistance>NEAR_SIZE) ? yInflectionDistance:0;
    }

    void convertToAbs(){
        finalY += initialY;
        finalX += initialX;
        xControlPoint1 += initialX;
        yControlPoint1 += initialY;

        if(doubleCtrPoint){
            xControlPoint2 += initialX;
            yControlPoint2 += initialY;
        }
    }

    string convertToString(){
      string strInstruction = "C ";
      strInstruction += to_string(initialX) + " " + to_string(initialY) + " ";
      strInstruction += to_string(xControlPoint1) + " " + to_string(yControlPoint1) + " ";
      if (doubleCtrPoint)
        strInstruction += to_string(xControlPoint2) + " " + to_string(yControlPoint2) + " ";
      
      strInstruction += to_string(finalX) + " " + to_string(finalY) + " ";
      return strInstruction;
    }
};

class smoothCurveto: public PathInstruction{
private:
    bool doubleCtrPoint;
    float xControlPoint1;
    float yControlPoint1;
    float xControlPoint2;
    float yControlPoint2;
public:
    smoothCurveto(float pFinalX, float pFinalY, float pXPoint1, float pYPoint1){
        finalX = pFinalX;
        finalY = pFinalY;
        xControlPoint1 = pXPoint1;
        yControlPoint1 = pYPoint1;
    }
    smoothCurveto(float pFinalX, float pFinalY, float pXPoint1, float pYPoint1, float pXPoint2, float pYPoint2){
        finalX = pFinalX;
        finalY = pFinalY;
        xControlPoint1 = pXPoint1;
        yControlPoint1 = pYPoint1;
        xControlPoint2 = pXPoint2;
        yControlPoint2 = pYPoint2;
    }

    void setControlPoint1(float pX, float pY){
        xControlPoint1 = pX;
        yControlPoint1 = pY;
    }
    void setControlPoint2(float pX, float pY){
        xControlPoint2 = pX;
        yControlPoint2 = pY;
    }

    bool isInRange(float pXPosition, float pYPosition){
        bool inXRange = (initialX<=(pXPosition+NEAR_SIZE)&&finalX>=(pXPosition-NEAR_SIZE));
        bool inYRange = (initialY<=(pYPosition+NEAR_SIZE)&&finalY>=(pYPosition-NEAR_SIZE));
        //bool inCtr1Range = (yControlPoint1<=(pYPosition+NEAR_SIZE)&&yControlPoint1>=(pYPosition-NEAR_SIZE));
        //bool inCtr2Range = (yControlPoint2<=(pYPosition+NEAR_SIZE)&&yControlPoint2>=(pYPosition-NEAR_SIZE));
        //return (inXRange&&(inCtr1Range||inCtr2Range));
        return (inXRange&&inYRange);
    }

    void adjustSize(float pXPosition, float pYPosition){
        float minX  = pXPosition-NEAR_SIZE;
        float maxX  = pXPosition+NEAR_SIZE;
        float minY  = pYPosition-NEAR_SIZE;
        float maxY  = pYPosition+NEAR_SIZE;

        initialX = (initialX < minX)  ? minX:initialX;
        finalX = (finalX > maxX) ? maxX:finalX;
        initialY = (initialY < minY)  ? minY:initialY;
        finalY = (finalY > maxY) ? maxY:finalY;

        float xInflectionDistance = xControlPoint1-pXPosition;
        xControlPoint1 += (xInflectionDistance>NEAR_SIZE) ? xInflectionDistance:0;

        float yInflectionDistance = yControlPoint1-pYPosition;
        yControlPoint1 += (yInflectionDistance>NEAR_SIZE) ? yInflectionDistance:0;
    }
    
    void convertToAbs(){
        finalY += initialY;
        finalX += initialX;
        xControlPoint1 += initialX;
        yControlPoint1 += initialY;

        if(doubleCtrPoint){
            xControlPoint2 += initialX;
            yControlPoint2 += initialY;
        }
    }

    string convertToString(){
      string strInstruction = "S ";
      strInstruction += to_string(initialX) + " " + to_string(initialY) + " ";
      strInstruction += to_string(xControlPoint1) + " " + to_string(yControlPoint1) + " ";
      if (doubleCtrPoint)
        strInstruction += to_string(xControlPoint2) + " " + to_string(yControlPoint2) + " ";
      
      strInstruction += to_string(finalX) + " " + to_string(finalY) + " ";
      return strInstruction;
    }
};