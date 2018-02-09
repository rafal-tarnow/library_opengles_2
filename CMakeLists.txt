cmake_minimum_required(VERSION 3.4.1)

project(library_opengles_2)


include_directories(
    TextRenderer
    RectangleRenderer
    TextureManager
    RenderableObjects
    RenderableObjects/SimpleObjects
    )

add_library (library_opengles_2 STATIC
    TextRenderer/TextRenderer_v1.cpp
    TextRenderer/TextRenderer_v2.cpp
    RectangleRenderer/Rectangle_Renderer.cpp
    RectangleRenderer/LineStrip_Renderer.cpp
    TextureManager/texture_manager.cpp
    RenderableObjects/GLSLShader.cpp
    RenderableObjects/RenderableObject_gles2.cpp
    RenderableObjects/SimpleObjects/GridLines.cpp
)