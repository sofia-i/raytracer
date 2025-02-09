//
// Created by Sofia Iannicelli on 1/22/25.
//

#include "Cylinder.h"

Cylinder::Cylinder(vec3<double> capCenter1, vec3<double> capCenter2, double radius,
                   const std::shared_ptr<Material>& mat, const std::string& description) :
        Geometry(mat, description),
        capCenter0(capCenter1), capCenter1(capCenter2), radius(radius) {
    cylinderD = getUnitVector(capCenter2 - capCenter1);
}

bool Cylinder::cylinderPtInBounds(const vec3<double>& pt) {
    return dot(cylinderD, pt - capCenter0) > 0 && dot(cylinderD, pt - capCenter1) < 0;
}

double Cylinder::calculateDistToOrigin(const vec3<double>& pt) {
    return dot(cylinderD, -pt); // FIXME
}

bool Cylinder::capPtInBounds(const vec3<double>& capCenter, const vec3<double>& pt) const {
    vec3<double> diff = pt - capCenter;
    return dot(diff, diff) < radius * radius;
}

double Cylinder::findRayGeoIntersectionT(Ray ray) {
    const vec3<double>& rayO = ray.getOrigin();
    const vec3<double>& rayD = ray.getDirection();
    vec3<double> cylinderPt = capCenter0;
    double t = -1;

    // Intersect with infinite cylinder
    vec3<double> aHelper = rayD - dot(rayD, cylinderD) * cylinderD;
    double a = dot(aHelper, aHelper);
    if(a != 0) {
        vec3<double> deltaP = rayO - cylinderPt;
        double b = 2 * dot(rayD - dot(rayD, cylinderD) * cylinderD,
                           deltaP - dot(deltaP, cylinderD) * cylinderD);
        vec3<double> cHelper = deltaP - dot(deltaP, cylinderD) * cylinderD;
        double c = dot(cHelper, cHelper) - radius * radius;

        // compute the discriminant of the quadratic formula
        double discriminant = b * b - 4 * a * c;
        // if the discriminant is positive, there are one or two intersection points
        if(discriminant >= 0) {
            double sqrtDisc = sqrt(discriminant);
            // calculate smaller intersection parameter
            double t0 = (-b - sqrtDisc) / (2 * a);
            // check in bounds
            bool t0InBounds = cylinderPtInBounds(ray.getPointOnRay(t0));
            // if positive and in bounds, update t
            if(t0 >= 0 && t0InBounds) {
                t = t0;
            }
            else {
                // calculate larger t-value
                double t1 = (-b + sqrtDisc) / (2 * a);
                // check in bounds
                bool t1InBounds = cylinderPtInBounds(ray.getPointOnRay(t1));
                // if positive and in bounds, update t
                if(t1 >= 0 && t1InBounds) {
                    t = t1;
                }
            }
        }
    }

    // Intersect with each cap plane
    double d;
    double denominator;
    denominator = dot(cylinderD, ray.getDirection());
    // Cap plane 1
    // cap plane normal is the cylinder direction
    d = calculateDistToOrigin(capCenter0);
    if(denominator != 0) {
        double t3 = -(dot(cylinderD, ray.getOrigin()) + d) / denominator;
        // double t3 = -(dot(cylinderD, ray.getOrigin() - capCenter0)) / denominator;
        // check if intersection is inside cap
        bool inBounds = capPtInBounds(capCenter0, ray.getPointOnRay(t3));
        if(t3 >= 0 && inBounds && (t == -1 || t3 < t)) {
            t = t3;
        }
    }

    // Cap plane 2
    d = calculateDistToOrigin(capCenter1);
    if(denominator != 0) {
        double t4 = -(dot(cylinderD, ray.getOrigin()) + d) / denominator;
        // check if intersection is inside cap
        bool inBounds = capPtInBounds(capCenter1, ray.getPointOnRay(t4));
        if(t4 >= 0 && inBounds && (t == -1 || t4 < t)) {
            t = t4;
        }
    }

    return t;
}

RayHit Cylinder::findRayHit(Ray ray) {
    double t = findRayGeoIntersectionT(ray);

    // TODO: ?
    if(t <= 0) return RayHit::Miss();

    vec3<double> hitPoint = ray.getPointOnRay(t);
    vec3<double> hitNormal;

    if((hitPoint - capCenter0).length() <= radius) {
        // cap 0
        hitNormal = -cylinderD;
    }
    else if((hitPoint - capCenter1).length() <= radius) {
        // cap 1
        hitNormal = cylinderD;
    }
    else {
        // intersected cylinder
        double tAlongCenterline = dot((hitPoint - capCenter0), cylinderD);
        vec3<double> centerlinePt = capCenter0 + tAlongCenterline * cylinderD;
        hitNormal = getUnitVector(hitPoint - centerlinePt);
    }

    bool backFace = dot(hitNormal, ray.getDirection()) > 0;
    if(backFace) {
        hitNormal = -hitNormal;
    }

    return {true, t, hitPoint, hitNormal, backFace, material} ;
}

Extent Cylinder::findExtent() {
    Extent extent = Extent(capCenter0);
    // https://www.gamedev.net/forums/topic/338522-bounding-box-for-a-cylinder/#:~:text=Bounds%20in%20direction%20X%20(same%20for%20Y%20and%20Z)%20can%20be%20found%20as%3A%0ALet%20A.X%3CB.X%20(otherwise%20swap%20points)%0AGood%20approximate%20lowest%20bound%20is%20A.X%2Dr%20and%20highest%20is%20B.X%2Br%20(precise%20for%20capsule).%20At%20worst%2C%20in%20one%20direction%20it%20can%20be%20larger%20than%20needed
    for(int d = 0; d < 3; ++d) {
        double a = capCenter0[d];
        double b = capCenter1[d];
        if(a > b) std::swap(a, b);
        extent.corners[0][d] = a - radius;
        extent.corners[1][d] = b + radius;
    }

    return extent;
}
