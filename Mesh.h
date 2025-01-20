//
// Created by Sofia Iannicelli on 1/17/25.
//

#ifndef RAYTRACER_2_MESH_H
#define RAYTRACER_2_MESH_H

#include "BaseObject.hpp"
#include <utility>
#include <vector>

struct Face {
    int vIdx[4];
};

class Mesh : public BaseObject {
public:
    Mesh(std::vector<vec3<double>> vertices, std::vector<Face> faces,
         double kd, double ks, double ka, vec3<double> objectColor, vec3<double> objectSpecular,
         double kgls, double refl, std::string description);

    ~Mesh() override = default; // I. destructor
    Mesh(const Mesh& other) = default; // II. copy constructor
    Mesh& operator=(const Mesh& other) = default; // III. copy assignment
    Mesh(Mesh&& other) noexcept = default;// IV. move constructor
    Mesh& operator=(Mesh&& other) noexcept = default; // V. move assignment

    double findRayObjectIntersection(Ray ray) override;
    vec3<double> getIntersectionNormal(vec3<double> intersectionPoint) override;

private:
    std::vector<vec3<double>> vertices;
    std::vector<Face> faces;

};


#endif //RAYTRACER_2_MESH_H
