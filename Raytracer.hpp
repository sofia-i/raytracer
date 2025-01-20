//
//  Raytracer.hpp
//  raytracer_2
//
//  Created by Sofia Iannicelli on 2/15/23.
//

#ifndef Raytracer_hpp
#define Raytracer_hpp

#include <cstdio>
#include "Scene.hpp"
#include "Camera.h"
#include "Ray.hpp"

struct WorldSpaceCoord {
    double maxU;
    double maxV;

    double uInc;
    double vInc;

    vec3<double> uAxis;
    vec3<double> vAxis;

    WorldSpaceCoord(double maxU, double maxV, double uInc, double vInc,
                    vec3<double> uAxis, vec3<double> vAxis) :
                            maxU(maxU), maxV(maxV), uInc(uInc), vInc(vInc),
                            uAxis(uAxis), vAxis(vAxis) {}
};

class Raytracer {
public:
    explicit Raytracer(Scene scene) : scene(scene), raysPerPixelPerSide(2) { }
    Raytracer(Scene scene, int raysPerPixelPerSide) : scene(scene), raysPerPixelPerSide(raysPerPixelPerSide) { }

    ~Raytracer();
    
    int*** raytrace(int numCols, int numRows);
    
    Scene* getScene() {
        return &scene;
    }
    
    Camera getCamera() {
        return scene.getCamera();
    }

private:
    const int MAX_NUM_RAYS = 5;
    const double EPSILON = 2e-8;
    const int raysPerPixelPerSide;

    // TODO: should scene be a member?
    Scene scene;
    // int numCols;
    // int numRows;

    /**
     *
     * @param target world-space coordinate to raytrace through
     * @return color tuple
     */
    vec3<int> getRayResult(vec3<double> target);
    vec3<int> getRayResult(Ray ray, int rayCount);

    WorldSpaceCoord calculateWorldSpaceCoords(int numCols, int numRows);
    vec3<int> illuminationEq(int objectIdx, vec3<double> normal, vec3<double> view,
                             const vec3<double> intersectPt);
    
};

#endif /* Raytracer_hpp */
