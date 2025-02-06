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
#include <cassert>
#include "Utils.h"

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

    // random helper
    double invRPPPS = 1. / raysPerPixelPerSide;
    RandomNumber randNum;
    std::uniform_real_distribution<double> dist = randNum.get_dist(-0.5, 0.5);

    // handle each pixel
    for(int i = 0; i < numRows; ++i) {
        for(int j = 0; j < numCols; ++j) {
            vec3<int> pixelColor(0, 0, 0);
            for(int m = 0; m < raysPerPixelPerSide; ++m) {
                for(int n = 0; n < raysPerPixelPerSide; ++n) {
                    double jitterU = randNum.get_random_double_from_dist(dist);
                    double jitterV = randNum.get_random_double_from_dist(dist);
                    double uCoord = ((j + 0.5 * invRPPPS) + ((double(m) + jitterU) * invRPPPS)) * worldCoords.uInc;
                    double vCoord = ((i + 0.5 * invRPPPS) + ((double(n) + jitterV) * invRPPPS)) * worldCoords.vInc;
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

double Raytracer::getInShadow(const vec3<double>& intersectPt, const std::shared_ptr<Light>& light) {
    vec3<double> shadowRayDirection;
    vec3<double> shadowRayOrigin;
    double inShadow = 0;

    // Iterate over each ray to light (1 for point & direction, multiple for area)
    for(int i = 0; i < light->shadowRayCount; ++i) {
        // calculate shadow ray
        double distToLight;
        bool hitLight;
        // find direction to and distance to light
        light->getPathToLight(intersectPt, i, hitLight, shadowRayDirection, distToLight);

        if(!hitLight) {
            continue;
        }

        // check for geo in the way of the path to the light
        shadowRayOrigin = intersectPt + EPSILON * shadowRayDirection;
        Ray shadowRay = Ray(shadowRayOrigin, shadowRayDirection);
        double inShadowPart = 0;
        // FIXME: don't have to get closest first...
        // FIXME: optimization with excluding already checked?
        double tCovered = 0;
        while(inShadowPart < 1.) {
            RayHit geoHitInfo = getClosestIntersection(shadowRay);

            // if nothing is hit or if you're past the light, stop looking
            if(!geoHitInfo.isHit || (tCovered + geoHitInfo.t) >= distToLight) break;

            if(geoHitInfo.material->getIsRefractive()) {
                inShadowPart += (1 - geoHitInfo.material->getRefractionK());
                // Start new ray after the point already hit
                tCovered += geoHitInfo.t;

                shadowRayOrigin = geoHitInfo.point + EPSILON * shadowRayDirection;
                shadowRay = Ray(shadowRayOrigin, shadowRayDirection);
            }
            else {
                inShadowPart += 1;
                break;
            }
        }

        inShadow += std::min(inShadowPart, 1.);
    }
    inShadow /= double(light->shadowRayCount);

    return inShadow;
}

inline vec3<double> Raytracer::getAmbient(const std::shared_ptr<Material>& mat) {
    return mat->getAmbientK() * scene.getAmbientLight() * mat->getDiffuseColor();
}

inline vec3<double> Raytracer::getDiffuse(const std::shared_ptr<Material>& mat,
                                          const std::shared_ptr<Light>& light,
                                          const vec3<double>& normal, const vec3<double>& toLight) {
    return mat->getDiffuseK() * light->getLightColor() * mat->getDiffuseColor() * std::max(0.0, dot(normal, toLight));
}

inline vec3<double> Raytracer::getSpecular(const std::shared_ptr<Material>& mat,
                                           const std::shared_ptr<Light>& light,
                                           const vec3<double>& normal, const vec3<double>& toLight,
                                           const vec3<double>& view) {
    vec3<double> reflection = ((2 * (dot(normal, toLight))) * normal) - toLight;
    double specularK = mat->getSpecularK();
    double glsK = mat->getGlsK();
    const vec3<double>& specularColor = mat->getSpecularColor();
    return specularK * light->getLightColor() * specularColor *
        std::pow(std::max(0.0, dot(view, reflection)), glsK);
}

/**
 * Compute illumination equation
 * includes intensity from ambient, specular, diffuse
 * not from reflection, transmission
 * @param mat
 * @param normal
 * @param view
 * @param intersectPt
 * @return
 */
vec3<int> Raytracer::illuminationEq(const std::shared_ptr<Material>& mat, const vec3<double>& normal,
                                    const vec3<double>& view, const vec3<double>& intersectPt) {
    // all the incoming vectors should be normalized
    assert(normal.isNormalized() && view.isNormalized() && "normal and view rays should be normalized");
    vec3<int> colorResult;
    vec3<double> colorSum(0.0, 0.0, 0.0);

    // compute ambient contribution
    colorSum += getAmbient(mat);

    for(auto&& light : scene.lights) {
        // compute shadow information
        double shadowAmt = getInShadow(intersectPt, light);

        // calculate intensity if not in complete shadow
        if(shadowAmt < 1) {
            vec3<double> toLight = light->getDirectionToLight(intersectPt);
            // compute diffuse contribution
            colorSum += (1 - shadowAmt) * getDiffuse(mat, light, normal, toLight);
            // compute specular contribution
            colorSum += (1 - shadowAmt) * getSpecular(mat, light, normal, toLight, view);
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
    std::stack<double> iors;
    iors.push(scene.getAmbientIor());
    return getRayResult(ray, 1, iors);
}

RayHit Raytracer::getClosestIntersection(const Ray& ray) {
    if(USE_BOUNDING_VOLUME) {
        return scene.bvh->findRayHit(ray);
    }
    else {
        vec3<double> normal;

        int closestGeoIdx = -1;
        RayHit closestHitInfo = RayHit::Miss();

        // iterate over all geo to test each
        for(int i = 0; i < scene.geo.size(); ++i) {
            RayHit hitInfo = scene.geo[i]->findRayHit(ray);
            // if the ray intersects the geo, check to see if the geo is the closest one hit (so far)
            if(hitInfo.isHit && (closestGeoIdx == -1 || hitInfo.t < closestHitInfo.t)) {
                // update the closest intersected geo
                closestGeoIdx = i;
                closestHitInfo = hitInfo;
            }
        }

        return closestHitInfo;
    }
}

void Raytracer::getIorAcrossIntersection(const std::shared_ptr<Material>& mat, bool isBackFace,
                                         double& iorIn, double& iorOut, double& iorRatio,
                                         std::stack<double>& iors) {
    // FIXME: if reflexive overlapping it won't work
    if(isBackFace && iors.size() > 1) {
        // coming out of material
        iorIn = iors.top();
        iors.pop();
        iorOut = iors.top();
        iorRatio = iorIn / iorOut;
    }
    else {
        iorIn = iors.top();
        iorOut = mat->getIOR();
        iorRatio = iorIn / iorOut;
        iors.push(iorOut);
    }
}

double Raytracer::getPortionReflected(const vec3<double>& normal, const vec3<double>& rayD, const double matRefl,
                                      const double& iorIn, const double& iorOut, const double& iorRatio) {
    // Schlick’s approximation https://blog.demofox.org/2017/01/09/raytracing-reflection-refraction-fresnel-total-internal-reflection-and-beers-law/
    double r0 = (iorIn - iorOut) / (iorIn + iorOut);
    r0 *= r0;
    double cosIn = -dot(normal, rayD);
    // account for backface
    if(cosIn < 0) {
        cosIn = -cosIn;
    }
    if(iorIn > iorOut) {
        double n = iorIn / iorOut;
        double sinOutSq = n * n * (1.0 - cosIn * cosIn);
        if(sinOutSq > 1.0) {
            return 1;
        }
        cosIn = sqrt(1.0 - sinOutSq);  // FIXME?
    }
    double cosTerm = (1 - cosIn);
    double reflFresnel = r0 + (1. - r0)*(cosTerm * cosTerm * cosTerm * cosTerm * cosTerm);
    if(reflFresnel > 1.0) {
        std::cerr << "gt one" << std::endl;
    }
    return matRefl + (1. - matRefl) * reflFresnel;
}

inline Ray Raytracer::getTransmissionRay(const std::shared_ptr<Material>& mat, const vec3<double>& normal,
                                         const vec3<double>& rayD, const vec3<double>& intersectPt,
                                         const double iorRatio) const {
    double cosIn = dot(normal, rayD);
    vec3<double> normalRef = normal;
    if(cosIn < 0) {
        // outside
        cosIn = -cosIn;
    }
    else {
        // inside
        normalRef = -normal;
    }
    // Find parallel and orthogonal portions of refraction direction
    vec3<double> refractDirP = iorRatio * (rayD + cosIn * normalRef);
    vec3<double> refractDirS = - std::sqrt(1. - std::pow(refractDirP.length(), 2)) * normalRef;

    vec3<double> refractDirection = refractDirP + refractDirS;
    // jitter refraction
    refractDirection += mat->getTransJitter() * vec3<double>::getRandom(-0.5, 0.5);
    refractDirection = getUnitVector(refractDirection);
    vec3<double> refractOrigin = intersectPt + (EPSILON * refractDirection);
    return Ray(refractOrigin, refractDirection);
}

inline vec3<int> Raytracer::getTransmission(const std::shared_ptr<Material>& mat, const vec3<double>& normal,
                                            const vec3<double>& rayD, const vec3<double>& intersectPt,
                                            double iorRatio, int rayCount, std::stack<double>& iors) {
    Ray transmissionRay = getTransmissionRay(mat, normal, rayD, intersectPt, iorRatio);
    return mat->getRefractionK() * getRayResult(transmissionRay, ++rayCount, iors);
}

inline vec3<int> Raytracer::getReflection(const std::shared_ptr<Material>& mat, const vec3<double>& normal,
                                          const vec3<double>& toView, const vec3<double>& intersectPt,
                                          int rayCount, std::stack<double>& iors) {
    if(mat->getRefl() == 0) {
        return {0, 0, 0};
    }
    vec3<double> reflectRayDirection = getUnitVector(((2 * (dot(normal, toView))) * normal) -
                                                     toView);
    // jitter reflection direction
    reflectRayDirection += mat->getReflJitter() * vec3<double>::getRandom(-0.5, 0.5);
    reflectRayDirection = getUnitVector(reflectRayDirection);
    vec3<double> reflectRayOrigin = intersectPt + (EPSILON * reflectRayDirection);
    Ray reflectionRay = Ray(reflectRayOrigin, reflectRayDirection);

    return mat->getRefl() * getRayResult(reflectionRay, ++rayCount, iors);
}

vec3<int> Raytracer::getRayResult(Ray ray, int rayCount, std::stack<double>& iors) {
    // if the maximum number of rays have been reached, this one will not contribute and stop recursion
    if(rayCount > MAX_NUM_RAYS) {
        return {0, 0, 0};
    }
    
    // Find the closest geo intersected by the ray
    // Intersection hit = getClosestIntersection(ray);
    RayHit hit = getClosestIntersection(ray);

    // If no geo was intersected, return the background color
    if(!hit.isHit) {
        return toIntVec3(255 * scene.getBackgroundColor());
    }

    vec3<int> colorResult(0, 0, 0);

    vec3<double> toView = getUnitVector(ray.getOrigin() - hit.point);
    vec3<int> primaryResult = illuminationEq(hit.material, hit.normal, toView, hit.point);
    colorResult += primaryResult;

    vec3<int> refractionResult(0, 0, 0);
    // Refraction & reflection
    if(hit.material->getIsRefractive()) {
        // find iorRatio (eta)
        double iorRatio;
        double iorIn;
        double iorOut;
        getIorAcrossIntersection(hit.material, hit.backFace, iorIn, iorOut, iorRatio, iors);

        double kTran = 0;
        double kRefl = getPortionReflected(hit.normal, ray.getDirection(),
                                           hit.material->getRefl(), iorIn, iorOut, iorRatio);
        if(kRefl < 1) {
            // compute refraction
            kTran = 1.0 - kRefl;
            refractionResult = getTransmission(hit.material, hit.normal, ray.getDirection(), hit.point, iorRatio,
                                               rayCount, iors);
        }
        // compute results from reflection
        vec3<int> reflectionResult = getReflection(hit.material, hit.normal, toView, hit.point, rayCount, iors);

        // combine reflection and refraction based on fresnel equations
        colorResult += kRefl * reflectionResult + kTran * refractionResult;
    }
    // Reflection (no refraction)
    else {
        vec3<int> reflectionResult = getReflection(hit.material, hit.normal, toView, hit.point, rayCount, iors);
        colorResult += reflectionResult;
    }

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
    vec3<double> vAxis = getUnitVector(cross(uAxis, viewRay));
    
    // calculate the u and v increments for a change in pixel
    double uInc = 2 * maxU / numCols;
    double vInc = 2 * maxV / numRows;

    WorldSpaceCoord coord(maxU, maxV, uInc, vInc, uAxis, vAxis);
    return coord;
    
}
