//
//  Raytracer.hpp
//  raytracer_2
//
//  Created by Sofia Iannicelli on 2/15/23.
//

#ifndef Raytracer_hpp
#define Raytracer_hpp

#include <stdio.h>
#include "Scene.hpp"
#include "Ray.hpp"

// #define MAX_NUM_RAYS 5;

class Raytracer {
private:
    const int MAX_NUM_RAYS = 5;
    const double EPSILON = 2e-8; //e-4;
    
    Scene scene;
    int numColumns;
    int numRows;
    
    vec3<double> u_axis;
    vec3<double> v_axis;
    
    double max_u;
    double max_v;
    
    double u_inc;
    double v_inc;
    
    vec3<int> getRayResult(vec3<double> target);
    vec3<int> getRayResult(Ray ray, int rayCount);
    
    bool rayIntersectsSphere(Ray ray, Sphere* sphere, double& t);
    
    void calculateWorldSpaceCoords();
    vec3<int> illuminationEq(Object* object, vec3<double> normal, vec3<double> view);
    
    
public:
    Raytracer(Scene scene, int numColumns, int numRows) : scene(scene) {
        this->numColumns = numColumns;
        this->numRows = numRows;
    }
    
    int*** raytrace(Scene scene, int numColumns, int numRows);
    
};

#endif /* Raytracer_hpp */
