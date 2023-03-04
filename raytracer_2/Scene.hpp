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
    Camera* camera;
    vec3<double> directionToLight;
    vec3<double> lightColor;
    vec3<double> ambientLight;
    vec3<double> backgroundColor;
    std::vector<Sphere*> spherePtrs;

public:
    Scene() {}
    Scene(Camera* camera, vec3<double> directionToLight, vec3<double> lightColor, vec3<double> ambientLight, vec3<double> backgroundColor,
          std::vector<Sphere*> spherePtrs) {
        this->camera = camera;
        this->directionToLight = getUnitVector(directionToLight);
        this->lightColor = lightColor;
        this->ambientLight = ambientLight;
        this->backgroundColor = backgroundColor;
        this->spherePtrs = spherePtrs;
    }
    
    ~Scene() {
        for(Sphere* spherePtr: spherePtrs) {
            // delete spherePtr;
        }
    }
    
    Camera* getCamera() { return camera; }
    vec3<double> getDirectionToLight() { return directionToLight; }
    vec3<double> getLightColor() { return lightColor; }
    vec3<double> getAmbientLight() { return ambientLight; }
    vec3<double> getBackgroundColor() { return backgroundColor; }
    std::vector<Sphere*> getSpherePtrs() { return spherePtrs; }

    friend std::ostream& operator<<(std::ostream& os, Scene const &scene) {
        os << scene.camera << std::endl;
        os << "Direction to Light: " << scene.directionToLight << std::endl;
        os << "Light Color: " << scene.lightColor << std::endl;
        os << "Ambient Light: " << scene.ambientLight << std::endl;
        os << "Background Color: " << scene.backgroundColor << std::endl;
        os << std::endl;
        for(int i = 0; i < scene.spherePtrs.size(); ++i) {
            os << "Sphere: " << *scene.spherePtrs[i] << std::endl;
        }
        return os;
    }

};

#endif /* Scene_hpp */
