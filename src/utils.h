//
// Created by varomix on 8/14/22.
//

#ifndef CPPNG_MIXTUREPROTOTYPE_SRC_UTILS_H_
#define CPPNG_MIXTUREPROTOTYPE_SRC_UTILS_H_
#include <GL/glew.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>

unsigned int load_texture(const char *img_path) {
    unsigned int texture_id;
    glGenTextures(1, &texture_id);

    stbi_set_flip_vertically_on_load(true);
    int width, height, n_channels;
    unsigned char *data = stbi_load(img_path, &width, &height, &n_channels, 0);


    // load image
//    std::cout << "texture 1 channels: " << n_channels << '\n';

    // check number of channels to load images with transparency like png with alpha
    if (data) {
        GLint format;
        if (n_channels == 1)
            format = GL_RED;
        else if (n_channels == 3)
            format = GL_RGB;
        else if (n_channels == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, texture_id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cerr << "Failed to load texture\n";
        stbi_image_free(data);
    }

    return texture_id;
}
#endif //CPPNG_MIXTUREPROTOTYPE_SRC_UTILS_H_
