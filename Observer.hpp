#include "Macros.hpp"
#include "SvgPath.hpp"
#include "Routing.hpp"
#include "rapidxml/rapidxml_ext.hpp" //Clases para manejo del DOM
#include "rapidxml/rapidxml_utils.hpp" //Clase File
#include <sstream>
#include <fstream>
using namespace rapidxml;
using namespace std;

void extractXMLData(xml_document<>* doc);
void extractNodeData(xml_node<>* node);

class Observer{
public:
   virtual void update(void* pPointer) = 0;
};

class Subject{
protected:
   vector<Observer*> observersVector;

public:
   virtual void notify(void* pPointer){
      for (unsigned int observerIndex = 0; observerIndex < observersVector.size(); observerIndex++){
         observersVector[observerIndex]->update(pPointer);
      }
   }
   virtual void attach(Observer * pObs){
      observersVector.push_back(pObs);
   }

   virtual void detach() = 0;
};

class SvgReader: public Subject{
private:
   vector<SvgPath*> svgPath;
   bool * redBools;
   bool * greenBools;
   bool * blueBools;
   double svgWidth, svgHeight;
public:
   SvgReader(bool pRedB[], bool pGreenB[], bool pBlueB[]){
      redBools = pRedB;
      greenBools = pGreenB;
      blueBools = pBlueB;
   }

   double getHeight(){
      return svgHeight;
   }

   double getWidth(){
      return svgWidth;
   }

   void loadSvg(char pSvgName[]){
      file<> file(pSvgName); // Lee y carga el archivo en memoria
      xml_document<> svgDoc; //Raíz del árbol DOM
      svgDoc.parse<0>(file.data()); //Parsea el XML en un DOM
      extractSize(&svgDoc); //Gets svgWidth, svgHeight.
	   xml_node<>* pathNode = svgDoc.first_node();
	   vector<SvgPath*> svgPathVector;
	   extractSvgPaths(pathNode, &svgPathVector);
      svgPath = svgPathVector;
   }
   //Recorre el elemento raíz del documento
   void extractSize(xml_document<>* doc){
   xml_node<>* node = doc->first_node();
      for (xml_attribute<>* attrib = node->first_attribute(); attrib != NULL; attrib = attrib->next_attribute()){
         string atribute = attrib->name();
         string val = attrib->value();
         if(atribute == "height"){
            svgHeight = stod(val);
         }
         else if(atribute == "width"){
            svgWidth = stod(val);
         }
      }
   }

   vector<SvgPath*> getPath(){
      return svgPath;
   }

   void detach(){
      observersVector.pop_back();
   }

   //Recorre los path del SVG y los agrega a un vector de objetos path.
   void extractSvgPaths(xml_node<>* node, vector<SvgPath*> * pPathVector){
      vector<PathInstruction*> pathInstructions;
      SvgPath * pathPtr;
      //int i = 0;
      for (node = node->first_node("path"); node != NULL; node = node->next_sibling("path")){
         //i++;
         if (node->type() == node_element){
            string colorAttribute = node->first_attribute("fill")->value();   //Color
            string redStr = colorAttribute.substr(1,2);
            string greenStr = colorAttribute.substr(3,2);
            string blueStr = colorAttribute.substr(5,2);

            int redInt = (stoi(redStr,0, 16)/NEAR_COLOR);
            int greenInt = (stoi(greenStr,0, 16)/NEAR_COLOR);
            int blueInt = (stoi(blueStr,0, 16)/NEAR_COLOR);
            //Si algún RGB se parece al de los buscados.
            if(redBools[redInt] || greenBools[greenInt] || blueBools[blueInt]){
               string dAttribute = node->first_attribute("d")->value();
               //string widthAttribute = node->first_attribute("stroke-width")->value();
               pathInstructions = convertStrToIns(dAttribute);
               //cout<<"ADI: "<<i<< " N_Instrucc: "<<pathInstructions.size()<< endl;
               //float width = stof(widthAttribute);
               pathPtr = new SvgPath(pathInstructions, colorAttribute);
               assignInitialPoints(pathPtr->getInstructions());
               pPathVector->push_back(pathPtr);
               //pathPtr->printAttributes();
               //extractSvgPaths(node, pPathVector);
            }
         }
      }
   }
};

class Selector: public Subject, public Observer{
private:
   vector<SvgPath*> * paths;
   vector<NewPath> * newPaths;
   vector<vector<float>> searchedPoints;
public: 
   Selector(vector<SvgPath*> * pPaths, vector<NewPath> * pNewPaths, vector<vector<float>> pPoints){
      paths = pPaths;
      newPaths = pNewPaths;
      searchedPoints = pPoints;
   }

   void setPathVector(vector<SvgPath*> * pPaths){
      paths = pPaths;
   }

   void tryPathsForPoint(float pXPoint, float pYPoint){
      for (unsigned int pathIndex = 0; pathIndex < paths->size(); pathIndex++){
         selectFromPath(paths->at(pathIndex), pXPoint, pYPoint, newPaths);
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

   /*void update(void * pPointer){
      unsigned int pathsAmount = paths->size();
      while(searchedPoints.empty()==false){
         vector<float> point = searchedPoints.back();
         searchedPoints.pop_back(); //Saca el punto de la lista
         for (unsigned int pathIndex = 0; pathIndex < pathsAmount; pathIndex++){
            selectFromPath(paths->at(pathIndex), point[0], point[1], newPaths);
            bool last = (searchedPoints.empty() && pathIndex == (pathsAmount-1));
            notify(&last);
         }
      }
   }*/

   void update(void * pPointer){
      while(searchedPoints.empty()==false){
         vector<float> point = searchedPoints.back();
         searchedPoints.pop_back();
         tryPathsForPoint(point[0], point[1]);
         bool last = searchedPoints.empty();
         notify(&last);
      }
   }

   void detach(){}

};

class Router: public Subject, public Observer{
private:
   vector<NewPath> * newPaths;
   double * heightPtr;
   double * widthPtr;
   double angle;
   int framesAmount;
public:
   Router(vector<NewPath> * pNewPaths, double pAngle, int pFrames, double * pHeight, double * pWidth){
      newPaths = pNewPaths;
      heightPtr = pHeight;
      widthPtr = pWidth;
      angle = pAngle;
      framesAmount = pFrames;
   }

   PathPoint* routingFunction(PathPoint*pPathPoint , double pHight, double pWidth, double pAngle, int pFrame){
      double distance = 0; //Guarda el result de calcular la distance entre PathPoints
      assignCoordinates(pPathPoint, pHight, pWidth);
      double result, pathX1, pathY1, xFinal, yFinal, costoRuta, position;
      double xInicial = pPathPoint->getXInicial();
      double yInicial = pPathPoint->getYInicial();
      int frameRes;
      Frame classFrame;
      Frame array[pFrame];
         
      for(int i = 0; i < pPathPoint->getFiguras().size(); i++)
      { //Recorre el vector de figuras
         endPoint(pPathPoint, pAngle, pHight, pWidth);
         pathX1 = xInitialize; pathY1 = yInitialize; xFinal = xEnd; yFinal = yEnd; position = workPos;
            
         //Calculamos la distance entre punto inicial y final
         //Raiz ((xFinal - pathX1)² + (yFinal-pathY1)²)
         distance = sqrt(pow(xFinal-pathX1,2) + pow(yFinal-pathY1,2)); 
         costoRuta = distance/pFrame; //
         createFrame(pPathPoint,pPathPoint->getFiguras()[i],pFrame,classFrame,pathX1,pathY1,pHight,pWidth, xFinal, yFinal);
      }
      sortingFrames(pPathPoint->getVectorVector());
      //frameOrder.frames = pPathPoint->getVectorVector();
      control = 0;
      cleanPoint(pPathPoint);
      return pPathPoint;
   }

   void update(void* pPointer){
      double width = *widthPtr;
      double height = *widthPtr;
      PathPoint p;
      string figureName;
      Figure f;
      int newSize = newPaths->size();
      cout<<"NEWSIZE: "<<newSize<<endl;
      for (int i = 0; i < newSize;  i++){
         //newPaths[i].printAttributes();
         p.setXYInitial(newPaths->at(i).getXIntersection(), newPaths->at(i).getXIntersection());
         for(int j = 0; j < newPaths->at(i).getInstructions().size(); j++){
            figureName = newPaths->at(i).getInstructions()[j]->getNameName();
            f.type = figureName;
            f.color = newPaths->at(i).getColor();
            p.setVectorFiguras(f);
         }
      }
      if(newSize>0)
         routingFunction(&p, height, width, angle, framesAmount);
      //sortingFrames(p.getVectorVector());
      bool last = *(bool*)pPointer;
      if(last){
         finalSorting(p.getVectorVector());
         vector<Figure> figures = p.getFiguras();
         cout<<"ENTRA : "<<figures.size()<<endl;
         notify(&figures);
      }
      newPaths->clear();
   }

   void detach(){}
};
