//
//  SceneParser.hpp
//  raytracer_2
//
//  Created by Sofia Iannicelli on 2/15/23.
//

#ifndef SceneParser_hpp
#define SceneParser_hpp

#include <stdio.h>

#include "Scene.hpp"
#include <string>
#include <iostream>

class SceneParser {
private:
    vec3<double> readInVector(std::ifstream& infile);
    Object* readInSphere(std::string objDescription, std::ifstream& infile);
    Object* readInTriangle(std::string objDescription, std::ifstream& infile);

public:
    Scene parseFile(std::string inputFilePath);
};

#endif /* SceneParser_hpp */
