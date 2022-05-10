#include "main.hpp"
 
string value;

void makeSvgAnimation(vector<vector<float>> pPoints, vector<string> pColors, float pRadians, int pFramesAmount){

   int boolSize = (255 / NEAR_COLOR) + 2;

   bool redBools[boolSize] = {false};
   bool greenBools[boolSize] = {false};
   bool blueBools[boolSize] = {false};
   for (unsigned int colorIndex = 0; colorIndex < pColors.size(); colorIndex++){
      string color = pColors[colorIndex];

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
   PathPoint pathPoint;

   double height, width;
   double * heightPtr = &height;
   double * widthPtr = &width;

   Generator * generator = new Generator(heightPtr, widthPtr);
   Router * router = new Router(&pathPoint, pRadians, pFramesAmount, heightPtr, widthPtr);
   Selector * selector = new Selector(&pathVector, &pathPoint, pPoints);
   SvgReader * reader = new SvgReader(redBools, greenBools, blueBools);

   reader->attach(selector);
   selector->attach(router);
   router->attach(generator);

   char svgName[] = SVG_NAME;
   reader->loadSvg(svgName);
   pathVector = reader->getPath();
   *heightPtr = reader->getHeight();
   *widthPtr = reader->getWidth();

   reader->notify(0);
}

int main(){
   vector<string> colors = {"#FFA367", "#57ABE7", "#5CADCE"};
   vector<vector<float>> points = {{350.25, 527}, {150.25, 600}, {325.25, 527}};
   makeSvgAnimation(points, colors, 45, 3);

   return 0;
}
