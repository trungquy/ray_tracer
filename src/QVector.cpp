#include "QVector.h"
//using std::ostream;

QVector::QVector() {
    x = 0;
    y = 0;
    z = 0;
    t = 0;
}

QVector::QVector(const QVector& _other) {
    QVector();
    x = _other.getX();
    y = _other.getY();
    z = _other.getZ();
    t = _other.getT();
}

void QVector::setX(double _x) {
    x = _x;
}

void QVector::setY(double _y) {
    y = _y;
}

void QVector::setZ(double _z) {
    z = _z;
}

void QVector::setT(double _t) {
    t = _t;
}

double QVector::getX() const {
    return x;
}

double QVector::getY() const {
    return y;
}

double QVector::getZ() const {
    return z;
}

double QVector::getT() const {
    return t;
}

string QVector::toString() {
    string str = "(" + std::to_string((long double) x) + "," + std::to_string((long double) y) + "," + std::to_string((long double) z) + ")";
    return str;
}

QVector& QVector::operator=(const QVector& _other) {
    if (this == &_other)
        return *this;
    x = _other.getX();
    y = _other.getY();
    z = _other.getZ();
    t = _other.getT();
    return *this;
}

bool QVector::operator==(const QVector& _other) {
    //return x == _other.getX() && y == _other.getY() && z == _other.getZ() && t == _other.getT();
    //return x == _other.getX() && y == _other.getY() && z == _other.getZ();
    return fabs(x - _other.getX()) < EPSILON && fabs(y - _other.getY()) < EPSILON && fabs(z - _other.getZ()) < EPSILON;
}

QVector QVector::operator+(const QVector& _other) const {
    double _x = x + _other.getX();
    double _y = y + _other.getY();
    double _z = z + _other.getZ();
    return QVector(_x, _y, _z);
}

QVector QVector::operator-(const QVector& _other) const {
    double _x = x - _other.getX();
    double _y = y - _other.getY();
    double _z = z - _other.getZ();
    return QVector(_x, _y, _z);
}

QVector QVector::operator-() const {
    return QVector(-x, -y, -z, t);
}
//scalar multiplication

QVector QVector::operator*(double c) const {
    return QVector(x*c, y*c, z*c, t);
}
//scalar division

QVector QVector::operator/(double c) const {
    return QVector(x / c, y / c, z / c, t);
}
//dot product

double QVector::operator*(const QVector& _other) const {
    return dotProduct(*this, _other);
}

QVector QVector::operator&(const QVector& _other) const {// cross product
    return crossProduct(*this, _other);
}

std::ostream& operator<<(std::ostream &os, const QVector &v) {
    //os<< "["<< v.x <<" "<< v.y <<" "<< v.z <<"]";
    //return os;
}

double QVector::dotProduct(const QVector& v1, const QVector& v2) {
    return v1.getX() * v2.getX() + v1.getY() * v2.getY() + v1.getZ() * v2.getZ();
}

QVector QVector::crossProduct(const QVector& v1, const QVector& v2) {
    double _x = v1.getY() * v2.getZ() - v2.getY() * v1.getZ();
    double _y = -(v1.getX() * v2.getZ() - v2.getX() * v1.getZ());
    double _z = v1.getX() * v2.getY() - v2.getX() * v1.getY();
    return QVector(_x, _y, _z);
}

double QVector::norm() const {
    return sqrt(x * x + y * y + z * z);
}

QVector& QVector::normalize() {
    double n = norm();
    x /= n;
    y /= n;
    z /= n;
    return *this;
}
// Distance to a point

double QVector::distance(const QVector& v, const QVector& p) {
    //return sqrt((v.getX()-p.getX())*(v.getX()-p.getX()) + (v.getY()-p.getY())*(v.getY()-p.getY()) + (v.getZ()-p.getZ())*(v.getZ()-p.getZ()));
    return (v - p).norm();
}

double QVector::distance(const QVector& p) {
    //return sqrt( (x-p.getX())*(x-p.getX()) + (y-p.getY())*(y-p.getY()) + (z-p.getZ())*(z-p.getZ()) );
    return (*this-p).norm();
}

double QVector::dot4(const QVector& p, const QVector& q) {
    return p.getX() * q.getX() + p.getY() * q.getY() + p.getZ() * q.getZ() + p.getT() * q.getT();
}

QVector QVector::reflect(QVector I, QVector N) {
    return I - N * ((I * N) / (N * N))*2;
}

QVector QVector::refract(const QVector& I, const QVector& N, double eta_I, double eta_T) {
    QVector NN = N / (N.norm());
    QVector K = NN * ((-I) * NN);
    double sin_phiT = (eta_I / eta_T)*(sqrt(1 - pow(QVector::cos(-I, NN), 2)));
    double cos_phiT = sqrt(1 - pow(sin_phiT, 2));
    QVector M = K.normalize() * I.norm() * sin_phiT;
    QVector N_ = -NN * I.norm() * cos_phiT;
    return M + N_;
}

double QVector::cos(QVector V1, QVector V2) {
    return (V1 * V2) / (V1.norm() * V2.norm());
}

QVector QVector::parseColor(int c) {
    double r = ((c & 0xff0000) / 0xffff) / 255.0;
    double g = ((c & 0x00ff00) / 0xff) / 255.0;
    double b = ((c & 0x0000ff)) / 255.0;
    return QVector(r, g, b);
}
