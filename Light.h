//
// Created by Sofia Iannicelli on 1/15/25.
//

#ifndef RAYTRACER_2_LIGHT_H
#define RAYTRACER_2_LIGHT_H

#include "vec3.hpp"
#include <float.h>

class Light {
public:
    explicit Light(vec3<double> lightColor) : lightColor(lightColor), shadowRayCount(1) {}
    Light(vec3<double> lightColor, int shadowRayCount) : lightColor(lightColor),
                                                         shadowRayCount(shadowRayCount) {}

    virtual ~Light() = default; // I. destructor
    Light(const Light& other) = default; // II. copy constructor
    Light& operator=(const Light& other) = default; // III. copy assignment
    Light(Light&& other) noexcept = default;// IV. move constructor
    Light& operator=(Light&& other) noexcept = default; // V. move assignment

    virtual vec3<double> getDirectionToLight(vec3<double> point) const = 0;
    virtual void getPathToLight(const vec3<double>& point, int index, bool& hit, vec3<double>& direction,
                                double& distance) const = 0;

    vec3<double> getLightColor() const { return lightColor; }

    virtual std::string toString() const = 0;

    friend std::ostream& operator<<(std::ostream& os, const Light& light) {
        os << light.toString();
        return os;
    }

    const int shadowRayCount;

private:
    vec3<double> lightColor;

};

class DirectionalLight : public Light{
public:
    DirectionalLight(vec3<double> lightColor, vec3<double> directionToLight) :
            Light(lightColor), directionToLight(directionToLight) { }

    ~DirectionalLight() override = default;  // I. destructor
    DirectionalLight(const DirectionalLight& other) = default;  // II. copy constructor
    DirectionalLight& operator=(const DirectionalLight& other) = default;  // III. copy assignment
    DirectionalLight(DirectionalLight&& other) noexcept = default;  // IV. move constructor
    DirectionalLight& operator=(DirectionalLight&& other) noexcept = default; // V. move assignment

    vec3<double> getDirectionToLight(vec3<double> point) const override {
        return getUnitVector(directionToLight);
    }

    void getPathToLight(const vec3<double>& point, int index, bool& hit, vec3<double>& direction,
                        double& distance) const override {
        hit = true;
        direction = directionToLight;
        distance = DBL_MAX;
    }

    std::string toString() const override {
        std::string str = "Directional Light\n";
        str += "\tcolor: " + this->getLightColor().toString() + "\n";
        str += "\tDirection to light: " + directionToLight.toString() + "\n";
        return str;
    }

private:
    vec3<double> directionToLight;

};

class PointLight : public Light {
public:
    PointLight(vec3<double> lightColor, vec3<double> position) : Light(lightColor),
            position(position) { }

    ~PointLight() override = default;
    PointLight(const PointLight& other) = default;  // II. copy constructor
    PointLight& operator=(const PointLight& other) = default;  // III. copy assignment
    PointLight(PointLight&& other) noexcept = default;  // IV. move constructor
    PointLight& operator=(PointLight&& other) noexcept = default; // V. move assignment

    vec3<double> getDirectionToLight(vec3<double> point) const override {
        return getUnitVector(position - point);
    }

    void getPathToLight(const vec3<double>& point, int index, bool& hit, vec3<double>& direction,
                        double& distance) const override {
        vec3<double> vectorToLight = position - point;
        hit = true;
        distance = vectorToLight.length();
        direction = getUnitVector(vectorToLight);
    }

    std::string toString() const override {
        std::string str = "Point Light\n";
        str += "\tcolor: " + this->getLightColor().toString() + "\n";
        str += "\tPosition: " + position.toString() + "\n";
        return str;
    }

private:
    vec3<double> position;

};

class AreaLight : public Light {
public:
    AreaLight(vec3<double> lightColor,
              vec3<double> center, vec3<double> aim, vec3<double> up,
              double width, double height, double sampleResolution) :
            Light(lightColor, int(width/sampleResolution) * int(height/sampleResolution)),
            center(center),
            wSampleCount(int(width/sampleResolution)), 
            hSampleCount(int(height/sampleResolution)) {
        normal = getUnitVector(aim - center);
        uAxis = getUnitVector(cross(getUnitVector(up), this->normal));
        vAxis = cross(this->normal, uAxis);
        blPos = center - (width / 2.) * uAxis - (height / 2. * vAxis);

        uInc = width/wSampleCount;
        vInc = height/hSampleCount;
    }

    ~AreaLight() override = default;
    AreaLight(const AreaLight& other) = default;  // II. copy constructor
    AreaLight& operator=(const AreaLight& other) = default;  // III. copy assignment
    AreaLight(AreaLight&& other) noexcept = default;  // IV. move constructor
    AreaLight& operator=(AreaLight&& other) noexcept = default; // V. move assignment

    vec3<double> getDirectionToLight(vec3<double> point) const override {
        return getUnitVector(center - point);
    }

    void getPathToLight(const vec3<double>& point, int index, bool& hit, vec3<double>& direction,
                        double& distance) const override {
        int vIdx = index / hSampleCount;
        int uIdx = index % hSampleCount;
        vec3<double> lightPt = blPos + (uIdx * uInc * uAxis) + (vIdx * vInc * vAxis);
        vec3<double> vectorToLight = lightPt - point;

        // check back face
        if(dot(normal, vectorToLight) > 0) {
            hit = false;
            return;
        }

        distance = vectorToLight.length();
        direction = getUnitVector(vectorToLight);
    }

    std::string toString() const override {
        std::string str = "Area Light\n";
        str += "\tColor: " + this->getLightColor().toString() + "\n";
        str += "\tCenter: " + center.toString() + "\n";
        return str;
    }

private:
    vec3<double> center;

    vec3<double> uAxis;
    vec3<double> vAxis;
    vec3<double> normal;

    vec3<double> blPos;  // bottom left position

    int wSampleCount;
    int hSampleCount;

    double uInc;
    double vInc;
};


#endif //RAYTRACER_2_LIGHT_H
