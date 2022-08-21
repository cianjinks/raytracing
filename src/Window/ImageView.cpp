#include "ImageView.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace raytracing {

ImageView::ImageView(uint32_t window_width, uint32_t window_height,
                     uint32_t image_width, uint32_t image_height)
    : m_WindowWidth(window_width),
      m_WindowHeight(window_height),
      m_FWindowWidth(window_width),
      m_FWindowHeight(window_height),
      m_ImageWidth(image_width),
      m_ImageHeight(image_height),
      m_FImageWidth(image_width),
      m_FImageHeight(image_height) {
    /* Init GLAD. */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        RT_ASSERT(false, "Failed to initialise GLAD!");
    }

    // clang-format off
    float vertices[] = {
        // Pos                                 // Texture Coord
        0.0f,           m_FWindowHeight, 0.0f, 0.0f, 1.0f,  // Top Left
        0.0f,           0.0f,            0.0f, 0.0f, 0.0f,  // Bottom Left
        m_FWindowWidth, 0.0f,            0.0f, 1.0f, 0.0f,  // Bottom Right
        m_FWindowWidth, m_FWindowHeight, 0.0f, 1.0f, 1.0f,  // Top Right
    };
    // clang-format on

    unsigned int indices[] = {0, 1, 2, 2, 3, 0};

    GLuint vaoID;
    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);

    GLuint vboID;
    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(float), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    GLuint iboID;
    glGenBuffers(1, &iboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices,
                 GL_STATIC_DRAW);

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    GLuint programID;
    programID = glCreateProgram();

    const char *vert_src = R"(
        #version 430 core

        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec2 aTex;

        uniform mat4 u_ProjectionMatrix;

        out vec2 vTex;

        void main()
        {
            gl_Position = u_ProjectionMatrix * vec4(aPos, 1.0);
            vTex = aTex;
        }
    )";

    GLuint vert_obj = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_obj, 1, &vert_src, nullptr);
    glCompileShader(vert_obj);
    glAttachShader(programID, vert_obj);
    glDeleteShader(vert_obj);

    const char *frag_src = R"(
        #version 430 core

        in vec2 vTex;

        uniform sampler2D Texture;

        out vec4 FragColor;

        void main()
        {
            FragColor = texture(Texture, vTex);
        }
    )";

    GLuint frag_obj = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_obj, 1, &frag_src, nullptr);
    glCompileShader(frag_obj);
    glAttachShader(programID, frag_obj);
    glDeleteShader(frag_obj);

    glLinkProgram(programID);
    glValidateProgram(programID);
    glUseProgram(programID);

    glm::mat4 ortho =
        glm::ortho(0.0f, m_FWindowWidth, 0.0f, m_FWindowHeight, -1.0f, 1.0f);

    GLint loc = glGetUniformLocation(programID, "u_ProjectionMatrix");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(ortho));

    m_Image = new Image(m_ImageWidth, m_ImageHeight);
    m_Image->Randomize();

    RT_LOG("Image View Initialised");
}

ImageView::~ImageView() { delete m_Image; }

void ImageView::OnUpdate() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, m_ImageWidth, m_ImageHeight, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, m_Image->GetData());
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

}  // namespace raytracing