#ifndef QBALL_H
#define QBALL_H

#include "QObject.h"
#include "QVector.h"

class QBall:public QObject {
private:
	QVector center;
	double radius; //radius
	QVector color;
private:
	QVector phit; // temporary hitting point
public:
	QBall();
	QBall(const QVector& _center, double _r, const QVector& _color);
	QBall(const QVector& _center, double _r, const QVector& _color,bool _is_sqecular, double _reflection, double _refraction, double _eta);
	QBall(const QBall& _other);
	QVector getCenter() const;
	void setCenter(const QVector& _center) ;
	double getRadius() const;
	void setRadius(double _r);
	QVector getColor() const {return color;};
	QVector getNormalAt(QVector phit);
	bool intersect(const QVector& rayOrig, const QVector& rayDir, double* hit);	
	void printInfo();
	int getObjType(){ return OBJ_TYPE_BALL;}
};

#endif /*QBALL_H*/