#include "ObserverPattern.hpp"
#include "main.hpp"
#ifndef GENERATOR_HEADER
#define GENERATOR_HEADER


class Generator : public Subject {
private:
    Observer* observer;

public:
    Generator() {}
    ~Generator() {}

    void attach(Observer* est) {
        observer = est;
    }

    void notify(void* dato) {
        cout<<"Notify de Generador"<<endl;
        observer->update(dato);
    }
};

//Falta comparar si el frame se sobrepasa

void makeGeneration(vector<Figure> pVector){
	string result, nameSVG, stroke_w;
	nameSVG = "sample"+to_string(controlFrame)+".svg";
	
	//Escribir un nuevo archivo
    file<> file(SVG_NAME); // Lee y carga el archivo en memoria
    xml_document<> myDoc; //Raíz del árbol DOM
   	myDoc.parse<0>(file.data()); //Parsea el XML en un DOM    

	for(int i = 0; i < pVector.size(); i++){
		result = "M "+to_string(pVector[i].route[0]) + " "+to_string(pVector[i].route[1])+" ";
		result += pVector[i].type + " "+ to_string(pVector[i].route[2])+" "+to_string((pVector[i].route[3]));
		stroke_w = to_string(pVector[i].strokeWidth);

		//Insertar un nuevo elemento
	   xml_node<> *newNode= myDoc.allocate_node(node_element,"path");
	   myDoc.first_node()->append_node(newNode ); //Elemento <path>
	   
	   xml_attribute<> *newAttr= myDoc.allocate_attribute("d",result.c_str());
	   newNode ->append_attribute(newAttr) ; //Atributo "d" para <path>
	   
	   xml_attribute<> *newAttr2  = myDoc.allocate_attribute("stroke", pVector[i].color.c_str());
	   newNode ->append_attribute(newAttr2); //Atributo "stroke" para <path>
	   
	   xml_attribute<> *newAttr3  = myDoc.allocate_attribute("stroke-width",stroke_w.c_str());
	   newNode->append_attribute(newAttr3); //Atributo "stroke" para <path>
	   
	    xml_attribute<> *newAttr4  = myDoc.allocate_attribute("fill","none");
	   newNode->append_attribute(newAttr4); //Atributo "stroke" para <path>
	   
	}
	controlFrame ++;
	ofstream copyFile(nameSVG); //Nuevo archivo
	stringstream ss;
	ss << *myDoc.first_node(); //Pasa el nodo raíz a ss
	string stringXML = ss.str(); //ss.toString
	copyFile << stringXML;
	copyFile.close(); //Escribe y cierra
	control++;	
		
}

#endif
