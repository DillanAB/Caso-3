#include "Observer.hpp"
#include "Router.hpp"
#ifndef GENERATOR_HEADER
#define GENERATOR_HEADER

vector<Figure> makeGeneration(vector<Figure> pVector, double pHeight, double pWidth );

class Generator: public Observer {

private:
	double * heightPtr;
	double * widthPtr;

public:
   Generator(double * pHeight, double * pWidth) {
		heightPtr = pHeight;
		widthPtr = pWidth;
	}
	void update(void * pPointer){
	vector<Figure> figures = *(vector<Figure>*)pPointer;
	double width = *widthPtr;
    double height = *widthPtr;		
	makeGeneration(figures, width, height);
	}
};

//Add the attribute in the tree and make the image

void addAttribute(string pResult, string pColor,string pFill, string pStroke, string pNameSVG, string pOldName){		
	//Read file, change as frames are created
    file<> file(pOldName.c_str());
    xml_document<> myDoc;
   	myDoc.parse<0>(file.data());
	   
	
	//Insert a new element
	xml_node<> *newNode= myDoc.allocate_node(node_element,"path");
	myDoc.first_node()->append_node(newNode ); 
	
	xml_attribute<> *newAttr= myDoc.allocate_attribute("d",pResult.c_str());
	newNode ->append_attribute(newAttr) ;
	
	xml_attribute<> *newAttr2  = myDoc.allocate_attribute("stroke", pColor.c_str());
	newNode ->append_attribute(newAttr2); 
	
	xml_attribute<> *newAttr3  = myDoc.allocate_attribute("stroke-width",pStroke.c_str());
	newNode->append_attribute(newAttr3); 
	
		xml_attribute<> *newAttr4  = myDoc.allocate_attribute("fill",pFill.c_str());
	newNode->append_attribute(newAttr4);
   
	//Create the svg with the attributes
	ofstream copyFile(pNameSVG);
	stringstream ss;
	ss << *myDoc.first_node();
	string stringXML = ss.str();
	copyFile << stringXML;
	copyFile.close();
}

// Check if the frame is exceeded
bool compareFrame(Figure pFigure, double pHeight, double pWidth){
	if(pFigure.type == "L")
	{
		if ((pFigure.route[0] > pHeight) || (pFigure.route[0] > pWidth ) || (pFigure.route[0] < 0) ||
		(pFigure.route[1] > pHeight) || (pFigure.route[1] > pWidth ) || (pFigure.route[1] < 0) ||
		(pFigure.route[2] > pHeight) || (pFigure.route[2] > pWidth ) || (pFigure.route[2] < 0) ||
		(pFigure.route[3] > pHeight) || (pFigure.route[3] > pWidth ) || (pFigure.route[3] < 0))
		return false;
	}
	else if (pFigure.type == "Q")
	{
		if ((pFigure.route[0] > pHeight) || (pFigure.route[0] > pWidth ) || (pFigure.route[0] < 0) ||
		(pFigure.route[1] > pHeight) || (pFigure.route[1] > pWidth ) || (pFigure.route[1] < 0) ||
		(pFigure.route[2] > pHeight) || (pFigure.route[2] > pWidth ) || (pFigure.route[2] < 0) ||
		(pFigure.route[3] > pHeight) || (pFigure.route[3] > pWidth ) || (pFigure.route[3] < 0) ||
		(pFigure.route[4] > pHeight) || (pFigure.route[4] > pWidth ) || (pFigure.route[4] < 0) ||
		(pFigure.route[5] > pHeight) || (pFigure.route[5] > pWidth ) || (pFigure.route[5] < 0) )	
		return false;		
	}
	else if ((pFigure.type == "H" ) || (pFigure.type == "V"))
	{
		if((pFigure.route[0] > pHeight) || (pFigure.route[0] > pWidth ) || (pFigure.route[0] < 0) ||
		(pFigure.route[1] > pHeight) || (pFigure.route[1] > pWidth ) || (pFigure.route[1] < 0) ||
		(pFigure.route[2] > pHeight) || (pFigure.route[2] > pWidth ) || (pFigure.route[2] < 0))
		return false;				
	}		
	return true;
}

/*
Receives a vector of object type figure. Each figure has the movements it must do
to create the frame.
The function is O(n), since the N of the function is only traversed once.
*/
vector<Figure> makeGeneration(vector<Figure> pVector, double pHeight, double pWidth ){
	string result, nameSVG, stroke_w, colorFigure, oldNameSVG, fill;
	vector<Figure> vectorResult;
	nameSVG = "sample"+to_string(controlFrame)+".svg";
	oldNameSVG = SVG_NAME;
	stroke_w = "3";

	for(int i = 0; i < pVector.size(); i++){
		result = "M "+to_string(pVector[i].route[0]) + " "+to_string(pVector[i].route[1])+" ";
		if(pVector[i].type == "L")
		{		
			result += pVector[i].type + " "+ to_string(pVector[i].route[2])+" "+to_string((pVector[i].route[3]));
			fill = "none";	
		}
		else if (pVector[i].type == "Q")
		{
			result += pVector[i].type + " "+ to_string(pVector[i].route[2])+" "+to_string((pVector[i].route[3]))+" "+
			to_string(pVector[i].route[4])+" "+ to_string(pVector[i].route[5]);
			fill = pVector[i].color;				
		}
		else if (pVector[i].type == "H")
		{
			result += pVector[i].type + " "+ to_string(pVector[i].route[2]);
			fill = "none";				
		}
		else if (pVector[i].type == "V")
		{
			result += pVector[i].type + " "+ to_string(pVector[i].route[2]);
			fill = "none";				
		}
		if(compareFrame(pVector[i], pHeight, pWidth)){
			vectorResult.push_back(pVector[i]);					
			addAttribute(result,pVector[i].color,fill,stroke_w,nameSVG,oldNameSVG);
			oldNameSVG = nameSVG; 
		}
	}
 	controlFrame ++;
	control++;	
	return vectorResult;		
}

#endif
