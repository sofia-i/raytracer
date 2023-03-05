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

template <typename T>
class vec3 {
public:
    T e[3];

    vec3() { }
    vec3(T e0, T e1, T e2) {
        e[0] = e0; e[1] = e1, e[2] = e2;
    }

    // allow access by x, y, z (acting as coordinates)
    T x() const { return e[0]; }
    T y() const { return e[1]; }
    T z() const { return e[2]; }

    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    double operator[](int i) const { return e[i]; }
    // double& operator[](int i) { return e[i]; }

    // compound operators
    vec3& operator+=(const vec3<T> &v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    vec3& operator*=(const vec3<T> &v) {
        e[0] *= v.e[0];
        e[1] *= v.e[1];
        e[2] *= v.e[2];
        return *this;
    }

    vec3& operator /=(const T val) {
        e[0] *= 1/val;
        e[1] *= 1/val;
        e[2] *= 1/val;
        return *this;
    }

    double length() const {
        return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
    }
    
    bool isNormalized() const {
        double length = this->length();
        const auto relative_difference_factor = 0.0001;
        const auto greater_magnitude = std::max(1.0, length);
        
        return std::abs(1.0 - length) < relative_difference_factor * greater_magnitude;
    }
    
    bool checkInBounds(T min, T max) {
        return e[0] >= min && e[0] <= max &&
                e[1] >= min && e[1] <= max &&
                e[2] >= min && e[2] <= max;
    }


};

// utility functions
template <typename T>
inline std::ostream& operator<<(std::ostream &out, const vec3<T> &v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

template <typename T>
inline vec3<T> operator+(const vec3<T> &u, const vec3<T> &v) {
    return vec3<T>(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

template <typename T>
inline vec3<T> operator-(const vec3<T> &u, const vec3<T> &v) {
    return vec3<T>(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

template <typename T>
inline vec3<T> operator*(const vec3<T> &u, const vec3<T> &v) {
    return vec3<T>(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

template <typename S, typename T>
inline vec3<T> operator*(const S k, const vec3<T> &v) {
    return vec3<T>(k * v.e[0], k * v.e[1], k * v.e[2]);
}

template <typename S, typename T>
inline vec3<T> operator*(const vec3<T> &v, const S k) {
    return vec3<T>(k * v.e[0], k * v.e[1], k * v.e[2]);
}

template <typename S, typename T>
inline vec3<T> operator/(const vec3<T> &v, const S k) {
    return vec3<T>((1/k) * v.e[0], (1/k) * v.e[1], (1/k) * v.e[2]);
}

template <typename T>
inline T dot(const vec3<T> &v, const vec3<T> &u) {
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

template <typename T>
inline vec3<T> cross(const vec3<T> &u, const vec3<T> &v) {
    return vec3<T>(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

template <typename T>
inline vec3<T> getUnitVector(const vec3<T> &v) {
    return v / v.length();
}

inline vec3<int> toIntVec3(const vec3<double> &v) {
    return vec3<int>((int)(v.e[0]), (int)(v.e[1]), (int)(v.e[2]));
}

inline vec3<int> clip(const vec3<int> &v, int low, int high) {
    int x = std::max(v.e[0], low);
    x = std::min(x, high);
    
    int y = std::max(v.e[1], low);
    y = std::min(y, high);
    
    int z = std::max(v.e[2], low);
    z = std::min(z, high);
    
    return vec3<int>(x, y, z);
}

#endif /* vec3_hpp */
