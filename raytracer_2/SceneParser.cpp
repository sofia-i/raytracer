//
//  SceneParser.cpp
//  raytracer_2
//
//  Created by Sofia Iannicelli on 2/15/23.
//

#include "SceneParser.hpp"

#include <fstream>
#include <vector>
#include "Sphere.h"
#include "vec3.hpp"

vec3 SceneParser::readInVector(std::ifstream& infile){
    double x; double y; double z;
    infile >> x;
    infile >> y;
    infile >> z;
    return vec3(x, y, z);
}

Scene SceneParser::parseFile(std::string inputFilePath) {
    std::ifstream infile;
    infile.open(inputFilePath);
    if(!infile.is_open()) {
        std::cout << "not right";
    }
    else {
        // std::cout << infile.rdbuf();
    }

    std::string description;
    infile >> description;
    vec3 cameraLookAt = readInVector(infile);

    infile >> description;
    vec3 cameraLookFrom = readInVector(infile);

    infile >> description;
    vec3 cameraLookUp = readInVector(infile);

    infile >> description;
    double fov;
    infile >> fov;

    Camera camera = Camera(cameraLookAt, cameraLookFrom, cameraLookUp, fov);

    infile >> description;
    vec3 directionToLight = readInVector(infile);

    infile >> description;
    vec3 lightColor = readInVector(infile);

    infile >> description;
    vec3 ambientLight = readInVector(infile);

    infile >> description;
    vec3 backgroundColor = readInVector(infile);

    std::vector<Sphere> spheres;
    while(!infile.eof()) {
        // take in sphere information
        std::string token;
        std::string sphereDescription;
        infile >> token;
        if (token == "#") {
            infile.ignore();
            getline(infile, sphereDescription);
        }

        infile >> description; // circle
        
        infile >> description;
        vec3 center = readInVector(infile);

        infile >> description;
        double radius;
        infile >> radius;

        infile >> description;
        double kd;
        infile >> kd;

        infile >> description;
        double ks;
        infile >> ks;

        infile >> description;
        double ka;
        infile >> ka;

        infile >> description;
        vec3 objectColor = readInVector(infile);

        infile >> description;
        vec3 objectSpecular = readInVector(infile);

        infile >> description;
        double kgls;
        infile >> kgls;

        // create sphere
        Sphere sphere = Sphere(center, radius, kd, ks, ka, objectColor, objectSpecular,
                               kgls, sphereDescription);

        spheres.push_back(sphere);
    }

    infile.close();

    // create scene
    Scene scene = Scene(camera, directionToLight, lightColor, ambientLight, backgroundColor,
                        spheres);

    return scene;
}

