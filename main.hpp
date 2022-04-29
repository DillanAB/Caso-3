#include <iostream>
#include "rapidxml/rapidxml_ext.hpp" //Clases para manejo del DOM
#include "rapidxml/rapidxml_utils.hpp" //Clase File
#include <sstream>
#include <fstream>
#include <ctime>
#include <cmath>
#include <vector>
#include <array>

using namespace std;
using namespace rapidxml; //Namespace de la librería

void extractXMLData(xml_document<>* doc);
void extractNodeData(xml_node<>* node);