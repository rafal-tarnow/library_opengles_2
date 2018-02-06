#include "GridLines.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

using namespace std;

CGridLines::CGridLines(float left, float right, float top, float bottom)
{
    m_top = top;
    m_bottom = bottom;
    m_left = left;
    m_right = right;

    if((m_right <= m_left) || (m_top <= m_bottom))
    {
        cout << "[ERROR][CGridLines | CGridLines] wrong parametes!!" << endl;
        exit(EXIT_FAILURE);
    }

    number_of_vertical_lines = (unsigned int)((m_right - m_left)/1.0f) + 1;
    number_of_horizontal_lines = (unsigned int)((m_top - m_bottom)/1.0f) + 1;

    //przygotowanie programu shaderowego
    shader.LoadFromFile(GL_VERTEX_SHADER, "data/shadery/color_line_shader.vert");
    shader.LoadFromFile(GL_FRAGMENT_SHADER, "data/shadery/color_line_shader.frag");

    shader.CreateAndLinkProgram();
    shader.Use();
    {
        shader.AddAttribute("vColour");
        shader.AddAttribute("vVertex");

        shader.AddUniform("MVP");
        shader.AddUniform("M");
        shader.AddUniform("M_IT");
    }
    shader.UnUse();

    Init();
}


CGridLines::~CGridLines(void)
{

}

void CGridLines::SetCustomUniforms()
{
    //cout << "CColouredLine::SetCustomUniforms()" << endl;
    //glUniform4fv(objectColor_location, 1, (const GLfloat *)glm::value_ptr(m_color));
}

void CGridLines::GetVertexAttribPointers()
{
    glVertexAttribPointer(shader["vVertex"], 3, GL_FLOAT, GL_FALSE,6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(shader["vVertex"]);

    glVertexAttribPointer(shader["vColour"], 3, GL_FLOAT, GL_FALSE,6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(shader["vColour"]);
}


int CGridLines::GetTotalVertices() {
    return number_of_vertical_lines * 4 + number_of_horizontal_lines * 4; //dla max vertices[7] = 8
}

int CGridLines::GetTotalIndices() {
    return number_of_vertical_lines * 2 + number_of_horizontal_lines * 2;
}

GLenum CGridLines::GetPrimitiveType() {
    return GL_LINES;
}

void CGridLines::FillVertexBuffer(GLfloat* pBuffer) {
    glm::vec3* vertices = (glm::vec3*)(pBuffer);


    float x = m_left;
    for(unsigned int i = 0; i < number_of_vertical_lines; i++)
    {
        //0
        vertices[i*4+0] = glm::vec3(x, m_top, 0); //vertex position
        vertices[i*4+1] = glm::vec3(0,1,0);                 //vertex colour

        //1
        vertices[i*4+2] = glm::vec3(x, m_bottom, 0); //vertex position
        vertices[i*4+3] = glm::vec3(0,1,0);                 //vertex colour

        x += 1.0;
    }

    float y = m_top;
    for(unsigned int i = number_of_vertical_lines; i < number_of_horizontal_lines + number_of_vertical_lines; i++)
    {
        //0
        vertices[i*4+0] = glm::vec3(m_left, y, 0); //vertex position
        vertices[i*4+1] = glm::vec3(0,1,0);                 //vertex colour

        //1
        vertices[i*4+2] = glm::vec3(m_right, y, 0); //vertex position
        vertices[i*4+3] = glm::vec3(0,1,0);                 //vertex colour

        y -= 1.0;
    }
}

void CGridLines::FillIndexBuffer(GLuint* pBuffer) {

    //wypełnienie tablicy indeksów
    GLuint* id=pBuffer;

    for(unsigned int i= 0; i < 2*number_of_vertical_lines + 2*number_of_horizontal_lines; i++)
    {
        *id++ = i;
    }


}
