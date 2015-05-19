#ifndef QPLANE_H
#define QPLANE_H

#include "QObject.h"
#include "QVector.h"

class QPlane: public QObject{
private:
	QVector point;  //a point 
	QVector normal; //a normal vector 	
private:
	QVector phit; // temporary hitting point
public:
	QPlane();
	QPlane(const QVector& _point, const QVector& _normal);
	QPlane(const QVector& _point, const QVector& _normal,bool _is_sqecular, double _reflection, double _refraction, double _eta);
	QVector getPoint() const;
	QVector getNormal() const;
	QVector getNormalAt(QVector phit);
	QVector getColor() const;
	QVector getHittingPoint() {return phit;};	
	bool intersect(const QVector& rayOrig, const QVector& rayDir, double* t);
	void printInfo();
	int getObjType(){ return OBJ_TYPE_PLANE;}
};
#endif