#pragma once
#include "../../opengl_includes.hpp"

static const GLchar *txt_vertex_shader_src = "#version 100                               \n"
                                            "                                           \n"
                                            "attribute vec3 position;                   \n"
                                            "attribute vec2 texCoord;                   \n"
                                            "                                           \n"
                                            "                                           \n"
                                            "uniform mat4 model;                        \n"
                                            "uniform mat4 view;                         \n"
                                            "uniform mat4 projection;                   \n"
                                            "                                           \n"
                                            "varying vec2 v_TexCoordinate;              \n"
                                            "                                           \n"
                                            "void main() {                              \n"
                                            "   gl_Position = projection * view * model * vec4(position, 1.0);  \n"
                                            "   v_TexCoordinate = texCoord;             \n"
                                            "}                                          \n";


static const GLchar *txt_fragment_shader_src = "#version 100                                 \n"
                                              "                                             \n"
                                              "precision highp float;                       \n"
                                              "                                             \n"
                                              "uniform sampler2D textureMap;                \n"
                                              "uniform vec4 textColor;                      \n"
                                              "                                             \n"
                                              "varying vec2 v_TexCoordinate;                \n"
                                              "                                             \n"
                                              "void main()                                  \n"
                                              "{                                            \n"
                                              "       vec4 sampled = vec4(1.0, 1.0, 1.0, texture2D(textureMap,v_TexCoordinate).a);                \n"
                                              "       gl_FragColor = textColor * sampled;                                                         \n"
                                              "}                                                                                                  \n";
