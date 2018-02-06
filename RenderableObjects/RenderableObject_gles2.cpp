#include "RenderableObject_gles2.h"
#include <glm/glm.hpp>


RenderableObject_gles2::RenderableObject_gles2(void)
{

}


RenderableObject_gles2::~RenderableObject_gles2(void)
{
    Destroy();
}

void RenderableObject_gles2::Init() {
    glGenBuffers(1, &vboVerticesID);
    glGenBuffers(1, &vboIndicesID);

    //pobranie liczby wierzchołów i indeksów
    totalVertices = GetTotalVertices();
    totalIndices  = GetTotalIndices();
    primType      = GetPrimitiveType();


    glBindBuffer (GL_ARRAY_BUFFER, vboVerticesID);
    {
        GLfloat * pBuffer = (GLfloat*)(new uint8_t [totalVertices * sizeof(glm::vec3)]);
        FillVertexBuffer(pBuffer);
        glBufferData (GL_ARRAY_BUFFER, totalVertices * sizeof(glm::vec3), pBuffer, GL_STATIC_DRAW);
        delete[] pBuffer;

    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GetVertexAttribPointers();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID);
    {
        GLuint * pIBuffer = (GLuint*)(new GLuint [totalIndices]);
        FillIndexBuffer(pIBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, totalIndices * sizeof(GLuint), pIBuffer, GL_STATIC_DRAW);
        delete[] pIBuffer;
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void RenderableObject_gles2::Destroy()
{
    shader.DeleteShaderProgram();

    glDeleteBuffers(1, &vboVerticesID);
    glDeleteBuffers(1, &vboIndicesID);
}


void RenderableObject_gles2::Render(const GLfloat* MVP, const GLfloat* M , const GLfloat * M_IT) {
    shader.Use();
    {
        glUniformMatrix4fv(shader("MVP"), 1, GL_FALSE, MVP);
        glUniformMatrix4fv(shader("M"), 1, GL_FALSE, M);
        glUniformMatrix3fv(shader("M_IT"), 1, GL_FALSE, M_IT);

        SetCustomUniforms();
        //glBindVertexArray(vaoID);

        glBindBuffer(GL_ARRAY_BUFFER, vboVerticesID);
        {

            GetVertexAttribPointers();
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID);


            glDrawElements(primType, totalIndices, GL_UNSIGNED_INT, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);

    }
    shader.UnUse();
}
