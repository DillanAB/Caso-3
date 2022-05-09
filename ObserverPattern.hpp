#ifndef OBSERVER_HEADER
#define OBSERVER_HEADER
#include "RoutingPoint.hpp"

//Este es el observador
class Observer{
public:
    virtual ~Observer(){};
    virtual void update(void* element) = 0;
    //virtual void notifySelection(void* element, string notices){};
    //virtual void notifyRouting (void* element, string notices){};
};

//Este es el que le manda una noficiación
class Subject {
public:
    virtual ~Subject(){};
    virtual void attach(Observer *Observer) = 0; //Agregar
    //virtual void detach(Observer *Observer) = 0; //Eliminar
    virtual void notify(void* element) = 0;   //Notifica al observador que haga algo
};

struct Object{
	
	double high, width;
	int angle;
	int frames;
	vector<double> point;
	PathPoint routingPoint;
	double xInitial, yInitial, frameController;
};

#endif
