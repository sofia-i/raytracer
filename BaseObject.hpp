//
//  BaseObject.hpp
//  raytracer_2
//
//  Created by Sofia Iannicelli on 3/4/23.
//

#ifndef Object_hpp
#define Object_hpp

#include <memory>
#include <cstdio>
#include <sstream>
#include <string>
#include <utility>
#include "vec3.hpp"
#include "Ray.hpp"
#include "Material.h"

class BaseObject {
public:
    BaseObject(const std::shared_ptr<Material>& material, std::string description) :
                material(material), description(std::move(description)) { }

    virtual ~BaseObject() = default; // I. destructor
    BaseObject(const BaseObject& other) = default; // II. copy constructor
    BaseObject& operator=(const BaseObject& other) = default; // III. copy assignment
    BaseObject(BaseObject&& other) noexcept = default;// IV. move constructor
    BaseObject& operator=(BaseObject&& other) noexcept = default; // V. move assignment

    virtual double findRayObjectIntersection(Ray ray) = 0;
    virtual double findRayObjectIntersection(Ray ray, vec3<double>& intersectNormal, bool& backFace) = 0;

    std::shared_ptr<Material> mat() const { return material; }
    std::shared_ptr<Material> getMaterial() const { return material; }
    std::string getDescription() const { return description; }

    friend std::ostream& operator<<(std::ostream& os, const BaseObject& obj) {
        os << obj.toString();
        return os;
    }
    
    virtual std::string toString() const {
        std::string s;
        std::stringstream ss(s);
        
        // ss << getDescription() << std::endl;
        ss << material->toString() << std::endl;
        
        return ss.str();
    }

protected:
    std::shared_ptr<Material> material;
    std::string description;
    
};

#endif /* Object_hpp */
