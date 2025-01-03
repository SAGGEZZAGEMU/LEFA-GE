#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>

/*
 * WebGL Water
 * http://madebyevan.com/webgl-water/
 *
 * Copyright 2011 Evan Wallace
 * Released under the MIT license
 */

class Cubemap {
public:
    Cubemap(const std::map<std::string, GLuint>& images) {
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_CUBE_MAP, id);
        glPixelStorei(GL_UNPACK_FLIP_Y_WEBGL, 1);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, reinterpret_cast<const void*>(images.at("xneg")));
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, reinterpret_cast<const void*>(images.at("xpos")));
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, reinterpret_cast<const void*>(images.at("yneg")));
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, reinterpret_cast<const void*>(images.at("ypos")));
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, reinterpret_cast<const void*>(images.at("zneg")));
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, reinterpret_cast<const void*>(images.at("zpos")));
    }

    void bind(GLuint unit = 0) {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_CUBE_MAP, id);
    }

    void unbind(GLuint unit = 0) {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

private:
    GLuint id;
};