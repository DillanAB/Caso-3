#include "Observer.hpp"
#include "SvgPath.hpp"
#include "RoutingPoint.hpp"
#include "Macros.hpp"

class Selector: public Subject, public Observer{
private:
   vector<SvgPath*> * paths;
   //vector<NewPath> * newPaths;
   PathPoint * pathPoint;
   vector<vector<float>> searchedPoints;
public: 
   Selector(vector<SvgPath*> * pPaths, PathPoint * pPathPoint, vector<vector<float>> pPoints){
      paths = pPaths;
      //newPaths = pNewPaths;
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

   //Recorre un Path y agrega nuevos path si hay instruciones que coincidan con el punto.
   void selectFromPath(SvgPath * pPath, float pXPoint, float pYPoint){
      vector<PathInstruction*> pathInstructions = pPath->getInstructions();
      vector<PathInstruction*> resultInstructions;
      pathPoint->setXYInitial(pXPoint, pYPoint);
      string figureName;
      Figure figure;
      for(unsigned int vectorIndex = 0; vectorIndex<pathInstructions.size(); vectorIndex++){
         PathInstruction * instPtr = pathInstructions[vectorIndex];
         //Poda
         if(instPtr->isInRange(pXPoint, pYPoint)){
            //Crear el point path
            figureName = instPtr->getNameName();
            figure.type = figureName;
            figure.color = pPath->getColor();
            pathPoint->setVectorFiguras(figure);
            //instPtr->adjustSize(pXPoint,pYPoint);
            //resultInstructions.push_back(instPtr);
         }
      }
      //if (resultInstructions.size()>0){
         //NewPath newPath(resultInstructions, pPath, pXPoint, pYPoint);
            //pNewPathVector->push_back(newPath);
      //}
   }

   /*void update(void * pPointer){
      unsigned int pathsAmount = paths->size();
      while(searchedPoints.empty()==false){
         vector<float> point = searchedPoints.back();
         searchedPoints.pop_back(); //Saca el punto de la lista
         for (unsigned int pathIndex = 0; pathIndex < pathsAmount; pathIndex++){
            selectFromPath(paths->at(pathIndex), point[0], point[1], newPaths);
            bool last = (searchedPoints.empty() && pathIndex == (pathsAmount-1));
            notify(&last);
         }
      }
   }*/

   void update(void * pPointer){
      while(searchedPoints.empty()==false){
         vector<float> point = searchedPoints.back();
         searchedPoints.pop_back();
         tryPathsForPoint(point[0], point[1]);
         bool last = searchedPoints.empty();
         notify(&last);
         //pathPoint->cleanFiguras();
      }
   }

   void detach(){}
};