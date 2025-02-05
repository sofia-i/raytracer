//
//  SceneParser.cpp
//  raytracer_2
//
//  Created by Sofia Iannicelli on 2/15/23.
//

#include "SceneParser.hpp"

#include <fstream>
#include <vector>
#include <cassert>
#include "Sphere.h"
#include "Triangle.hpp"
#include "Cylinder.h"
#include "vec3.hpp"

SceneParser::SceneParser() {
    strToElement["CameraLookAt"] = CAMERA_LOOK_AT;
    strToElement["CameraLookFrom"] = CAMERA_LOOK_FROM;
    strToElement["CameraLookUp"] = CAMERA_LOOK_UP;
    strToElement["FieldOfView"] = FIELD_OF_VIEW;
    strToElement["AmbientLight"] = AMBIENT_LIGHT;
    strToElement["DirectionalLight"] = DIRECTIONAL_LIGHT;
    strToElement["PointLight"] = POINT_LIGHT;
    strToElement["AreaLight"] = AREA_LIGHT;
    strToElement["BackgroundColor"] = BACKGROUND_COLOR;
    strToElement["Sphere"] = SPHERE;
    strToElement["Triangle"] = TRIANGLE;
    strToElement["Material"] = MATERIAL;
    strToElement["RefractiveMaterial"] = REFRACTIVE_MATERIAL;
    strToElement["Cylinder"] = CYLINDER;

    elemToStr[CAMERA_LOOK_AT] = "CameraLookAt";
    elemToStr[CAMERA_LOOK_FROM] = "CameraLookFrom";
    elemToStr[CAMERA_LOOK_UP] = "CameraLookUp";
    elemToStr[FIELD_OF_VIEW] = "FieldOfView";
    elemToStr[AMBIENT_LIGHT] = "AmbientLight";
    elemToStr[DIRECTIONAL_LIGHT] = "DirectionalLight";
    elemToStr[POINT_LIGHT] = "PointLight";
    elemToStr[AREA_LIGHT] = "AreaLight";
    elemToStr[BACKGROUND_COLOR] = "BackgroundColor";
    elemToStr[SPHERE] = "Sphere";
    elemToStr[TRIANGLE] = "Triangle";
    elemToStr[MATERIAL] = "Material";
    elemToStr[REFRACTIVE_MATERIAL] = "RefractiveMaterial";
    elemToStr[CYLINDER] = "Cylinder";
}

vec3<double> SceneParser::readInVector(std::ifstream& infile){
    double x; double y; double z;
    infile >> x;
    infile >> y;
    infile >> z;
    return {x, y, z};
}

Scene SceneParser::parseFile(const std::string& input_file_path) {
    std::ifstream infile;
    infile.open(input_file_path);
    if(!infile.is_open()) {
        std::cerr << "failed to open input file" << std::endl;
    }

    SceneElement requiredElem[] = {CAMERA_LOOK_AT, CAMERA_LOOK_FROM, CAMERA_LOOK_UP,
                                 FIELD_OF_VIEW, AMBIENT_LIGHT, BACKGROUND_COLOR};
    std::unordered_map<SceneElement, bool> requiredElemFound;
    for(SceneElement elem : requiredElem) {
        requiredElemFound[elem] = false;
    }

    std::string description;
    SceneElement elem;

    vec3<double> camera_look_at;
    vec3<double> camera_look_from;
    vec3<double> camera_look_up;
    double fov;
    vec3<double> ambient_light;
    vec3<double> background_color;
    std::vector<std::shared_ptr<Geometry>> geo;
    std::vector<std::shared_ptr<Light>> lights;
    std::vector<std::shared_ptr<Material>> materials;

    while(infile >> description) {
        if(description == "#") {
            std::cerr << "throwing away comment" << std::endl;
            std::getline(infile, description);
            continue;
        }
        if(!strToElement.count(description)) {
            throw std::invalid_argument("didn't recognize " + description);
        }
        elem = strToElement[description];
        if(requiredElemFound.count(elem)) {
            requiredElemFound[elem] = true;
        }
        switch(elem) {
            case CAMERA_LOOK_AT: {
                camera_look_at  = readInVector(infile);
                break;
            }
            case CAMERA_LOOK_FROM: {
                camera_look_from = readInVector(infile);
                break;
            }
            case CAMERA_LOOK_UP: {
                camera_look_up = readInVector(infile);
                break;
            }
            case FIELD_OF_VIEW: {
                infile >> fov;
                break;
            }
            case AMBIENT_LIGHT: {
                ambient_light = readInVector(infile);
                break;
            }
            case DIRECTIONAL_LIGHT: {
                lights.push_back(std::move(readInDirectionalLight(infile)));
                break;
            }
            case POINT_LIGHT: {
                lights.push_back(readInPointLight(infile));
                break;
            }
            case AREA_LIGHT: {
                lights.push_back(readInAreaLight(infile));
                break;
            }
            case BACKGROUND_COLOR: {
                background_color = readInVector(infile);
                break;
            }
            case MATERIAL: {
                materials.push_back(std::move(readInMaterial(infile)));
                break;
            }
            case REFRACTIVE_MATERIAL: {
                materials.push_back(std::move(readInRefractiveMaterial(infile)));
                break;
            }
            case SPHERE: {
                std::string obj_description;
                geo.push_back(std::move(readInSphere(obj_description, infile, materials)));
                break;
            }
            case TRIANGLE: {
                std::string obj_description;
                geo.push_back(std::move(readInTriangle(obj_description, infile, materials)));
                break;
            }
            case CYLINDER: {
                std::string obj_description;
                geo.push_back(std::move(readInCylinder(obj_description, infile, materials)));
                break;
            }
            default:
                break;
        }
    }

    infile.close();

    for(SceneElement rElem : requiredElem) {
        if(!requiredElemFound[rElem]) {
            throw std::runtime_error("Invalid input: " + elemToStr[elem] + " missing.");
        }
    }

    Camera camera = Camera(camera_look_at, camera_look_from, camera_look_up, fov);

    // create scene
    Scene scene = Scene(camera, ambient_light, background_color);
    for(auto & g : geo) {
        scene.geo.push_back(std::move(g));
    }

    for(auto & light : lights) {
        scene.lights.push_back(std::move(light));
    }

    return scene;
}

std::shared_ptr<Geometry> SceneParser::readInSphere(const std::string& obj_description, std::ifstream& infile,
                                                      const std::vector<std::shared_ptr<Material>>& mats) {
    std::string description;
    infile >> description;
    vec3<double> center = readInVector(infile);

    infile >> description;
    double radius;
    infile >> radius;

    infile >> description;
    int matIdx;
    infile >> matIdx;

    if(matIdx < 0 || matIdx >= mats.size()) {
        std::cerr << "Material index " << matIdx << " invalid. Defaulting to 0." << std::endl;
        matIdx = 0;
    }

    // create sphere
    return std::make_shared<Sphere>(center, radius, mats[matIdx], obj_description);
}

std::shared_ptr<Geometry> SceneParser::readInTriangle(const std::string& obj_description, std::ifstream& infile,
                                                        const std::vector<std::shared_ptr<Material>>& mats) {
    std::string description;
    
    // take in the vertices
    std::vector<vec3<double>> vertices;
    for(int i = 0; i < 3; ++i) {
        vec3<double> vertex = readInVector(infile);
        vertices.push_back(vertex);
    }

    infile >> description;
    int matIdx;
    infile >> matIdx;

    if(matIdx < 0 || matIdx >= mats.size()) {
        std::cerr << "Material index " << matIdx << " invalid. Defaulting to 0." << std::endl;
        matIdx = 0;
    }

    // create triangle
    return std::make_shared<Triangle>(vertices, mats[matIdx], obj_description);
}

std::shared_ptr<Geometry> SceneParser::readInCylinder(const std::string& obj_description, std::ifstream& infile,
                                                        const std::vector<std::shared_ptr<Material>>& mats) {
    std::string description;

    vec3<double> capCenter1 = readInVector(infile);
    vec3<double> capCenter2 = readInVector(infile);

    infile >> description;
    double radius;
    infile >> radius;

    infile >> description;
    int matIdx;
    infile >> matIdx;

    // create cylinder
    return std::make_shared<Cylinder>(capCenter1, capCenter2, radius, mats[matIdx], obj_description);
}

std::shared_ptr<Light> SceneParser::readInDirectionalLight(std::ifstream& infile) {
    std::string description;
    infile >> description;
    vec3<double> light_color = readInVector(infile);
    infile >> description;
    vec3<double> to_light = readInVector(infile);

    return std::make_shared<DirectionalLight>(light_color, to_light);
}

std::shared_ptr<Light> SceneParser::readInPointLight(std::ifstream& infile) {
    std::string description;
    infile >> description;
    vec3<double> light_color = readInVector(infile);
    infile >> description;
    vec3<double> position = readInVector(infile);

    return std::make_shared<PointLight>(light_color, position);
}

std::shared_ptr<Light> SceneParser::readInAreaLight(std::ifstream& infile) {
    std::string description;

    infile >> description;
    vec3<double> light_color = readInVector(infile);

    infile >> description;
    vec3<double> center = readInVector(infile);

    infile >> description;
    vec3<double> aim = readInVector(infile);

    infile >> description;
    vec3<double> up = readInVector(infile);

    infile >> description;
    double width;
    infile >> width;

    infile >> description;
    double height;
    infile >> height;

    infile >> description;
    double resolution;
    infile >> resolution;

    return std::make_shared<AreaLight>(light_color, center, aim, up, width, height, resolution);
}

std::shared_ptr<Material> SceneParser::readInMaterial(std::ifstream& infile) {
    std::string description;

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
    vec3<double> color = readInVector(infile);

    infile >> description;
    vec3<double> specular = readInVector(infile);

    infile >> description;
    double kgls;
    infile >> kgls;

    infile >> description;
    double refl;
    infile >> refl;

    infile >> description;
    double rJitter;
    infile >> rJitter;

    infile >> description;
    double tJitter;
    infile >> tJitter;

    // create material
    return std::make_shared<Material>(kd, ks, ka, kgls, color, specular,
                                      refl, rJitter, tJitter);
}

std::shared_ptr<Material> SceneParser::readInRefractiveMaterial(std::ifstream& infile) {
    std::string description;

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
    vec3<double> color = readInVector(infile);

    infile >> description;
    vec3<double> specular = readInVector(infile);

    infile >> description;
    double kgls;
    infile >> kgls;

    infile >> description;
    double refl;
    infile >> refl;

    infile >> description;
    double rJitter;
    infile >> rJitter;

    infile >> description;
    double tJitter;
    infile >> tJitter;

    infile >> description;
    double ior;
    infile >> ior;

    infile >> description;
    double refractionK;
    infile >> refractionK;

    return std::make_shared<Material>(kd, ks, ka, kgls, color, specular,
                                      refl, rJitter, tJitter, ior, refractionK);
}
