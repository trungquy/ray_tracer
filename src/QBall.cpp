#include "QBall.h"

QBall::QBall() {
    center = QVector();
}

QBall::QBall(const QVector& _center, double _r, const QVector& _color) {
    QObject();
    center = _center;
    radius = _r;
    color = _color;
}

QBall::QBall(const QVector& _center, double _r, const QVector& _color, bool _is_sqecular, double _reflection, double _refraction, double _eta) {
    center = _center;
    radius = _r;
    color = _color;
    is_sqecular = _is_sqecular;
    reflection = _reflection;
    refraction = _refraction;
    eta = _eta;
}

QBall::QBall(const QBall& _other) {
    center = _other.getCenter();
    radius = _other.getRadius();
}

QVector QBall::getCenter() const {
    return center;
}

void QBall::setCenter(const QVector& _center) {
    center = _center;
}

double QBall::getRadius() const {
    return radius;
}

void QBall::setRadius(double _r) {
    radius = _r;
}

QVector QBall::getNormalAt(QVector phit) {
    return (phit - center).normalize();
}

bool QBall::intersect(const QVector& rayOrig, const QVector& rayDir, double* t) {
    double a = rayDir*rayDir;
    double b = rayDir * (rayOrig - center);
    double c = (rayOrig - center)*(rayOrig - center) - radius*radius;
    double d = b * b - a*c;
    if (d < EPSILON) // d<=0
        return false;
    double t1 = (-b - sqrt(d)) / a;
    double t2 = (-b + sqrt(d)) / a;
    if (t1 < 0 && t2 < 0)
        return false;
    // choose smaller positive t for 1st hitting point	
    *t = t1 < t2 ? (t1 > 0 ? t1 : t2) : (t2 > 0 ? t2 : t1);
    //if (abs((int)(*t)) < EPSILON)
    if (fabs(*t) < EPSILON)
        return false;
    phit = rayOrig + rayDir * (*t);
    return true;
}

void QBall::printInfo() {
    cout << "\tType:  Ball" << endl;
    cout << "\tData:" << endl;
    cout << "\t\tCenter:" << center.toString() << endl;
    cout << "\t\tRadius:" << radius << endl;
    cout << "\t\t[reflection, refraction, eta] = " << getObjectInfo() << endl;
}