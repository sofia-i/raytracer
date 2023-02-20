//
//  Sphere.h
//  raytracer_2
//
//  Created by Sofia Iannicelli on 2/15/23.
//

#ifndef Sphere_h
#define Sphere_h

#include "vec3.hpp"

class Sphere {
private:
    vec3<double> center;
    double radius;
    double kd;
    double ks;
    double ka;
    vec3<double> objectColor;
    vec3<double> objectSpecular;
    double kgls;
    std::string description;

public:
    Sphere(vec3<double> center, double radius, double kd, double ks, double ka,
           vec3<double> objectColor, vec3<double> objectSpecular, double kgls) {
        this->center = center;
        this->radius = radius;
        this->kd = kd;
        this->ks = ks;
        this->ka = ka;
        this->objectColor = objectColor;
        this->objectSpecular = objectSpecular;
        this->kgls = kgls;
        this->description = "";
    }

    Sphere(vec3<double> center, double radius, double kd, double ks, double ka,
           vec3<double> objectColor, vec3<double> objectSpecular, double kgls, std::string description) {
        this->center = center;
        this->radius = radius;
        this->kd = kd;
        this->ks = ks;
        this->ka = ka;
        this->objectColor = objectColor;
        this->objectSpecular = objectSpecular;
        this->kgls = kgls;
        this->description = description;
    }

    vec3<double> getCenter() const { return center; }
    double getRadius() const { return radius; }
    double getKd() const { return kd; }
    double getKs() const { return ks; }
    double getKa() const { return ka; }
    vec3<double> getObjectColor() const { return objectColor; }
    vec3<double> getObjectSpecular() const { return objectSpecular; }
    double getKgls() const { return kgls; }
    std::string getDescription() const { return description; }

    friend std::ostream& operator<<(std::ostream& os, Sphere const &sphere) {
        os << sphere.getDescription() << std::endl;
        os << "Center: " << sphere.getCenter() << std::endl;
        os << "Radius: " << sphere.getRadius() << std::endl;
        os << "Kd: " << sphere.getKd() << std::endl;
        os << "Ks: " << sphere.getKs() << std::endl;
        os << "Ka: " << sphere.getKa() << std::endl;
        os << "Object Color: " << sphere.getObjectColor() << std::endl;
        os << "Object specular: " << sphere.getObjectSpecular() << std::endl;
        os << "Kgls: " << sphere.getKgls() << std::endl;
        return os;
    }
};


#endif /* Sphere_h */
