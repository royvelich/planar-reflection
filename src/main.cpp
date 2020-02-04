// ImGui includes
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// OpenGL includes
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <nfd.h>

// STL includes
#include <string>
#include <memory>

// Local includes
#include "material.h"
#include "point_light.h"
#include "obj_model.h"
#include "shader_program.h"
#include "camera.h"

/**
 * Defines
 */
#define VERTEX_SHADER_PATH "..//shaders//vertex.glsl"
#define FRAGMENT_SHADER_PATH "..//shaders//fragment.glsl"
#define INITIAL_WIDTH 1024
#define INITIAL_HEIGHT 768

/**
 * Main
 */
int main(int, char**)
{
	// Scene objects
    std::vector<std::unique_ptr<ObjModel>> models;
    std::vector<std::unique_ptr<PointLight>> point_lights;
    std::vector<std::unique_ptr<Camera>> cameras;
    uint32_t active_camera = 0;
    uint32_t active_light = 0;

	// OpenGL objects
    GLFWwindow* window;
    glm::vec4 clear_color(0.45f, 0.55f, 0.60f, 1.00f);
    int width = INITIAL_WIDTH;
    int height = INITIAL_HEIGHT;
	
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
    window = glfwCreateWindow(INITIAL_WIDTH, INITIAL_HEIGHT, "Planar Reflection Demo", NULL, NULL);
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
    const char* glsl_version = "#version 330";
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

	// Create shader program
    ShaderProgram shader_program(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);

	/**
	 * Create scene objects
	 */

	// Camera
	auto camera = std::make_unique<Camera>(
		glm::vec3(0,3,-5), 
		glm::vec3(0,0,0), 
		glm::vec3(0,1,0),
		float(width) / float(height),
        0.2,
        5);

	// Point light
	auto point_light = std::make_unique<PointLight>(
        camera->GetEye(),
        glm::vec3(1,1,1),
        glm::vec3(1,1,1));

	// Move scene objects to corresponding lists
    cameras.push_back(std::move(camera));
	point_lights.push_back(std::move(point_light));

    /**
     * Main loop
     */
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        /**
         * ImGui stuff
         */
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
                auto model = std::make_unique<ObjModel>(std::string(file_path_ptr));
                models.push_back(std::move(model));
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
        ImGui::Render();

		/**
		 * Scene rendering
		 */

    	// Prepare new frame
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
        glClear(GL_COLOR_BUFFER_BIT);

    	// Render ImGui
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    	// Reset aspect ratio
        cameras[active_camera]->SetAspectRatio(static_cast<float>(width) / static_cast<float>(height));

    	// Set shader program to use
        shader_program.Use();

        // Set lights
        shader_program.SetUniform("point_light.position", point_lights[active_light]->GetPosition());
        shader_program.SetUniform("point_light.ambient", point_lights[active_light]->GetAmbientLight());
        shader_program.SetUniform("point_light.diffuse", point_lights[active_light]->GetDiffuseLight());

        // Set view and projection transformations
        shader_program.SetUniform("view", cameras[active_camera]->GetViewTransform());
        shader_program.SetUniform("projection", cameras[active_camera]->GetProjectionTransform());
    	
    	for (auto& model : models)
        {
            // Set transformations
            shader_program.SetUniform("model", model->GetModelTransform());

            // Set material
            shader_program.SetUniform("material.ambient", model->GetMaterial().GetAmbientColor());
            shader_program.SetUniform("material.diffuse", model->GetMaterial().GetDiffuseColor());

            // Render model
            model->Render();
    	}

    	// Swap buffers
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