#ifndef OBSERVER_HEADER
#define OBSERVER_HEADER
#include <vector>
using namespace std;

class Observer{
public:
   virtual void update(void* pPointer) = 0;
};

class Subject{
protected:
   vector<Observer*> observersVector;

public:
   virtual void notify(void* pPointer){
      for (unsigned int observerIndex = 0; observerIndex < observersVector.size(); observerIndex++){
         observersVector[observerIndex]->update(pPointer);
      }
   }
   virtual void attach(Observer * pObs){
      observersVector.push_back(pObs);
   }

   virtual void detach() = 0;
};
#endif