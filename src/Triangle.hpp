//
//  Triangle.h
//  raytracer_2
//
//  Created by Sofia Iannicelli on 3/4/23.
//

#ifndef Triangle_hpp
#define Triangle_hpp

#include <cstdio>
#include <vector>
#include "Geometry.h"
#include "vec2.hpp"

class Triangle : public Geometry {
public:
    Triangle(const std::vector<vec3<double>>& vertices, const std::vector<vec2<double>>& uvs,
             std::shared_ptr<Material> mat, const std::string& description);
    Triangle(const std::vector<vec3<double>>& vertices, std::shared_ptr<Material> mat, const std::string& description);

    ~Triangle() override = default; // I. destructor
    Triangle(const Triangle& other) = default; // II. copy constructor
    Triangle& operator=(const Triangle& other) = default; // III. copy assignment
    Triangle(Triangle&& other) noexcept = default;// IV. move constructor
    Triangle& operator=(Triangle&& other) noexcept = default; // V. move assignment

    RayHit findRayHit(Ray ray) override;

    Extent findExtent() override;

    void getUV(const vec3<double>& point, double& u, double& v);

    std::vector<vec3<double>> getVertices() { return vertices; }
    
    std::string toString() const override {
        std::string str;
        std::stringstream ss(str);
        
        ss << getDescription() << std::endl;
        // print out the vertices
        ss << Geometry::toString() << std::endl;
        
        return ss.str();
    }

private:
    double findRayGeoIntersectionT(Ray ray);

private:
    void calculatePlaneNormal();
    double calculateDistToOrigin();

    vec3<double> planeNormal;
    double distToOrigin;
    bool doubleSided = true;
    std::vector<vec3<double>> vertices;
    std::vector<vec2<double>> uvs;
};


#endif /* Triangle_h */
