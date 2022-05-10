#include "Observer.hpp"
#include "SvgPath.hpp"
#include "Macros.hpp"
#include "rapidxml/rapidxml_ext.hpp" 
#include "rapidxml/rapidxml_utils.hpp" 
#include <sstream>
#include <fstream>
using namespace rapidxml;

void extractXMLData(xml_document<>* doc);
void extractNodeData(xml_node<>* node);

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
      file<> file(pSvgName); 
      xml_document<> svgDoc; 
      svgDoc.parse<0>(file.data()); 
      extractSize(&svgDoc); //Gets svgWidth, svgHeight.
	   xml_node<>* pathNode = svgDoc.first_node();
	   vector<SvgPath*> svgPathVector;
	   extractSvgPaths(pathNode, &svgPathVector);
      svgPath = svgPathVector;
   }
   //Obtains the dimentions of the SVG.
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
            //Compare the path color to the colors searched, if is similar proccess the path.
            if(redBools[redInt] || greenBools[greenInt] || blueBools[blueInt]){
               string dAttribute = node->first_attribute("d")->value();
               pathInstructions = convertStrToIns(dAttribute);
               pathPtr = new SvgPath(pathInstructions, colorAttribute);
               assignInitialPoints(pathPtr->getInstructions());
               pPathVector->push_back(pathPtr);
            }
         }
      }
   }
};