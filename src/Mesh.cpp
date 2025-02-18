//
// Created by Sofia Iannicelli on 1/17/25.
//

#include "Mesh.h"

Mesh::Mesh(std::vector<vec3<double>> vertices, std::vector<Face> faces, const std::shared_ptr<Material>& material,
           std::string description) :
                Geometry(material, std::move(description)),
                vertices(std::move(vertices)), faces(std::move(faces)) { }

double Mesh::findRayGeoIntersectionT(Ray ray) {
    // TODO
    return 0;
}

RayHit Mesh::findRayHit(Ray ray) {
    // TODO
    double t = findRayGeoIntersectionT(ray);
}


