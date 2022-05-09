#include "ObserverPattern.h"
#include "Selection.h"
#include "Generator.h"
#include "Routing.h"

#ifndef ANIMATIONGENERATOR_HEADER
#define ANIMATIONGENERATOR_HEADER

class AnimationGenerator : public Observer {
private:
    Selection * selection;
    Routing * routing;
    Generator * generator;
    //La clase que divide los putos y se los va pasando a seleccion como parametro
    //También puede estar la otra clase que me indica ordena los path por frames
public:
    AnimationGenerator(){}
    ~AnimationGenerator() {}
	void update(void* dato) {
	// (int*) = castear el puntero void a puntero a int
	// *(int*) = valor del int al que apunta
		int value = *(int*)dato;
		
	    switch (value){
	        case 1:
	            selection->aviso();
	            break;
	        case 2:
	            routing->routingFunction();
	            break;
	        case 3:
	            //generador;
	            break;
	        default:
	            break;
	    }
}

//    void update(void* profe) {
//        GeneratorSVG value = *(GeneratorSVG*)profe;
//        cout << "El GeneratorSVG " << value.getNombre() << " creo una tarea" << endl;
//    }
} ;

#endif
