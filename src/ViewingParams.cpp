#include "ViewingParams.h"

ViewingParams::ViewingParams(){
}
ViewingParams::ViewingParams(const QVector& _eye, const QVector& _dir, const QVector& _headUp ){
	eye = _eye;
	dir	= _dir;
	headUp = _headUp;
	this->updateEyeCoordinate();
	this->updateVewingTranformationMatrix();
}
std::vector<QVector> ViewingParams::updateEyeCoordinate(){
	QVector w = -dir;
	QVector u = dir & headUp;//cross product
	QVector v = w & u;
	w.normalize(); u.normalize(); v.normalize();
	eyeCoord.clear();
	eyeCoord.push_back(u);// append one element to a vector
	eyeCoord.push_back(v);
	eyeCoord.push_back(w);
	return eyeCoord;
}
std::vector<QVector> ViewingParams::getEyeCoordinate(){
	return eyeCoord;
}
std::vector<QVector> ViewingParams::updateVewingTranformationMatrix(){
	QVector row1(eyeCoord[0].getX(),eyeCoord[1].getX(),eyeCoord[2].getX(),eye.getX());
	QVector row2(eyeCoord[0].getY(),eyeCoord[1].getY(),eyeCoord[2].getY(),eye.getY());
	QVector row3(eyeCoord[0].getZ(),eyeCoord[1].getZ(),eyeCoord[2].getZ(),eye.getZ());		
	QVector row4(0,0,0,1);
	transMatrix.clear();
	transMatrix.push_back(row1);
	transMatrix.push_back(row2);
	transMatrix.push_back(row3);
	transMatrix.push_back(row4);
	return transMatrix;
}
std::vector<QVector> ViewingParams::getVewingTranformationMatrix(){
	return transMatrix;
}
//transform a point from eye coordinate to world coordinate using viewing transformation matrix
QVector ViewingParams::calPosInWorldCoord(const QVector& p ){
	//P_w = M . P_e	
	double _x = QVector::dot4(transMatrix[0],p);	
	double _y = QVector::dot4(transMatrix[1],p);	
	double _z = QVector::dot4(transMatrix[2],p);
	return QVector(_x, _y, _z,1);
}