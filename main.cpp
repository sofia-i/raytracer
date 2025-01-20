//
//  main.cpp
//  raytracer_2
//
//  Created by Sofia Iannicelli on 2/15/23.
//

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

#include "Scene.hpp"
#include "SceneParser.hpp"
#include "Raytracer.hpp"
#include "PpmWriter.hpp"

void runRaytrace(Raytracer* raytracer, int numColumns, int numRows, const std::string& outputFilePath,
                 const std::string& magicNumber, int maxColorVal) {
    // do the raytracing process to get the color results for each pixel
    int*** pixelColors = raytracer->raytrace(numColumns, numRows);
    
    std::ofstream outputFile = PpmWriter::writePpm(outputFilePath, magicNumber, numColumns, numRows,
                                                   maxColorVal, pixelColors);

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
}

int main(int argc, char *argv[]) {
    std::string usage_instructions = "Usage: <inputFilePath> <outputFilePath>";

    if(argc != 3) {
        std::cerr << usage_instructions << std::endl;
        return EXIT_FAILURE;
    }
    // first command-line argument: input_file_path
    std::string inputFilePath(argv[1]);
    // Second argument: output_file_path
    std::string outputFilePath(argv[2]);

    // parse the scene information from the input file
    SceneParser sceneParser;
    Scene scene = sceneParser.parseFile(inputFilePath);
    std::cout << scene << std::endl;

    // send the results to a ppm file for output
    std::string magicNumber = "P3";
    int numColumns = 1024;
    int numRows = 1024;
    int maxColorVal = 255;
    
    Raytracer* raytracer = new Raytracer(scene);

    runRaytrace(raytracer, numColumns, numRows, outputFilePath, magicNumber, maxColorVal);
    // runRaytrace(raytracer, scene, num_columns, num_rows, outputFilePath, magicNumber, maxColorVal);

    return 0;
}
