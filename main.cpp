#include "main.hpp"
#define SVG_NAME "tesla.svg"
#define COLOR_FORMAT string
#define NEAR_COLOR 20
using namespace std;

void makeSvgAnimation(float pAbsPoints[][2], vector<COLOR_FORMAT> pColors, float pAngle, int pFrames){
  
}

//void 

int main(){
  /*
  clock_t t0 = clock();
  clock_t t1 = clock();
  double time = (double(t1-t0)/CLOCKS_PER_SEC);

  for(int i = 0; sv.getInstructions().size();i++){
    vector<PathInstruction*> ins = sv.getInstructions();
    ins[i]->printValues();
  }*/


	//cout<<svgPathVector.size()<<endl;
	//assignInitialPoints(svgPathVector[0]->getInstructions());


  vector<string> colors = {"#FFA367", "#57ABE7", "#5CADCE"};
  int boolSize = (255/NEAR_COLOR)+2;

  bool redBools[boolSize] = {false};
  bool greenBools[boolSize] = {false};
  bool blueBools[boolSize] = {false};
  for (unsigned int colorIndex = 0; colorIndex < colors.size(); colorIndex++){
    string color = colors[colorIndex];
    string redStr = color.substr(1,2);
    string greenStr = color.substr(3,2);
    string blueStr = color.substr(5,2);

    int redInt = stoi(redStr,0, 16);
    redBools[redInt/NEAR_COLOR] = true;
    int greenInt = stoi(greenStr,0, 16);
    greenBools[greenInt/NEAR_COLOR] = true;
    int blueInt = stoi(blueStr,0, 16);
    blueBools[blueInt/NEAR_COLOR] = true;
  }
  

  vector<SvgPath*> pathVector;
  vector<NewPath> newPathVector;

  vector<vector<float>> points = {{34,567}, {350.25,527}};
  Selector * selector = new Selector(&pathVector, &newPathVector, points);


  SvgReader * reader = new SvgReader(redBools, greenBools, blueBools);
  reader->attach(selector);
  char svgName[] = SVG_NAME;
  reader->loadSvg(svgName);
  pathVector = reader->getPath();
  reader->notify();
  cout<<"#Paths: "<<pathVector.size()<<"| SvgReader Notifica"<< endl;

  
  /*
	for (int i = 0; i < svgPathVector.size();  i++){
		selectFromPath(svgPathVector[i], 0, 0, &newPathVector);
	}*/



	cout<<"Tam de newVec: "<<newPathVector.size()<<endl;
	for (int i = 0; i < newPathVector.size();  i++){
	  //newPathVector[i].printAttributes();
	}



  //int f = stoi("FFFFFF",0, 16);
  
  /*string color = "#FFAABB";
  
  string red = color.substr(1,2);
  string green = color.substr(3,2);
  string blue = color.substr(5,2);

  cout<<"RED: "<<red<<endl;
  cout<<"GREEN: "<<green<<endl;
  cout<<"BLUE: "<<blue<<endl;
  color.erase(0,1); 
  cout<<"wqe: "<<color<<endl;*/
  return 0;
}