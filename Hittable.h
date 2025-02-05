//
// Created by Sofia Iannicelli on 2/4/25.
//

#ifndef RAYTRACER_2_HITTABLE_H
#define RAYTRACER_2_HITTABLE_H

#include "Material.h"

struct RayHit {
    RayHit() = default;
    RayHit(bool isHit, double t, const vec3<double>& intersectPt, const vec3<double>& normal,
           bool backFace, const std::shared_ptr<Material>& mat) :
            isHit(isHit), t(t), point(intersectPt), normal(normal), backFace(backFace), material(mat) {}

    static RayHit Miss() { return RayHit(false); }

    // int objIndex;
    bool isHit;
    double t;
    vec3<double> point;
    vec3<double> normal;
    bool backFace;
    std::shared_ptr<Material> material;

private:
    explicit RayHit(bool isHit) : isHit(isHit) {}
};

class Hittable {
public:
    virtual RayHit findRayHit(Ray ray) = 0;
};

#endif //RAYTRACER_2_HITTABLE_H
