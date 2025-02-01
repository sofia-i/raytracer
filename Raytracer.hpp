//
//  Raytracer.hpp
//  raytracer_2
//
//  Created by Sofia Iannicelli on 2/15/23.
//

#ifndef Raytracer_hpp
#define Raytracer_hpp

#include <cstdio>
#include <stack>
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

struct Intersection {
    Intersection(int objIndex, double t, const vec3<double>& intersectPt, const vec3<double>& normal,
                 bool backFace) :
            objIndex(objIndex), t(t), point(intersectPt), normal(normal), backFace(backFace) {}

    int objIndex;
    double t;
    vec3<double> point;
    vec3<double> normal;
    bool backFace;
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

    /**
     *
     * @param target world-space coordinate to raytrace through
     * @return color tuple
     */
    vec3<int> getRayResult(vec3<double> target);
    vec3<int> getRayResult(Ray ray, int rayCount, std::stack<double>& iors);

    Intersection getClosestIntersection(const Ray& ray);
    double getInShadow(const vec3<double>& intersectPt, const std::shared_ptr<Light>& light);

    WorldSpaceCoord calculateWorldSpaceCoords(int numCols, int numRows);
    vec3<int> illuminationEq(int objectIdx, const vec3<double>& normal, const vec3<double>& view,
                             const vec3<double>& intersectPt);

    inline vec3<double> getAmbient(int objectIdx);
    inline vec3<double> getDiffuse(int objectIdx, const std::shared_ptr<Light>& light,
                                   const vec3<double>& normal, const vec3<double>& toLight);
    inline vec3<double> getSpecular(int objectIdx, const std::shared_ptr<Light>& light,
                                    const vec3<double>& normal, const vec3<double>& toLight,
                                    const vec3<double>& view);
    inline vec3<int> getTransmission(int objectIdx, const vec3<double>& normal, const vec3<double>& rayD,
                                     const vec3<double>& intersectPt, double iorRatio, int rayCount,
                                     std::stack<double>& iors);
    inline Ray getTransmissionRay(const int objIdx, const vec3<double>& normal, const vec3<double>& rayD,
                                  const vec3<double>& intersectPt, double iorRatio) const;
    inline vec3<int> getReflection(int objectIdx, const vec3<double>& normal, const vec3<double>& toView,
                                   const vec3<double>& intersectPt, int rayCount,
                                   std::stack<double>& iors);

    void getIorAcrossIntersection(int objIdx, bool isBackFace, double& iorIn, double& iorOut, double& iorRatio,
                                  std::stack<double>& iors);
    double getPortionReflected(const vec3<double>& normal, const vec3<double>& rayD, const double matRefl,
                               const double& iorIn, const double& iorOut, const double& iorRatio);
};

#endif /* Raytracer_hpp */
