//
//  Scene.hpp
//  raytracer_2
//
//  Created by Sofia Iannicelli on 2/15/23.
//

#ifndef Scene_hpp
#define Scene_hpp

#include <cstdio>
#include <vector>
#include "Camera.h"
#include "Geometry.h"
#include "BoundingBox.h"
#include "BoundingVolumeHierarchy.h"
#include "Light.h"
#include "Utils.h"

class Scene {
public:
    Scene(Camera &camera, vec3<double> ambient_light, vec3<double> backgroundColor)  :
            camera(camera), ambient_light(ambient_light), backgroundColor(backgroundColor),
            ambient_ior(IndexOfRefraction::AIR) { }

    void process() {
        bvh = std::make_shared<MedianSplit>();
        bvh->constructHierarchy(geo);
    }

    Camera getCamera() { return camera; }
    vec3<double> getAmbientLight() const { return ambient_light; }
    vec3<double> getBackgroundColor() const { return backgroundColor; }
    double getAmbientIor() const { return ambient_ior; }

    std::vector<std::shared_ptr<Geometry>> geo;
    std::vector<std::shared_ptr<Light>> lights;
    std::shared_ptr<BoundingVolumeHierarchy> bvh;

    friend std::ostream& operator<<(std::ostream& os, Scene const &scene) {
        os << "Scene: " << std::endl;
        os << "\t" << scene.camera << std::endl;
        os << "\tAmbient Light: " << scene.ambient_light << std::endl;
        os << "\tBackground Color: " << scene.backgroundColor << std::endl;
        os << std::endl;
        for(auto&& light: scene.lights) {
            os << "\t" << *light << std::endl;
        }
        for(auto&& g : scene.geo) {
            os << "\tGeo: " << g->toString() << std::endl;
        }
        return os;
    }

    Camera camera;

private:
    vec3<double> ambient_light;
    vec3<double> backgroundColor;

    double ambient_ior;  // index of refraction

};

#endif /* Scene_hpp */
