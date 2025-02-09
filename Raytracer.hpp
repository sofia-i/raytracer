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
#include "Hittable.h"

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
    const bool LOG_TIME = true;
    const bool USE_BOUNDING_VOLUME = true;

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

    RayHit getClosestIntersection(const Ray& ray);
    double getInShadow(const vec3<double>& intersectPt, const std::shared_ptr<Light>& light);

    WorldSpaceCoord calculateWorldSpaceCoords(int numCols, int numRows);
    vec3<int> illuminationEq(const std::shared_ptr<Material>& mat, const vec3<double>& normal, const vec3<double>& view,
                             const vec3<double>& intersectPt);

    inline vec3<double> getAmbient(const std::shared_ptr<Material>& mat);
    inline vec3<double> getDiffuse(const std::shared_ptr<Material>& mat, const std::shared_ptr<Light>& light,
                                   const vec3<double>& normal, const vec3<double>& toLight);
    inline vec3<double> getSpecular(const std::shared_ptr<Material>& mat, const std::shared_ptr<Light>& light,
                                    const vec3<double>& normal, const vec3<double>& toLight,
                                    const vec3<double>& view);
    inline vec3<int> getTransmission(const std::shared_ptr<Material>& mat, const vec3<double>& normal,
                                     const vec3<double>& rayD, const vec3<double>& intersectPt,
                                     double iorRatio, int rayCount, std::stack<double>& iors);
    inline Ray getTransmissionRay(const std::shared_ptr<Material>& mat, const vec3<double>& normal,
                                  const vec3<double>& rayD, const vec3<double>& intersectPt, double iorRatio) const;
    inline vec3<int> getReflection(const std::shared_ptr<Material>& mat, const vec3<double>& normal,
                                   const vec3<double>& toView, const vec3<double>& intersectPt, int rayCount,
                                   std::stack<double>& iors);

    void getIorAcrossIntersection(const std::shared_ptr<Material>& mat, bool isBackFace,
                                  double& iorIn, double& iorOut, double& iorRatio,
                                  std::stack<double>& iors);
    double getPortionReflected(const vec3<double>& normal, const vec3<double>& rayD, const double matRefl,
                               const double& iorIn, const double& iorOut, const double& iorRatio);

    void showProgress(int index, int total);

};

#endif /* Raytracer_hpp */
