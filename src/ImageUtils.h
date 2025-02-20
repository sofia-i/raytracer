//
// Created by Sofia Iannicelli on 2/18/25.
//

#ifndef RAYTRACER_2_IMAGEUTILS_H
#define RAYTRACER_2_IMAGEUTILS_H

/**
 * Wrapper for stbi_image loading
 */
class ImageUtils {
public:
    /**
     * Load image intensity values into array
     * @param filename file where image is stored
     * @param[out] width returns pixel width
     * @param[out] height returns pixel height
     * @param[out] channels_in_file returns total channels
     * @param desired_channels how many channels to extract
     * @return array of image values
     */
    static unsigned char *load_img(const char* filename, int *width, int *height,
                                   int *channels_in_file, int desired_channels);

};


#endif //RAYTRACER_2_IMAGEUTILS_H
