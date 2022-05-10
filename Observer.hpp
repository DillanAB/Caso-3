#include "SvgPath.hpp"
#include "rapidxml/rapidxml_ext.hpp" //Clases para manejo del DOM
#include "rapidxml/rapidxml_utils.hpp" //Clase File
#include <sstream>
#include <fstream>
#define NEAR_COLOR 20
using namespace rapidxml; //Namespace de la librería
using namespace std;

void extractXMLData(xml_document<>* doc);
void extractNodeData(xml_node<>* node);

class Observer{
public:
   virtual void update() = 0;
};

class Subject{
protected:
   vector<Observer*> observersVector;

public:
   virtual void notify(){
      for (unsigned int observerIndex = 0; observerIndex < observersVector.size(); observerIndex++){
         observersVector[observerIndex]->update();
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


   void update(){
      while(searchedPoints.empty()==false){
         vector<float> point = searchedPoints.back();
         searchedPoints.pop_back();
         tryPathsForPoint(point[0], point[1]);
         notify();
      }
   }

   void detach(){}

};
