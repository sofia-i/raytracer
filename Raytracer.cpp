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

int*** Raytracer::raytrace(int numCols, int numRows) {
    // initialize pixelColors multi-dimensional array
    int*** pixelColors = new int**[numRows];
    for(int i = 0; i < numRows; ++i) {
        pixelColors[i] = new int*[numCols];
        for(int j = 0; j < numCols; ++j) {
            pixelColors[i][j] = new int[3];
        }
    }

    WorldSpaceCoord worldCoords  = calculateWorldSpaceCoords(numCols, numRows);
    
    // handle each pixel
    for(int i = 0; i < numRows; ++i) {
        for(int j = 0; j < numCols; ++j) {
            vec3<int> pixelColor(0, 0, 0);
            for(int m = 0; m < raysPerPixelPerSide; ++m) {
                for(int n = 0; n < raysPerPixelPerSide; ++n) {
                    double uCoord = ((j - 1.) + ((double(m)+1) / raysPerPixelPerSide)) * worldCoords.uInc;
                    double vCoord = ((i - 1.) + ((double(n)+1) / raysPerPixelPerSide)) * worldCoords.vInc;
                    pixelColor += getRayResult((uCoord - worldCoords.maxU) * worldCoords.uAxis +
                                               (worldCoords.maxV - vCoord) * worldCoords.vAxis);
                }
            }
            pixelColor = pixelColor / double(raysPerPixelPerSide * raysPerPixelPerSide);
            pixelColors[i][j][0] = pixelColor[0];
            pixelColors[i][j][1] = pixelColor[1];
            pixelColors[i][j][2] = pixelColor[2];
        }
    }
    
    return pixelColors;
}

/* Compute illumination equation */
// includes intensity from ambient, specular, diffuse
// not from reflection, transmission
vec3<int> Raytracer::illuminationEq(int objectIdx, const vec3<double> normal, const vec3<double> view,
                                    const vec3<double> intersectPt) {
    // all the incoming vectors should be normalized
    if(!(normal.isNormalized() && view.isNormalized())) {
        std::cerr << "not normalized" << std::endl;
    }
    vec3<int> colorResult;
    vec3<double> colorSum;

    // compute ambient contribution
    vec3<double> ambient = scene.objects[objectIdx]->getAmbientK() * scene.getAmbientLight() *
            scene.objects[objectIdx]->getDiffuseColor();
    colorSum = ambient;

    for(auto&& light : scene.lights) {
        vec3<double> toLight = light->getDirectionToLight(intersectPt);

        // compute shadow information
        vec3<double> shadowRayDirection = toLight;
        vec3<double> shadowRayOrigin = intersectPt + EPSILON * shadowRayDirection;
        Ray shadow_ray = Ray(shadowRayOrigin, shadowRayDirection);
        bool inShadow = false;
        // go over all the objects to see if it hits any
        for(auto&& otherObj: scene.objects) {
            double t = otherObj->findRayObjectIntersection(shadow_ray);
            if(t > 0) {
                inShadow = true;
                break;
            }
        }

        // calculate intensity if not in shadow
        if(!inShadow) {
            // compute diffuse contribution
            vec3<double> diffuse = scene.objects[objectIdx]->getDiffuseK() * light->getLightColor() *
                    scene.objects[objectIdx]->getDiffuseColor() * std::max(0.0, dot(normal, toLight));

            // compute specular contribution
            vec3<double> reflection = ((2 * (dot(normal, toLight))) * normal) - toLight;
            double specularK = scene.objects[objectIdx]->getSpecularK();
            double glsK = scene.objects[objectIdx]->getGlsK();
            vec3<double> specular = specularK * light->getLightColor() * scene.objects[objectIdx]->getSpecularColor() *
                    std::pow(std::max(0.0, dot(view, reflection)), glsK);

            colorSum += diffuse + specular;
        }
    }

    colorResult = toIntVec3(255 * colorSum);
    return colorResult;
}

vec3<int> Raytracer::getRayResult(vec3<double> target) {
    // computes result of raytracing through a specified world-space coordinate
    vec3<double> rayOrigin = scene.getCamera().getLookFrom();
    vec3<double> rayDirection = target - rayOrigin;
    Ray ray = Ray(rayOrigin, rayDirection);
    return getRayResult(ray, 1);
}

vec3<int> Raytracer::getRayResult(Ray ray, int rayCount) {
    // if the maximum number of rays have been reached, this one will not contribute and stop recursion
    if(rayCount > MAX_NUM_RAYS) {
        return {0, 0, 0};
    }
    
    // Find the closest object intersected by the ray
    int closestObjIdx = -1;
    // BaseObject* closestIntersectObj = nullptr;
    double intersectT = 0.0;  // ray-space intersection point
    vec3<double> intersectPt;
    vec3<double> intersectNormal;

    // iterate over all objects to test each
    for(int i = 0; i < scene.objects.size(); ++i) {
        double t = scene.objects[i]->findRayObjectIntersection(ray);
        // if the ray intersects the object, check to see if the object is the first one hit (so far)
        if(t > 0) {
            if(closestObjIdx == -1 || t < intersectT) {
                // update the closest intersected object
                closestObjIdx = i;
                intersectT = t;
                intersectPt = ray.getPointOnRay(t);
                intersectNormal = getUnitVector(scene.objects[i]->getIntersectionNormal(intersectPt));
            }
        }
    }
    
    // If no object was intersected, return the background color
    if(closestObjIdx == -1) {
        return toIntVec3(255 * scene.getBackgroundColor());
    }

    vec3<double> toView = getUnitVector(ray.getOrigin() - intersectPt);
    vec3<int> primaryResult = illuminationEq(closestObjIdx, intersectNormal,
                                             toView, intersectPt);

    vec3<int> refractionResult(0, 0, 0);
    // TODO: transmission
    if(scene.objects[closestObjIdx]->getIsRefractive()) {
        vec3<double> refractDirection;
        vec3<double> refractOrigin = intersectPt + (EPSILON * refractDirection);
        Ray refractionRay = Ray(refractOrigin, refractDirection);
        refractionResult += scene.objects[closestObjIdx]->getRefractionK() *
                getRayResult(refractionRay, ++rayCount);
    }

    // compute results from reflection
    vec3<double> reflectRayDirection = getUnitVector(((2 * (dot(intersectNormal, toView))) * intersectNormal) -
                                                     toView);
    vec3<double> reflectRayOrigin = intersectPt + (EPSILON * reflectRayDirection);
    Ray reflectionRay = Ray(reflectRayOrigin, reflectRayDirection);

    vec3<int> reflectionResult = scene.objects[closestObjIdx]->getRefl() * getRayResult(reflectionRay, ++rayCount);

    // combine

    vec3<int> colorResult = primaryResult + reflectionResult + refractionResult;

    // make sure not to have overflow
    colorResult = clip(colorResult, 0, 255);
    
    return colorResult;
}

WorldSpaceCoord Raytracer::calculateWorldSpaceCoords(int numCols, int numRows) {
    Camera camera = scene.getCamera();
    
    vec3<double> viewRay = (camera.getLookAt() - camera.getLookFrom());
    double distToCenter = viewRay.length();

    // calculate the maximum u and v values based on the FOV
    double maxU = distToCenter * tan(camera.getFovXRad()/2);
    double maxV = distToCenter * tan(camera.getFovYRad()/2);
    
    // calculate the u and v axes
    vec3<double> uAxis = getUnitVector(cross(viewRay, camera.getCameraLookUp()));
    // v_axis = getUnitVector(cross(view_ray, u_axis));
    vec3<double> vAxis = getUnitVector(cross(uAxis, viewRay));
    
    // calculate the u and v increments for a change in pixel
    double uInc = 2 * maxU / numCols;
    double vInc = 2 * maxV / numRows;

    WorldSpaceCoord coord(maxU, maxV, uInc, vInc, uAxis, vAxis);
    return coord;
    
}
