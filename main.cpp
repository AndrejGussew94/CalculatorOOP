#include <iostream>
#include <string>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers


// repair diving on null (bug error with stod and crasjing the programm)

// make settings button
// make result below the table and make local save
// make more comfortable window



GLFWwindow* window;

static std::string display_str = "0";
static double current_value = 0.0;
static double pending_value = 0.0;
static char pending_operation = '_';
static bool is_new_number = true;

const float button_width = 80.0f;
const float button_height = 50.0f;

void process_cal_input(const char* label) {
    std::string s(label);
    if (s.length() == 1 && std::isdigit(s[0])) {
        if (is_new_number || display_str == "0") {
            display_str = s;
            is_new_number = false;
        } 
        else {
            display_str += s;
        }
    }  
    
    else if (s == "C") {
        display_str = "0";
        current_value = 0.0;
        pending_value = 0.0;
        pending_operation = '_';
        is_new_number = true;
    }

    else if (s == "CE") {
        current_value = 0.0;
        display_str = "0";
    }

    else if (s == "delete") {
        if (display_str.length() > 1) {
            display_str.pop_back();
        }
        else {
            display_str = "0";
            is_new_number = true;
        }

    }

    else if (s == ".") {
        if (is_new_number) {
            display_str = "0.";
            is_new_number = false;
        }
        else if (display_str.find('.') == std::string::npos) {
            display_str += s;
        }  
    }

    else if (s == "+" || s == "-" || s == "*" || s == "/") {
        if (pending_operation != '_') {
            // execute previous operation
        }
        pending_value = std::stod(display_str);
        pending_operation = s[0];
        is_new_number = true;
    }

    else if (s == "=") {
        if (pending_operation != '_') {
            current_value = std::stod(display_str);
            double result = pending_value;
            switch (pending_operation)
            {
            case '+':
                result += current_value;
                break;
            
            case '-':
                result -= current_value;
                break;

            case '*':
                result *= current_value;
                break;

            case '/':
                if (current_value == 0) {
                    display_str = "Cannot divide by zero";
                    pending_operation = '_';
                    is_new_number = true;
                    return;
                }
                else result /= current_value;
                break;

            }
            display_str = std::to_string(result);
            pending_operation = '_';
            is_new_number = true;
        }
    }

    else if (s == "+/-") {
        if (display_str == "0") return;
        if (display_str.find('-') == std::string::npos) display_str = '-' + display_str;
        else display_str = display_str.erase(0, 1);
    }
     


}

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

void calculator_button(const char* label) {
    if (ImGui::Button(label, ImVec2(button_width, button_height))) {
        process_cal_input(label);
    }

}

void calculator_GUI() {
    static char operation = '+';
    
    static float a = 0.0f; 
    static float b = 0.0f; 
    static float result = 0.0f;

    // glfwSetWindowSize(window, 4 * button_width, 8 * button_height);

    ImGui::SetWindowSize(ImVec2(2 * button_width, 2 * button_height));
    ImGui::SetNextWindowPos(ImVec2(10, 10));
    ImGui::Begin("Calculator");
    
    ImGui::PushItemWidth(ImGui::GetWindowWidth() - 1000);
    ImGui::InputText("##display", (char*) display_str.c_str(), display_str.size() + 1);
    ImGui::PopItemWidth();

    ImGui::Separator();

    calculator_button("delete"); ImGui::SameLine();
    calculator_button("CE"); ImGui::SameLine();
    calculator_button("C"); ImGui::SameLine();
    calculator_button("/");
    calculator_button("7"); ImGui::SameLine();
    calculator_button("8"); ImGui::SameLine();
    calculator_button("9"); ImGui::SameLine();
    calculator_button("*");
    calculator_button("4"); ImGui::SameLine();
    calculator_button("5"); ImGui::SameLine();
    calculator_button("6"); ImGui::SameLine();
    calculator_button("-");
    calculator_button("1"); ImGui::SameLine();
    calculator_button("2"); ImGui::SameLine();
    calculator_button("3"); ImGui::SameLine();
    calculator_button("+");
    calculator_button("+/-"); ImGui::SameLine();
    calculator_button("0"); ImGui::SameLine();
    calculator_button("."); ImGui::SameLine();
    calculator_button("=");

    ImGui::Text("result: %.2f %c %.2f = %.2f", a, operation, b, result);

    ImGui::End();
}

// Main code
int main(int, char**)
{
    
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only

    // Create window with graphics context
    float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor()); // Valid on GLFW 3.3+ only
    window = glfwCreateWindow((int)(1280 * main_scale), (int)(800 * main_scale), "Dear ImGui GLFW+OpenGL3 example", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    style.FontScaleDpi = main_scale;        // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for documentation purpose)

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
#ifdef __EMSCRIPTEN__
    ImGui_ImplGlfw_InstallEmscriptenCallbacks(window, "#canvas");
#endif
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details. If you like the default font but want it to scale better, consider using the 'ProggyVector' from the same author!
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
    //style.FontSizeBase = 20.0f;
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf");
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf");
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf");
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf");
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf");
    //IM_ASSERT(font != nullptr);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(1.00f, 0.55f, 0.60f, 1.00f);

    // Main loop


    while (!glfwWindowShouldClose(window))
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
       
        
        glfwPollEvents();
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        calculator_GUI();
        
        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}