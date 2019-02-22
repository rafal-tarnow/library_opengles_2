cmake_minimum_required(VERSION 3.4.1)

project(library_opengles_2)

include_directories(
    Shader
    TextRenderer
    RectangleRenderer
    TextureManager
    RenderableObjects
    RenderableObjects/SimpleObjects
    )

add_library (library_opengles_2 STATIC
    TextRenderer/TextRenderer_v1.cpp
    TextRenderer/TextRenderer_v2.cpp
    TextRenderer/stb_rect_pack.cpp
    RectangleRenderer/Rectangle_Renderer.cpp
    RectangleRenderer/Primitive_Renderer.cpp
    RectangleRenderer/TriangleStrip_Renderer.cpp
    TextureManager/texture_manager.cpp
    ShaderManager/shader_manager.cpp
    RenderableObjects/GLSLShader.cpp
    RenderableObjects/RenderableObject_gles2.cpp
    RenderableObjects/SimpleObjects/GridLines.cpp
    Resources/Resources.cpp
    Debug/Debug.cpp
    )

