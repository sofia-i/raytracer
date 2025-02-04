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

double Cylinder::findRayObjectIntersection(Ray ray) {
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

double Cylinder::findRayObjectIntersection(Ray ray, vec3<double> &intersectNormal, bool &backFace) {
    double t = findRayObjectIntersection(ray);

    if(t < 0) {
        return t;
    }

    vec3<double> intersectPt = ray.getPointOnRay(t);
    if((intersectPt - capCenter0).length() <= radius) {
        // cap 0
        intersectNormal = -cylinderD;
    }
    else if((intersectPt - capCenter1).length() <= radius) {
        // cap 1
        intersectNormal = cylinderD;
    }
    else {
        // intersected cylinder
        double tAlongCenterline = dot((intersectPt - capCenter0), cylinderD);
        vec3<double> centerlinePt = capCenter0 + tAlongCenterline * cylinderD;
        intersectNormal = getUnitVector(intersectPt - centerlinePt);
    }

    backFace = dot(intersectNormal, ray.getDirection()) > 0;
    if(backFace) {
        intersectNormal = -intersectNormal;
    }
    return t;
}

Extent Cylinder::findExtent() {
    Extent extent = Extent(capCenter0);
    vec3<double> normal = getDir();

    vec3<double> dir1 = getUnitVector(vec3<double>(-normal.y(), normal.x(), 0));
    vec3<double> dir2 = getUnitVector(vec3<double>(-normal.z(), 0, normal.x()));

    extent.update(capCenter0 - dir1 * radius);
    extent.update(capCenter0 + dir1 * radius);
    extent.update(capCenter0 - dir2 * radius);
    extent.update(capCenter0 + dir2 * radius);
    extent.update(capCenter1 - dir1 * radius);
    extent.update(capCenter1 + dir1 * radius);
    extent.update(capCenter1 - dir2 * radius);
    extent.update(capCenter1 + dir2 * radius);

    return extent;
}
