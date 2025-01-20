//
//  BaseObject.hpp
//  raytracer_2
//
//  Created by Sofia Iannicelli on 3/4/23.
//

#ifndef Object_hpp
#define Object_hpp

#include <cstdio>
#include <sstream>
#include <string>
#include <utility>
#include "vec3.hpp"
#include "Ray.hpp"

struct Material {
    Material(double diffuseK, double specularK, double ambientK, double glsK,
             vec3<double> diffuseColor, vec3<double> specularColor, double refl) :
                diffuseK(diffuseK), specularK(specularK), ambientK(ambientK), glsK(glsK),
                diffuseColor(diffuseColor), specularColor(specularColor), refl(refl) {}

    double getDiffuseK() const { return diffuseK; }
    double getSpecularK() const { return specularK; }
    double getAmbientK() const { return ambientK; }
    vec3<double> getDiffuseColor() const { return diffuseColor; }
    vec3<double> getSpecularColor() const { return specularColor; }
    double getGlsK() const { return glsK; }
    double getRefl() const { return refl; }

private:
    double diffuseK;  // diffuse coefficient
    double specularK;  // specular coefficient
    double ambientK;  // ambient coefficient
    double glsK;  // gloss coefficient
    vec3<double> diffuseColor;
    vec3<double> specularColor;
    double refl;
};

class BaseObject {
public:
    virtual double findRayObjectIntersection(Ray ray) = 0;
    virtual vec3<double> getIntersectionNormal(vec3<double> intersectionPoint) = 0;

public:
    BaseObject(double diffuseK, double specularK, double ambientK, double glsK,
               vec3<double> diffuseColor, vec3<double> specularColor, double refl, std::string description) :
                diffuseK(diffuseK), specularK(specularK), ambientK(ambientK), glsK(glsK),
                diffuseColor(diffuseColor), specularColor(specularColor),
                description(std::move(description)), refl(refl), refractive(false) { }
    BaseObject(double diffuseK, double specularK, double ambientK, double glsK,
               vec3<double> diffuseColor, vec3<double> specularColor,
               double refl, double ior, double refractionK,
               std::string description) :
            diffuseK(diffuseK), specularK(specularK), ambientK(ambientK), glsK(glsK),
            diffuseColor(diffuseColor), specularColor(specularColor),
            description(std::move(description)), refl(refl), refractive(true), ior(ior), refractionK(refractionK) { }

    virtual ~BaseObject() = default; // I. destructor
    BaseObject(const BaseObject& other) = default; // II. copy constructor
    BaseObject& operator=(const BaseObject& other) = default; // III. copy assignment
    BaseObject(BaseObject&& other) noexcept = default;// IV. move constructor
    BaseObject& operator=(BaseObject&& other) noexcept = default; // V. move assignment

    double getDiffuseK() const { return diffuseK; }
    double getSpecularK() const { return specularK; }
    double getAmbientK() const { return ambientK; }
    vec3<double> getDiffuseColor() const { return diffuseColor; }
    vec3<double> getSpecularColor() const { return specularColor; }
    double getGlsK() const { return glsK; }
    double getRefl() const { return refl; }
    std::string getDescription() const { return description; }
    bool getIsRefractive() const { return refractive; }
    double getIOR() const { return ior; }
    double getRefractionK() const { return refractionK; }

    friend std::ostream& operator<<(std::ostream& os, const BaseObject& obj) {
        os << obj.toString();
        return os;
    }
    
    virtual std::string toString() const {
        std::string s;
        std::stringstream ss(s);
        
        // ss << getDescription() << std::endl;
        ss << "\tKd: " << getDiffuseK() << std::endl;
        ss << "\tKs: " << getSpecularK() << std::endl;
        ss << "\tKa: " << getAmbientK() << std::endl;
        ss << "\tObject Color: " << getDiffuseColor() << std::endl;
        ss << "\tObject specular: " << getSpecularColor() << std::endl;
        ss << "\tKgls: " << getGlsK() << std::endl;
        ss << "\tReflectivity: " << getRefl() << std::endl;
        
        return ss.str();
    }

protected:
    double diffuseK;  // diffuse coefficient
    double specularK;  // specular coefficient
    double ambientK;  // ambient coefficient
    double glsK;  // gloss coefficient
    vec3<double> diffuseColor;
    vec3<double> specularColor;
    std::string description;
    double refl;

    bool refractive;
    double ior;
    double refractionK;
    
};

#endif /* Object_hpp */
