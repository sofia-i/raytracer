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
    vec3 cameraLookAt;
    vec3 cameraLookFrom;
    vec3 cameraLookUp;
    double fieldOfView;

public:
    Camera(vec3 camLookAt, vec3 camLookFrom, vec3 camLookUp, double fov) {
        this->cameraLookAt = camLookAt;
        this->cameraLookFrom = camLookFrom;
        this->cameraLookUp = camLookUp;
        this->fieldOfView = fov;
    }

    Camera() : Camera(
            vec3(0.0, 0.0, 0.0),
            vec3(0.0, 0.0, 1.0),
            vec3(0.0, 1.0, 0.0),
            90.0) {}

    void setCameraLookAt(vec3 camLookAt) { cameraLookAt = camLookAt; }
    vec3 getCameraLookAt() { return cameraLookAt; }

    void setCameraLookFrom(vec3 camLookFrom) { cameraLookFrom = camLookFrom; }
    vec3 getCameraLookFrom() { return cameraLookFrom; }

    void setCameraLookUp(vec3 camLookUp) { cameraLookUp = camLookUp; }
    vec3 getCameraLookUp() { return cameraLookUp; }

    void setFOV(double fov) { fieldOfView = fov; }
    double getFOV() { return fieldOfView; }
    double getFOVRad() { return fieldOfView * PI / 180; }

    friend std::ostream& operator<<(std::ostream& os, Camera const &camera) {
        os << "Camera" << std::endl;
        os << "Camera Looking At " << camera.cameraLookAt << std::endl;
        os << "Camera Looking From " << camera.cameraLookFrom << std::endl;
        os << "Camera Look Up " << camera.cameraLookUp << std::endl;
        os << "Field of View " << camera.fieldOfView << std::endl;
        return os;
    }
};


#endif /* Camera_h */
