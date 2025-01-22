//
//  Sphere.h
//  raytracer_2
//
//  Created by Sofia Iannicelli on 2/15/23.
//

#ifndef Sphere_h
#define Sphere_h

#include "vec3.hpp"
#include "BaseObject.hpp"

class Sphere : public BaseObject {
private:
    vec3<double> center;
    double radius;

public:
    Sphere(vec3<double> center, double radius, double kd, double ks, double ka,
           vec3<double> objectColor, vec3<double> objectSpecular,
           double kgls, double refl) :
            BaseObject(kd, ks, ka, kgls, objectColor, objectSpecular, refl, "") {
        this->center = center;
        this->radius = radius;
    }
    Sphere(vec3<double> center, double radius, double kd, double ks, double ka,
           vec3<double> objectColor, vec3<double> objectSpecular, double kgls, double refl, std::string description) :
            BaseObject(kd, ks, ka, kgls, objectColor, objectSpecular, refl, description) {
        this->center = center;
        this->radius = radius;
    }

    ~Sphere() override = default; // I. destructor
    Sphere(const Sphere& other) = default; // II. copy constructor
    Sphere& operator=(const Sphere& other) = default; // III. copy assignment
    Sphere(Sphere&& other) noexcept = default;// IV. move constructor
    Sphere& operator=(Sphere&& other) noexcept = default; // V. move assignment

    vec3<double> getCenter() const { return center; }
    double getRadius() const { return radius; }

    double findRayObjectIntersection(Ray ray) override;
    double findRayObjectIntersection(Ray ray, vec3<double>& intersectNormal) override;
    
    std::string toString() const override {
        std::string str;
        std::stringstream ss(str);
        
        ss << getDescription() << std::endl;
        ss << "\tCenter: " << getCenter() << std::endl;
        ss << "\tRadius: " << getRadius() << std::endl;
        ss << BaseObject::toString() << std::endl;
         
        return ss.str();
    }
};


#endif /* Sphere_h */
