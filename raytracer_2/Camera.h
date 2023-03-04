//
//  Camera.h
//  raytracer_2
//
//  Created by Sofia Iannicelli on 2/15/23.
//

#ifndef Camera_h
#define Camera_h

#include "vec3.hpp"

#define PI 3.14159265

class Camera {
private:
    vec3<double> cameraLookAt;
    vec3<double> cameraLookFrom;
    vec3<double> cameraLookUp;
    double fovX;
    double fovY;

public:
    Camera(vec3<double> camLookAt, vec3<double> camLookFrom, vec3<double> camLookUp, double fov) {
        this->cameraLookAt = camLookAt;
        this->cameraLookFrom = camLookFrom;
        this->cameraLookUp = camLookUp;
        this->fovX = fov;
        this->fovY = fov;
    }
    
    Camera(vec3<double> camLookAt, vec3<double> camLookFrom, vec3<double> camLookUp, double fovX, double fovY) {
        this->cameraLookAt = camLookAt;
        this->cameraLookFrom = camLookFrom;
        this->cameraLookUp = camLookUp;
        this->fovX = fovX;
        this->fovY = fovY;
    }

    Camera() : Camera(
            vec3<double>(0.0, 0.0, 0.0),
            vec3<double>(0.0, 0.0, 1.0),
            vec3<double>(0.0, 1.0, 0.0),
            90.0) {}

    void setCameraLookAt(vec3<double> camLookAt) { cameraLookAt = camLookAt; }
    vec3<double> getCameraLookAt() { return cameraLookAt; }

    void setCameraLookFrom(vec3<double> camLookFrom) { cameraLookFrom = camLookFrom; }
    vec3<double> getCameraLookFrom() { return cameraLookFrom; }

    void setCameraLookUp(vec3<double> camLookUp) { cameraLookUp = camLookUp; }
    vec3<double> getCameraLookUp() { return cameraLookUp; }

    void setFOVx(double fovX) { this->fovX = fovX; }
    void setFOVy(double fovY) { this->fovY = fovY; }
    double getFOVx() { return fovX; }
    double getFOVy() { return fovY; }
    double getFOVxRad() { return fovX * PI / 180; }
    double getFOVyRad() { return fovY * PI / 180; }

    friend std::ostream& operator<<(std::ostream& os, Camera const &camera) {
        os << "Camera" << std::endl;
        os << "Camera Looking At " << camera.cameraLookAt << std::endl;
        os << "Camera Looking From " << camera.cameraLookFrom << std::endl;
        os << "Camera Look Up " << camera.cameraLookUp << std::endl;
        os << "Field of View: x " << camera.fovX << " y " << camera.fovY << std::endl;
        return os;
    }
};


#endif /* Camera_h */
