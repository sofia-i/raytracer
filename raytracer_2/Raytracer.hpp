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
    
    vec3 getRayResult(double x, double y);
    vec3 getRayResult(Ray ray, double weight, int rayCount);
    
    bool rayIntersectsSphere(Ray ray, Sphere sphere, double& t);
    
    void calculateWorldSpaceCoords();
    vec3 illuminationEq(Sphere sphere, vec3 normal, vec3 view);
    
public:
    Raytracer(Scene scene, int numColumns, int numRows) : scene(scene) {
        this->numColumns = numColumns;
        this->numRows = numRows;
    }
    
    double*** raytrace(Scene scene, int numColumns, int numRows) {
        // initialize pixelColors multi-dimensional array
        double*** pixelColors = new double**[numRows];
        for(int i = 0; i < numRows; ++i) {
            pixelColors[i] = new double*[numColumns];
            for(int j = 0; j < numColumns; ++j) {
                pixelColors[i][j] = new double[3];
            }
        }
        
        calculateWorldSpaceCoords();
        
        // handle each pixel
        for(int i = 0; i < numRows; ++i) {
            for(int j = 0; j < numColumns; ++j) {
                vec3 pixelColor = getRayResult(leftmost_x + j * x_inc, topmost_y - i * y_inc);
                pixelColors[i][j][0] = pixelColor[0];
                pixelColors[i][j][1] = pixelColor[1];
                pixelColors[i][j][2] = pixelColor[2];
            }
        }
        
        return pixelColors;
    }
    
};

#endif /* Raytracer_hpp */
