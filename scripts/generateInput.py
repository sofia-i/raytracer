import random


def create_camera_info(lookAt, lookFrom, lookUp, fov):
    info = "CameraLookAt " + " ".join(str(l) for l in lookAt) + "\n"
    info += "CameraLookFrom " + " ".join(str(l) for l in lookFrom) + "\n"
    info += "CameraLookUp " + " ".join(str(l) for l in lookUp) + "\n"
    info += "FieldOfView " + str(fov) + "\n"
    info += "\n"
    return info

def create_ambient_info(ambientLight, backgroundColor):
    info = "AmbientLight " + " ".join(str(a) for a in ambientLight) + "\n"
    info += "BackgroundColor " + " ".join(str(b) for b in backgroundColor) + "\n"
    info += "\n"
    return info

def create_directional_light(color, direction, comment=None):
    light_str = ""
    if comment is not None:
        light_str += "# " + comment + "\n"
    light_str += "DirectionalLight\n"
    light_str += "\t" + "LightColor " + " ".join([str(c) for c in color]) + "\n"
    light_str += "\t" + "DirectionToLight " + " ".join([str(d) for d in direction]) + "\n"
    light_str += "\n"
    return light_str

def create_point_light(color, position):
    light_str = "DirectionalLight\n"
    light_str += "\t" + "LightColor " + " ".join([str(c) for c in color]) + "\n"
    light_str += "\t" + "Position " + " ".join([str(p) for p in position]) + "\n"
    light_str += "\n"
    return light_str

def create_area_light(color, center, aim, up, width, height, resolution):
    light = "AreaLight\n"
    light += "\t" + "LightColor " + " ".join(str(c) for c in color) + "\n"
    light += "\t" + "Center " + " ".join(str(c) for c in center) + "\n"
    light += "\t" + "Aim " + " ".join(str(a) for a in aim) + "\n"
    light += "\t" + "Up " + " ".join(str(u) for u in up) + "\n"
    light += "\t" + "Width " + str(width) + "\n"
    light += "\t" + "Height " + str(height) + "\n"
    light += "\t" + "Resolution " + str(resolution) + "\n"
    light += "\n"
    return light

def create_material(kd, ks, ka, od, os, kgls, refl, rJitter, tJitter):
    mat = "Material\n"
    mat += "\t" + "Kd" + " " + str(kd) + "\n"
    mat += "\t" + "Ks" + " " + str(ks) + "\n"
    mat += "\t" + "Ka" + " " + str(ka) + "\n"
    mat += "\t" + "Od" + " " + " ".join([str(d) for d in od]) + "\n"
    mat += "\t" + "Os" + " " + " ".join([str(d) for d in os]) + "\n"
    mat += "\t" + "Kgls" + " " + str(kgls) + "\n"
    mat += "\t" + "Refl" + " " + str(refl) + "\n"
    mat += "\t" + "rJitter" + " " + str(rJitter) + "\n"
    mat += "\t" + "tJitter" + " " + str(tJitter) + "\n"
    mat += "\n"

    return mat

def create_refractive_material(kd, ks, ka, od, os, kgls, refl, rJitter, tJitter, ior, kt):
    mat = "RefractiveMaterial\n"
    mat += "\t" + "Kd" + " " + str(kd) + "\n"
    mat += "\t" + "Ks" + " " + str(ks) + "\n"
    mat += "\t" + "Ka" + " " + str(ka) + "\n"
    mat += "\t" + "Od" + " " + " ".join([str(d) for d in od]) + "\n"
    mat += "\t" + "Os" + " " + " ".join([str(d) for d in os]) + "\n"
    mat += "\t" + "Kgls" + " " + str(kgls) + "\n"
    mat += "\t" + "Refl" + " " + str(refl) + "\n"
    mat += "\t" + "rJitter" + " " + str(rJitter) + "\n"
    mat += "\t" + "tJitter" + " " + str(tJitter) + "\n"
    mat += "\t" + "ior" + " " + str(ior) + "\n"
    mat += "\t" + "kt" + " " + str(kt) + "\n"
    mat += "\n"

    return mat

def create_sphere(center, radius, material):
    sphere = "Sphere" + "\n"
    sphere += "\t" + "Center " + " ".join(str(c) for c in center) + "\n"
    sphere += "\t" + "Radius " + str(radius) + "\n"
    sphere += "\t" + "Material " + str(material) + "\n"
    sphere += "\n"
    return sphere

def create_cylinder(center1, center2, radius, material):
    cylinder = "Cylinder" + "\n"
    cylinder += "\t" + " ".join(str(c) for c in center1) + "\n"
    cylinder += "\t" + " ".join(str(c) for c in center2) + "\n"
    cylinder += "\t" + "Radius " + str(radius)
    cylinder += "\t" + "Material " + str(material)
    cylinder += "\n"
    return cylinder

def create_triangle(p1, p2, p3, material):
    tri = "Triangle" + "\n"
    tri += "\t" + " ".join(str(p) for p in p1) + "\n"
    tri += "\t" + " ".join(str(p) for p in p2) + "\n"
    tri += "\t" + " ".join(str(p) for p in p3) + "\n"
    tri += "\t" + "Material " + str(material) + "\n"
    tri += "\n"
    return tri

def get_random_num_in_range(min, max):
    return random.uniform(min, max)

def get_random_vec_in_range(min, max):
    vec = [0, 0, 0]
    if isinstance(min, list) and isinstance(max, list):
        vec = [random.uniform(min[i], max[i]) for i in range(len(min))]
    else:
        vec = [random.uniform(min, max) for i in range(3)]
    return vec

num_materials = 25
num_spheres = 10000

with(open("inputs/temp_generated.txt", "w") as f):
    f.write(create_camera_info([0,0,0], [0,0,2], [0,1,0], 45))
    f.write(create_ambient_info([0.05, 0.05, 0.05], [0.2, 0.2, 0.2]))

    f.write(create_point_light([1, 1, 1], [0, 1, .5]))
    f.write(create_directional_light([1, 1, 1], [1, 1, 1]))
    
    for i in range(num_materials):
        f.write(create_material(0.4, 0.4, 0.1, 
                                get_random_vec_in_range(0, 1),
                                [1, 1, 1],
                                16, 0, 0, 0))
    
    for i in range(num_spheres):
        f.write(create_sphere(get_random_vec_in_range(-3, 1),  # center
                              get_random_num_in_range(0.01, 0.07),  # radius
                              random.randint(0, num_materials - 1)))
        # f.write(create_sphere([-2 + 0.1 * (i // 10), -2 + 0.5 * (i % 10), -(i % 20)],
        #                      0.2, 0))

