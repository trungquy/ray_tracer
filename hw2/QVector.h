#ifndef QVECTOR_H
#define QVECTOR_H

#include <string>
#include <math.h>
#include "define.h"
using namespace std;

class QVector {
private:
    double x;
    double y;
    double z;
    double t; // Point = 1, vector = 0 or and arbitrary values for 4-D vector
public:
    QVector();

    QVector(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {
        t = 0;
    };

    QVector(double _x, double _y, double _z, double _t) : x(_x), y(_y), z(_z), t(_t) {
    };
    QVector(const QVector& _other);
    void setX(double _x);
    void setY(double _y);
    void setZ(double _z);
    void setT(double _t);
    std::string toString();
    double getX() const;
    double getY() const;
    double getZ() const;
    double getT() const;

    QVector& operator=(const QVector& _other);
    bool operator==(const QVector& _other);

    QVector operator+(const QVector& _other) const;
    QVector operator-(const QVector& _other) const;
    QVector operator-() const;
    QVector operator*(double c) const; //scalar multiplication
    double operator*(const QVector& _other) const; // dot product
    QVector operator/(double c) const; //scalar multiplication
    QVector operator&(const QVector& _other) const; // cross product	
    friend std::ostream& operator<<(std::ostream &os, const QVector &v);

    static double dotProduct(const QVector& v1, const QVector& v2);
    static QVector crossProduct(const QVector& v1, const QVector& v2);
    QVector& normalize();
    double norm() const;

    // Distance to a point
    static double distance(const QVector& v, const QVector& p);
    double distance(const QVector& p);
    // Dot product with four elements
    static double dot4(const QVector& p, const QVector& q);
    static QVector reflect(QVector I, QVector N);
    static QVector refract(const QVector& I, const QVector& N, double eta_I, double eta_T);
    static double cos(QVector V1, QVector V2);
    static QVector parseColor(int c);
};

#endif