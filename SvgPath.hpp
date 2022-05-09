#include <iostream>
#include <vector>
#include "PathInstruction.hpp"
#define COLOR_FORMAT string


class SvgPath{
protected:
  COLOR_FORMAT color;
  vector<PathInstruction*> instructions;
public: 
  SvgPath(){}

  SvgPath(vector<PathInstruction*> pInstructions, COLOR_FORMAT pColor){
  	instructions = pInstructions;
		color = pColor;
  }

  vector<PathInstruction*> getInstructions(){
  	return instructions;
  }
  void addInstruction(PathInstruction* pInstructrion){
    instructions.push_back(pInstructrion);
  }

	COLOR_FORMAT getColor(){
		return color;
	}

  void printAttributes(){
	  cout<<"Color: "<<color<<endl;
		cout<<"#Instructions: "<<instructions.size()<<endl;
		for (unsigned int instIndex = 0; instIndex < instructions.size(); instIndex++){
			cout<<"#"<<instIndex<<endl;
            cout<<instructions[instIndex]->convertToString()<<endl;
			//cout<<"Ix: "<<instructions[instIndex]->getInitialX()<<" Iy: "<<instructions[instIndex]->getInitialY()<<endl;
			//cout<<"Fx: "<<instructions[instIndex]->getFinalX()<<" Fy: "<<instructions[instIndex]->getFinalY()<<endl;
		}
		cout<<endl;
	}
};

class NewPath: public SvgPath{
private:
  float xIntersected;
  float yIntersected;
public: 
  NewPath(vector<PathInstruction*> pInstructions, SvgPath * pBasePath, float pXInter, float pYInter){
	instructions = pInstructions;
    xIntersected = pXInter;
    yIntersected = pYInter;
    color = pBasePath->getColor();
  }
  float getXIntersection(){
    return xIntersected;
  }
  float getYIntersected(){
    return yIntersected;
  }
  void setAtttibutes(COLOR_FORMAT pColor){
    color = pColor;
  }
};

void callAbsoluteConstructor(char pInstructionType, vector<PathInstruction*> * pPathInstructions, vector<float> pDirections){
    int vecSize;
    //cout<<"Ascii que entra a Abs: "<<pInstructionType<<endl;
    switch (pInstructionType){
    case 'M':
        cout<<"Entra a M X = "<<pDirections[0]<<endl;
        pPathInstructions->push_back(new Movement(pDirections[0], pDirections[1]));
        break;

    case 'H':
        pPathInstructions->push_back(new HorizontalLine(pDirections[0]));
        break;

    case 'V':
        pPathInstructions->push_back(new VerticalLine(pDirections[0]));
        break;

    case 'L':
        pPathInstructions->push_back(new Line(pDirections[0], pDirections[1]));
        break;

    case 'C':
        vecSize = pDirections.size();
        if (vecSize>4){ //Wih two control points.
            pPathInstructions->push_back(new Curveto(pDirections[4], pDirections[5], pDirections[0], pDirections[1], pDirections[2], pDirections[3]));
        }
        else{ //Wih one control point.
            pPathInstructions->push_back(new Curveto(pDirections[2], pDirections[3], pDirections[0], pDirections[1]));
        }
        break;

    case 'S':
        vecSize = pDirections.size();
        if (vecSize>4){ //Wih two control points.
            pPathInstructions->push_back(new smoothCurveto(pDirections[4], pDirections[5], pDirections[0], pDirections[1], pDirections[2], pDirections[3]));
        }
        else{ //Wih one control point.
            pPathInstructions->push_back(new smoothCurveto(pDirections[2], pDirections[3], pDirections[0], pDirections[1]));
        }
        break;

    case 'Q':
        vecSize = pDirections.size();
        if (vecSize>4){ //Wih two control points.
            pPathInstructions->push_back(new QuadraticCurve(pDirections[4], pDirections[5], pDirections[0], pDirections[1], pDirections[2], pDirections[3]));
        }
        else{ //Wih one control point.
            pPathInstructions->push_back(new QuadraticCurve(pDirections[2], pDirections[3], pDirections[0], pDirections[1]));
        }
        break;

    case 'T':
        vecSize = pDirections.size();
        if (vecSize>4){ //Wih two control points.
            pPathInstructions->push_back(new SmoothQuadratic(pDirections[4], pDirections[5], pDirections[0], pDirections[1], pDirections[2], pDirections[3]));
        }
        else{ //Wih one control point.
            pPathInstructions->push_back(new SmoothQuadratic(pDirections[2], pDirections[3], pDirections[0], pDirections[1]));
        }
        break;

    case 'A':
        break;

    default:
    return;
        break;
    }
    pPathInstructions->back()->setAbsolute(true);
}

void callRelativeConstructors(char pInstructionType, vector<PathInstruction*> * pPathInstructions, vector<float> pDirections){
    int vecSize;
    //cout<<"Ascii que entra: "<<pInstructionType<<endl;
    switch (pInstructionType){
    case 'm':
        cout<<"Entra a m"<<endl;
        pPathInstructions->push_back(new Movement(pDirections[0], pDirections[1]));
        break;

    case 'h':
        pPathInstructions->push_back(new HorizontalLine(pDirections[0]));
        break;

    case 'v':
        pPathInstructions->push_back(new VerticalLine(pDirections[0]));
        break;

    case 'l':
        pPathInstructions->push_back(new Line(pDirections[0], pDirections[1]));
        break;

    case 'c':
        vecSize = pDirections.size();
        if (vecSize>4){ //Wih two control points.
            pPathInstructions->push_back(new Curveto(pDirections[4], pDirections[5], pDirections[0], pDirections[1], pDirections[2], pDirections[3]));
        }
        else{ //Wih one control point.
            pPathInstructions->push_back(new Curveto(pDirections[2], pDirections[3], pDirections[0], pDirections[1]));
        }
        break;

    case 's':
        vecSize = pDirections.size();
        if (vecSize>4){ //Wih two control points.
            pPathInstructions->push_back(new Curveto(pDirections[4], pDirections[5], pDirections[0], pDirections[1], pDirections[2], pDirections[3]));
        }
        else{ //Wih one control point.
            pPathInstructions->push_back(new Curveto(pDirections[2], pDirections[3], pDirections[0], pDirections[1]));
        }
        break;
    
    case 'q':
        vecSize = pDirections.size();
        if (vecSize>4){ //Wih two control points.
            pPathInstructions->push_back(new QuadraticCurve(pDirections[4], pDirections[5], pDirections[0], pDirections[1], pDirections[2], pDirections[3]));
        }
        else{ //Wih one control point.
            pPathInstructions->push_back(new QuadraticCurve(pDirections[2], pDirections[3], pDirections[0], pDirections[1]));
        }
        break;

    case 't':
      vecSize = pDirections.size();
      if (vecSize>4){ //Wih two control points.
        pPathInstructions->push_back(new SmoothQuadratic(pDirections[4], pDirections[5], pDirections[0], pDirections[1], pDirections[2], pDirections[3]));
      }
      else{ //Wih one control point.
        pPathInstructions->push_back(new SmoothQuadratic(pDirections[2], pDirections[3], pDirections[0], pDirections[1]));
      }
      break; 

    case 'a':
        break;

    
    default:
        return;
        break;
    }
    pPathInstructions->back()->setAbsolute(false);
}

//LA QUE CAMBIÉ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
vector<PathInstruction*> convertStrToIns(string pStringD){
   vector<float> positions;
   vector<PathInstruction*> pathInstructions;
   unsigned int strSize = pStringD.size();
   unsigned int numberBeginIndex = 0;
   unsigned int posCounter;
   bool readingNumber = false;
   bool negativeNumber = false;
   char charInst = pStringD[0];
   string strNumber = "";
   for(unsigned int strIndex = 1; strIndex<strSize; strIndex++){
      char charString = pStringD[strIndex];
      int asciiValue = (int)charString;
      //cout<<"INT: "<<charString<<endl;

      if((asciiValue>=48 && asciiValue<=57) || charString=='.'){
         if(readingNumber==false){
            readingNumber = true;
            numberBeginIndex = strIndex;
         }
         strNumber += charString;
      }
      else if(charString=='-'){
         if(readingNumber){
            //pStringD.substr(numberBeginIndex,((strIndex+1)-numberBeginIndex));
            float number = stof(strNumber);
            number = (negativeNumber) ? (number * -1):number;
            positions.push_back(number);
            strNumber = "";
         }
         negativeNumber = true;
      }
      else if(charString==' ' || charString==','){
         if(readingNumber){
            float number = stof(strNumber);
            number = (negativeNumber) ? (number * -1):number;
            positions.push_back(number);
            strNumber = "";
         }
         readingNumber = false;
         negativeNumber = false;
      }
      else if(asciiValue>=65){
            if(readingNumber){
                float number = stof(strNumber);
                number = (negativeNumber) ? (number * -1):number;
                positions.push_back(number);
                strNumber = "";
            }
            if((int)charInst>=97){
                callRelativeConstructors(charInst, &pathInstructions, positions);
            }
            else{
      	        callAbsoluteConstructor(charInst, &pathInstructions, positions);
            }
            positions.clear();
            charInst = charString;
            readingNumber = false;
            negativeNumber = false;
      }
   }
   return pathInstructions;
}


//Recorre un vector de instrucciones y les asigna su punto inicial, convierte a absoluto si es necesario.
void assignInitialPoints(std::vector<PathInstruction*> pInstructions){
	float xPosition = pInstructions[0]->getFinalX();
	float yPosition = pInstructions[0]->getFinalY();
	pInstructions[0]->setInitialPoint(xPosition, yPosition);
	for (int instIndex = 1; instIndex<pInstructions.size(); instIndex++){
		pInstructions[instIndex]->setInitialPoint(xPosition, yPosition);

		if (pInstructions[instIndex]->getAbsolute()){
            xPosition = pInstructions[instIndex]->getFinalX();
		    yPosition = pInstructions[instIndex]->getFinalY();
            //if (pInstructions[instIndex]->isPositive()==false)
                //pInstructions[instIndex]->invertInitFinalPoint();
        }
        else{
            xPosition += pInstructions[instIndex]->getFinalX();
		    yPosition += pInstructions[instIndex]->getFinalY();
            pInstructions[instIndex]->convertToAbs();
            //if (pInstructions[instIndex]->isPositive()==false)
                //pInstructions[instIndex]->invertInitFinalPoint();
        }
	}
}