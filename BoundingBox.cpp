//
// Created by Sofia Iannicelli on 1/31/25.
//

#include <cfloat>
#include <utility>
#include "BoundingBox.h"

GeoBoundingBox::GeoBoundingBox(Extent extent, const std::shared_ptr<Geometry>& geoPtr) :
        extent(extent), geoPtr(geoPtr) { }

GeoBoundingBox::GeoBoundingBox(const std::shared_ptr<Geometry>& geoPtr) : extent(geoPtr->findExtent()), geoPtr(geoPtr)
{
}

bool GeoBoundingBox::rayHitsBox(Ray ray) {
    // https://education.siggraph.org/static/HyperGraph/raytrace/rtinter3.htm
    // https://medium.com/@bromanz/another-view-on-the-classic-ray-aabb-intersection-algorithm-for-bvh-traversal-41125138b525
    double tNear = -DBL_MAX;
    double tFar = DBL_MAX;
    for(int d = 0; d < 3; ++d) {
        double t1 = (extent.corners[0][d] - ray.getOrigin()[d]) * ray.getInvDir()[d];
        double t2 = (extent.corners[1][d] - ray.getOrigin()[d]) * ray.getInvDir()[d];

        tNear = std::max(tNear, std::min(t1, t2));
        tFar = std::min(tFar, std::max(t1, t2));
        if(tNear > tFar) return false;
        if(tFar < 0) return false;
    }
    return true;
}

RayHit GeoBoundingBox::findRayHit(Ray ray) {
    // check intersection with the box
    if(!rayHitsBox(ray)) return RayHit::Miss();

    // intersected with box, so now check geo
    return geoPtr->findRayHit(ray);
}

MSBoundingBox::MSBoundingBox(Extent bounds) : bounds(bounds) { }

MSBoundingBox::MSBoundingBox(Extent bounds, std::vector<uint> gbbIndexes): bounds(bounds),
                                                                           gbbIndexes(std::move(gbbIndexes)) {}

MSBBHit MSBoundingBox::findRayIntersection(Ray ray) {
    // TODO: can update to make more efficient
    // https://education.siggraph.org/static/HyperGraph/raytrace/rtinter3.htm
    // https://medium.com/@bromanz/another-view-on-the-classic-ray-aabb-intersection-algorithm-for-bvh-traversal-41125138b525
    double tNear = -DBL_MAX;
    double tFar = DBL_MAX;
    for(int d = 0; d < 3; ++d) {
        double t1 = (bounds.corners[0][d] - ray.getOrigin()[d]) * ray.getInvDir()[d];
        double t2 = (bounds.corners[1][d] - ray.getOrigin()[d]) * ray.getInvDir()[d];

        tNear = std::max(tNear, std::min(t1, t2));
        tFar = std::min(tFar, std::max(t1, t2));
        if(tNear > tFar) return MSBBHit::Miss();
        if(tFar < 0) return MSBBHit::Miss();
    }

    return {true, tNear, tFar};
}
