#include "QPlane.h"

QPlane::QPlane(){
	point = QVector();
	normal = QVector();
}
QPlane::QPlane(const QVector& _point, const QVector& _normal){
	QObject();
	point = _point;
	normal = _normal;
	normal.normalize();
}
QPlane::QPlane(const QVector& _point, const QVector& _normal,bool _is_sqecular, double _reflection, double _refraction, double _eta){
	point = _point;
	normal = _normal;
	normal.normalize();
	is_sqecular = _is_sqecular;
	reflection = _reflection;
	refraction = _refraction;
	eta = _eta;
}
QVector QPlane::getPoint() const {
	return point;
}
QVector QPlane::getNormal() const{
	return normal;
}
QVector QPlane::getNormalAt(QVector phit){
	return normal.normalize();
}

QVector QPlane::getColor() const{
	int sq = (int)floor(phit.getX()/250) + (int)floor(phit.getY()/250);			
	if(sq % 2 == 0){
		return QVector::parseColor(COLOR_CHECKBORAD1);
	}else{
		return QVector::parseColor(COLOR_WHITE);
	}
}

bool QPlane::intersect(const QVector& rayOrig, const QVector& rayDir, double* t){
	double k = rayDir*normal;
	if( abs(k) < EPSILON){//k == 0		
		return false;
	}
	*t = (-(rayOrig - point)*normal)/k;
	if(*t<EPSILON)
		return false;
	phit = rayOrig + rayDir*(*t);
	return true;
}

void QPlane::printInfo(){
	cout<<"\tType:  Plane"<<endl;
	cout<<"\tData:"<<endl;
	cout<<"\t\tPoint:"<<point.toString()<<endl;
	cout<<"\t\tNormal Vector:"<<normal.toString()<<endl;
	cout<<"\t\t[reflection, refraction, eta] = "<<getObjectInfo()<<endl;
}
