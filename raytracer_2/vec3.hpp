//
//  vec3.hpp
//  raytracer_2
//
//  Created by Sofia Iannicelli on 2/15/23.
//

#ifndef vec3_hpp
#define vec3_hpp

#include <stdio.h>

#include <cmath>
#include <iostream>


class vec3 {
public:
    double e[3];

    vec3() {
        e[0] = 0;
        e[1] = 0;
        e[2] = 0;
    }
    vec3(double e0, double e1, double e2) {
        e[0] = e0; e[1] = e1, e[2] = e2;
    }

    // allow access by x, y, z (acting as coordinates)
    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    double operator[](int i) const { return e[i]; }
    // double& operator[](int i) { return e[i]; }

    // compound operators
    vec3& operator+=(const vec3 &v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    vec3& operator*=(const vec3 &v) {
        e[0] *= v.e[0];
        e[1] *= v.e[1];
        e[2] *= v.e[2];
        return *this;
    }

    vec3& operator /=(const double val) {
        e[0] *= 1/val;
        e[1] *= 1/val;
        e[2] *= 1/val;
        return *this;
    }

    double length() const {
        return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
    }


};

// utility functions
inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(const double k, const vec3 &v) {
    return vec3(k * v.e[0], k * v.e[1], k * v.e[2]);
}

inline vec3 operator*(const vec3 &v, const double k) {
    return vec3(k * v.e[0], k * v.e[1], k * v.e[2]);
}

inline vec3 operator/(const vec3 &v, const double k) {
    return vec3((1/k) * v.e[0], (1/k) * v.e[1], (1/k) * v.e[2]);
}

inline double dot(const vec3 &v, const vec3 &u) {
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

inline vec3 getUnitVector(const vec3 &v) {
    return v / v.length();
}

#endif /* vec3_hpp */
