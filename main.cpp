#include "main.hpp"
#include "PathInstruction.hpp"
#define SVG_NAME "sample.svg"
#define COLOR_FORMAT string
#include "Routing.hpp"
#include "Generator.hpp"


string value;
double height, width;

using namespace std;


bool isNear(float pPosition1, float pPosition2){
    float diference = abs(pPosition1 - pPosition2);
    if(diference < NEAR_SIZE)
      return true;
    return false;
}

class SvgPath{
protected:
  COLOR_FORMAT color;
  vector<PathInstruction*> instructions;
  float strokeWidth;
public: 
  SvgPath(){}
  SvgPath(vector<PathInstruction*> pInstructions, COLOR_FORMAT pColor, float pStrokeWidth){
  	instructions = pInstructions;
		color = pColor;
		strokeWidth = pStrokeWidth;
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

	float getWidth(){
		return strokeWidth;
	}

  void printAttributes(){
	    cout<<"Color: "<<color<<endl;
	    cout<<"Stroke width: "<<strokeWidth<<endl;
		cout<<"#Instructions: "<<instructions.size()<<endl;
		for (unsigned int instIndex = 0; instIndex < instructions.size(); instIndex++){
			cout<<"#"<<instIndex<<endl;
			cout<<"Ix: "<<instructions[instIndex]->getInitialX()<<" Iy: "<<instructions[instIndex]->getInitialY()<<endl;
			cout<<"Fx: "<<instructions[instIndex]->getFinalX()<<" Fy: "<<instructions[instIndex]->getFinalY()<<endl;
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
		strokeWidth = pBasePath->getWidth();
  }
  float getXIntersection(){
    return xIntersected;
  }
  float getYIntersected(){
    return yIntersected;
  }
  void setAtttibutes(COLOR_FORMAT pColor, float pStrokeWidth){
    color = pColor;
    strokeWidth = pStrokeWidth;
  }
};

void callAbsoluteConstructor(char pInstructionType, vector<PathInstruction*> * pPathInstructions, vector<float> pDirections){
    int vecSize;
    //cout<<"Ascii que entra a Abs: "<<pInstructionType<<endl;
    switch (pInstructionType){
    case 'M':
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
            pPathInstructions->push_back(new smoothCurveto(pDirections[4], pDirections[5], pDirections[0], pDirections[1], pDirections[2], pDirections[3]));
        }
        else{ //Wih one control point.
            pPathInstructions->push_back(new smoothCurveto(pDirections[2], pDirections[3], pDirections[0], pDirections[1]));
        }
        break;

    case 'A':
        break;

    default:
        break;
    }
    pPathInstructions->back()->setAbsolute(true);
}

void callRelativeConstructors(char pInstructionType, vector<PathInstruction*> * pPathInstructions, vector<float> pDirections){
    int vecSize;
    //cout<<"Ascii que entra: "<<pInstructionType<<endl;
    switch (pInstructionType){
    case 'm':
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
            pPathInstructions->push_back(new smoothCurveto(pDirections[4], pDirections[5], pDirections[0], pDirections[1], pDirections[2], pDirections[3]));
        }
        else{ //Wih one control point.
            pPathInstructions->push_back(new smoothCurveto(pDirections[2], pDirections[3], pDirections[0], pDirections[1]));
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
        break;
    }
    pPathInstructions->back()->setAbsolute(false);
}

string takeStringInstruction(string pStringD, vector<PathInstruction*> * pPathInstructions){
  vector<float> positions;
  string stringNumber = "";
  char instructionChar;
  bool readingNumber;
  bool negativeNumber = false;
  bool continueReading = true;
  unsigned int stringIndex=0;
  unsigned int stringSize = pStringD.length();

  for (stringIndex; stringSize; stringIndex++){
    char charString = pStringD[stringIndex];
    int asciiValue = (int)charString;

    if (charString==' '){
    	continue;
    }
    if (asciiValue>=65){
      instructionChar = charString;
      while (continueReading){
        stringIndex++;
        charString = pStringD[stringIndex];
        asciiValue = (int)charString;

        if ((charString==' ' || charString==',' || asciiValue>=65 || stringIndex>=stringSize) && readingNumber){
        	if (asciiValue<=45 && stringIndex<=stringSize){
          	float number = stof(stringNumber);
            number = (negativeNumber) ? (number * -1):number;
            positions.push_back(number);
            stringNumber = "";
            readingNumber = false;
            negativeNumber = false;
          }
          else{
            continueReading = (asciiValue>=65 || stringIndex>=stringSize) ? false:true;
          }
        }
        if (asciiValue<=57 && charString!=','){
          if(charString == '-'){
            negativeNumber = true;
            continue;
          }
          readingNumber = true;
          stringNumber += charString;
        }
      }
      if((int)instructionChar>=97){
        callRelativeConstructors(instructionChar, pPathInstructions, positions);}
      else{
      	callAbsoluteConstructor(instructionChar, pPathInstructions, positions);}
      break;
    }
  }
  return pStringD.erase(0,stringIndex);
}

vector<PathInstruction*> convertStrToIns(string pStringD){
	vector<PathInstruction*> pathInstructions;
	while(pStringD!=""){
		pStringD = takeStringInstruction(pStringD, &pathInstructions);
	}
	return pathInstructions;
}

//Recorre un vector de instrucciones y les asigna su punto inicial, convierte a absoluto si es neccesario.
void assignInitialPoints(vector<PathInstruction*> pInstructions){
	float xPosition = pInstructions[0]->getFinalX();
	float yPosition = pInstructions[0]->getFinalY();
	pInstructions[0]->setInitialPoint(xPosition, yPosition);
	for (int instIndex = 1; instIndex<pInstructions.size(); instIndex++){
		pInstructions[instIndex]->setInitialPoint(xPosition, yPosition);
		if (pInstructions[instIndex]->getAbsolute()==false)
			pInstructions[instIndex]->convertToAbs();

    if (pInstructions[instIndex]->isPositive()==false)
      pInstructions[instIndex]->invertInitFinalPoint();
        
		xPosition = pInstructions[instIndex]->getFinalX();
		yPosition = pInstructions[instIndex]->getFinalY();
	}
}

//Recorre los path del SVG y los agrega a un vector de objetos path.
void extractSvgPaths(xml_node<>* node, vector<SvgPath*> * pPathVector){
	vector<PathInstruction*> pathInstructions;
	SvgPath * pathPtr;
  for (node = node->first_node("path"); node != NULL; node = node->next_sibling("path")){
    if (node->type() == node_element){
      string dAttribute = node->first_attribute("d")->value();
      string colorAttribute = node->first_attribute("stroke")->value();
			string widthAttribute = node->first_attribute("stroke-width")->value();

			pathInstructions = convertStrToIns(dAttribute);
			float width = stof(widthAttribute);
			pathPtr = new SvgPath(pathInstructions, colorAttribute, width);
            assignInitialPoints(pathPtr->getInstructions());
			pPathVector->push_back(pathPtr);
      extractSvgPaths(node, pPathVector);
    }
  }
}

//Recorre un Path y agrega nuevos path si hay instruciones que coincidan con el punto.
void selectFromPath(SvgPath * pPath, float pXPoint, float pYPoint, vector<NewPath> * pNewPathVector){
  vector<PathInstruction*> pathInstructions = pPath->getInstructions();
  vector<PathInstruction*> resultInstructions;
  for(unsigned int vectorIndex = 0; vectorIndex<pathInstructions.size(); vectorIndex++){
		PathInstruction * instPtr = pathInstructions[vectorIndex];
    //Poda
    if(instPtr->isInRange(pXPoint, pYPoint)){
      instPtr->adjustSize(pXPoint,pYPoint);
      resultInstructions.push_back(instPtr);
    }
  }
  if (resultInstructions.size()>0){
    NewPath newPath(resultInstructions, pPath, pXPoint, pYPoint);
		pNewPathVector->push_back(newPath);
  }
}

void assignValue(){
   string widthSvg,highSVG;
    bool control = false;

  for(int i = 4; i < value.size(); i++){
    if((isdigit(value[i]) || value[i] == '.') && control == false){
      widthSvg += value[i];
    }else if(isdigit(value[i]) || value[i] == '.'){
      control = true;
      highSVG += value[i];
    }else
      control = true;
  }
  height = stod(highSVG);
  width = stod(widthSvg);
//cout<<"Ancho: "<<width<<" Largo: "<<height;
}

//Recorre el elemento raíz del documento
void extractXMLData(xml_document<>* doc){
  xml_node<>* node = doc->first_node();
  for (xml_attribute<>* attrib = node->first_attribute(); attrib != NULL; attrib = attrib->next_attribute())
  {
    string atribute = attrib->name();
    string val = attrib->value();
    if(atribute == "viewBox")
	{
       value = attrib->value();
       assignValue();
    }
    else if(atribute == "height")
	{
      height = stod(val);
    }
    else if(atribute == "width"){
      width = stod(val);
    }
  }
}


int main(){
  /*
  clock_t t0 = clock();
  clock_t t1 = clock();
  double time = (double(t1-t0)/CLOCKS_PER_SEC);

  for(int i = 0; sv.getInstructions().size();i++){
    vector<PathInstruction*> ins = sv.getInstructions();
    ins[i]->printValues();
  }*/

	file<> file(SVG_NAME); // Lee y carga el archivo en memoria
	xml_document<> svgDoc; //Raíz del árbol DOM
	svgDoc.parse<0>(file.data()); //Parsea el XML en un DOM
	xml_node<>* pathNode = svgDoc.first_node();
	

	//Obtener ancho y largo de la imagen
	extractXMLData(&svgDoc);

	vector<SvgPath*> svgPathVector;
	extractSvgPaths(pathNode, &svgPathVector);

	//cout<<svgPathVector.size()<<endl;

	assignInitialPoints(svgPathVector[0]->getInstructions());
	PathPoint p;
	vector<NewPath> newPathVector;
	for (int i = 0; i < svgPathVector.size();  i++){
		selectFromPath(svgPathVector[i], 100, 150, &newPathVector);
	}

  
	cout<<"Tam de newVec: "<<newPathVector.size()<<endl;
	string figureName;
	Figure f;
	for (int i = 0; i < newPathVector.size();  i++){
		newPathVector[i].printAttributes();
		p.setXYInitial(newPathVector[i].getXIntersection(), newPathVector[i].getYIntersected());		
		for(int j = 0; j < newPathVector[i].getInstructions().size(); j++){
			figureName = newPathVector[i].getInstructions()[j]->getNameName();
			f.type = figureName;
			f.strokeWidth = newPathVector[i].getWidth();
			f.color = newPathVector[i].getColor();
			p.setVectorFiguras(f);
		}
		
	}

	routingFunction(&p, width,height, 360, 3);
	cout<<"Luego del enrutamienro: "<<frameOrder.frames[0].size()<<endl;
	
	for(int i = 0; i < frameOrder.frames.size(); i++){
		makeGeneration(frameOrder.frames[i]); 
    }

  return 0;
}
