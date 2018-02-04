INCLUDEPATH += $$PWD/..
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/TextRenderer/TextRenderer_v1.hpp \
    $$PWD/TextRenderer/TextRenderer_v2.hpp \
    $$PWD/RectangleRenderer/Rectangle_Renderer.hpp \
    $$PWD/RectangleRenderer/LineStrip_Renderer.hpp \
    $$PWD/TextureManager/texture_manager.hpp \
    $$PWD/RenderableObjects/RenderableObject_gles2.h \
    $$PWD/RenderableObjects/GLSLShader.h \
    $$PWD/RenderableObjects/SimpleObjects/GridLines.hpp

SOURCES += \
    $$PWD/TextRenderer/TextRenderer_v1.cpp \
    $$PWD/TextRenderer/TextRenderer_v2.cpp \
    $$PWD/RectangleRenderer/Rectangle_Renderer.cpp \
    $$PWD/RectangleRenderer/LineStrip_Renderer.cpp \
    $$PWD/TextureManager/texture_manager.cpp \
    $$PWD/RenderableObjects/GLSLShader.cpp \
    $$PWD/RenderableObjects/RenderableObject_gles2.cpp \
    $$PWD/RenderableObjects/SimpleObjects/GridLines.cpp

