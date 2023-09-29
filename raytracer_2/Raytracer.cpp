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
#include <algorithm>

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
            // TODO: remove
            if(i == 570 && j == 500) {
                std::string stop = "stop here";
            }
            vec3<int> pixelColor = getRayResult((j * u_inc - max_u) * u_axis + (max_v - i * v_inc) * v_axis);
            pixelColors[i][j][0] = pixelColor[0];
            pixelColors[i][j][1] = pixelColor[1];
            pixelColors[i][j][2] = pixelColor[2];
        }
    }
    
    return pixelColors;
}

/* Compute illumination equation */
vec3<int> Raytracer::illuminationEq(Object* object, const vec3<double> normal, const vec3<double> view) {
    // all of the incoming vectors should be normalized
    if(!(normal.isNormalized() && view.isNormalized())) {
        std::cerr << "not normalized" << std::endl;
    }
    
    vec3<int> colorResult;
    vec3<double> toLight = scene.getDirectionToLight();
    
    // compute ambient contribution
    vec3<double> ambient = object->getKa() * scene.getAmbientLight() * object->getObjectColor();
    // compute diffuse contribution
    vec3<double> diffuse = object->getKd() * scene.getLightColor() * object->getObjectColor() * std::max(0.0, dot(normal, toLight));
    
    vec3<double> reflection = ((2 * (dot(normal, toLight))) * normal) - toLight;
    // compute specular contribution
    vec3<double> specular = object->getKs() * scene.getLightColor() * object->getObjectSpecular() * std::pow(std::max(0.0, dot(view, reflection)), object->getKgls());
    
    vec3<double> colorSum = ambient + diffuse + specular;
    colorResult = toIntVec3(255 * colorSum);
    return colorResult;
}

vec3<int> Raytracer::getRayResult(vec3<double> target) {
    // computes result of raytracing through a specified world-space coordinate
    vec3<double> rayOrigin = scene.getCamera()->getCameraLookFrom();
    vec3<double> rayDirection = target - rayOrigin;
    Ray ray = Ray(rayOrigin, rayDirection);
    return getRayResult(ray, 1);
}

vec3<int> Raytracer::getRayResult(Ray ray, int rayCount) {
    // if the maximum number of rays have been reached, this one will not contribute and stop recursion
    if(rayCount > MAX_NUM_RAYS) {
        return vec3<int>(0, 0, 0);
    }
    
    // find the closest object intersected by the ray
    Object* closestIntersectedObject = NULL;
    double intersectionT = 0.0;
    vec3<double> intersectionPoint;
    vec3<double> intersectionNormal;
    
    // iterate over all objects to test each
    std::vector<Object*> objects = scene.getObjectPtrs();
    for(Object* object : objects) {
        // if the ray intersects the object, check to see if the object is the first one hit (so far)
        double t = object->findRayObjectIntersection(ray);
        if(t > 0) {
            if (closestIntersectedObject == NULL || t < intersectionT) {
                // update the closest intersected object
                closestIntersectedObject = object;
                intersectionT = t;
                intersectionPoint = ray.getPointOnRay(t);
                intersectionNormal = getUnitVector(object->getIntersectionNormal(intersectionPoint));
            }
        }
    }
    
    // at this point, we should know the closest object intersected
    // if no sphere was intersected, return the background color
    if(closestIntersectedObject == NULL) {
        return toIntVec3(255 * scene.getBackgroundColor());
    }
    
    vec3<double> toLight = getUnitVector(scene.getDirectionToLight());
    vec3<double> toView = getUnitVector(ray.getOrigin() - intersectionPoint);
    
    // compute the color at that point based on the intersected object
    vec3<int> primaryResult = illuminationEq(closestIntersectedObject, intersectionNormal, toView);
    
    // compute results from reflection
    vec3<double> reflectionRayDirection = getUnitVector(((2 * (dot(intersectionNormal, toView))) * intersectionNormal) - toView);
    vec3<double> reflectionRayOrigin = intersectionPoint + (EPSILON * reflectionRayDirection);
    Ray reflectionRay = Ray(reflectionRayOrigin, reflectionRayDirection);
    
    vec3<int> reflectionResult = getRayResult(reflectionRay, rayCount + 1);
    
    // compute shadow information
    vec3<double> shadowRayDirection = toLight;
    vec3<double> shadowRayOrigin = intersectionPoint + EPSILON * shadowRayDirection;
    Ray shadowRay = Ray(shadowRayOrigin, shadowRayDirection);
    bool inShadow = false;
    // go over all the objects to see if it hits any
    for(Object* object : objects) {
        double t = object->findRayObjectIntersection(shadowRay);
        if(t > 0) {
            inShadow = true;
            break;
        }
    }
    
    vec3<int> colorResult = closestIntersectedObject->getRefl() * reflectionResult;
    
    if(!inShadow) {
        colorResult += primaryResult;
    }
    
    // make sure not to have overflow
    colorResult = clip(colorResult, 0, 255);
    
    return colorResult; // TODO in program 6: add other results
}

void Raytracer::calculateWorldSpaceCoords() {
    Camera* camera = scene.getCamera();
    
    vec3<double> view_ray = (camera->getCameraLookAt() - camera->getCameraLookFrom());
    double dist_to_center = view_ray.length();
    
    // calculate the maximum u and v values based on the FOV
    max_u = dist_to_center * tan(camera->getFOVxRad()/2);
    max_v = dist_to_center * tan(camera->getFOVyRad()/2);
    
    // calculate the u and v axes
    u_axis = getUnitVector(cross(view_ray, camera->getCameraLookUp()));
    // v_axis = getUnitVector(cross(view_ray, u_axis));
    v_axis = getUnitVector(cross(u_axis, view_ray));
    
    // calculate the u and v increments for a change in pixel
    u_inc = 2 * max_u / numColumns;
    v_inc = 2 * max_v / numRows;
    
}
