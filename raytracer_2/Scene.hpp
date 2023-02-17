//
//  Scene.hpp
//  raytracer_2
//
//  Created by Sofia Iannicelli on 2/15/23.
//

#ifndef Scene_hpp
#define Scene_hpp

#include <stdio.h>
#include <vector>
#include "Camera.h"
#include "Sphere.h"

class Scene {
private:
    Camera camera;
    vec3 directionToLight;
    vec3 lightColor;
    vec3 ambientLight;
    vec3 backgroundColor;
    std::vector<Sphere> spheres;

public:
    Scene(Camera camera, vec3 directionToLight, vec3 lightColor, vec3 ambientLight, vec3 backgroundColor,
          std::vector<Sphere> &spheres) {
        this->camera = camera;
        this->directionToLight = directionToLight;
        this->lightColor = lightColor;
        this->ambientLight = ambientLight;
        this->backgroundColor = backgroundColor;
        this->spheres = spheres;
    }
    
    Camera getCamera() { return camera; }
    vec3 getDirectionToLight() { return directionToLight; }
    vec3 getLightColor() { return lightColor; }
    vec3 getAmbientLight() { return ambientLight; }
    vec3 getBackgroundColor() { return backgroundColor; }
    std::vector<Sphere> getSpheres() { return spheres; }

    friend std::ostream& operator<<(std::ostream& os, Scene const &scene) {
        os << "Camera" << scene.camera << std::endl;
        os << "Direction to Light: " << scene.directionToLight << std::endl;
        os << "Light Color: " << scene.lightColor << std::endl;
        os << "Ambient Light: " << scene.ambientLight << std::endl;
        os << "Background Color: " << scene.backgroundColor << std::endl;
        for(int i = 0; i < scene.spheres.size(); ++i) {
            os << "Sphere: " << scene.spheres[i] << std::endl;
        }
        return os;
    }

};

#endif /* Scene_hpp */
