#include "Macros.hpp"
#include <iostream>
#include <cmath>
#include <vector>
#include <array>

#ifndef INSTRUCTION_HEADER
#define INSTRUCTION_HEADER

using namespace std;

class PathInstruction{
protected:
    bool inverted;
    bool absolute;
    string color;
    float initialX, finalX, initialY, finalY;
    float minX, maxX, minY, maxY;
    string name_;
public:
    //PathInstruction();
    virtual bool isInRange(float pXPosition, float pYPosition) = 0;
    virtual void adjustSize(float pXPosition, float pYPosition) = 0;
    virtual void convertToAbs() = 0;
    virtual string convertToString() = 0;

    string getNameName(){
    	return name_;
	}
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
    void setXMinMax(){
       if(initialX<finalX){
          minX = initialX;
          maxX = finalX; 
       }
       else{
          maxX = initialX;
          minX = finalX;
       }
    }
    void setYMinMax(){
       if(initialY<finalY){
          minY = initialY;
          maxY = finalY; 
       }
       else{
          maxY = initialY;
          minY = finalY;
       }
    }
};

class HorizontalLine: public PathInstruction{
public:
  HorizontalLine(float pFinalX){
    finalX = pFinalX;
    name_ = "H";
  }
  HorizontalLine(float pInitialX, float pFinalX, float pYPosition, string pColor){
    color    = pColor;
    initialX = pInitialX;
    finalX   = pFinalX;
    initialY = finalY = pYPosition;
    name_ = "H";
  }

   bool isInRange(float pXPosition, float pYPosition){
      bool inXRange = (minX<=(pXPosition+NEAR_SIZE)&&maxX>=(pXPosition-NEAR_SIZE));
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
        name_ = "V";
    }
    VerticalLine(float pInitialY, float pFinalY, float pXPosition, string pColor){
        color    = pColor;
        initialY = pInitialY;
        finalY   = pFinalY;
        initialX = finalX = pXPosition;
        name_ = "V";
    }

    bool isInRange(float pXPosition, float pYPosition){
        bool inYRange = (minY<=(pYPosition+NEAR_SIZE)&&maxY>=(pYPosition-NEAR_SIZE));
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
        name_ = "L";
    }
    Line(float pInitialX, float pFinalX, float pFirstY, float pFinalY){
        initialX = pInitialX;
        finalX   = pFinalX;
        initialY = pFirstY;
        finalY   = pFinalY;
        type = (initialY<finalY) ? INCREASING:DECREASING;
        name_ = "L";
    }
    bool isInRange(float pXPosition, float pYPosition){
        bool inXRange, inYRange;
        if(INCREASING){
            inXRange = (minX<=(pXPosition+NEAR_SIZE)&&maxX>=(pXPosition-NEAR_SIZE));
            inYRange = (minY>=(pYPosition-NEAR_SIZE)&&maxY>=(pYPosition+NEAR_SIZE));
        }
        else{
            inXRange = (minX<=(pXPosition+NEAR_SIZE)&&maxX>=(pXPosition-NEAR_SIZE));
            inYRange = (minY<=(pYPosition+NEAR_SIZE)&&maxY>=(pXPosition-NEAR_SIZE));
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
        name_ = "Q";
    }

    QuadraticCurve(float pFinalX, float pFinalY, float pXPoint1, float pYPoint1, float pXPoint2, float pYPoint2){
        finalX = pFinalX;
        finalY = pFinalY;
        xControlPoint1 = pXPoint1;
        yControlPoint1 = pYPoint1;
        xControlPoint2 = pXPoint2;
        yControlPoint2 = pYPoint2;
        doubleCtrPoint = true;
        name_ = "Q";
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
        name_ = "T";
    }

    SmoothQuadratic(float pFinalX, float pFinalY, float pXPoint1, float pYPoint1, float pXPoint2, float pYPoint2){
        finalX = pFinalX;
        finalY = pFinalY;
        xControlPoint1 = pXPoint1;
        yControlPoint1 = pYPoint1;
        xControlPoint2 = pXPoint2;
        yControlPoint2 = pYPoint2;
        doubleCtrPoint = true;
        name_ = "T";
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
    name_ = "M";
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
        name_ = "C";
    }
     Curveto(float pFinalX, float pFinalY, float pXPoint1, float pYPoint1, float pXPoint2, float pYPoint2){
        finalX = pFinalX;
        finalY = pFinalY;
        xControlPoint1 = pXPoint1;
        yControlPoint1 = pYPoint1;
        xControlPoint2 = pXPoint2;
        yControlPoint2 = pYPoint2;
        name_ = "C";
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
        name_ = "T";
    }
    smoothCurveto(float pFinalX, float pFinalY, float pXPoint1, float pYPoint1, float pXPoint2, float pYPoint2){
        finalX = pFinalX;
        finalY = pFinalY;
        xControlPoint1 = pXPoint1;
        yControlPoint1 = pYPoint1;
        xControlPoint2 = pXPoint2;
        yControlPoint2 = pYPoint2;
        name_ = "T";
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

#endif