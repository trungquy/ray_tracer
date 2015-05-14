#ifndef QOBJECT_H
#define QOBJECT_H
#include "QVector.h"
#include "define.h"
#include <iostream>
#include <string>
class QObject{
protected:
	double reflection; /*reflection rate: mirror = 1 , diffuse=0 */
	double refraction; /* transparency: opaque = 0, glass = 1*/
	double eta;
	bool is_sqecular;/*1: specular obj; 0: diffuse obj*/
public:
	QObject() {reflection=0; refraction=0;eta=1;is_sqecular=true;};
	double getEta(){return eta;};
	double getReflection(){return reflection;};
	double getRefraction(){return refraction;};
	bool isSqecular(){return is_sqecular;};

	// Virtual method functions
	virtual QVector getColor() const {return QVector::parseColor(COLOR_BLACK);};
	virtual QVector getNormalAt(QVector phit) {
		return QVector(0, 0, 0);
	}
	virtual int getObjType(){ return OBJ_TYPE_OBJ;}
	QVector getHittingColor(const QVector& rayOrig, const QVector& rayDir, double t, QVector lightSrc){
		QVector phit = rayOrig + rayDir*t;
		QVector nhit = getNormalAt(phit);
		double cosTheta = QVector::cos(lightSrc - phit,nhit);
		double alpha = ((1-MIN_DARK)*cosTheta + MIN_DARK + 1)/2;
		QVector chit = getColor()*alpha;//color of hitting point

		QVector R = QVector::reflect(phit-lightSrc,nhit);
		double costheta2 = pow(QVector::cos(R,rayOrig-phit),N_HL);
		if(costheta2 > HIGHLIGHT_MIN){
			QVector w = QVector::parseColor(COLOR_WHITE);
			QVector HC = (chit*(1-costheta2) + w*(costheta2-HIGHLIGHT_MIN))/(1-HIGHLIGHT_MIN);
			chit = HC;
		}
		return chit;
	}

	virtual bool intersect(const QVector& rayOrig, const QVector& rayDir, double* t){
		return false;
	}

	virtual void printInfo(){};
	std::string getObjectInfo(){
		return QVector(reflection, refraction, eta).toString();		
	}

};
#endif