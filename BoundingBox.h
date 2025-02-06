//
// Created by Sofia Iannicelli on 1/31/25.
//

#ifndef RAYTRACER_2_BOUNDINGBOX_H
#define RAYTRACER_2_BOUNDINGBOX_H

#include <vector>
#include <memory>
#include "Hittable.h"
#include "Cylinder.h"
#include "Triangle.hpp"
#include "Sphere.h"

/**
 * Geometry axis-aligned bounding box
 */
class GeoBoundingBox : public Hittable {
public:
    explicit GeoBoundingBox(Extent extent, const std::shared_ptr<Geometry>& geoPtr);
    explicit GeoBoundingBox(const std::shared_ptr<Geometry>& geoPtr);

public:
    Extent getExtent() const { return extent; }

    RayHit findRayHit(Ray ray) override;

private:
    bool rayHitsBox(Ray ray);

private:
    Extent extent;
    std::shared_ptr<Geometry> geoPtr;
};

struct MSBBHit {
public:
    MSBBHit(bool isHit, double tNear, double tFar) : isHit(isHit), tNear(tNear), tFar(tFar) {
        tEnter = std::max(0., tNear);
    }
    static MSBBHit Miss() { return MSBBHit(false); }

    bool isHit;
    double tNear;
    double tFar;
    double tEnter;

private:
    explicit MSBBHit(bool isHit) : isHit(isHit) {}
};

/**
 * Bounding box for use with median-split
 */
class MSBoundingBox {
public:
    MSBoundingBox() = default;
    explicit MSBoundingBox(Extent bounds);
    MSBoundingBox(Extent bounds, std::vector<uint> gbbIndexes);

    MSBBHit findRayIntersection(Ray ray);

    std::vector<uint> getGbbIndexes() { return gbbIndexes; }

private:
    Extent bounds;
    std::vector<uint> gbbIndexes;  // geo bounding box indexes
};

#endif //RAYTRACER_2_BOUNDINGBOX_H
