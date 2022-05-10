#include "Observer.hpp"
#include "Router.hpp"
#ifndef GENERATOR_HEADER
#define GENERATOR_HEADER
void makeGeneration(vector<Figure> pVector, double pHeight, double pWidth );

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
		//cout<<"HOLA GEN : "<<figures.size()<<endl;		
		makeGeneration(figures, width, height);
	}
};

void addAttribute(string pResult, string pColor,string pFill, string pStroke, string pNameSVG, string pOldName){		
	//Lee el archivo, cambia conforme se crean frames
    file<> file(pOldName.c_str());
    xml_document<> myDoc;
   	myDoc.parse<0>(file.data());
	   
	//Insertar un nuevo elemento
   xml_node<> *newNode= myDoc.allocate_node(node_element,"path");
   myDoc.first_node()->append_node(newNode ); //Elemento <path>
   
   xml_attribute<> *newAttr= myDoc.allocate_attribute("d",pResult.c_str());
   newNode ->append_attribute(newAttr) ; //Atributo "d" para <path>
   
   xml_attribute<> *newAttr2  = myDoc.allocate_attribute("stroke", pColor.c_str());
   newNode ->append_attribute(newAttr2); //Atributo "stroke" para <path>
   
   xml_attribute<> *newAttr3  = myDoc.allocate_attribute("stroke-width",pStroke.c_str());
   newNode->append_attribute(newAttr3); //Atributo "stroke" para <path>
   
    xml_attribute<> *newAttr4  = myDoc.allocate_attribute("fill",pFill.c_str());
   newNode->append_attribute(newAttr4); //Atributo "stroke" para <path>
   
	//Crear el svg con los atributos
	ofstream copyFile(pNameSVG);
	stringstream ss;
	ss << *myDoc.first_node();
	string stringXML = ss.str();
	copyFile << stringXML;
	copyFile.close();
}

//Comparar si el frame se sobrepasa
bool compareFrame(Figure pFigure, double pHeight, double pWidth){
	if(pFigure.type == "L")
	{
		if ((pFigure.route[0] > pHeight) || (pFigure.route[0] > pWidth ) || (pFigure.route[0] < 0) ||
		(pFigure.route[1] > pHeight) || (pFigure.route[1] > pWidth ) || (pFigure.route[1] < 0) ||
		(pFigure.route[2] > pHeight) || (pFigure.route[2] > pWidth ) || (pFigure.route[2] < 0) ||
		(pFigure.route[3] > pHeight) || (pFigure.route[3] > pWidth ) || (pFigure.route[3] < 0))
		return true;
	}
	else if (pFigure.type == "Q")
	{
		if ((pFigure.route[0] > pHeight) || (pFigure.route[0] > pWidth ) || (pFigure.route[0] < 0) ||
		(pFigure.route[1] > pHeight) || (pFigure.route[1] > pWidth ) || (pFigure.route[1] < 0) ||
		(pFigure.route[2] > pHeight) || (pFigure.route[2] > pWidth ) || (pFigure.route[2] < 0) ||
		(pFigure.route[3] > pHeight) || (pFigure.route[3] > pWidth ) || (pFigure.route[3] < 0) ||
		(pFigure.route[4] > pHeight) || (pFigure.route[4] > pWidth ) || (pFigure.route[4] < 0) ||
		(pFigure.route[5] > pHeight) || (pFigure.route[5] > pWidth ) || (pFigure.route[5] < 0) )	
		return true;		
	}
	else if ((pFigure.type == "H" ) || (pFigure.type == "V"))
	{
		if((pFigure.route[0] > pHeight) || (pFigure.route[0] > pWidth ) || (pFigure.route[0] < 0) ||
		(pFigure.route[1] > pHeight) || (pFigure.route[1] > pWidth ) || (pFigure.route[1] < 0) ||
		(pFigure.route[2] > pHeight) || (pFigure.route[2] > pWidth ) || (pFigure.route[2] < 0))
		return true;				
	}		
	return false;
}

//Falta comparar si el frame se sobrepasa
void makeGeneration(vector<Figure> pVector, double pHeight, double pWidth ){
	string result, nameSVG, stroke_w, colorFigure, oldNameSVG, fill;
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
		if(!compareFrame(pVector[i], pHeight, pWidth)){					
			addAttribute(result,pVector[i].color,fill,stroke_w,nameSVG,oldNameSVG);
			oldNameSVG = nameSVG; //Se cambia con el nuevo nombre para que ingrese a ese nuevo svg en la nueva corrida	
		}
	}
 	controlFrame++;
	control++;			
}

#endif
