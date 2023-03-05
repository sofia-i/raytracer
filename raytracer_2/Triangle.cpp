//
//  Triangle.cpp
//  raytracer_2
//
//  Created by Sofia Iannicelli on 3/4/23.
//

#include "Triangle.hpp"
#include "Ray.hpp"

void Triangle::calculatePlaneNormal() {
    // assuming the vertices are specified in CCW order
    vec3<double> vector1 = getUnitVector(vertices.at(0) - vertices.at(1));
    vec3<double> vector2 = getUnitVector(vertices.at(2) - vertices.at(1));
    
    vec3<double> normal = cross(vector2, vector1);
    this->planeNormal = getUnitVector(normal);
}

double Triangle::findRayObjectIntersection(Ray ray) {
    // check if the ray intersects the plane containing the triangle
    // information about the plane equation
    double a = planeNormal[0];
    double b = planeNormal[1];
    double c = planeNormal[2];
    
    vec3<double> vertex = vertices.at(0);
    double d = -a * vertex.x() - b * vertex.y() - c * vertex.z();
    
    // extract out the ray info
    vec3<double> ray_o = ray.getOrigin();
    vec3<double> ray_d = ray.getDirection();
    
    double denominator = (a * ray_d.x() + b * ray_d.y() + c * ray_d.z());
    if(denominator == 0) { return -1; }
    double t = -(a * ray_o.x() + b * ray_o.y() + c * ray_o.z() + d) / denominator;
    
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

vec3<double> Triangle::getIntersectionNormal(vec3<double> intersectionPoint) {
    return getUnitVector(planeNormal);
}
