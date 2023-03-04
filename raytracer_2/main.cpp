//
//  main.cpp
//  raytracer_2
//
//  Created by Sofia Iannicelli on 2/15/23.
//

#include <iostream>
#include <fstream>
#include <string>

#include "Scene.hpp"
#include "SceneParser.hpp"
#include "Raytracer.hpp"
#include "PpmWriter.hpp"

int main() {
    /*
    // configure file info
    std::string input_suffix = "1";
    std::string outputFilePath = "/Users/sofiaiannicelli/Documents/BYU_WINTER_2023/graphics/raytracer_2/raytracer_2/outputs/output_image" + input_suffix + ".ppm";
    std::string inputFilePath = "/Users/sofiaiannicelli/Documents/BYU_WINTER_2023/graphics/raytracer_2/raytracer_2/inputs/input" + input_suffix + ".txt";

    // parse the scene information from the input file
    SceneParser sceneParser;
    Scene scene = sceneParser.parseFile(inputFilePath);
    std::cout << scene << std::endl;
    
    // send the results to a ppm file for output
    std::string magicNumber = "P3";
    int numColumns = 256;
    int numRows = 256;
    int maxColorVal = 255;
    
    Raytracer raytracer = Raytracer(scene, numColumns, numRows);
    
    // do the raytracing process to get the color results for each pixel
    int*** pixelColors = raytracer.raytrace(scene, numColumns, numRows);
    
    std::ofstream outputFile = PpmWriter::writePpm(outputFilePath, magicNumber, numColumns, numRows, maxColorVal, pixelColors);

    // close file
    outputFile.close();
    
    // deallocate memory
    for(int i = 0; i < numRows; ++i) {
        for(int j = 0; j < numColumns; ++j) {
            delete[] pixelColors[i][j];
        }
    }
    for(int i = 0; i < numRows; ++i) {
        delete[] pixelColors[i];
    }
    delete[] pixelColors;
     */
    vec3<double> sphere_center = vec3<double>(0, 0, 0);
    double radius = .4;
    double kd = 0.7;
    double ks = 0.2;
    double ka = 0.1;
    vec3<double> objectColor = vec3<double>(1.0, 0.0, 1.0);
    vec3<double> objectSpecular = vec3<double>(1.0, 1.0, 1.0);
    double kgls = 16.0;
    Sphere* sphere = new Sphere(sphere_center, radius, kd, ks, ka, objectColor, objectSpecular, kgls);
    Raytracer raytracer = Raytracer();
    
    vec3<double> ray_origin = vec3<double>(0, 0, 1);
    vec3<double> ray_direction = vec3<double>(-0.049, 0.396, -.0916);
    // direction: (-.049, 0.396, -.0916)
    // origin: (0, 0, 1)
    Ray ray = Ray(ray_origin, ray_direction);
    std::cout << raytracer.illuminationEqRay(sphere, ray);
    
    return 0;
}
