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
#include "Triangle.hpp"
#include "vec3.hpp"

SceneParser::SceneParser() {
    strToElement["CameraLookAt"] = CAMERA_LOOK_AT;
    strToElement["CameraLookFrom"] = CAMERA_LOOK_FROM;
    strToElement["CameraLookUp"] = CAMERA_LOOK_UP;
    strToElement["FieldOfView"] = FIELD_OF_VIEW;
    strToElement["AmbientLight"] = AMBIENT_LIGHT;
    strToElement["DirectionalLight"] = DIRECTIONAL_LIGHT;
    strToElement["PointLight"] = POINT_LIGHT;
    strToElement["BackgroundColor"] = BACKGROUND_COLOR;
    strToElement["Sphere"] = SPHERE;
    strToElement["Triangle"] = TRIANGLE;

    elemToStr[CAMERA_LOOK_AT] = "CameraLookAt";
    elemToStr[CAMERA_LOOK_FROM] = "CameraLookFrom";
    elemToStr[CAMERA_LOOK_UP] = "CameraLookUp";
    elemToStr[FIELD_OF_VIEW] = "FieldOfView";
    elemToStr[AMBIENT_LIGHT] = "AmbientLight";
    elemToStr[DIRECTIONAL_LIGHT] = "DirectionalLight";
    elemToStr[POINT_LIGHT] = "PointLight";
    elemToStr[BACKGROUND_COLOR] = "BackgroundColor";
    elemToStr[SPHERE] = "Sphere";
    elemToStr[TRIANGLE] = "Triangle";
}

vec3<double> SceneParser::readInVector(std::ifstream& infile){
    double x; double y; double z;
    infile >> x;
    infile >> y;
    infile >> z;
    return {x, y, z};
}

Scene SceneParser::parseFile(std::string input_file_path) {
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
    std::vector<std::shared_ptr<BaseObject>> objects;
    std::vector<std::shared_ptr<Light>> lights;
    // std::vector<BaseObject*> objects;
    // std::vector<Light*> lights;

    while(infile >> description) {
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
                // lights.push_back(readInDirectionalLight(infile));
                break;
            }
            case POINT_LIGHT: {
                lights.push_back(readInPointLight(infile));
                break;
            }
            case BACKGROUND_COLOR: {
                background_color = readInVector(infile);
                break;
            }
            case SPHERE: case TRIANGLE: {
                std::string obj_description;
                std::unique_ptr<BaseObject> object;
                if(elem == SPHERE) {
                    objects.push_back(std::move(readInSphere(obj_description, infile)));
                }
                else if(elem == TRIANGLE) {
                    objects.push_back(std::move(readInTriangle(obj_description, infile)));
                }
                // objects.push_back(std::move(object));
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
    for(auto & object : objects) {
        scene.objects.push_back(std::move(object));
    }

    for(auto & light : lights) {
        scene.lights.push_back(std::move(light));
    }

    return scene;
}

std::shared_ptr<BaseObject> SceneParser::readInSphere(std::string obj_description, std::ifstream& infile) {
    std::string description;
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
    
    infile >> description;
    double refl;
    infile >> refl;

    // create sphere
    return std::make_shared<Sphere>(center, radius, kd, ks, ka, objectColor, objectSpecular,
                             kgls, refl, obj_description);
    // return std::unique_ptr<BaseObject>(new Sphere(center, radius, kd, ks, ka, objectColor, objectSpecular,
    //                                           kgls, refl, obj_description));
}

std::shared_ptr<BaseObject> SceneParser::readInTriangle(std::string obj_description, std::ifstream& infile) {
    std::string description;
    
    // take in the vertices
    std::vector<vec3<double>> vertices;
    for(int i = 0; i < 3; ++i) {
        vec3<double> vertex = readInVector(infile);
        vertices.push_back(vertex);
    }

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
    
    infile >> description;
    double refl;
    infile >> refl;

    // create triangle
    return std::make_shared<Triangle>(vertices, kd, ks, ka, objectColor, objectSpecular,
                                      kgls, refl, obj_description);
    // std::unique_ptr<BaseObject> triangle(new Triangle(vertices, kd, ks, ka, objectColor, objectSpecular,
    //                                              kgls, refl, obj_description));
    // return triangle;
}

std::shared_ptr<Light> SceneParser::readInDirectionalLight(std::ifstream& infile) {
    std::string description;
    infile >> description;
    vec3<double> light_color = readInVector(infile);
    infile >> description;
    vec3<double> to_light = readInVector(infile);

    return std::make_shared<DirectionalLight>(light_color, to_light);
    // return std::unique_ptr<Light>(new DirectionalLight(light_color, to_light));
}

std::shared_ptr<Light> SceneParser::readInPointLight(std::ifstream& infile) {
    std::string description;
    infile >> description;
    vec3<double> light_color = readInVector(infile);
    infile >> description;
    vec3<double> position = readInVector(infile);

    return std::make_shared<PointLight>(light_color, position);
    // return std::unique_ptr<Light>(new PointLight(light_color, position));
}
