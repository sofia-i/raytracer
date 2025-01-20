//
//  main.cpp
//  raytracer_2
//
//  Created by Sofia Iannicelli on 2/15/23.
//

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

#include "Scene.hpp"
#include "SceneParser.hpp"
#include "Raytracer.hpp"
#include "PpmWriter.hpp"

void runRaytrace(Raytracer* raytracer, int numColumns, int numRows, std::string outputFilePath, std::string magicNumber, int maxColorVal) {
    
    // do the raytracing process to get the color results for each pixel
    int*** pixelColors = raytracer->raytrace(numColumns, numRows);
    
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
}

void getLoopCameraUpVectors(vec3<double>* upVectors, int numSteps, double originalAngle, double endAngle) {
    double rotationInterval;
    double totalRotation = endAngle - originalAngle;
    rotationInterval = totalRotation / (numSteps-1);
    
    for(int i = 0; i < numSteps; ++i) {
        double rotationAngle = originalAngle + i * rotationInterval;
        vec3<double> cameraLookUp = vec3<double>(cos(rotationAngle), sin(rotationAngle), 0.0);
        upVectors[i] = cameraLookUp;
    }
}

void getLoopLightDirectionVectors(vec3<double>* lightVectors, int numSteps, double startAngle, double endAngle) {
    double totalRotation = endAngle - startAngle;
    double rotationInterval = totalRotation / (numSteps - 1);
    
    for(int i = 0; i < numSteps; ++i) {
        double rotationAngle = startAngle + i * rotationInterval;
        vec3<double> cameraLookUp = vec3<double>(0.0, sin(rotationAngle), cos(rotationAngle));
        lightVectors[i] = cameraLookUp;
    }
}

void getLookFromAtAround(vec3<double>* lookFromVectors, vec3<double>* lookAtVectors, vec3<double> center, double distanceFromTargetCenter, double focalLength, int numSteps, double originalAngle, double endAngle) {
    double totalRotation = endAngle - originalAngle;
    double rotationInterval = totalRotation / (numSteps - 1);
    
    for(int i = 0; i < numSteps; ++i) {
        double rotationAngle = originalAngle + i * rotationInterval;
        vec3<double> lookFrom = vec3<double>(center.x() + distanceFromTargetCenter * cos(rotationAngle), center.y(), center.z() + distanceFromTargetCenter * sin(rotationAngle));
        vec3<double> lookAt = lookFrom + focalLength * getUnitVector(center - lookFrom);
        // std::cout << lookFrom << std::endl;
        lookFromVectors[i] = lookFrom;
        lookAtVectors[i] = lookAt;
    }
}

void do_a_flip(std::string outputFolderPath, Raytracer* raytracer, Scene scene, int numColumns, int numRows, std::string magicNumber, int maxColorVal) {
    
    std::string fileNamePrefix = "output_image";
    std::string fileNameSuffix = ".ppm";
    Camera* camera = raytracer->getCamera();
    
    // rotate the whole scene
    int numSteps = 120;
    vec3<double> upVectors[numSteps];
    double originalAngle = M_PI / 2;
    double endAngle = originalAngle + 2 * M_PI;
    getLoopCameraUpVectors(upVectors, numSteps, originalAngle, endAngle);
    
    for(int i = 0; i < numSteps; ++i) {
        // double rotationAngle = originalAngle + i * rotationInterval;
        // vec3<double> cameraLookUp = vec3<double>(cos(rotationAngle), sin(rotationAngle), 0.0);
        vec3<double> cameraLookUp = upVectors[i];
        
        camera->setCameraLookUp(cameraLookUp);
        
        std::string outputFilePath = outputFolderPath + "/" + fileNamePrefix + "flip" + std::to_string(i) +  fileNameSuffix;
        
        runRaytrace(raytracer, numColumns, numRows, outputFilePath, magicNumber, maxColorVal);
    }
    
    vec3<double> lightVectors[numSteps];
    originalAngle = M_PI / 2;
    endAngle = originalAngle + (M_PI / 2) - (M_PI / 8);
    getLoopLightDirectionVectors(lightVectors, numSteps, originalAngle, endAngle);
    
    for(int i = 0; i < numSteps; ++i) {
        vec3<double> lightVector = lightVectors[i];
        raytracer->getScene()->setDirectionToLight(lightVector);
        
        std::string outputFilePath = outputFolderPath + "/" + fileNamePrefix + "light" + std::to_string(i) +  fileNameSuffix;
        
        runRaytrace(raytracer, numColumns, numRows, outputFilePath, magicNumber, maxColorVal);
    }
    
    vec3<double> lookFromVectors[numSteps];
    vec3<double> lookAtVectors[numSteps];
    double lookOriginalAngle = (M_PI / 2);
    double lookEndAngle = lookOriginalAngle + M_PI * 2;
    vec3<double> center = vec3<double>(0.0, 0.0, -1.0);
    double distanceToCenter = std::abs(camera->getCameraLookFrom().z() - center.z());
    double focalLength = (camera->getCameraLookAt() - camera->getCameraLookFrom()).length();
    std::cout << "dist to center " << distanceToCenter << std::endl;
    std::cout << "focal length " << focalLength << std::endl;
    
    std::cout << "original look at " << raytracer->getCamera()->getCameraLookAt() << std::endl;
    std::cout << "original look from " << raytracer->getCamera()->getCameraLookFrom() << std::endl;
    
    
    getLookFromAtAround(lookFromVectors, lookAtVectors, center, distanceToCenter, focalLength, numSteps, lookOriginalAngle, lookEndAngle);
    for(int i = 0; i < numSteps; ++i) {
        vec3<double> lookFrom = lookFromVectors[i];
        vec3<double> lookAt = lookAtVectors[i];
        camera->setCameraLookFrom(lookFrom);
        camera->setCameraLookAt(lookAt);
        
        std::cout << "from " << lookFrom.x() << "," << lookFrom.z() << std::endl;
        std::cout << "at " << lookAt.x() << "," << lookAt.z() << std::endl;
        
        std::string outputFilePath = outputFolderPath + "/" + fileNamePrefix + "rotate" + std::to_string(i) +  fileNameSuffix;
        
        // runRaytrace(raytracer, numColumns, numRows, outputFilePath, magicNumber, maxColorVal);
    }
}

int main() {
    // configure file info
    std::string outputFolderPath = "/Users/sofiaiannicelli/Documents/Past_School_Files/BYU_WINTER_2023/graphics/raytracer_2/raytracer_2/outputs/test";
    std::string outputFilePath = "/Users/sofiaiannicelli/Documents/Past_School_Files/BYU_WINTER_2023/graphics/raytracer_2/raytracer_2/outputs/output_image_1_libby1.ppm";
    std::string inputFilePath = "/Users/sofiaiannicelli/Documents/Past_School_Files/BYU_WINTER_2023/graphics/raytracer_2/raytracer_2/inputs/input_1_1.txt";
    // std::string inputFilePath = "/Users/sofiaiannicelli/Documents/Past_School_Files/BYU_WINTER_2023/graphics/raytracer_2/raytracer_2/inputs/simple.txt";
    

    // parse the scene information from the input file
    SceneParser sceneParser;
    Scene scene = sceneParser.parseFile(inputFilePath);
    std::cout << scene << std::endl;
    
    // send the results to a ppm file for output
    std::string magicNumber = "P3";
    int numColumns = 1024;
    int numRows = 1024;
    int maxColorVal = 255;
    
    Raytracer raytracer = Raytracer(scene, numColumns, numRows);
    
    // run_raytrace(raytracer, scene, numColumns, numRows, outputFilePath, magicNumber, maxColorVal);
    do_a_flip(outputFolderPath, &raytracer, scene, numColumns, numRows, magicNumber, maxColorVal);
    
    return 0;
}
