#ifndef QTRIANGLE_H
#define QTRIANGLE_H
#include "QObject.h"
#include "QVector.h"

class QTriangle:public QObject {
private:
	QVector p0;
	QVector p1;
	QVector p2;
	QVector color;
private:
	//pre-computed values
	QVector u_hat;
	QVector v_hat;
	QVector normal;
public:
	QTriangle();
	QTriangle(const QVector& _p0,const QVector& _p1,const QVector& _p2, const QVector& _color,bool _is_sqecular, double _reflection, double _refraction, double _eta);	
	QVector getColor() const {return color;};
	QVector getNormalAt(QVector phit){ return normal;};
	QVector getP0() const {return p0;}
	QVector getP1() const {return p1;}
	QVector getP2() const {return p2;}
	bool intersect(const QVector& rayOrig, const QVector& rayDir, double* t);	
	void printInfo();
	int getObjType(){ return OBJ_TYPE_TRIANGLE;}
};
#endif