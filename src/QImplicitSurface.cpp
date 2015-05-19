#include "QImplicitSurface.h"

QImplicitSurface::QImplicitSurface(){
	QObject();
}
QImplicitSurface::QImplicitSurface(const std::vector<QVector>& _centers, double _T, const QVector& _color){
	QObject();
	centers = _centers;
	T = _T;
	color = _color;	
	calBoundary();
}
QImplicitSurface::QImplicitSurface(const std::vector<QVector>& _centers, double _T, const QVector& _color,bool _is_sqecular, double _reflection, double _refraction, double _eta){
	QObject();
	centers = _centers;
	T = _T;
	color = _color;
	is_sqecular = _is_sqecular;
	reflection = _reflection;
	refraction = _refraction;
	eta = _eta;
	a = 130;
	b = 0.000005;
	//b = 0.001;
	calBoundary();
}

std::vector<QVector> QImplicitSurface::getCenters() const{
	return centers;
}

void QImplicitSurface::setCenters(const std::vector<QVector>& _center){
	centers = _center;
}
double QImplicitSurface::getT() const{
	return T;
}
void QImplicitSurface::setT(double _T){
	T = _T;
}

QVector QImplicitSurface::getNormalAt(QVector phit){	
	QVector dx(EPSILON,0,0);
	QVector dy(0,EPSILON,0);
	QVector dz(0,0,EPSILON);
	double dfx = calTotalEnergy(phit+dx)-calTotalEnergy(phit-dx);
	double dfy = calTotalEnergy(phit+dy)-calTotalEnergy(phit-dy);
	double dfz = calTotalEnergy(phit+dz)-calTotalEnergy(phit-dz);	
	QVector normal(dfx,dfy,dfz);
	return -(normal);	
}
bool QImplicitSurface::intervalApproximate(double t1, double t2, double *t,const QVector& rayOrig, const QVector& rayDir){
	double m = (t1+t2)/2;	
	// Calculate interval of f([t1, t2])
	double f_min=0, f_max=0;
	calEnergyRange(t1,t2,rayOrig, rayDir,&f_min,&f_max);
	
	if(!(f_min<=EPSILON && f_max>=EPSILON))
		return false;
	if(fabs(t2-t1) < EPSILON){
		*t = m;		
		return true;
	}
	if(intervalApproximate(t1,m,t,rayOrig,rayDir))
		return true;
	return intervalApproximate(m,t2,t,rayOrig,rayDir);
}
bool QImplicitSurface::intersect(const QVector& rayOrig, const QVector& rayDir, double* t){
	//calculate initial interval	
	// check whether ray hit the boundary volume (Bmin, Bmax) or not
	double t1, t2;
	if (calInitialRange(rayOrig,rayDir,&t1,&t2)){
		QVector p1 = rayOrig + rayDir*t1;
		QVector p2 = rayOrig + rayDir*t2;		
		return intervalApproximate(t1,t2,t,rayOrig,rayDir) && *t>EPSILON;		
	}
	return false;
}
bool QImplicitSurface::calInitialRange(const QVector& rayOrig, const QVector& rayDir, double *t1, double *t2){
	double tmin_x, tmin_y, tmin_z;
	double tmax_x, tmax_y, tmax_z;
	if (rayDir.getX() == 0){
		tmin_x = -INFINITY;
		tmax_x = INFINITY;
	} else {
		tmin_x = (Bmin.getX()-rayOrig.getX())/rayDir.getX();
		tmax_x = (Bmax.getX()-rayOrig.getX())/rayDir.getX();
		if (rayDir.getX() < 0){
			swap(&tmin_x,&tmax_x);
		}
	}
	if (rayDir.getY() == 0){
		tmin_y = -INFINITY;
		tmax_y = INFINITY;
	} else {
		tmin_y = (Bmin.getY()-rayOrig.getY())/rayDir.getY();
		tmax_y = (Bmax.getY()-rayOrig.getY())/rayDir.getY();
		if (rayDir.getY() < 0){
			swap(&tmin_y,&tmax_y);
		}
	}
	if (rayDir.getZ() == 0){
		tmin_z = -INFINITY;
		tmax_z = INFINITY;
	} else {
		tmin_z = (Bmin.getZ()-rayOrig.getZ())/rayDir.getZ();
		tmax_z = (Bmax.getZ()-rayOrig.getZ())/rayDir.getZ();
		if (rayDir.getZ() < 0){
			swap(&tmin_z,&tmax_z);
		}
	}
	*t1 = std::max(std::max(tmin_x,tmin_y),tmin_z);
	*t2 = std::min(std::min(tmax_x,tmax_y),tmax_z);	
	return *t1<*t2;
	/*if (*t1>*t2){
		swap(t1,t2);
	}*/
	//return true;	
	//return *t1 > 0 && *t1<*t2;
}

void QImplicitSurface::calQuadRange(double coff_a, double coff_b, double coff_c, double t1, double t2, double *r1, double *r2){
	if (fabs(coff_a)<EPSILON) {
		*r1 = coff_b*t1 + coff_c;
		*r2 = coff_b*t2 + coff_c;
		if (coff_b<0) swap(r1, r2);
		return;
	}
	double extp = -coff_b/2/coff_a;
	double v1 = coff_a*t1*t1 + coff_b*t1 +coff_c;
	double v2 = coff_a*t2*t2 + coff_b*t2 + coff_c;
	double extrema = coff_c-coff_b*coff_b/4/coff_a;
	if (extp > t1 && extp < t2){
		*r1 = std::min(std::min(v1,v2),extrema);
		*r2 = std::max(std::max(v1,v2),extrema);
	}else{
		*r1 = std::min(v1,v2);
		*r2 = std::max(v1,v2);
	}
}

void QImplicitSurface::calEnergyRange(double t1, double t2,const QVector& rayOrig, const QVector& rayDir, double *f_min, double *f_max){
	*f_min=-T, *f_max=-T;
	double r_min, r_max;
	for (int i=0;i<centers.size();i++){
		double coff_a = rayDir*rayDir;
		double coff_b = (rayOrig-centers[i])*rayDir*2;
		double coff_c = (rayOrig-centers[i])*(rayOrig-centers[i]);
		calQuadRange(coff_a,coff_b,coff_c,t1,t2,&r_min,&r_max);
		*f_min += a*exp(-b*r_max);
		*f_max += a*exp(-b*r_min);
	}
}
double QImplicitSurface::calEnergy(const QVector& c,const QVector& p){	
	return a*exp(-b*pow(QVector::distance(p,c),2));	
}
double QImplicitSurface::calTotalEnergy(const QVector& p){
	double ret = 0;
	for (int i = 0; i<centers.size();i++){
		ret += calEnergy(centers[i],p);
	}
	return ret;
}
double QImplicitSurface::calTotalEnergy(const QVector& rayOrig, const QVector& rayDir,double t){
	QVector p = rayOrig + rayDir*t;
	return calTotalEnergy(p);
}
void QImplicitSurface::calBoundary(){
	Bmin = QVector(INFINITY, INFINITY, INFINITY);
	Bmax = QVector(-INFINITY, -INFINITY, -INFINITY);
	for (int i=0;i<centers.size();i++){
		// calculate total energy at i-th center
		double e = calTotalEnergy(centers[i]);	
		double r_max = 2*sqrt(log(e/T)/b);//multiply by 2 to make sure it contain objects
		//r_max = 300;
		QVector p1 = centers[i]-QVector(r_max,r_max,r_max);
		QVector p2 = centers[i]+QVector(r_max,r_max,r_max);
		if(Bmin.getX() > p1.getX()){Bmin.setX(p1.getX());}
		if(Bmax.getX() < p2.getX()){Bmax.setX(p2.getX());}
		if(Bmin.getY() > p1.getY()){Bmin.setY(p1.getY());}
		if(Bmax.getY() < p2.getY()){Bmax.setY(p2.getY());}
		if(Bmin.getZ() > p1.getZ()){Bmin.setZ(p1.getZ());}
		if(Bmax.getZ() < p2.getZ()){Bmax.setZ(p2.getZ());}
	}	
}
void QImplicitSurface::printInfo(){

}