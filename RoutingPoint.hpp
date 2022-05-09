//#include <vector>
//#include <array>
#ifndef ROUTINGPOINT_HEADER
#define ROUTINGPOINT_HEADER

struct Figure{
    string type;
    double route[10];
    string color;
    float strokeWidth;
};

struct Ordering{
    vector<vector<Figure>> frames;
} frameOrder;

struct Frame{
    int name;
    Figure array[100];
};

class PathPoint {
	
	private: 		
		double xyInitial[2]{0,0};
	    double xySecond[2]{0,0};
	    double xyThird[2]{0,0};
	    double xyFourth[2]{0,0};
	    double xyEnd[2]{0,0}; //Hasta donde debería llegar la imagen
	    Frame array[100];
	    double xIncrease, yIncrease = 0;
	    vector<Figure> figuras;
	    vector<vector<Figure>> frames;
	    vector<PathPoint> routing;
	    int frame;
	    double route;
    
    public:
    	void cleanFiguras(){
    		figuras.clear();
		}	
		void cleanFrames(){
			frames.clear();
		}
		void cleanrouting(){
			routing.clear();
		}		
    	vector<Figure> getFiguras(){
    		return figuras;
		}
    	void setXYInitial(double x, double y){
    		xyInitial[0] = x;
    		xyInitial[1] = y;
		}
		void setxyFinal(double x, double y){
    		xyEnd[0] = x;
    		xyEnd[1] = y;
		}
		void setxySecond(double x, double y){
			xySecond[0] = x;
			xySecond[1] = y;
		}
		void setxyThird(double x, double y){
			xyThird[0] = x;
			xyThird[1] = y;
		}
		void setxyFourth(double x, double y){
			xyFourth[0] = x;
			xyFourth[1] = y;
		}
		void setxyIncrease(double a, double a2){
			xIncrease = a; yIncrease = a2;
		}
		void setVectorFiguras(Figure f){
			figuras.push_back(f);
		}
		void setFrame(int frm){
			frame = frm;
		}
		void setroute(double rut){
			route = rut;
		}
		void setVectorVectorFiguraTodo(vector<vector<Figure>> vec){
			frames = vec;
		}
		void setVectorVectorFiguras(vector<Figure> f){
			frames.push_back(f);
		}
		void setVectorVectorFigurasFigura(Figure f, int num){
			frames[num].push_back(f);
		}
		void setrouting(PathPoint p){
			routing.push_back(p);
		}
		void setArray(int pos, Frame f){
			array[pos] = f;
		}
		void setArrayPos(int pos, Figure fig){
			array[pos].array[pos] = fig;
		}
		//
		double getXInicial(){
    		return xyInitial[0];
		}
		double getYInicial(){
			return xyInitial[1];
		}
		
		double getXFinal(){
    		return xyEnd[0];
		}
		double getYFinal(){
			return xyEnd[1];
		}		
		double getX2(){
			return xySecond[0];
		}
		double getY2(){
			return xySecond[1];
		}
		double getX3(){
			return xyThird[0];
		}
		double getY3(){
			return xyThird[1];
		}
		double getX4(){
			return xyFourth[0];
		}
		double getY4(){
			return xyFourth[1];
		}
		double getxIncrease(){
			return xIncrease;
		}
		double getyIncrease(){
			return yIncrease;
		}
		Figure getVectorFiguras(int num){
			return figuras[num];
		}
		int getFrame(int frm){
			return frame;
		}
		double getroute(){
			return route;
		}
		vector<Figure> getVectorVectorFigura(int num){
			return frames[num];
		}
		PathPoint getroutingPunto(int num){
			return routing[num];
		}
		vector<vector<Figure>> getVectorVector(){
			return frames;
		}
};

#endif
