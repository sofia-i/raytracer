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
#include <array>
#include "Sphere.h"
#include "Triangle.hpp"
#include "Cylinder.h"
#include "vec3.hpp"

std::unordered_map<std::string, MaterialElement> MaterialParser::strToElem = {
        {"Kd", DIFFUSE_K},
        {"Ks", SPECULAR_K},
        {"Ka", AMBIENT_K},
        {"Od", DIFFUSE_COLOR},
        {"Os", SPECULAR_COLOR},
        {"Kgls", GLS_K},
        {"Refl", REFLECTION_K},
        {"rJitter", REFLECTION_JITTER},
        {"Kt", TRANSMISSION_K},
        {"kt", TRANSMISSION_K},
        {"tJitter", TRANSMISSION_JITTER},
        {"ior", IOR}
};
std::unordered_map<MaterialElement, std::string> MaterialParser::elemToString = {
        {DIFFUSE_K, "Kd"},
        {SPECULAR_K, "Ks"},
        {AMBIENT_K, "Ka"},
        {DIFFUSE_COLOR, "Od"},
        {SPECULAR_COLOR, "Os"},
        {GLS_K, "Kgls"},
        {REFLECTION_K, "Refl"},
        {REFLECTION_JITTER, "rJitter"},
        {TRANSMISSION_K, "Kt"},
        {TRANSMISSION_K, "kt"},
        {TRANSMISSION_JITTER, "tJitter"},
        {IOR, "ior"}
};

bool MaterialParser::findElem(std::string str, MaterialElement& elem) {
    auto result = strToElem.find(str);
    if(result == strToElem.end()) {
        return false;
    }

    elem = result->second;
    return true;
}

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

vec3<double> SceneParser::readInVector(std::stringstream &instream) {
    double x, y, z;
    instream >> x;
    instream >> y;
    instream >> z;
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

    std::string line;
    std::stringstream ss;
    while(getline(infile, line)) {
        if(line.empty()) {
            continue;
        }
        // clear string stream
        ss.clear();
        ss.str(std::string());
        // Load the line into string stream
        ss << line;
        ss >> description;

        // check if the line is a comment (starts with #)
        if(description == "#") {
            continue;
        }

        // make sure the description is a recognized scene element
        if(!strToElement.count(description)) {
            throw std::invalid_argument("didn't recognize " + description);
        }

        // Get the scene element to parse
        elem = strToElement[description];
        if(requiredElemFound.count(elem)) {
            requiredElemFound[elem] = true;
        }
        // Parse the element
        switch(elem) {
            case CAMERA_LOOK_AT: {
                camera_look_at  = readInVector(ss);
                break;
            }
            case CAMERA_LOOK_FROM: {
                camera_look_from = readInVector(ss);
                break;
            }
            case CAMERA_LOOK_UP: {
                camera_look_up = readInVector(ss);
                break;
            }
            case FIELD_OF_VIEW: {
                ss >> fov;
                break;
            }
            case AMBIENT_LIGHT: {
                ambient_light = readInVector(ss);
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
                background_color = readInVector(ss);
                break;
            }
            case MATERIAL: case REFRACTIVE_MATERIAL: {
                materials.push_back(std::move(readInMaterial(infile)));
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

    // Make sure all of the required elements were input
    for(SceneElement rElem : requiredElem) {
        if(!requiredElemFound[rElem]) {
            throw std::runtime_error("Invalid input: " + elemToStr[rElem] + " missing.");
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

    scene.process();
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

    std::vector<MaterialElement> required = {DIFFUSE_K, SPECULAR_K, AMBIENT_K,
                                  DIFFUSE_COLOR, SPECULAR_COLOR,
                                  GLS_K, REFLECTION_K};
    std::vector<MaterialElement> included;

    double kd, ks, ka, kgls, refl, ior;
    vec3<double> diffuseColor, specularColor;
    // variables with default values
    double rJitter = 0;
    double kRefraction = 0.0;
    double tJitter = 0;

    std::string identifier;
    std::string line;
    std::stringstream ss;

    while(true) {
        getline(infile, line);
        // if you find a blank line, stop reading
        if(line.empty()) {
            break;
        }

        // Load the line into a stream
        ss.clear();
        ss << line;
        ss >> identifier;

        // Find the element associated with the identifier in the input
        MaterialElement elem;
        bool valid = MaterialParser::findElem(identifier, elem);
        if(!valid) {
            throw std::invalid_argument("invalid material specifier: " + identifier);
        }

        included.push_back(elem);
        // Parse the element
        switch(elem) {
            case DIFFUSE_K : {
                ss >> kd;
                break;
            }
            case SPECULAR_K : {
                ss >> ks;
                break;
            }
            case AMBIENT_K : {
                ss >> ka;
                break;
            }
            case DIFFUSE_COLOR : {
                diffuseColor = readInVector(ss);
                break;
            }
            case SPECULAR_COLOR : {
                specularColor = readInVector(ss);
                break;
            }
            case GLS_K : {
                ss >> kgls;
                break;
            }
            case REFLECTION_K : {
                ss >> refl;
                break;
            }
            case REFLECTION_JITTER : {
                ss >> rJitter;
                break;
            }
            case TRANSMISSION_K : {
                ss >> kRefraction;
                break;
            }
            case TRANSMISSION_JITTER : {
                ss >> tJitter;
                break;
            }
            case IOR : {
                ss >> ior;
                break;
            }
            default : {
                break;
            }
        };
    }

    if(kRefraction > 0.0) {
        required.push_back(IOR);
    }

    // Make sure all required elements were included
    std::string message;
    if(!hasAllRequired(required, included, message)) {
        throw std::invalid_argument("Missing required material information." + message);
    }

    // create material
    return std::make_shared<Material>(kd, ks, ka, kgls, diffuseColor, specularColor,
                                      refl, rJitter, tJitter, ior, kRefraction);
}

bool SceneParser::hasAllRequired(std::vector<MaterialElement> required, std::vector<MaterialElement> included,
                                 std::string& message) {
    for(MaterialElement r : required) {
        if(std::find(included.begin(), included.end(), r) == included.end()) {
            message = "Missing " + MaterialParser::elemToString[r];
            return false;
        }
    }
    return true;
}
