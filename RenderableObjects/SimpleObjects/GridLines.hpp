#pragma once
#include "../RenderableObject_gles2.h"
#include <glm/glm.hpp>

class CGridLines :
    public RenderableObject_gles2
{
public:
    CGridLines(float left, float right, float top, float bottom);
    virtual ~CGridLines(void);
    int GetTotalVertices();
    int GetTotalIndices();
    GLenum GetPrimitiveType();
    void GetVertexAttribPointers();


    void FillVertexBuffer( GLfloat* pBuffer);
    void FillIndexBuffer( GLuint* pBuffer);

    void SetCustomUniforms();

private:

    float m_top = 10.0f;
    float m_bottom = -20.0f;
    float m_left = -10.0f;
    float m_right = 20.0f;

    unsigned int number_of_vertical_lines = 20;
    unsigned int number_of_horizontal_lines = 20;
};
