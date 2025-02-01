//
// Created by Sofia Iannicelli on 1/22/25.
//

#ifndef RAYTRACER_2_CYLINDER_H
#define RAYTRACER_2_CYLINDER_H

#include "Geometry.h"

class Cylinder : public Geometry {
public:
    Cylinder(vec3<double> capCenter1, vec3<double> capCenter2, double radius,
             const std::shared_ptr<Material>& mat, const std::string& description);

    ~Cylinder() override = default; // I. destructor
    Cylinder(const Cylinder& other) = default; // II. copy constructor
    Cylinder& operator=(const Cylinder& other) = default; // III. copy assignment
    Cylinder(Cylinder&& other) noexcept = default;// IV. move constructor
    Cylinder& operator=(Cylinder&& other) noexcept = default; // V. move assignment

    vec3<double> getCapCenter1() const { return capCenter0; }
    vec3<double> getCapCenter2() const { return capCenter1; }
    vec3<double> getDir() const { return cylinderD; }
    double getRadius() const { return radius; }

    double findRayObjectIntersection(Ray ray) override;
    double findRayObjectIntersection(Ray ray, vec3<double>& intersectNormal, bool& backFace) override;

    std::string toString() const override {
        std::string str;
        std::stringstream ss(str);

        ss << getDescription() << std::endl;
        ss << "\tCap Centers: " << capCenter0 << "; " << capCenter1 << std::endl;
        ss << "\tRadius: " << getRadius() << std::endl;
        ss << Geometry::toString() << std::endl;

        return ss.str();
    }

private:
    vec3<double> capCenter0;
    vec3<double> capCenter1;
    double radius;

    vec3<double> cylinderD;

    bool cylinderPtInBounds(const vec3<double>& pt);
    bool capPtInBounds(const vec3<double>& capCenter, const vec3<double>& pt);
    double calculateDistToOrigin(const vec3<double>& pt);
};


#endif //RAYTRACER_2_CYLINDER_H
