#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cstring>

class OESTextureFloatLinear {
public:
    OESTextureFloatLinear() {}
};

bool supportsOESTextureFloatLinear(GLFWwindow* window) {
    // Need floating point textures in the first place
    if (!glewIsSupported("GL_ARB_texture_float")) {
        return false;
    }

    // Create a render target
    GLuint framebuffer, byteTexture;
    glGenFramebuffers(1, &framebuffer);
    glGenTextures(1, &byteTexture);
    glBindTexture(GL_TEXTURE_2D, byteTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, byteTexture, 0);

    // Create a simple floating-point texture with value of 0.5 in the center
    std::vector<float> rgba = {
        2, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
    };
    GLuint floatTexture;
    glGenTextures(1, &floatTexture);
    glBindTexture(GL_TEXTURE_2D, floatTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_FLOAT, rgba.data());

    // Create the test shader
    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec2 vertex;
        void main() {
            gl_Position = vec4(vertex, 0.0, 1.0);
        }
    )";
    const char* fragmentShaderSource = R"(
        #version 330 core
        uniform sampler2D texture;
        out vec4 FragColor;
        void main() {
            FragColor = texture2D(texture, vec2(0.5));
        }
    )";

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Create a buffer containing a single point
    GLuint VBO;
    float point[] = {0.0f, 0.0f};
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    // Render the point and read back the rendered pixel
    GLubyte pixel[4];
    glUseProgram(shaderProgram);
    glViewport(0, 0, 1, 1);
    glBindTexture(GL_TEXTURE_2D, floatTexture);
    glDrawArrays(GL_POINTS, 0, 1);
    glReadPixels(0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);

    // Clean up
    glDeleteFramebuffers(1, &framebuffer);
    glDeleteTextures(1, &byteTexture);
    glDeleteTextures(1, &floatTexture);
    glDeleteProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteBuffers(1, &VBO);

    // The center sample will only have a value of 0.5 if linear filtering works
    return pixel[0] == 127 || pixel[0] == 128;
}

OESTextureFloatLinear* getOESTextureFloatLinear(GLFWwindow* window) {
    static OESTextureFloatLinear* extension = nullptr;
    if (extension == nullptr) {
        extension = new OESTextureFloatLinear();
    }
    return extension;
}

const char* getExtension(GLFWwindow* window, const char* name) {
    if (strcmp(name, "OES_texture_float_linear") == 0) {
        return reinterpret_cast<const char*>(getOESTextureFloatLinear(window));
    }
    return glfwGetProcAddress(name);
}

std::vector<const char*> getSupportedExtensions(GLFWwindow* window) {
    std::vector<const char*> extensions;
    GLint n, i;
    glGetIntegerv(GL_NUM_EXTENSIONS, &n);
    for (i = 0; i < n; i++) {
        extensions.push_back(reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i)));
    }
    if (std::find(extensions.begin(), extensions.end(), "OES_texture_float_linear") == extensions.end()) {
        extensions.push_back("OES_texture_float_linear");
    }
    return extensions;
}

int main() {
    if (!glfwInit()) {
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        return -1;
    }

    if (supportsOESTextureFloatLinear(window)) {
        // Install the polyfill
        glfwSetExtensionProc(getExtension);
    }

    while (!glfwWindowShouldClose(window)) {
        // Render here

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}