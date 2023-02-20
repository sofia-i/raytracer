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
    
    Scene scene;
    int numColumns;
    int numRows;
    
    double leftmost_x;
    double topmost_y;
    double x_inc;
    double y_inc;
    
    vec3<int> getRayResult(double x, double y);
    vec3<int> getRayResult(Ray ray, double weight, int rayCount);
    
    bool rayIntersectsSphere(Ray ray, Sphere* sphere, double& t);
    
    void calculateWorldSpaceCoords();
    vec3<int> illuminationEq(Sphere* sphere, vec3<double> normal, vec3<double> view);
    
public:
    Raytracer(Scene scene, int numColumns, int numRows) : scene(scene) {
        this->numColumns = numColumns;
        this->numRows = numRows;
    }
    
    int*** raytrace(Scene scene, int numColumns, int numRows);
    
};

#endif /* Raytracer_hpp */
