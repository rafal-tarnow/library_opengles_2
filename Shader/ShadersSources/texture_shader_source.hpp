#pragma once
#include "../../opengl_includes.hpp"

static const GLchar* texture_vertex_shader_source =
        "#version 100                           \n"
        "                                       \n"
        "attribute vec3 position;               \n"
        "attribute vec2 texCoord;               \n"
        "                                       \n"
        "uniform mat4 model;                    \n"
        "uniform mat4 view;                     \n"
        "uniform mat4 projection;               \n"
        "                                       \n"
        "varying vec2 v_TexCoordinate;          \n"
        "                                       \n"
        "void main() {                          \n"
        "   gl_Position =  projection * view * model * vec4(position, 1.0);  \n"
        "   v_TexCoordinate = texCoord;         \n"
        "}                                      \n";


static const GLchar* texture_fragment_shader_source =
        "#version 100                                               \n"
        "                                                           \n"
        "precision mediump float;                                   \n"
        "                                                           \n"
        "uniform sampler2D textureUnit;                             \n"
        "                                                           \n"
        "varying vec2 v_TexCoordinate;                              \n"
        "                                                           \n"
        "void main() {                                              \n"
        "   gl_FragColor = texture2D(textureUnit,v_TexCoordinate);  \n"
        "}                                                          \n";
