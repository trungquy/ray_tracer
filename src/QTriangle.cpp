#include "QTriangle.h"

QTriangle::QTriangle(){
	p0 = QVector();
	p1 = QVector();
	p2 = QVector();
	color = QVector();
}
QTriangle::QTriangle(const QVector& _p0,const QVector& _p1,const QVector& _p2, const QVector& _color,bool _is_sqecular, double _reflection, double _refraction, double _eta){
	p0 = _p0;
	p1 = _p1;
	p2 = _p2;
	color = _color;
	is_sqecular = _is_sqecular;
	reflection = _reflection;
	refraction = _refraction;
	eta = _eta;
	
	QVector p0p2 = p2-p0;
	QVector p0p1 = p1-p0;
	normal =  (p0p1 & p0p2).normalize(); //cross product	
	u_hat = (p0p2 & normal) / ((p0p1 & p0p2) * normal);
	v_hat = (normal & p0p1) / ((p0p1 & p0p2) * normal);
}

bool QTriangle::intersect(const QVector& rayOrig, const QVector& rayDir, double* t){
	double k = rayDir*normal;
	if( fabs(k) < EPSILON){//k == 0		
		return false;
	}
	*t = (-(rayOrig - p0)*normal)/k;
	if(*t<EPSILON)
		return false;
	QVector phit = rayOrig + rayDir*(*t);
	double u = (phit - p0)*u_hat;
	double v = (phit - p0)*v_hat;
	if( u > 0 && v > 0 && u+v < 1)
		return true;
	return false;
}
void QTriangle::printInfo(){

}