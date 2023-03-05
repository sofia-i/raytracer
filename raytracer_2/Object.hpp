//
//  Object.hpp
//  raytracer_2
//
//  Created by Sofia Iannicelli on 3/4/23.
//

#ifndef Object_hpp
#define Object_hpp

#include <stdio.h>
#include <sstream>
#include <string>
#include "vec3.hpp"
#include "Ray.hpp"

class Object {
protected:
    double kd;
    double ks;
    double ka;
    double kgls;
    vec3<double> objectColor;
    vec3<double> objectSpecular;
    std::string description;
    double refl;
    
public:
    virtual double findRayObjectIntersection(Ray ray) = 0;
    virtual vec3<double> getIntersectionNormal(vec3<double> intersectionPoint) = 0;

public:
    Object(double kd, double ks, double ka, double kgls, vec3<double> objectColor, vec3<double> objectSpecular, double refl, std::string description) {
        this->kd = kd;
        this->ks = ks;
        this->ka = ka;
        this->kgls = kgls;
        this->objectColor = objectColor;
        this->objectSpecular = objectSpecular;
        this->description = description;
        this->refl = refl;
    }
    
    double getKd() const { return kd; }
    double getKs() const { return ks; }
    double getKa() const { return ka; }
    vec3<double> getObjectColor() const { return objectColor; }
    vec3<double> getObjectSpecular() const { return objectSpecular; }
    double getKgls() const { return kgls; }
    double getRefl() const { return refl; }
    std::string getDescription() const { return description; }
    
    virtual std::string toString() const {
        std::string s = "";
        std::stringstream ss(s);
        
        ss << getDescription() << std::endl;
        ss << "Kd: " << getKd() << std::endl;
        ss << "Ks: " << getKs() << std::endl;
        ss << "Ka: " << getKa() << std::endl;
        ss << "Object Color: " << getObjectColor() << std::endl;
        ss << "Object specular: " << getObjectSpecular() << std::endl;
        ss << "Kgls: " << getKgls() << std::endl;
        ss << "Reflectivity: " << getRefl() << std::endl;
        
        return ss.str();
        
    }
    
};

#endif /* Object_hpp */
