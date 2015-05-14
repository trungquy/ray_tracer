#ifndef QIMPLICITSURFACE_H
#define QIMPLICITSURFACE_H

#include <vector>
#include "QObject.h"
#include "QVector.h"

class QImplicitSurface: public QObject{
private:
	std::vector<QVector> centers;
	double T; // iso value T
	QVector color;
	double a;
	double b;
private:
	QVector Bmin;
	QVector Bmax;
private:
	double calEnergy(const QVector& c,const QVector& p);	
	double calTotalEnergy(const QVector& p);
	double calTotalEnergy(const QVector& rayOrig, const QVector& rayDir,double t);
	bool intervalApproximate(double t1, double t2, double *t,const QVector& rayOrig, const QVector& rayDir);
	void calBoundary();
	bool calInitialRange(const QVector& rayOrig, const QVector& rayDir, double *t1, double *t2);
	void calQuadRange(double coff_a, double coff_b, double coff_c, double t1, double t2, double *r1, double *r2);
	void calEnergyRange(double t1, double t2,const QVector& rayOrig, const QVector& rayDir, double *f_min, double *f_max);
	void swap(double *t1,double *t2){double t=*t1;*t1=*t2;*t2=t;};

public:
	QImplicitSurface();
	QImplicitSurface(const std::vector<QVector>& _centers, double _T, const QVector& _color);
	QImplicitSurface(const std::vector<QVector>& _centers, double _T, const QVector& _color,bool _is_sqecular, double _reflection, double _refraction, double _eta);
	std::vector<QVector> getCenters() const;
	void setCenters(const std::vector<QVector>& _center);
	double getT() const;
	void setT(double _T);
	QVector getColor() const {return color;};
	QVector getNormalAt(QVector phit);
	bool intersect(const QVector& rayOrig, const QVector& rayDir, double* t);
	void printInfo();
	int getObjType(){ return OBJ_TYPE_IMPLICIT;}

};
#endif