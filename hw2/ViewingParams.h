#ifndef VIEWINGPARAMS_H
#define VIEWINGPARAMS_H

#include "QVector.h"
#include <vector>

class ViewingParams{
private:
	//input parameters
	QVector eye;
	QVector dir; //direction vector
	QVector headUp; //head-up vector
	
	//calculated values
	std::vector<QVector> eyeCoord;
	std::vector<QVector> transMatrix;
public:
	ViewingParams();
	ViewingParams(const QVector& _eye, const QVector& _dir, const QVector& _headUp );
	QVector getEye() const {return eye;};
	QVector getDirection() const {return dir;};
	QVector getHeadUp() const {return headUp;};
	std::vector<QVector> updateEyeCoordinate();
	std::vector<QVector> getEyeCoordinate();
	std::vector<QVector> updateVewingTranformationMatrix();
	std::vector<QVector> getVewingTranformationMatrix();
	//convert a point in eye coordinate to world coordinate using viewing transformation matrix
	QVector calPosInWorldCoord(const QVector& p );	
};
#endif