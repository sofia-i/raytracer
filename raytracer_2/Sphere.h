//
//  Sphere.h
//  raytracer_2
//
//  Created by Sofia Iannicelli on 2/15/23.
//

#ifndef Sphere_h
#define Sphere_h

#include "vec3.hpp"
#include "Object.hpp"

class Sphere : public Object {
private:
    vec3<double> center;
    double radius;

public:
    Sphere(vec3<double> center, double radius, double kd, double ks, double ka,
           vec3<double> objectColor, vec3<double> objectSpecular, double kgls, double refl) : Object(kd, ks, ka, kgls, objectColor, objectSpecular, refl, "") {
        this->center = center;
        this->radius = radius;
    }
    Sphere(vec3<double> center, double radius, double kd, double ks, double ka,
           vec3<double> objectColor, vec3<double> objectSpecular, double kgls, double refl, std::string description) : Object(kd, ks, ka, kgls, objectColor, objectSpecular, refl, description) {
        this->center = center;
        this->radius = radius;
    }

    vec3<double> getCenter() const { return center; }
    double getRadius() const { return radius; }
    
    double findRayObjectIntersection(Ray ray);
    vec3<double> getIntersectionNormal(vec3<double> intersectionPoint);
    
    std::string toString() const {
        std::string str = "";
        std::stringstream ss(str);
        
        ss << getDescription() << std::endl;
        ss << "Center: " << getCenter() << std::endl;
        ss << "Radius: " << getRadius() << std::endl;
        ss << Object::toString() << std::endl;
         
        return ss.str();
    }
};


#endif /* Sphere_h */
