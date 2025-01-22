//
// Created by Sofia Iannicelli on 1/17/25.
//

#include "Mesh.h"

Mesh::Mesh(std::vector<vec3<double>> vertices, std::vector<Face> faces,
           double kd, double ks, double ka, vec3<double> objectColor, vec3<double> objectSpecular,
           double kgls, double refl, std::string description) :
                BaseObject(kd, ks, ka, kgls, objectColor, objectSpecular, refl, std::move(description)),
                vertices(std::move(vertices)), faces(std::move(faces)) {

}

double Mesh::findRayObjectIntersection(Ray ray) {
    // TODO
    return 0;
}

double Mesh::findRayObjectIntersection(Ray ray, vec3<double>& intersectNormal) {
    // TODO
    double t = findRayObjectIntersection(ray);
    return 0;
}


