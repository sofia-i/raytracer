//
//  Ray.hpp
//  raytracer_2
//
//  Created by Sofia Iannicelli on 2/16/23.
//

#ifndef Ray_hpp
#define Ray_hpp

#include <stdio.h>
#include "vec3.hpp"

class Ray {
private:
    vec3<double> origin;
    vec3<double> direction;
    
public:
    Ray(vec3<double> origin, vec3<double> direction) {
        this->origin = origin;
        this->direction = getUnitVector(direction);
    }
    
    vec3<double> getDirection() { return direction; }
    vec3<double> getOrigin() { return origin; }
    vec3<double> getPointOnRay(double t) {
        return origin + t * direction;
    }
    
};

#endif /* Ray_hpp */
