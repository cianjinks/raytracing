#include "ImageView.h"

#include <glad/glad.h>
#include <nfd.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "Input.h"
#include "Window/UI.h"

namespace raytracing {

ImageView::ImageView(uint32_t window_width, uint32_t window_height,
                     uint32_t image_width, uint32_t image_height)
    : m_WindowWidth(window_width),
      m_WindowHeight(window_height),
      m_FWindowWidth((float)window_width),
      m_FWindowHeight((float)window_height),
      m_ImageWidth(image_width),
      m_ImageHeight(image_height),
      m_FImageWidth((float)image_width),
      m_FImageHeight((float)image_height) {
    RT_PROFILE_FUNC_N("ImageView Init");

    // clang-format off
    float vertices[] = {
        // Pos                               // Texture Coord
        -(m_FImageWidth / 2.0f),  (m_FImageHeight / 2.0f), 0.0f, 0.0f, 1.0f,  // Top Left
        -(m_FImageWidth / 2.0f), -(m_FImageHeight / 2.0f), 0.0f, 0.0f, 0.0f,  // Bottom Left
         (m_FImageWidth / 2.0f), -(m_FImageHeight / 2.0f), 0.0f, 1.0f, 0.0f,  // Bottom Right
         (m_FImageWidth / 2.0f),  (m_FImageHeight / 2.0f), 0.0f, 1.0f, 1.0f,  // Top Right
    };
    // clang-format on

    unsigned int indices[] = {0, 1, 2, 2, 3, 0};

    GLuint vaoID;
    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);

    GLuint vboID;
    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(float), vertices,
                 GL_DYNAMIC_DRAW);
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

    GLuint m_ShaderProgramID = glCreateProgram();

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
    glAttachShader(m_ShaderProgramID, vert_obj);
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
    glAttachShader(m_ShaderProgramID, frag_obj);
    glDeleteShader(frag_obj);

    glLinkProgram(m_ShaderProgramID);
    glValidateProgram(m_ShaderProgramID);
    glUseProgram(m_ShaderProgramID);

    m_Camera = CreateU<ImageCamera>(m_FWindowWidth, m_FWindowHeight);
    m_CameraUniformID =
        glGetUniformLocation(m_ShaderProgramID, "u_ProjectionMatrix");

    m_Image = CreateS<Image2D3u8>(m_ImageWidth, m_ImageHeight);
    m_Image->Randomize(FastRandom::GetTimeSeed());

    RT_LOG("Image View Initialised");
}

ImageView::~ImageView() {}

void ImageView::OnUpdate() {
    RT_PROFILE_FUNC;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glUniformMatrix4fv(m_CameraUniformID, 1, GL_FALSE,
                       glm::value_ptr(m_Camera->GetProjectionView()));

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, m_ImageWidth, m_ImageHeight, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, m_Image->GetData());
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    m_Camera->OnUpdate();
}

void ImageView::OnEvent(Event &event) {
    RT_PROFILE_FUNC;

    EventDispatcher dispatcher(event);

    dispatcher.Dispatch<WindowResizeEvent>(
        EventType::WINDOW_RESIZE, [this](auto &event) {
            ResizeWindow(event.GetWidth(), event.GetHeight());
        },
        false);

    dispatcher.Dispatch<KeyEvent>(
        EventType::KEY_PRESS, [this](auto &event) {
            int key = event.GetKey();
            if (key == GLFW_KEY_Z) {
                /* Center image based on height. */
                m_Camera->Center(m_FImageHeight / m_FWindowHeight);
            }
        },
        false);

    m_Camera->OnEvent(event);
}

void ImageView::UI() {
    RT_PROFILE_FUNC;

    ImGui::Text("Image Settings");
    ImGui::Text("Z: Frame Image");

    int image_dim[2] = {(int)m_ImageWidth, (int)m_ImageHeight};
    // ImGui::SliderInt("Image Width", &image_dim[0], 0, (int)m_WindowWidth);
    // ImGui::SliderInt("Image Height", &image_dim[1], 0, (int)m_WindowHeight);
    if (UI::InputInt2("Image Dimension", image_dim)) {
        ResizeImage(image_dim[0], image_dim[1]);
    }
    if (UI::ButtonND("Save Image")) {
        nfdchar_t *path;
        nfdfilteritem_t filterItem[1] = {{"Portable Network Graphics", "png"}};
        nfdresult_t result = NFD_SaveDialog(&path, filterItem, 1, nullptr, "image.png");
        if (result == NFD_OKAY) {
            m_Image->Save(path);
            NFD_FreePath(path);
        } else if (result != NFD_CANCEL) {
            RT_ERROR("Native File Dialog Error: {}", NFD_GetError());
        }
    }

    glm::uvec2 mouse_coord = GetMousePixel();
    ImGui::Text("(%d, %d)", mouse_coord.x, mouse_coord.y);
}

void ImageView::ResizeWindow(uint32_t width, uint32_t height) {
    m_WindowWidth = width;
    m_WindowHeight = height;
    m_FWindowWidth = (float)width;
    m_FWindowHeight = (float)height;
    glViewport(0, 0, m_WindowWidth, m_WindowHeight);
}

void ImageView::ResizeImage(uint32_t width, uint32_t height) {
    m_ImageWidth = width;
    m_ImageHeight = height;
    m_FImageWidth = (float)width;
    m_FImageHeight = (float)height;

    // clang-format off
    float vertices[] = {
        // Pos                               // Texture Coord
        -(m_FImageWidth / 2.0f),  (m_FImageHeight / 2.0f), 0.0f, 0.0f, 1.0f,  // Top Left
        -(m_FImageWidth / 2.0f), -(m_FImageHeight / 2.0f), 0.0f, 0.0f, 0.0f,  // Bottom Left
         (m_FImageWidth / 2.0f), -(m_FImageHeight / 2.0f), 0.0f, 1.0f, 0.0f,  // Bottom Right
         (m_FImageWidth / 2.0f),  (m_FImageHeight / 2.0f), 0.0f, 1.0f, 1.0f,  // Top Right
    };
    // clang-format on

    /* TODO: VBO is never unbound so we can do this. Probably not good though.
     */
    glBufferSubData(GL_ARRAY_BUFFER, 0, 20 * sizeof(float), vertices);

    m_Image->Resize(width, height);
    m_Image->Randomize(FastRandom::GetTimeSeed());
}

glm::uvec2 ImageView::GetMousePixel() {
    glm::dvec2 mouse_coord = Input::GetMouseCoord();

    /* Transform to NDC. */
    mouse_coord.y = m_FWindowHeight - mouse_coord.y;
    mouse_coord.x = ((mouse_coord.x * 2.0f) / m_FWindowWidth) - 1.0f;
    mouse_coord.y = ((mouse_coord.y * 2.0f) / m_FWindowHeight) - 1.0f;

    /* Inverse ProjectionView to retrieve vertex position. */
    glm::dvec4 transform = glm::inverse(m_Camera->GetProjectionView()) * glm::dvec4(mouse_coord, 0, 1);

    /* Vertex position is between -(m_ImageWidth / 2) -> (m_ImageWidth / 2), same for m_ImageHeight. */
    /* Transform into pixel coordinate. */
    mouse_coord.x = transform.x + (m_FImageWidth / 2.0f);
    mouse_coord.y = transform.y + (m_FImageHeight / 2.0f);

    return {static_cast<uint32_t>(mouse_coord.x), static_cast<uint32_t>(mouse_coord.y)};
}

}  // namespace raytracing