//
// Created by Sofia Iannicelli on 1/15/25.
//

#ifndef RAYTRACER_2_LIGHT_H
#define RAYTRACER_2_LIGHT_H

#include "vec3.hpp"

class Light {
public:
    explicit Light(vec3<double> lightColor) : lightColor(lightColor) {}

    virtual ~Light() = default; // I. destructor
    Light(const Light& other) = default; // II. copy constructor
    Light& operator=(const Light& other) = default; // III. copy assignment
    Light(Light&& other) noexcept = default;// IV. move constructor
    Light& operator=(Light&& other) noexcept = default; // V. move assignment

    virtual vec3<double> getDirectionToLight(vec3<double> point) const = 0;

    vec3<double> getLightColor() const { return lightColor; }

    virtual std::string toString() const = 0;

    friend std::ostream& operator<<(std::ostream& os, const Light& light) {
        os << light.toString();
        return os;
    }

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

    std::string toString() const override {
        std::string str = "Point Light\n";
        str += "\tcolor: " + this->getLightColor().toString() + "\n";
        str += "\tPosition: " + position.toString() + "\n";
        return str;
    }

private:
    vec3<double> position;

};


#endif //RAYTRACER_2_LIGHT_H
