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
    vec3 origin;
    vec3 direction;
    
public:
    Ray(vec3 origin, vec3 direction) {
        this->origin = origin;
        this->direction = direction;
    }
    
    vec3 getDirection() { return direction; }
    vec3 getOrigin() { return origin; }
    vec3 getPointOnRay(double t) {
        return origin + t * direction;
    }
    
};

#endif /* Ray_hpp */
