#include "main.hpp"
#define SVG_NAME "tesla.svg"
#define COLOR_FORMAT string
#define NEAR_COLOR 20
//#include "Generator.hpp"

string value;
double height, width;

int main(){
   vector<string> colors = {"#FFA367", "#57ABE7", "#5CADCE"};
   int boolSize = (255 / NEAR_COLOR) + 2;

   bool redBools[boolSize] = {false};
   bool greenBools[boolSize] = {false};
   bool blueBools[boolSize] = {false};
   for (unsigned int colorIndex = 0; colorIndex < colors.size(); colorIndex++){
      string color = colors[colorIndex];
      string redStr = color.substr(1, 2);
      string greenStr = color.substr(3, 2);
      string blueStr = color.substr(5, 2);

      int redInt = stoi(redStr, 0, 16);
      redBools[redInt / NEAR_COLOR] = true;
      int greenInt = stoi(greenStr, 0, 16);
      greenBools[greenInt / NEAR_COLOR] = true;
      int blueInt = stoi(blueStr, 0, 16);
      blueBools[blueInt / NEAR_COLOR] = true;
   }

   vector<SvgPath *> pathVector;
   vector<NewPath> newPathVector;

   vector<vector<float>> points = {{34, 567}, {350.25, 527}};
   Selector * selector = new Selector(&pathVector, &newPathVector, points);

   SvgReader *reader = new SvgReader(redBools, greenBools, blueBools);
   reader->attach(selector);
   char svgName[] = SVG_NAME;
   reader->loadSvg(svgName);
   pathVector = reader->getPath();
   reader->notify();
   cout << "#Paths: " << pathVector.size() << "| SvgReader Notifica" << endl;
   cout << "Tam de newVec: " << newPathVector.size() << endl;

   /*
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
   }*/
   return 0;
}
