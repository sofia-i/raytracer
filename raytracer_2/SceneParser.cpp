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

vec3<double> SceneParser::readInVector(std::ifstream& infile){
    double x; double y; double z;
    infile >> x;
    infile >> y;
    infile >> z;
    return vec3<double>(x, y, z);
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
    vec3<double> cameraLookAt = readInVector(infile);

    infile >> description;
    vec3<double> cameraLookFrom = readInVector(infile);

    infile >> description;
    vec3<double> cameraLookUp = readInVector(infile);

    infile >> description;
    double fov;
    infile >> fov;

    Camera* camera = new Camera(cameraLookAt, cameraLookFrom, cameraLookUp, fov);

    infile >> description;
    vec3<double> directionToLight = readInVector(infile);

    infile >> description;
    vec3<double> lightColor = readInVector(infile);

    infile >> description;
    vec3<double> ambientLight = readInVector(infile);

    infile >> description;
    vec3<double> backgroundColor = readInVector(infile);

    std::vector<Sphere*> spherePtrs;
    while(!infile.eof()) {
        // take in sphere information
        std::string token;
        std::string sphereDescription;
        infile >> token;
        if (token == "#") {
            infile.ignore();
            getline(infile, sphereDescription);
        }
        else {
            break;
        }

        infile >> description; // circle
        
        infile >> description;
        vec3<double> center = readInVector(infile);

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
        vec3<double> objectColor = readInVector(infile);

        infile >> description;
        vec3<double> objectSpecular = readInVector(infile);

        infile >> description;
        double kgls;
        infile >> kgls;

        // create sphere
        Sphere* sphere = new Sphere(center, radius, kd, ks, ka, objectColor, objectSpecular,
                               kgls, sphereDescription);

        spherePtrs.push_back(sphere);
    }

    infile.close();

    // create scene
    Scene scene = Scene(camera, directionToLight, lightColor, ambientLight, backgroundColor,
                        spherePtrs);

    return scene;
}

