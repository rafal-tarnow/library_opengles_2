#pragma once
#include "GLSLShader.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class RenderableObject_gles2
{
public:
    RenderableObject_gles2(void);
    virtual ~RenderableObject_gles2(void);
    void Render(const float* MVP, const GLfloat * M, const GLfloat * M_IT = glm::value_ptr(glm::mat3(1)));

    virtual int GetTotalVertices()=0;
    virtual int GetTotalIndices()=0;
    virtual GLenum GetPrimitiveType() =0;
    virtual void GetVertexAttribPointers() =0;

    virtual void FillVertexBuffer(GLfloat* pBuffer)=0;
    virtual void FillIndexBuffer(GLuint* pBuffer)=0;

    virtual void SetCustomUniforms(){}

    void Init();
    void Destroy();

protected:
   // GLuint vaoID;
    GLuint vboVerticesID;
    GLuint vboIndicesID;

    GLSLShader shader;

    GLenum primType;
    int totalVertices, totalIndices;
};
