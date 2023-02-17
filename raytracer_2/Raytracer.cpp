//
//  Raytracer.cpp
//  raytracer_2
//
//  Created by Sofia Iannicelli on 2/15/23.
//

#include "Raytracer.hpp"
#include "Ray.hpp"
#include <cmath>
#include <iostream>

/* Compute illumination equation */
vec3 Raytracer::illuminationEq(Sphere sphere, vec3 normal, vec3 view) {
    vec3 colorResult;
    vec3 toLight = scene.getDirectionToLight();
    
    // compute ambient contribution
    vec3 ambient = sphere.getKa() * scene.getAmbientLight() * sphere.getObjectColor();
    // compute diffuse contribution
    vec3 diffuse = sphere.getKd() * scene.getLightColor() * sphere.getObjectColor() * std::max(0.0, dot(normal, toLight));
    // compute specular contribution
    vec3 reflection = 2 * normal * (dot(normal, toLight)) - toLight;
    vec3 specular = sphere.getKs() * scene.getLightColor() * sphere.getObjectSpecular() * std::max(0.0, pow(dot(view, reflection), sphere.getKgls()));
    
    colorResult = ambient + diffuse + specular;
    return colorResult;
}

/* determine if ray intersects a sphere and update the t value if it does */
bool Raytracer::rayIntersectsSphere(Ray ray, Sphere sphere, double& t) {
    // extract information from the ray and sphere
    vec3 ray_o = ray.getOrigin();
    vec3 ray_d = ray.getDirection();
    
    vec3 sphere_c = sphere.getCenter();
    double radius = sphere.getRadius();
    
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
        return false;
    }
    else {
        // calculate smaller intersection parameter
        double t0 = (-b - sqrt(discriminant)) / 2;
        // if positive, update t and return
        if(t0 > 0) {
            t = t0;
            return true;
        }
        else {
            // calculate larger t-value
            double t1 = (-b + sqrt(discriminant)) / 2;
            if(t1 <= 0) {
                // no intersection (intersection point behind ray)
                return false;
            }
            t = t1;
            return true;
        }
    }
}

vec3 Raytracer::getRayResult(double x, double y) {
    // computes result of raytracing through a specified world-space coordinate
    vec3 rayOrigin = scene.getCamera().getCameraLookFrom();
    vec3 rayDirection = vec3(x, y, 0) - rayOrigin;
    Ray ray = Ray(rayOrigin, rayDirection);
    return getRayResult(ray, 1.0, 1);
}

vec3 Raytracer::getRayResult(Ray ray, double weight, int rayCount) {
    // if the maximum number of rays have been reached, this one will not contribute and stop recursion
    if(rayCount > MAX_NUM_RAYS) {
        return vec3(0, 0, 0);
    }
    
    // find the closest sphere intersected by the sphere
    Sphere* closestIntersectedObject = NULL;
    double intersectionT = 0.0;
    vec3 intersectionPoint;
    vec3 intersectionNormal;
    
    // iterate over all spheres to test each
    for(Sphere sphere : scene.getSpheres()) {
        double t;
        // if the ray intersects the sphere, check to see if the sphere is the first one hit (so far)
        if(rayIntersectsSphere(ray, sphere, t)) {
            if (closestIntersectedObject == NULL || t < intersectionT) {
                closestIntersectedObject = &sphere;
                intersectionT = t;
                intersectionPoint = ray.getPointOnRay(t);
                intersectionNormal = (intersectionPoint - sphere.getCenter()) / sphere.getRadius();
            }
        }
    }
    
    // at this point, we should know the closest sphere intersected
    // if no sphere was intersected, return the background color
    if(closestIntersectedObject == NULL) {
        return scene.getBackgroundColor();
    }
    
    // compute the color at that point based on the intersected object
    vec3 viewDir = -ray.getDirection();
    vec3 colorResult = weight * illuminationEq(*closestIntersectedObject, intersectionNormal, viewDir);
    
    // compute results from reflection, transmission, and shadow rays
    
    
    return colorResult; // TODO in program 6: add other results
}


void Raytracer::calculateWorldSpaceCoords() {
    Camera camera = scene.getCamera();
    
    vec3 view_ray = (camera.getCameraLookAt() - camera.getCameraLookFrom());
    double dist_to_center = view_ray.length();
    double x = dist_to_center * tan(camera.getFOVRad() / 2);
    double y = dist_to_center * tan(camera.getFOVRad() / 2);
    
    leftmost_x = -x;
    topmost_y = y;
    x_inc = 2 * x / numColumns;
    y_inc = 2 * y / numRows;
}
