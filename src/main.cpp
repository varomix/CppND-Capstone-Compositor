//
// Created by varomix on 8/12/22.
//

#include "imgui.h"
#include "shader.h"
#include "utils.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include <exception>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>

#include "api/mix_camera.h"

const unsigned int WIDTH = 1080;
const unsigned int HEIGHT = 1080;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 1.0f));

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

static void glfw_error_callback(int error, const char *description) {
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char **) {
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    const char *glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+
    //    only glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            //
    //    Required on Mac

    // Create window with graphics context
    GLFWwindow *window =
        glfwCreateWindow(WIDTH, HEIGHT, "MIXTURE by Varo Castaneda", NULL, NULL);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //@formatter:off
    // quad
    float vertices[] = {
        // positions                    // colors                   // uvs
        +1.0f, +1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // top right
        +1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // bottom right
        -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -1.0f, +1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, // top left
    };

    unsigned int indices[] = {0, 1, 3, 1, 2, 3};

    float quadVertices[] = {
            // positions         // texCoords
            -1.0f, 1.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 1.0f, 0.0f,

            -1.0f, 1.0f, 0.0f, 1.0f,
            1.0f, -1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 1.0f
        };
    //@formatter:on


    // Initialize GLEW
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW!\n");
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    // screen quad VAO
    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    //    position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)nullptr);
    glEnableVertexAttribArray(0);

    //    color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //    uvs attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // shader
    Shader buffer_shader("../src/shaders/buffer.vert",
                         "../src/shaders/buffer.frag");

    Shader comp_shader("../src/shaders/composite_ops.vert",
                       "../src/shaders/composite_ops.frag");



    // load textures to shader uniforms
//    buffer_shader.use();
//    buffer_shader.set_int("texture1", 0);

//    comp_shader.use();
//    comp_shader.set_int("back_layer", 0);
//    comp_shader.set_int("front_layer", 1);

    // framebuffers
    unsigned int comp_buffer;
    glGenFramebuffers(1, &comp_buffer);
    glBindFramebuffer(GL_FRAMEBUFFER, comp_buffer);
    // color attachment for final comp buffer
    unsigned int texture_color_buffer;
    glGenTextures(1, &texture_color_buffer);
    glBindTexture(GL_TEXTURE_2D, texture_color_buffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_color_buffer, 0);

    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER,
                          GL_DEPTH24_STENCIL8,
                          WIDTH,
                          HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                              GL_DEPTH_STENCIL_ATTACHMENT,
                              GL_RENDERBUFFER,
                              rbo); // now actually attach it
    // check the framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "ERROR::FRAMEBUFFER:: COMP Framebuffer is not complete!\n";
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // back frame buffer
    unsigned int back_buffer;
    glGenFramebuffers(1, &back_buffer);
    glBindFramebuffer(GL_FRAMEBUFFER, back_buffer);
    // attach the texture
    unsigned int texture_back = load_texture("../images/sky_02.jpg");
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_back, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "ERROR::FRAMEBUFFER:: BACK Framebuffer is not complete!\n";
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // front frame buffer
    unsigned int front_buffer;
    glGenFramebuffers(1, &front_buffer);
    glBindFramebuffer(GL_FRAMEBUFFER, front_buffer);
    // attach the texture
    unsigned int texture_front = load_texture("../images/moon.png");
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_front, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "ERROR::FRAMEBUFFER:: FRONT Framebuffer is not complete!\n";
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // load and create a texture
    unsigned int texture_back_02 = load_texture("../images/sky_01.jpg");
    unsigned int texture_front_02 = load_texture("../images/engel.png");
    unsigned int texture_front_03 = load_texture("../images/pin-up.png");

    // TRANSFORM LAYER SETTINGS
    // back layer
    //    glm::mat4 trans_back = glm::mat4(1.0);
    glm::vec3 back_transform = glm::vec3(0.5, 1.5, 0.0);
    float back_rotation{0};
    glm::vec3 back_scale = glm::vec3(1.0, 1.0, 1.0);

    glm::mat4 trans_back = glm::translate(glm::mat4(1.0), back_transform);
    //    trans_back = glm::rotate(trans_back, glm::radians(back_rotation),
    //    glm::vec3(0.0, 0.0, 1.0)); trans_back = glm::scale(trans_back,
    //    back_scale);

    // front layer
    glm::mat4 trans_front = glm::mat4(1.0f);
    glm::vec3 front_transform = glm::vec3(1.0, 0.0, 0.0);
    float front_rotation{0};
    glm::vec3 front_scale = glm::vec3(1.0, 1.0, 1.0);

    trans_front = glm::translate(trans_front, front_transform);
    trans_front = glm::rotate(trans_front, glm::radians(front_rotation),
                              glm::vec3(0.0, 0.0, 1.0));
    trans_front = glm::scale(trans_front, front_scale);

    //     Setup Dear ImGui context
    //    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableGamepad;            // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
    //    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable
    //    Multi-Viewport / Platform Windows
    io.ConfigViewportsNoAutoMerge = true;
    io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    //     When viewports are enabled we tweak WindowRounding/WindowBg so platform
    //     windows can look identical to regular ones.
    ImGuiStyle &style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    io.Fonts->AddFontFromFileTTF(
        "../src/vendor/imgui/misc/fonts/Roboto-Medium.ttf", 20.0f);

    // Our state
    bool change_back = false;
    bool open_file = false;
    bool show_demo_window = false;
    bool show_another_window = false;
    int current_op = 0;
    int front_current_index = 0;
    int back_current_index = 0;
    ImVec4 clear_color = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);

    float xpos{0};

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Properties"); // Create a window called "Hello, world!"
            // and append into it.

//            ImGui::SliderFloat3("Position", &back_transform.x, -2.0f, 2.0f);
//            ImGui::SliderFloat("Rotation", &back_rotation, 0, 360);
//            ImGui::SliderFloat("front Rotation", &front_rotation, 0, 360);

            {
                const char *front_items[] = {"Moon", "Engel", "Pin Up"};
                ImGui::Combo("Front Image", &front_current_index, front_items, IM_ARRAYSIZE(front_items));
            }

            {
                const char *back_items[] = {"Blue Sky", "Red Sky"};
                ImGui::Combo("Background Image", &back_current_index, back_items, IM_ARRAYSIZE(back_items));
            }

            {
                const char *items[] =
                    {"Over", "Add", "Subtract", "Mix", "Multiply", "Screen", "Maximum", "Minimum", "In", "Out", "Atop"};
                ImGui::Combo("Operation", &current_op, items, IM_ARRAYSIZE(items));
            }

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                        1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
                     clear_color.z * clear_color.w, clear_color.w);


        // draw back
        glBindFramebuffer(GL_FRAMEBUFFER, back_buffer);
        glDisable(GL_DEPTH_TEST);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        buffer_shader.use();

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
        buffer_shader.set_mat4("view", view);
        buffer_shader.set_mat4("projection", projection);
        model = glm::translate(model, back_transform);
        model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
//        buffer_shader.set_mat4("model", model);


        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        // draw front
        glBindFramebuffer(GL_FRAMEBUFFER, front_buffer);
        glDisable(GL_DEPTH_TEST);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        buffer_shader.use();
        model = glm::translate(model, front_transform);
//        buffer_shader.set_mat4("model", model);
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        // Draw Default COMP BUFFER
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        comp_shader.use();
        comp_shader.set_int("operation", current_op);
        glBindVertexArray(VAO);

        // use color buffer attachment
        glBindTexture(GL_TEXTURE_2D, texture_color_buffer);

        // pass buffers to comp shader
        comp_shader.set_int("back_layer", 0);
        glActiveTexture(GL_TEXTURE0);
        if (back_current_index == 0)
            glBindTexture(GL_TEXTURE_2D, back_buffer);
        else if (back_current_index == 1)
            glBindTexture(GL_TEXTURE_2D, texture_back_02);

        comp_shader.set_int("front_layer", 1);
        glActiveTexture(GL_TEXTURE1);

        if (front_current_index == 0)
            glBindTexture(GL_TEXTURE_2D, front_buffer);
        else if (front_current_index == 1)
            glBindTexture(GL_TEXTURE_2D, texture_front_02);
        else if (front_current_index == 2)
            glBindTexture(GL_TEXTURE_2D, texture_front_03);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &quadVBO);
    glDeleteBuffers(1, &EBO);

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
