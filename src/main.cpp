#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <nfd.h>
#include <string>
#include "obj_model.h"
#include "shader_program.h"

/**
 * Defines
 */
#define VERTEX_SHADER_PATH "..//shaders//vertex.glsl"
#define FRAGMENT_SHADER_PATH "..//shaders//fragment.glsl"

/**
 * Function definitions
 */
void LoadModel(GLuint& vao, GLuint& vbo, GLuint& ibo, const nfdchar_t* file_path_ptr);

/**
 * Main
 */
int main(int, char**)
{
    GLFWwindow* window;
    GLuint vbo;
    GLuint ibo;
    GLuint vao;
	
    // Initialize GLFW
    if (!glfwInit())
    {
        return -1;
    }

	// Use modern OpenGL version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create window-mode rendering context
    window = glfwCreateWindow(1024, 768, "Planar Reflection Demo", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Set the previously created window as the current rendering context
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        return -1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    const char* glsl_version = "#version 130";
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Create shader program
    ShaderProgram shader_program(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Menu");
    	
        if (ImGui::Button("Load model..."))
        {
            nfdchar_t* file_path_ptr = NULL;
            nfdresult_t result = NFD_OpenDialog("obj;png,jpg", NULL, &file_path_ptr);
            if (result == NFD_OKAY)
            {
                LoadModel(vao, vbo, ibo, file_path_ptr);
            }
            else if (result == NFD_CANCEL)
            {
            	
            }
            else
            {
            	
            }
        }

        ImGui::ColorEdit3("Clear color", (float*)&clear_color);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    	
        ImGui::End();


        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());



    	
        glfwSwapBuffers(window);
    }

    /**
     * Cleanup
     */

	// Cleanup ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

	// Cleanup GLFW
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

/**
 * LoadModel
 */
void LoadModel(GLuint& vao, GLuint& vbo, GLuint& ibo, const nfdchar_t* file_path_ptr)
{
    // Cleanup previous allocated buffers
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);

    // Load obj file
    std::string file_path(file_path_ptr);
    ObjModel obj_model(file_path);
    auto& vertices = obj_model.GetVertices();
    auto& vertex_indices = obj_model.GetVertexIndices();

    // Create VBO (vertex buffer object) on GPU, and copy vertex data from CPU
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

    // Create VAO (vertex array object) and assign attributes
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    // Create IBO (index buffer object)
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex_indices), &vertex_indices[0], GL_STATIC_DRAW);

    glBindVertexArray(0);
}