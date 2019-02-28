#pragma once
#include "../../opengl_includes.hpp"

static const GLchar* colour_vertex_shader_source =
        "#version 100                           \n"
        "//Rectangle_Renderer vertex shader     \n"
        "attribute vec3 position;               \n"
        "attribute vec2 texCoord;               \n"
        "                                       \n"
        "uniform mat4 model;                    \n"
        "uniform mat4 view;                     \n"
        "uniform mat4 projection;               \n"
        "                                       \n"
        "void main() {                          \n"
        "   gl_Position =  projection * view * model * vec4(position, 1.0);  \n"
        "}                                      \n";


static const GLchar* colour_fragment_shader_source =
        "#version 100                                               \n"
        "//Rectangle_Renderer fragmet shader                        \n"
        "precision mediump float;                                   \n"
        "                                                           \n"
        "void main() {                                              \n"
        "   gl_FragColor = vec4(1.0,0.0,0.0,1.0);    \n"
        "}                                                          \n";
