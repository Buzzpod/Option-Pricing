#include <stdio.h>
#include <GLFW/glfw3.h>
#define IMGUI_IMPL_OPENGL_LOADER_GL3W
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Option.hpp"
#include "AsianOption.hpp"
#include "PricingEngine.hpp"

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible.\n");
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize ImGui
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    // GUI variables
    double S = 100.0;
    double K = 100.0;
    double r = 0.05;
    double T = 1.0;
    double sigma = 0.2;
    double U = 0.0;
    double L = 0.0;
    int t = 0;

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Text("Hello, world!");

        ImGui::SliderFloat("Initial stock price", &S, 0.0f, 200.0f);
        ImGui::SliderFloat("Strike price", &K, 0.0f, 200.0f);
        ImGui::SliderFloat("Risk-free rate", &r, 0.0f, 0.1f);
        ImGui::SliderFloat("Time to maturity", &T, 0.0f, 5.0f);
        ImGui::SliderFloat("Volatility", &sigma, 0.0f, 1.0f);
        ImGui::SliderFloat("Upper barrier", &U, 0.0f, 200.0f);
        ImGui::SliderFloat("Lower barrier", &L, 0.0f, 200.0f);
        ImGui::SliderInt("Number of time steps", &t, 0, 100);

        if (ImGui::Button("Calculate")) {
            AsianOption asianOption(S, K, r, T, sigma, U, L, t);
            PricingEngine pricingEngine;
            double price = pricingEngine.calculate(asianOption);
            ImGui::Text("Option Price: %f", price);
        }

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Destroy the window and terminate GLFW
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

