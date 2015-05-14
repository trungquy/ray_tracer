#ifndef SCREEN_PARAMS
#define SCREEN_PARAMS

#include "QVector.h"
class ScreenParams {
private:
	double nx;
	double ny;
	double theta;// field of view
private:
	double d;// distance from eye to screen
public:
	ScreenParams(){d=-1;};
	ScreenParams(double _nx, double _ny, double _theta):nx(_nx), ny(_ny), theta(_theta){		
		update_distance2eye();
	};
	double get_nx(){return nx;};
	double get_ny(){return ny;};
	double get_theta(){return theta;};
	double update_distance2eye(){d=ny/(2*tan(theta/2)); return d;};
	double get_distance2eye(){		
		return d;
	}
};
#endif