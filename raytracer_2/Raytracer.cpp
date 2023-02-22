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

int*** Raytracer::raytrace(Scene scene, int numColumns, int numRows) {
    // initialize pixelColors multi-dimensional array
    int*** pixelColors = new int**[numRows];
    for(int i = 0; i < numRows; ++i) {
        pixelColors[i] = new int*[numColumns];
        for(int j = 0; j < numColumns; ++j) {
            pixelColors[i][j] = new int[3];
        }
    }
    calculateWorldSpaceCoords();
    
    // handle each pixel
    for(int i = 0; i < numRows; ++i) {
        for(int j = 0; j < numColumns; ++j) {
            // vec3 pixelColor = getRayResult(leftmost_x + j * x_inc, topmost_y - i * y_inc);
            vec3<int> pixelColor = getRayResult((-max_u + j * u_inc) * u_axis + (max_v - i * v_inc) * v_axis);
            pixelColors[i][j][0] = pixelColor[0];
            pixelColors[i][j][1] = pixelColor[1];
            pixelColors[i][j][2] = pixelColor[2];
        }
    }
    
    return pixelColors;
}

/* Compute illumination equation */
vec3<int> Raytracer::illuminationEq(Sphere* sphere, vec3<double> normal, vec3<double> view) {
    // all of the incoming vectors should be normalized
    if(!(normal.isNormalized() && view.isNormalized())) {
        std::cerr << "not normalized" << std::endl;
    }
    
    vec3<int> colorResult;
    vec3<double> toLight = scene.getDirectionToLight();
    
    // compute ambient contribution
    vec3<double> ambient = sphere->getKa() * scene.getAmbientLight() * sphere->getObjectColor();
    // compute diffuse contribution
    vec3<double> diffuse = sphere->getKd() * scene.getLightColor() * sphere->getObjectColor() * std::max(0.0, dot(normal, toLight));
    // compute specular contribution
    vec3<double> reflection = 2 * normal * (dot(normal, toLight)) - toLight;
    vec3<double> specular = sphere->getKs() * scene.getLightColor() * sphere->getObjectSpecular() * std::pow(std::max(0.0, dot(view, reflection)), sphere->getKgls());
    // std::max(0.0, pow(dot(view, reflection), sphere.getKgls()));
    
    vec3<double> colorSum = ambient + diffuse + specular;
    colorResult = toIntVec3(255 * colorSum);
    return colorResult;
}

/* determine if ray intersects a sphere and update the t value if it does */
bool Raytracer::rayIntersectsSphere(Ray ray, Sphere* sphere, double& t) {
    // extract information from the ray and sphere
    vec3 ray_o = ray.getOrigin();
    vec3 ray_d = ray.getDirection();
    
    vec3 sphere_c = sphere->getCenter();
    double radius = sphere->getRadius();
    
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

vec3<int> Raytracer::getRayResult(vec3<double> target) {
    // computes result of raytracing through a specified world-space coordinate
    vec3<double> rayOrigin = scene.getCamera().getCameraLookFrom();
    vec3<double> rayDirection = target - rayOrigin;
    Ray ray = Ray(rayOrigin, rayDirection);
    return getRayResult(ray, 1.0, 1);
}

vec3<int> Raytracer::getRayResult(Ray ray, double weight, int rayCount) {
    // if the maximum number of rays have been reached, this one will not contribute and stop recursion
    if(rayCount > MAX_NUM_RAYS) {
        return vec3<int>(0, 0, 0);
    }
    
    // find the closest sphere intersected by the sphere
    Sphere* closestIntersectedObject = NULL;
    double intersectionT = 0.0;
    vec3<double> intersectionPoint;
    vec3<double> intersectionNormal;
    
    // iterate over all spheres to test each
    std::vector<Sphere*> spheres = scene.getSpherePtrs();
    for(Sphere* spherePtr : spheres) {
        double t;
        // if the ray intersects the sphere, check to see if the sphere is the first one hit (so far)
        if(rayIntersectsSphere(ray, spherePtr, t)) {
            if (closestIntersectedObject == NULL || t < intersectionT) {
                // update the closest intersected object
                closestIntersectedObject = spherePtr;
                intersectionT = t;
                intersectionPoint = ray.getPointOnRay(t);
                intersectionNormal = getUnitVector(intersectionPoint - spherePtr->getCenter());
            }
        }
    }
    
    // at this point, we should know the closest sphere intersected
    // if no sphere was intersected, return the background color
    if(closestIntersectedObject == NULL) {
        vec3<int> rayResult = toIntVec3(255 * scene.getBackgroundColor());
        if(!rayResult.checkInBounds(0, 255)) {
            std::cerr << rayResult << " not in bounds" << std::endl;
        }
        return rayResult;
    }
    
    // compute the color at that point based on the intersected object
    vec3<double> toView = -ray.getDirection(); // illumination equation wants the view vector TO the eye
    vec3<int> colorResult = weight * illuminationEq(closestIntersectedObject, intersectionNormal, toView);
    
    // compute results from reflection, transmission, and shadow rays
    
    
    return colorResult; // TODO in program 6: add other results
}


// TODO: extend to allow for non-xy planar viewplanes
/**
 void Raytracer::calculateWorldSpaceCoords() {
 Camera camera = scene.getCamera();
 
 vec3<double> view_ray = (camera.getCameraLookAt() - camera.getCameraLookFrom());
 double dist_to_center = view_ray.length();
 double x = dist_to_center * tan(camera.getFOVRad() / 2);
 double y = dist_to_center * tan(camera.getFOVRad() / 2);
 
 leftmost_x = -x;
 topmost_y = y;
 x_inc = 2 * x / numColumns;
 y_inc = 2 * y / numRows;
 }
 */

void Raytracer::calculateWorldSpaceCoords() {
    Camera camera = scene.getCamera();
    
    vec3<double> view_ray = (camera.getCameraLookAt() - camera.getCameraLookFrom());
    double dist_to_center = view_ray.length();
    
    // double x = dist_to_center * tan(camera.getFOVRad()/2);
    // double y = dist_to_center * tan(camera.getFOVRad()/2);
    
    // calculate the maximum u and v values based on the FOV
    max_u = dist_to_center * tan(camera.getFOVRad()/2);
    max_v = dist_to_center * tan(camera.getFOVRad()/2);
    
    // calculate the u and v axes
    u_axis = getUnitVector(cross(view_ray, camera.getCameraLookUp()));
    // v_axis = getUnitVector(cross(view_ray, u_axis));
    v_axis = getUnitVector(cross(u_axis, view_ray));
    
    // calculate the u and v increments for a change in pixel
    u_inc = 2 * max_u / numColumns;
    v_inc = 2 * max_v / numRows;
    
}
