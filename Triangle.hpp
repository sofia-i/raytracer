//
//  Triangle.h
//  raytracer_2
//
//  Created by Sofia Iannicelli on 3/4/23.
//

#ifndef Triangle_hpp
#define Triangle_hpp

#include <cstdio>
#include <vector>
#include "BaseObject.hpp"

class Triangle : public BaseObject {
public:
    Triangle(std::vector<vec3<double>> vertices, double kd, double ks, double ka,
             vec3<double> objectColor, vec3<double> objectSpecular, double kgls, double refl):
            BaseObject(kd, ks, ka, kgls, objectColor, objectSpecular, refl, "") {
        this->vertices = vertices;
        calculatePlaneNormal();
        distToOrigin = calculateDistToOrigin();
    }
    
    Triangle(std::vector<vec3<double>> vertices, double kd, double ks, double ka,
             vec3<double> objectColor, vec3<double> objectSpecular, double kgls, double refl, std::string description):
            BaseObject(kd, ks, ka, kgls, objectColor, objectSpecular, refl, description) {
        this->vertices = vertices;
        calculatePlaneNormal();
        distToOrigin = calculateDistToOrigin();
    }

    ~Triangle() override = default; // I. destructor
    Triangle(const Triangle& other) = default; // II. copy constructor
    Triangle& operator=(const Triangle& other) = default; // III. copy assignment
    Triangle(Triangle&& other) noexcept = default;// IV. move constructor
    Triangle& operator=(Triangle&& other) noexcept = default; // V. move assignment

    double findRayObjectIntersection(Ray ray) override;
    double findRayObjectIntersection(Ray ray, vec3<double>& intersectNormal) override;
    
    std::string toString() const override {
        std::string str = "";
        std::stringstream ss(str);
        
        ss << getDescription() << std::endl;
        // print out the vertices
        ss << BaseObject::toString() << std::endl;
        
        return ss.str();
    }

private:
    std::vector<vec3<double>> vertices;
    vec3<double> planeNormal;
    double distToOrigin;

    void calculatePlaneNormal();
    double calculateDistToOrigin();
};


#endif /* Triangle_h */
