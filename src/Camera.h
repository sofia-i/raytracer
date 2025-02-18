//
//  Camera.h
//  raytracer_2
//
//  Created by Sofia Iannicelli on 2/15/23.
//

#ifndef Camera_h
#define Camera_h

#include "vec3.hpp"
#include <cmath>

// #define PI 3.14159265

class Camera {
public:
    Camera(vec3<double> lookAt, vec3<double> lookFrom, vec3<double> lookUp, double fov) :
            lookAt(lookAt), lookFrom(lookFrom), lookUp(lookUp),
            fovX(fov), fovY(fov) { }
    
    Camera(vec3<double> lookAt, vec3<double> lookFrom, vec3<double> lookUp, double fovX, double fovY) :
        lookAt(lookAt), lookFrom(lookFrom), lookUp(lookUp), fovX(fovX), fovY(fovY) { }

    Camera() : Camera(
            vec3<double>(0.0, 0.0, 0.0),  // at
            vec3<double>(0.0, 0.0, 1.0),  // from
            vec3<double>(0.0, 1.0, 0.0),  // up
            90.0) {}

    void setLookAt(vec3<double> newLookAt) { this->lookAt = newLookAt; }
    vec3<double> getLookAt() { return lookAt; }

    void setLookFrom(vec3<double> newLookFrom) { this->lookFrom = newLookFrom; }
    vec3<double> getLookFrom() { return lookFrom; }

    void setCameraLookUp(vec3<double> newLookUp) { this->lookUp = newLookUp; }
    vec3<double> getCameraLookUp() { return lookUp; }

    void setFovX(double newFovX) { this->fovX = newFovX; }
    void setFovY(double newFovY) { this->fovY = newFovY; }
    double getFovX() { return fovX; }
    double getFovY() { return fovY; }
    double getFovXRad() { return fovX * M_PI / 180; }
    double getFovYRad() { return fovY * M_PI / 180; }

    friend std::ostream& operator<<(std::ostream& os, Camera const &camera) {
        os << "Camera" << std::endl;
        os << "\t" << "Looking At " << camera.lookAt << std::endl;
        os << "\t" << "Looking From " << camera.lookFrom << std::endl;
        os << "\t" << "Look Up " << camera.lookUp << std::endl;
        os << "\t" << "Field of View: x " << camera.fovX << " y " << camera.fovY << std::endl;
        return os;
    }

private:
    vec3<double> lookAt;
    vec3<double> lookFrom;
    vec3<double> lookUp;
    double fovX;
    double fovY;
};


#endif /* Camera_h */
