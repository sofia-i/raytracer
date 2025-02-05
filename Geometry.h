//
// Created by Sofia Iannicelli on 1/31/25.
//

#ifndef RAYTRACER_2_GEOMETRY_H
#define RAYTRACER_2_GEOMETRY_H

#include <memory>
#include <cstdio>
#include <sstream>
#include <string>
#include <utility>
#include "vec3.hpp"
#include "Ray.hpp"
#include "Hittable.h"
#include "Material.h"
#include "Extent.h"

class Geometry : public Hittable {
public:
    Geometry(const std::shared_ptr<Material>& material, std::string description) :
            material(material), description(std::move(description)) { }

    virtual ~Geometry() = default; // I. destructor
    Geometry(const Geometry& other) = default; // II. copy constructor
    Geometry& operator=(const Geometry& other) = default; // III. copy assignment
    Geometry(Geometry&& other) noexcept = default;// IV. move constructor
    Geometry& operator=(Geometry&& other) noexcept = default; // V. move assignment

    virtual Extent findExtent() = 0;

    std::shared_ptr<Material> mat() const { return material; }
    std::shared_ptr<Material> getMaterial() const { return material; }
    std::string getDescription() const { return description; }

    friend std::ostream& operator<<(std::ostream& os, const Geometry& obj) {
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


#endif //RAYTRACER_2_GEOMETRY_H
