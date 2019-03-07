#pragma once
#include "../../opengl_includes.hpp"

static const GLchar* colour_vertex_shader_source =
        "#version 100                           \n"
        "                                       \n"
        "attribute vec3 position;               \n"
        "attribute vec2 texCoord;               \n"
        "                                       \n"
        "uniform mat4 model;                    \n"
        "uniform mat4 view;                     \n"
        "uniform mat4 projection;               \n"
        "uniform float pointSize;"
        "                                       \n"
        "void main() {                          \n"
        "   gl_Position =  projection * view * model * vec4(position, 1.0);  \n"
        "   gl_PointSize = pointSize;                \n"
        "}                                      \n";


static const GLchar* colour_fragment_shader_source =
        "#version 100                                               \n"
        "                                                           \n"
        "precision mediump float;                                   \n"
        "                                                           \n"
        "uniform vec4 colour;                                       \n"
        "                                                           \n"
        "void main() {                                              \n"
        "   gl_FragColor = colour;                                  \n"
        "}                                                          \n";
