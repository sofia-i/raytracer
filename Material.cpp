//
// Created by Sofia Iannicelli on 1/21/25.
//

#include "Material.h"

Material::Material(double diffuseK, double specularK, double ambientK, double glsK,
         vec3<double> diffuseColor, vec3<double> specularColor, double refl) :
            diffuseK(diffuseK), specularK(specularK), ambientK(ambientK), glsK(glsK),
            diffuseColor(diffuseColor), specularColor(specularColor), refl(refl),
            refractive(false) {}

Material::Material(double diffuseK, double specularK, double ambientK, double glsK,
         vec3<double> diffuseColor, vec3<double> specularColor,
         double refl, double ior, double refractionK) :
            diffuseK(diffuseK), specularK(specularK), ambientK(ambientK), glsK(glsK),
            diffuseColor(diffuseColor), specularColor(specularColor), refl(refl),
            ior(ior), refractionK(refractionK), refractive(true) {}
