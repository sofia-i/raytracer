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
#include <sstream>

enum SceneElement {
    CAMERA_LOOK_AT,
    CAMERA_LOOK_FROM,
    CAMERA_LOOK_UP,
    FIELD_OF_VIEW,
    AMBIENT_LIGHT,
    DIRECTIONAL_LIGHT,
    POINT_LIGHT,
    AREA_LIGHT,
    BACKGROUND_COLOR,
    SPHERE,
    TRIANGLE,
    CYLINDER,
    MATERIAL,
    REFRACTIVE_MATERIAL
};

enum MaterialElement {
    DIFFUSE_K,
    SPECULAR_K,
    AMBIENT_K,
    DIFFUSE,
    DIFFUSE_COLOR,
    DIFFUSE_TEXTURE,
    SPECULAR_COLOR,
    GLS_K,
    REFLECTION_K,
    REFLECTION_JITTER,
    TRANSMISSION_K,
    TRANSMISSION_JITTER,
    IOR
};

struct MaterialParser {
    static std::unordered_map<std::string, MaterialElement> strToElem;
    static std::unordered_map<MaterialElement, std::string> elemToString;

    static bool findElem(std::string str, MaterialElement& elem);
};

class SceneParser {
public:
    SceneParser();

    Scene parseFile(const std::string& inputFilePath);

private:
    std::unordered_map<std::string, SceneElement> strToElement;
    std::unordered_map<SceneElement, std::string> elemToStr;

    static vec2<double> readInVec2(std::ifstream& infile);
    static vec2<double> readInVec2(std::stringstream& instream);
    static vec3<double> readInVector(std::ifstream& infile);
    static vec3<double> readInVector(std::stringstream& instream);
    std::shared_ptr<Geometry> readInSphere(const std::string& obj_description, std::ifstream& infile,
                                             const std::vector<std::shared_ptr<Material>>& mats);
    std::shared_ptr<Geometry> readInTriangle(const std::string& obj_description, std::ifstream& infile,
                                               const std::vector<std::shared_ptr<Material>>& mats);
    std::shared_ptr<Geometry> readInCylinder(const std::string& obj_description, std::ifstream& infile,
                                               const std::vector<std::shared_ptr<Material>>& mats);
    std::shared_ptr<Material> readInMaterial(std::ifstream& infile);
    std::shared_ptr<Light> readInDirectionalLight(std::ifstream& infile);
    std::shared_ptr<Light> readInPointLight(std::ifstream& infile);
    std::shared_ptr<Light> readInAreaLight(std::ifstream& infile);

    bool hasAllRequired(std::vector<MaterialElement> required, std::vector<MaterialElement> included,
                        std::string& message);
};

#endif /* SceneParser_hpp */
