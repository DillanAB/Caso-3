#include "Observer.hpp"
#include "SvgPath.hpp"
#include "RoutingPoint.hpp"
#include "Macros.hpp"

class Selector: public Subject, public Observer{
private:
   vector<SvgPath*> * paths;
   PathPoint * pathPoint;
   vector<vector<float>> searchedPoints;
public: 
   Selector(vector<SvgPath*> * pPaths, PathPoint * pPathPoint, vector<vector<float>> pPoints){
      paths = pPaths;
      pathPoint = pPathPoint;
      searchedPoints = pPoints;
   }

   void setPathVector(vector<SvgPath*> * pPaths){
      paths = pPaths;
   }

   void tryPathsForPoint(float pXPoint, float pYPoint){
      for (unsigned int pathIndex = 0; pathIndex < paths->size(); pathIndex++){
         selectFromPath(paths->at(pathIndex), pXPoint, pYPoint);
      }
   }

   /*
   Selects the instrctions of a path that matches with a searched point.
   N = The path.
   Solution vector = The path.
   O(N)
   */
   void selectFromPath(SvgPath * pPath, float pXPoint, float pYPoint){
      vector<PathInstruction*> pathInstructions = pPath->getInstructions();
      vector<PathInstruction*> resultInstructions;
      pathPoint->setXYInitial(pXPoint, pYPoint);
      string figureName;
      Figure figure;
      for(unsigned int vectorIndex = 0; vectorIndex<pathInstructions.size(); vectorIndex++){
         PathInstruction * instPtr = pathInstructions[vectorIndex];
         //Poda
         //The instruction is discarded if the point is not in the X range.
         if(instPtr->isInXRange(pXPoint)){
            //Compare the point with all the ranges.
            if(instPtr->isInRange(pXPoint, pYPoint)){
               //Create the pathPoint.
               figureName = instPtr->getNameName();
               figure.type = figureName;
               figure.color = pPath->getColor();
               pathPoint->setVectorFiguras(figure);
            }
         }
      }
   }

   void update(void * pPointer){
      while(searchedPoints.empty()==false){
         vector<float> point = searchedPoints.back();
         searchedPoints.pop_back();
         tryPathsForPoint(point[0], point[1]);
         bool last = searchedPoints.empty(); //Notifies when is the last iteration.
         notify(&last);
      }
   }

   void detach(){}
};