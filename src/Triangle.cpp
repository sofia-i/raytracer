//
//  Triangle.cpp
//  raytracer_2
//
//  Created by Sofia Iannicelli on 3/4/23.
//

#include "Triangle.hpp"
#include "Ray.hpp"

Triangle::Triangle(const std::vector<vec3<double>>& vertices, const std::vector<vec2<double>>& uvs,
         std::shared_ptr<Material> mat, const std::string& description) :
            Geometry(mat, std::move(description)), vertices(vertices), uvs(uvs) {
    calculatePlaneNormal();
    distToOrigin = calculateDistToOrigin();
}

Triangle::Triangle(const std::vector<vec3<double>>& vertices, std::shared_ptr<Material> mat, const std::string& description) :
        Triangle(vertices,
                 {{1, 0}, {0, 1}, {0, 0}},
                 mat,
                 description) {}

double Triangle::calculateDistToOrigin() {
    return dot(planeNormal, -vertices[0]);
}

void Triangle::calculatePlaneNormal() {
    // assuming the vertices are specified in CCW order
    vec3<double> vector1 = getUnitVector(vertices.at(0) - vertices.at(1));
    vec3<double> vector2 = getUnitVector(vertices.at(2) - vertices.at(1));
    
    vec3<double> normal = cross(vector2, vector1);
    this->planeNormal = getUnitVector(normal);
}

double Triangle::findRayGeoIntersectionT(Ray ray) {
    /*
     * check if the ray intersects the plane containing the triangle
     */

    double d = calculateDistToOrigin();

    // extract out the ray info
    vec3<double> ray_o = ray.getOrigin();
    vec3<double> ray_d = ray.getDirection();

    double denominator = dot(planeNormal, ray_d);
    if(denominator == 0) { return -1; }
    double t = -(dot(planeNormal, ray_o) + d) / denominator;

    if(t < 0) {
        return t;
    }

    vec3<double> intersectionPt = ray.getPointOnRay(t);
    // check if the intersection point is inside the triangle
    for(int i = 0; i < 3; ++i) {
        vec3<double> testVertex = vertices.at(i);
        vec3<double> nextVertex = vertices.at((i + 1) % 3);

        vec3<double> testNormal = cross((nextVertex - testVertex), (intersectionPt - testVertex));

        if(dot(planeNormal, testNormal) < 0) {
            return -1;
        }
    }

    return t;
}

RayHit Triangle::findRayHit(Ray ray) {
    double t = findRayGeoIntersectionT(ray);
    // miss if t is negative
    // FIXME: negative here breaks for triangle on bounding box
    if(t < 0) return RayHit::Miss();

    // Intersected, so calculate intersect information
    vec3<double> hitNormal;
    bool backFace = false;
    if(dot(ray.getDirection(), planeNormal) < 0.0) {
        // The ray and the plane normal are facing in opposite directions (front face)
        hitNormal = planeNormal;
    }
    else {
        // back face
        hitNormal = -planeNormal;
        if(!doubleSided) {
            backFace = true;
        }
    }

    vec3<double> hitPoint = ray.getPointOnRay(t);

    double u, v;
    getUV(hitPoint, u, v);

    return {true, t, u, v, hitPoint, hitNormal, backFace, material};
}

Extent Triangle::findExtent() {
    Extent extent(vertices.at(0));
    for(auto vert : vertices) {
        extent.update(vert);
    }
    return extent;
}

void Triangle::getUV(const vec3<double> &point, double &u, double &v) {
    // use barycentric coordinates
    // https://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates
    vec3<double> v0 = vertices[1] - vertices[0];
    vec3<double> v1 = vertices[2] - vertices[0];
    vec3<double> v2 = point - vertices[0];

    double d00 = dot(v0, v0);
    double d01 = dot(v0, v1);
    double d11 = dot(v1, v1);
    double d20 = dot(v2, v0);
    double d21 = dot(v2, v1);
    double denom = d00 * d11 - d01 * d01;

    double w1 = (d11 * d20 - d01 * d21) / denom;
    double w2 = (d00 * d21 - d01 * d20) / denom;
    double w0 = 1.0 - w2 - w1;

    u = w0 * uvs[0][0] + w1 * uvs[1][0] + w2 * uvs[2][0];
    v = w0 * uvs[0][1] + w1 * uvs[1][1] + w2 * uvs[2][1];
}

