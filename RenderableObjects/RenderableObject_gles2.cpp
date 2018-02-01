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
    //przygotowanie VAO i VBO
    //glGenVertexArrays(1, &vaoID);
    glGenBuffers(1, &vboVerticesID);
    glGenBuffers(1, &vboIndicesID);

    //pobranie liczby wierzchołów i indeksów
    totalVertices = GetTotalVertices();
    totalIndices  = GetTotalIndices();
    primType      = GetPrimitiveType();

    //alokacja buforów
    //glBindVertexArray(vaoID);
    //{
    glBindBuffer (GL_ARRAY_BUFFER, vboVerticesID);
    glBufferData (GL_ARRAY_BUFFER, totalVertices * sizeof(glm::vec3), 0, GL_STATIC_DRAW);

    {
        //    GLfloat* pBuffer = static_cast<GLfloat*>(glMapBufferRange(GL_ARRAY_BUFFER, 0, totalVertices * sizeof(glm::vec3), GL_MAP_WRITE_BIT));
        //    FillVertexBuffer(pBuffer);
        //    glUnmapBuffer(GL_ARRAY_BUFFER);

        GLfloat * pBuffer = (GLfloat*)(new uint8_t [totalVertices * sizeof(glm::vec3)]);
        FillVertexBuffer(pBuffer);
        delete[] pBuffer;

    }
    GetVertexAttribPointers();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, totalIndices * sizeof(GLuint), 0, GL_STATIC_DRAW);

    {
        //    GLuint* pIBuffer = static_cast<GLuint*>(glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, totalIndices * sizeof(GLuint), GL_MAP_WRITE_BIT));
        //    FillIndexBuffer(pIBuffer);
        //    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

        GLuint * pIBuffer = (GLuint*)(new GLuint [totalIndices]);
        FillIndexBuffer(pIBuffer);
        delete[] pIBuffer;
    }

    //}
    //glBindVertexArray(0);
}

void RenderableObject_gles2::Destroy() {
    //likwidacja programu shaderowego
    shader.DeleteShaderProgram();

    //likwidacja vao i vbo
    glDeleteBuffers(1, &vboVerticesID);
    glDeleteBuffers(1, &vboIndicesID);
    // glDeleteVertexArrays(1, &vaoID);
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
