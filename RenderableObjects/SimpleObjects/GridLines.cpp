#include "GridLines.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

using namespace std;

static const GLchar * color_line_shader_vertex =
    "#version 310 es            \n"
    "                           \n"
    "layout(location = 0) in vec3 vVertex;	//położenie w przestrzeni obiektu \n"
    "layout(location = 1) in vec3 vColour;  \n"
    "                                       \n"
    "                                       \n"
    "out vec3 Colour;                       \n"
    "                                       \n"
    "//uniform                              \n"
    "uniform mat4 MVP;	//połączona macierz modelu, widoku i rzutowania \n"
    "uniform mat4 M;                        \n"
    "uniform mat3 M_IT; //matrix without translations   \n"
    "                                       \n"
    "void main()                            \n"
    "{                                      \n"
    "Colour = vColour;                      \n"
    "gl_Position = MVP*vec4(vVertex, 1.0);  \n"
    "}                                      \n"
;

static const GLchar * color_line_shader_fragment =
    "#version 310 es                \n"
    "precision mediump float;       \n"
    "                               \n"
    "//layout(location = 0) out vec4 vFragColor;	//wyjściowy kolor fragmentu \n"
    "                               \n"
    "out vec4 fragmentColor;        \n"
    "                               \n"
    "in vec3 Colour;                \n"
    "                               \n"
    "void main()                    \n"
    "{                              \n"
    "   fragmentColor = vec4(Colour, 1.0);  \n"
    "   //gl_FragColor = vec4(Colour, 1.0); \n"
    "   //vFragColor = vec4(Colour, 1.0);   \n"
    "}                              \n"
;



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
    shader.LoadFromCString(GL_VERTEX_SHADER, color_line_shader_vertex);
    shader.LoadFromCString(GL_FRAGMENT_SHADER, color_line_shader_fragment);

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
