//
//  Sphere.h
//  raytracer_2
//
//  Created by Sofia Iannicelli on 2/15/23.
//

#ifndef Sphere_h
#define Sphere_h

#include "vec3.hpp"
#include "Geometry.h"

class Sphere : public Geometry {
public:
    Sphere(vec3<double> center, double radius, const std::shared_ptr<Material>& mat, const std::string& description) :
        Geometry(mat, std::move(description)), center(center), radius(radius) {}

    ~Sphere() override = default; // I. destructor
    Sphere(const Sphere& other) = default; // II. copy constructor
    Sphere& operator=(const Sphere& other) = default; // III. copy assignment
    Sphere(Sphere&& other) noexcept = default;// IV. move constructor
    Sphere& operator=(Sphere&& other) noexcept = default; // V. move assignment

    vec3<double> getCenter() const { return center; }
    double getRadius() const { return radius; }

    RayHit findRayHit(Ray ray) override;

    Extent findExtent() override;

    void getUV(const vec3<double>& point, double& u, double& v);
    
    std::string toString() const override {
        std::string str;
        std::stringstream ss(str);
        
        ss << getDescription() << std::endl;
        ss << "\tCenter: " << getCenter() << std::endl;
        ss << "\tRadius: " << getRadius() << std::endl;
        ss << Geometry::toString() << std::endl;
         
        return ss.str();
    }

private:
    double findRayGeoIntersectionT(Ray ray);

private:
    vec3<double> center;
    double radius;
};


#endif /* Sphere_h */
