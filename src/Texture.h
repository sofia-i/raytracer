//
// Created by Sofia Iannicelli on 2/19/25.
//

#ifndef RAYTRACER_2_TEXTURE_H
#define RAYTRACER_2_TEXTURE_H

#include "vec3.hpp"
#include "Image.h"

class Texture {
public:
    virtual vec3<double> getIntensity(double u, double v, const vec3<double>& point) const = 0;

    virtual std::string toString() const = 0;

    friend std::ostream& operator<<(std::ostream& os, const Texture& tex) {
        os << tex.toString();
        return os;
    }
};

class ColorTexture : public Texture {
public:
    explicit ColorTexture(vec3<double> color) : color(color) { }

    vec3<double> getIntensity(double u, double v, const vec3<double>& point) const override {
        return color;
    }

    std::string toString() const override {
        return color.toString();
    }

private:
    vec3<double> color;
};

class ImageTexture : public Texture {
public:
    explicit ImageTexture(const std::string& filename) {
        image = Image::getLoadedImage(filename);
    }

    vec3<double> getIntensity(double u, double v, const vec3<double>& point) const override {
        return image->getPixelValue(u, v);
    }

    std::string toString() const override {
        return "image texture";
    }

private:
    std::shared_ptr<Image> image;
};


#endif //RAYTRACER_2_TEXTURE_H
