//
//  Sphere.cpp
//  raytracer_2
//
//  Created by Sofia Iannicelli on 3/4/23.
//

#include "Sphere.h"
#include "vec3.hpp"
#include "Ray.hpp"

double Sphere::findRayObjectIntersection(Ray ray) {
    // extract information from the ray and sphere
    vec3<double> ray_o = ray.getOrigin();
    vec3<double> ray_d = ray.getDirection();
    
    vec3<double> sphere_c = this->getCenter();
    double radius = this->getRadius();
    
    // attempting to solve for intersection - compute B and C in the quadratic formula
    double b = 2 *
    (ray_d.x() * ray_o.x() - ray_d.x() * sphere_c.x()
     + ray_d.y() * ray_o.y() - ray_d.y() * sphere_c.y()
     + ray_d.z() * ray_o.z() - ray_d.z() * sphere_c.z());
    double c = pow(ray_o.x(), 2) - 2 * ray_o.x() * sphere_c.x() + pow(sphere_c.x(), 2)
    + pow(ray_o.y(), 2) - 2 * ray_o.y() * sphere_c.y() + pow(sphere_c.y(), 2)
    + pow(ray_o.z(), 2) - 2 * ray_o.z() * sphere_c.z() + pow(sphere_c.z(), 2) - pow(radius, 2);
    
    // compute the discriminant of the quadratic formula
    double discriminant = pow(b, 2) - 4 * c;
    // if the discriminant is negative, there is no intersection point
    if(discriminant < 0) {
        return -1.0;
    }
    else {
        // calculate smaller intersection parameter
        double t0 = (-b - sqrt(discriminant)) / 2;
        // if positive, update t and return
        if(t0 > 0) {
            return t0;
        }
        else {
            // calculate larger t-value
            double t1 = (-b + sqrt(discriminant)) / 2;
            return t1;
        }
    }
}

vec3<double> Sphere::getIntersectionNormal(vec3<double> intersectionPoint) {
    return getUnitVector(intersectionPoint - this->getCenter());
}
