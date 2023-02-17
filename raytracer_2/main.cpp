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
    // configure file info
    std::string outputFilePath = "/Users/sofiaiannicelli/Documents/BYU_WINTER_2023/graphics/raytracer_2/raytracer_2/outputs/output_image.ppm";
    std::string inputFilePath = "/Users/sofiaiannicelli/Documents/BYU_WINTER_2023/graphics/raytracer_2/raytracer_2/inputs/input1.txt";

    // parse the scene information from the input file
    SceneParser sceneParser;
    Scene scene = sceneParser.parseFile(inputFilePath);
    std::cout << scene << std::endl;
    
    // send the results to a ppm file for output
    std::string magicNumber = "P3";
    int numColumns = 100;
    int numRows = 100;
    int maxColorVal = 255;
    
    Raytracer raytracer = Raytracer(scene, numColumns, numRows);
    
    // do the raytracing process to get the color results for each pixel
    double*** pixelColors = raytracer.raytrace(scene, numColumns, numRows);
    
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
    return 0;
}
