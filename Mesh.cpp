//
// Created by Sofia Iannicelli on 1/17/25.
//

#include "Mesh.h"

Mesh::Mesh(std::vector<vec3<double>> vertices, std::vector<Face> faces, const std::shared_ptr<Material>& material,
           std::string description) :
                BaseObject(material, std::move(description)),
                vertices(std::move(vertices)), faces(std::move(faces)) { }

double Mesh::findRayObjectIntersection(Ray ray) {
    // TODO
    return 0;
}

double Mesh::findRayObjectIntersection(Ray ray, vec3<double>& intersectNormal, bool& backFace) {
    // TODO
    double t = findRayObjectIntersection(ray);
    return 0;
}


