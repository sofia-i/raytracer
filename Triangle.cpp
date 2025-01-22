//
//  Triangle.cpp
//  raytracer_2
//
//  Created by Sofia Iannicelli on 3/4/23.
//

#include "Triangle.hpp"
#include "Ray.hpp"

double Triangle::calculateDistToOrigin() {
    return dot(planeNormal, -vertices[0]);
}

void Triangle::calculatePlaneNormal() {
    // assuming the vertices are specified in CCW order
    vec3<double> vector1 = getUnitVector(vertices.at(0) - vertices.at(1));
    vec3<double> vector2 = getUnitVector(vertices.at(2) - vertices.at(1));
    
    vec3<double> normal = cross(vector2, vector1);
    this->planeNormal = getUnitVector(normal);
}

double Triangle::findRayObjectIntersection(Ray ray) {
    // check if the ray intersects the plane containing the triangle

    // vec3<double> vertex = vertices.at(0);
    // double d = -a * vertex.x() - b * vertex.y() - c * vertex.z();
    double d = calculateDistToOrigin();

    // extract out the ray info
    vec3<double> ray_o = ray.getOrigin();
    vec3<double> ray_d = ray.getDirection();

    double denominator = dot(planeNormal, ray_d);
    if(denominator == 0) { return -1; }
    double t = -(dot(planeNormal, ray_o) + d) / denominator;

    if(t < 0) {
        return t;
    }

    vec3<double> intersectionPt = ray.getPointOnRay(t);
    // check if the intersection point is inside the triangle
    for(int i = 0; i < 3; ++i) {
        vec3<double> testVertex = vertices.at(i);
        vec3<double> nextVertex = vertices.at((i + 1) % 3);

        vec3<double> testNormal = cross((nextVertex - testVertex), (intersectionPt - testVertex));

        if(dot(planeNormal, testNormal) < 0) {
            return -1;
        }
    }

    return t;
}

double Triangle::findRayObjectIntersection(Ray ray, vec3<double>& intersectNormal, bool& backFace) {
    double t = findRayObjectIntersection(ray);
    // if intersected, calculate intersect normal and back face
    if(t > 0) {
        if(dot(ray.getDirection(), planeNormal) < 0.0) {
            // The ray and the plane normal are facing in opposite directions (front face)
            intersectNormal = planeNormal;
            backFace = false;
        }
        else {
            // back face
            intersectNormal = -planeNormal;
            backFace = false;  // don't label as back face (FIXME?)
        }
    }
    return t;
}

