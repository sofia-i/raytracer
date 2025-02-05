//
// Created by Sofia Iannicelli on 1/17/25.
//

#ifndef RAYTRACER_2_MESH_H
#define RAYTRACER_2_MESH_H

#include "Geometry.h"
#include <utility>
#include <vector>

struct Face {
    int vIdx[4];
};

class Mesh : public Geometry {
public:
    Mesh(std::vector<vec3<double>> vertices, std::vector<Face> faces, const std::shared_ptr<Material>& material,
         std::string description);

    ~Mesh() override = default; // I. destructor
    Mesh(const Mesh& other) = default; // II. copy constructor
    Mesh& operator=(const Mesh& other) = default; // III. copy assignment
    Mesh(Mesh&& other) noexcept = default;// IV. move constructor
    Mesh& operator=(Mesh&& other) noexcept = default; // V. move assignment

    GeoHit findRayGeoIntersection(Ray ray) override;

protected:
    double findRayGeoIntersectionT(Ray ray) override;

private:
    std::vector<vec3<double>> vertices;
    std::vector<Face> faces;

};


#endif //RAYTRACER_2_MESH_H
