//
//  Triangle.h
//  raytracer_2
//
//  Created by Sofia Iannicelli on 3/4/23.
//

#ifndef Triangle_hpp
#define Triangle_hpp

#include <stdio.h>
#include <vector>
#include "Object.hpp"

class Triangle : public Object {
private:
    std::vector<vec3<double>> vertices;
    vec3<double> planeNormal;
    
    void calculatePlaneNormal();
    
public:
    Triangle(std::vector<vec3<double>> vertices, double kd, double ks, double ka, vec3<double> objectColor, vec3<double> objectSpecular, double kgls, double refl): Object(kd, ks, ka, kgls, objectColor, objectSpecular, refl, "") {
        this->vertices = vertices;
        calculatePlaneNormal();
    }
    
    Triangle(std::vector<vec3<double>> vertices, double kd, double ks, double ka, vec3<double> objectColor, vec3<double> objectSpecular, double kgls, double refl, std::string description): Object(kd, ks, ka, kgls, objectColor, objectSpecular, refl, description) {
        this->vertices = vertices;
        calculatePlaneNormal();
    }
    
    double findRayObjectIntersection(Ray ray);
    vec3<double> getIntersectionNormal(vec3<double> intersectionPoint);
    
    std::string toString() const {
        std::string str = "";
        std::stringstream ss(str);
        
        ss << getDescription() << std::endl;
        // print out the vertices
        ss << Object::toString() << std::endl;
        
        return ss.str();
    }
};


#endif /* Triangle_h */
