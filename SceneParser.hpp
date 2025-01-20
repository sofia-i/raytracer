//
//  SceneParser.hpp
//  raytracer_2
//
//  Created by Sofia Iannicelli on 2/15/23.
//

#ifndef SceneParser_hpp
#define SceneParser_hpp

#include <cstdio>

#include "Scene.hpp"
#include <string>
#include <iostream>

enum SceneElement {
    CAMERA_LOOK_AT,
    CAMERA_LOOK_FROM,
    CAMERA_LOOK_UP,
    FIELD_OF_VIEW,
    AMBIENT_LIGHT,
    DIRECTIONAL_LIGHT,
    POINT_LIGHT,
    BACKGROUND_COLOR,
    SPHERE,
    TRIANGLE
};

class SceneParser {
public:
    SceneParser();

    Scene parseFile(std::string inputFilePath);

private:
    std::unordered_map<std::string, SceneElement> strToElement;
    std::unordered_map<SceneElement, std::string> elemToStr;

    vec3<double> readInVector(std::ifstream& infile);
    std::shared_ptr<BaseObject> readInSphere(std::string obj_description, std::ifstream& infile);
    std::shared_ptr<BaseObject> readInTriangle(std::string obj_description, std::ifstream& infile);
    std::shared_ptr<Light> readInDirectionalLight(std::ifstream& infile);
    std::shared_ptr<Light> readInPointLight(std::ifstream& infile);
};

#endif /* SceneParser_hpp */
