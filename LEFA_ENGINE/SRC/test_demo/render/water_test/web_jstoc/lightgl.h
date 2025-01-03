#include <iostream>
#include <vector>
#include <stdexcept>
#include <functional>
#include <array>

class Matrix {
public:
    static void identity(Matrix& m) {
        // Implementation of identity matrix
    }

    static Matrix multiply(const Matrix& a, const Matrix& b, Matrix& result) {
        // Implementation of matrix multiplication
        return result;
    }

    static Matrix perspective(float fov, float aspect, float near, float far, Matrix& result) {
        // Implementation of perspective matrix
        return result;
    }

    static Matrix frustum(float l, float r, float b, float t, float n, float f, Matrix& result) {
        // Implementation of frustum matrix
        return result;
    }

    static Matrix ortho(float l, float r, float b, float t, float n, float f, Matrix& result) {
        // Implementation of orthographic matrix
        return result;
    }

    static Matrix scale(float x, float y, float z, Matrix& result) {
        // Implementation of scale matrix
        return result;
    }

    static Matrix translate(float x, float y, float z, Matrix& result) {
        // Implementation of translate matrix
        return result;
    }

    static Matrix rotate(float a, float x, float y, float z, Matrix& result) {
        // Implementation of rotate matrix
        return result;
    }

    static Matrix lookAt(float ex, float ey, float ez, float cx, float cy, float cz, float ux, float uy, float uz, Matrix& result) {
        // Implementation of lookAt matrix
        return result;
    }

    static Matrix inverse(const Matrix& m, Matrix& result) {
        // Implementation of matrix inversion
        return result;
    }

    std::array<float, 16> m;
};

class Vector {
public:
    float x, y, z;

    Vector(float x, float y, float z) : x(x), y(y), z(z) {}

    Vector transformPoint(const Vector& v) {
        // Implementation of point transformation
        return v;
    }
};

class GL {
public:
    static const int MODELVIEW = 1;
    static const int PROJECTION = 2;

    static void create() {
        // Implementation of create function
    }

    static void matrixMode(int mode) {
        // Implementation of matrixMode function
    }

    static void loadIdentity() {
        // Implementation of loadIdentity function
    }

    static void loadMatrix(const Matrix& m) {
        // Implementation of loadMatrix function
    }

    static void multMatrix(const Matrix& m) {
        // Implementation of multMatrix function
    }

    static void perspective(float fov, float aspect, float near, float far) {
        // Implementation of perspective function
    }

    static void frustum(float l, float r, float b, float t, float n, float f) {
        // Implementation of frustum function
    }

    static void ortho(float l, float r, float b, float t, float n, float f) {
        // Implementation of ortho function
    }

    static void scale(float x, float y, float z) {
        // Implementation of scale function
    }

    static void translate(float x, float y, float z) {
        // Implementation of translate function
    }

    static void rotate(float a, float x, float y, float z) {
        // Implementation of rotate function
    }

    static void lookAt(float ex, float ey, float ez, float cx, float cy, float cz, float ux, float uy, float uz) {
        // Implementation of lookAt function
    }

    static void pushMatrix() {
        // Implementation of pushMatrix function
    }

    static void popMatrix() {
        // Implementation of popMatrix function
    }

    static Vector project(float objX, float objY, float objZ) {
        // Implementation of project function
        return Vector(0, 0, 0);
    }

    static Vector unProject(float winX, float winY, float winZ) {
        // Implementation of unProject function
        return Vector(0, 0, 0);
    }

    static void addMatrixStack() {
        // Implementation of addMatrixStack function
    }

    static void addImmediateMode() {
        // Implementation of addImmediateMode function
    }

    static void addEventListeners() {
        // Implementation of addEventListeners function
    }
};

class GL {
public:
    static std::unordered_map<int, bool> keys;
    static void makeCurrent() {
        // Implementation for context switching
    }
    static void animate();
    static void fullscreen(const std::unordered_map<std::string, int>& options);
    static void onUpdate(float deltaTime) {
        // Update logic
    }
    static void onDraw() {
        // Draw logic
    }
};

std::unordered_map<int, bool> GL::keys;

std::string mapKeyCode(int code) {
    std::unordered_map<int, std::string> named = {
        {8, "BACKSPACE"},
        {9, "TAB"},
        {13, "ENTER"},
        {16, "SHIFT"},
        {27, "ESCAPE"},
        {32, "SPACE"},
        {37, "LEFT"},
        {38, "UP"},
        {39, "RIGHT"},
        {40, "DOWN"}
    };
    if (named.find(code) != named.end()) {
        return named[code];
    }
    return (code >= 65 && code <= 90) ? std::string(1, static_cast<char>(code)) : "";
}

void on(void* element, const std::string& name, std::function<void(int)> callback) {
    // Add event listener logic
}

void off(void* element, const std::string& name, std::function<void(int)> callback) {
    // Remove event listener logic
}

void keyDownHandler(int keyCode, bool altKey, bool ctrlKey, bool metaKey) {
    if (!altKey && !ctrlKey && !metaKey) {
        std::string key = mapKeyCode(keyCode);
        if (!key.empty()) GL::keys[key] = true;
        GL::keys[keyCode] = true;
    }
}

void keyUpHandler(int keyCode, bool altKey, bool ctrlKey, bool metaKey) {
    if (!altKey && !ctrlKey && !metaKey) {
        std::string key = mapKeyCode(keyCode);
        if (!key.empty()) GL::keys[key] = false;
        GL::keys[keyCode] = false;
    }
}

class Matrix {
public:
    float m[16];

    Matrix() {
        for (int i = 0; i < 16; i++) m[i] = 0;
        m[0] = m[5] = m[10] = m[15] = 1;
    }

    Matrix inverse() {
        Matrix result;
        // Inversion logic
        return result;
    }

    Matrix transpose() {
        Matrix result;
        // Transpose logic
        return result;
    }

    Matrix multiply(const Matrix& matrix) {
        Matrix result;
        // Multiplication logic
        return result;
    }

    // Other methods...
};

void GL::animate() {
    auto post = [](std::function<void()> callback) {
        // Request animation frame logic
    };
    auto time = std::chrono::steady_clock::now();
    auto context = GL();
    auto update = [&]() {
        auto now = std::chrono::steady_clock::now();
        if (GL::onUpdate) GL::onUpdate(std::chrono::duration<float>(now - time).count());
        if (GL::onDraw) GL::onDraw();
        post(update);
        time = now;
    };
    update();
}

void GL::fullscreen(const std::unordered_map<std::string, int>& options) {
    int top = options.count("paddingTop") ? options.at("paddingTop") : 0;
    int left = options.count("paddingLeft") ? options.at("paddingLeft") : 0;
    int right = options.count("paddingRight") ? options.at("paddingRight") : 0;
    int bottom = options.count("paddingBottom") ? options.at("paddingBottom") : 0;

    // Fullscreen logic
    // Resize logic
}

class Matrix {
public:
    std::vector<float> m;

    Matrix() : m(16, 0.0f) {}

    // Transpose the matrix
    static Matrix transpose(const Matrix& matrix, Matrix* result = nullptr) {
        Matrix res = result ? *result : Matrix();
        auto& r = res.m;
        const auto& m = matrix.m;

        r[0] = m[0]; r[1] = m[4]; r[2] = m[8];  r[3] = m[12];
        r[4] = m[1]; r[5] = m[5]; r[6] = m[9];  r[7] = m[13];
        r[8] = m[2]; r[9] = m[6]; r[10] = m[10]; r[11] = m[14];
        r[12] = m[3]; r[13] = m[7]; r[14] = m[11]; r[15] = m[15];

        return res;
    }

    // Multiply two matrices
    static Matrix multiply(const Matrix& left, const Matrix& right, Matrix* result = nullptr) {
        Matrix res = result ? *result : Matrix();
        auto& r = res.m;
        const auto& a = left.m;
        const auto& b = right.m;

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                r[i*4 + j] = a[i*4] * b[j] + a[i*4 + 1] * b[j + 4] + 
                             a[i*4 + 2] * b[j + 8] + a[i*4 + 3] * b[j + 12];
            }
        }

        return res;
    }

    // Create an identity matrix
    static Matrix identity(Matrix* result = nullptr) {
        Matrix res = result ? *result : Matrix();
        auto& m = res.m;
        std::fill(m.begin(), m.end(), 0.0f);
        m[0] = m[5] = m[10] = m[15] = 1.0f;
        return res;
    }

    // Create a perspective projection matrix
    static Matrix perspective(float fov, float aspect, float near, float far, Matrix* result = nullptr) {
        float y = std::tan(fov * M_PI / 360) * near;
        float x = y * aspect;
        return frustum(-x, x, -y, y, near, far, result);
    }

    // Create a frustum projection matrix
    static Matrix frustum(float l, float r, float b, float t, float n, float f, Matrix* result = nullptr) {
        Matrix res = result ? *result : Matrix();
        auto& m = res.m;

        m[0] = 2 * n / (r - l);
        m[5] = 2 * n / (t - b);
        m[2] = (r + l) / (r - l);
        m[6] = (t + b) / (t - b);
        m[10] = -(f + n) / (f - n);
        m[14] = -1;
        m[11] = -2 * f * n / (f - n);
        m[15] = 0;

        return res;
    }

    // Create an orthographic projection matrix
    static Matrix ortho(float l, float r, float b, float t, float n, float f, Matrix* result = nullptr) {
        Matrix res = result ? *result : Matrix();
        auto& m = res.m;

        m[0] = 2 / (r - l);
        m[5] = 2 / (t - b);
        m[10] = -2 / (f - n);
        m[3] = -(r + l) / (r - l);
        m[7] = -(t + b) / (t - b);
        m[11] = -(f + n) / (f - n);
        m[15] = 1;

        return res;
    }

    // Create a scaling matrix
    static Matrix scale(float x, float y, float z, Matrix* result = nullptr) {
        Matrix res = result ? *result : Matrix();
        auto& m = res.m;

        m[0] = x;
        m[5] = y;
        m[10] = z;
        m[15] = 1;

        return res;
    }

    // Create a translation matrix
    static Matrix translate(float x, float y, float z, Matrix* result = nullptr) {
        Matrix res = result ? *result : Matrix();
        auto& m = res.m;

        m[0] = m[5] = m[10] = m[15] = 1;
        m[3] = x;
        m[7] = y;
        m[11] = z;

        return res;
    }

    // Create a rotation matrix
    static Matrix rotate(float a, float x, float y, float z, Matrix* result = nullptr) {
        if (a == 0 || (x == 0 && y == 0 && z == 0)) {
            return identity(result);
        }

        Matrix res = result ? *result : Matrix();
        auto& m = res.m;

        float d = std::sqrt(x*x + y*y + z*z);
        a *= M_PI / 180; x /= d; y /= d; z /= d;
        float c = std::cos(a), s = std::sin(a), t = 1 - c;

        m[0] = x * x * t + c;
        m[1] = x * y * t - z * s;
        m[2] = x * z * t + y * s;

        m[4] = y * x * t + z * s;
        m[5] = y * y * t + c;
        m[6] = y * z * t - x * s;

        m[8] = z * x * t - y * s;
        m[9] = z * y * t + x * s;
        m[10] = z * z * t + c;

        m[15] = 1;

        return res;
    }

    // Create a look-at matrix
    static Matrix lookAt(float ex, float ey, float ez, float cx, float cy, float cz, 
                         float ux, float uy, float uz, Matrix* result = nullptr) {
        Matrix res = result ? *result : Matrix();
        auto& m = res.m;

        // Implementation of lookAt function...
        // (This part requires a Vector class which is not provided in the original code)

        return res;
    }
};

class Vector {
public:
    std::array<float, 3> data;

    Vector() : data{0, 0, 0} {}
    Vector(float x, float y, float z) : data{x, y, z} {}

    static Vector fromArray(const std::array<float, 3>& arr) {
        return Vector(arr[0], arr[1], arr[2]);
    }

    std::array<float, 3> toArray() const {
        return data;
    }

    Vector subtract(const Vector& other) const {
        return Vector(data[0] - other.data[0], data[1] - other.data[1], data[2] - other.data[2]);
    }

    Vector cross(const Vector& other) const {
        return Vector(data[1] * other.data[2] - data[2] * other.data[1],
                      data[2] * other.data[0] - data[0] * other.data[2],
                      data[0] * other.data[1] - data[1] * other.data[0]);
    }

    float length() const {
        return std::sqrt(data[0] * data[0] + data[1] * data[1] + data[2] * data[2]);
    }

    Vector unit() const {
        float len = length();
        return Vector(data[0] / len, data[1] / len, data[2] / len);
    }

    Vector add(const Vector& other) const {
        return Vector(data[0] + other.data[0], data[1] + other.data[1], data[2] + other.data[2]);
    }

    Vector negative() const {
        return Vector(-data[0], -data[1], -data[2]);
    }

    Vector divide(float scalar) const {
        return Vector(data[0] / scalar, data[1] / scalar, data[2] / scalar);
    }

    static Vector min(const Vector& a, const Vector& b) {
        return Vector(std::min(a.data[0], b.data[0]), std::min(a.data[1], b.data[1]), std::min(a.data[2], b.data[2]));
    }

    static Vector max(const Vector& a, const Vector& b) {
        return Vector(std::max(a.data[0], b.data[0]), std::max(a.data[1], b.data[1]), std::max(a.data[2], b.data[2]));
    }
};

class Indexer {
public:
    std::vector<std::vector<int>> unique;
    std::vector<int> indices;
    std::unordered_map<std::string, int> map;

    int add(const std::vector<int>& obj) {
        std::string key = std::to_string(obj[0]) + "," + std::to_string(obj[1]) + "," + std::to_string(obj[2]);
        if (map.find(key) == map.end()) {
            map[key] = unique.size();
            unique.push_back(obj);
        }
        return map[key];
    }
};

class Buffer {
public:
    unsigned int buffer;
    unsigned int target;
    std::vector<std::vector<float>> data;

    Buffer(unsigned int target) : target(target), buffer(0) {}

    void compile(unsigned int type) {
        std::vector<float> flatData;
        for (const auto& vec : data) {
            flatData.insert(flatData.end(), vec.begin(), vec.end());
        }
        // Simulate buffer upload (in real code, OpenGL functions would be called here)
        buffer = 1; // Placeholder for buffer ID
        std::cout << "Buffer compiled with " << flatData.size() << " elements." << std::endl;
    }
};

class Mesh {
public:
    std::unordered_map<std::string, Buffer*> vertexBuffers;
    std::unordered_map<std::string, Buffer*> indexBuffers;
    std::vector<std::vector<float>> vertices;
    std::vector<std::vector<float>> coords;
    std::vector<std::vector<float>> normals;
    std::vector<std::vector<int>> triangles;
    std::vector<std::vector<int>> lines;

    Mesh() {
        addVertexBuffer("vertices");
    }

    void addVertexBuffer(const std::string& name) {
        vertexBuffers[name] = new Buffer(0); // Placeholder for target
        vertices.resize(0);
    }

    void addIndexBuffer(const std::string& name) {
        indexBuffers[name] = new Buffer(0); // Placeholder for target
        triangles.resize(0);
    }

    void compile() {
        for (auto& pair : vertexBuffers) {
            Buffer* buffer = pair.second;
            buffer->data = vertices; // Assuming vertices are assigned to the buffer
            buffer->compile(0); // Placeholder for type
        }

        for (auto& pair : indexBuffers) {
            Buffer* buffer = pair.second;
            buffer->data = triangles; // Assuming triangles are assigned to the buffer
            buffer->compile(0); // Placeholder for type
        }
    }

    // Additional methods would be implemented here...
};

// Example usage
int main() {
    Mesh mesh;
    mesh.vertices.push_back({0, 0, 0});
    mesh.vertices.push_back({1, 0, 0});
    mesh.vertices.push_back({0, 1, 0});
    mesh.vertices.push_back({1, 1, 0});
    mesh.compile();
    return 0;
}



// ### GL.Mesh.sphere([options])
//
// Generates a geodesic sphere of radius 1. The `options` argument specifies
// options to pass to the mesh constructor in addition to the `detail` option,
// which controls the tesselation level. The detail is `6` by default.
// Example usage:
//
//     Mesh mesh1 = Mesh::sphere();
//     Mesh mesh2 = Mesh::sphere({ {"detail", 2} });

struct Mesh {
    std::vector<Eigen::Vector3f> vertices;
    std::vector<Eigen::Vector3f> normals;
    std::vector<unsigned int> triangles;

    static Mesh sphere(const std::unordered_map<std::string, int>& options = {}) {
        int detail = options.count("detail") ? options.at("detail") : 6;
        Mesh mesh;
        std::unordered_map<std::string, Eigen::Vector3f> indexer;

        auto tri = [](const Eigen::Vector3f& a, const Eigen::Vector3f& b, const Eigen::Vector3f& c, bool flip) {
            return flip ? std::vector<Eigen::Vector3f>{a, c, b} : std::vector<Eigen::Vector3f>{a, b, c};
        };

        auto fix = [](float x) {
            return x + (x - x * x) / 2;
        };

        for (int octant = 0; octant < 8; ++octant) {
            Eigen::Vector3f scale = pickOctant(octant);
            bool flip = scale.x() * scale.y() * scale.z() > 0;
            std::vector<Eigen::Vector3f> data;

            for (int i = 0; i <= detail; ++i) {
                for (int j = 0; i + j <= detail; ++j) {
                    float a = static_cast<float>(i) / detail;
                    float b = static_cast<float>(j) / detail;
                    float c = static_cast<float>(detail - i - j) / detail;
                    Eigen::Vector3f vertex(fix(a), fix(b), fix(c));
                    vertex = vertex.normalized() * scale.array();
                    data.push_back(vertex);
                }

                if (i > 0) {
                    for (int j = 0; i + j <= detail; ++j) {
                        int a = (i - 1) * (detail + 1) + ((i - 1) - (i - 1) * (i - 1)) / 2 + j;
                        int b = i * (detail + 1) + (i - i * i) / 2 + j;
                        mesh.triangles.push_back(a);
                        mesh.triangles.push_back(a + 1);
                        mesh.triangles.push_back(b);
                        if (i + j < detail) {
                            mesh.triangles.push_back(b);
                            mesh.triangles.push_back(a + 1);
                            mesh.triangles.push_back(b + 1);
                        }
                    }
                }
            }
        }

        mesh.vertices = data;
        mesh.normals = mesh.vertices;
        return mesh;
    }

    Eigen::Vector3f pickOctant(int octant) {
        // Implement the logic to pick the octant
        // This is a placeholder implementation
        return Eigen::Vector3f(1, 1, 1);
    }

    void compile() {
        // Implement the logic to compile the mesh
    }
};

// ### GL.Mesh.load(json[, options])
//
// Creates a mesh from the JSON generated by the `convert/convert.py` script.
// Example usage:
//
//     std::unordered_map<std::string, std::vector<std::vector<float>>> data = {
//       {"vertices", {{0, 0, 0}, {1, 0, 0}, {0, 1, 0}}},
//       {"triangles", {{0, 1, 2}}}
//     };
//     Mesh mesh = Mesh::load(data);

Mesh Mesh::load(const std::unordered_map<std::string, std::vector<std::vector<float>>>& json,
                const std::unordered_map<std::string, bool>& options = {}) {
    Mesh mesh;
    mesh.vertices.reserve(json.at("vertices").size());
    for (const auto& vertex : json.at("vertices")) {
        mesh.vertices.emplace_back(vertex[0], vertex[1], vertex[2]);
    }
    if (options.count("coords") && options.at("coords")) {
        // Implement logic to load coords
    }
    if (options.count("normals") && options.at("normals")) {
        mesh.normals.reserve(json.at("normals").size());
        for (const auto& normal : json.at("normals")) {
            mesh.normals.emplace_back(normal[0], normal[1], normal[2]);
        }
    }
    if (options.count("colors") && options.at("colors")) {
        // Implement logic to load colors
    }
    if (options.count("triangles") && options.at("triangles")) {
        mesh.triangles.reserve(json.at("triangles").size() * 3);
        for (const auto& triangle : json.at("triangles")) {
            mesh.triangles.push_back(triangle[0]);
            mesh.triangles.push_back(triangle[1]);
            mesh.triangles.push_back(triangle[2]);
        }
    }
    if (options.count("lines") && options.at("lines")) {
        // Implement logic to load lines
    }
    mesh.compile();
    return mesh;
}

class Texture {
public:
    Texture(int width, int height, const std::unordered_map<std::string, int>& options = {}) {
        this->id = glCreateTexture();
        this->width = width;
        this->height = height;
        this->format = options.count("format") ? options.at("format") : GL_RGBA;
        this->type = options.count("type") ? options.at("type") : GL_UNSIGNED_BYTE;
        int magFilter = options.count("filter") ? options.at("filter") : (options.count("magFilter") ? options.at("magFilter") : GL_LINEAR);
        int minFilter = options.count("filter") ? options.at("filter") : (options.count("minFilter") ? options.at("minFilter") : GL_LINEAR);
        
        if (this->type == GL_FLOAT) {
            if (!canUseFloatingPointTextures()) {
                throw std::runtime_error("OES_texture_float is required but not supported");
            }
            if ((minFilter != GL_NEAREST || magFilter != GL_NEAREST) && !canUseFloatingPointLinearFiltering()) {
                throw std::runtime_error("OES_texture_float_linear is required but not supported");
            }
        } else if (this->type == GL_HALF_FLOAT_OES) {
            if (!canUseHalfFloatingPointTextures()) {
                throw std::runtime_error("OES_texture_half_float is required but not supported");
            }
            if ((minFilter != GL_NEAREST || magFilter != GL_NEAREST) && !canUseHalfFloatingPointLinearFiltering()) {
                throw std::runtime_error("OES_texture_half_float_linear is required but not supported");
            }
        }
        
        glBindTexture(GL_TEXTURE_2D, this->id);
        glPixelStorei(GL_UNPACK_FLIP_Y_WEBGL, 1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, options.count("wrap") ? options.at("wrap") : (options.count("wrapS") ? options.at("wrapS") : GL_CLAMP_TO_EDGE));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, options.count("wrap") ? options.at("wrap") : (options.count("wrapT") ? options.at("wrapT") : GL_CLAMP_TO_EDGE));
        glTexImage2D(GL_TEXTURE_2D, 0, this->format, width, height, 0, this->format, this->type, nullptr);
    }

    void bind(int unit = 0) {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, this->id);
    }

    void unbind(int unit = 0) {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    bool canDrawTo() {
        framebuffer = framebuffer ? framebuffer : glCreateFramebuffer();
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->id, 0);
        bool result = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return result;
    }

    void drawTo(std::function<void()> callback) {
        GLint v[4];
        glGetIntegerv(GL_VIEWPORT, v);
        framebuffer = framebuffer ? framebuffer : glCreateFramebuffer();
        renderbuffer = renderbuffer ? renderbuffer : glCreateRenderbuffer();
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
        if (this->width != renderbuffer.width || this->height != renderbuffer.height) {
            renderbuffer.width = this->width;
            renderbuffer.height = this->height;
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, this->width, this->height);
        }
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->id, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            throw std::runtime_error("Rendering to this texture is not supported (incomplete framebuffer)");
        }
        glViewport(0, 0, this->width, this->height);

        callback();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glViewport(v[0], v[1], v[2], v[3]);
    }

    void swapWith(Texture& other) {
        std::swap(other.id, this->id);
        std::swap(other.width, this->width);
        std::swap(other.height, this->height);
    }

    static Texture fromImage(const Image& image, const std::unordered_map<std::string, int>& options = {}) {
        Texture texture(image.width, image.height, options);
        try {
            glTexImage2D(GL_TEXTURE_2D, 0, texture.format, texture.format, texture.type, image);
        } catch (...) {
            throw std::runtime_error("image not loaded for security reasons (serve this page over \"http://\" instead)");
        }
        if (options.count("minFilter") && options.at("minFilter") != GL_NEAREST && options.at("minFilter") != GL_LINEAR) {
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        return texture;
    }

    static Texture fromURL(const std::string& url, const std::unordered_map<std::string, int>& options = {}) {
        // Implementation for loading from URL and creating a checkerboard texture
    }

    static bool canUseFloatingPointTextures() {
        return glGetExtension("OES_texture_float") != nullptr;
    }

    static bool canUseFloatingPointLinearFiltering() {
        return glGetExtension("OES_texture_float_linear") != nullptr;
    }

    static bool canUseHalfFloatingPointTextures() {
        return glGetExtension("OES_texture_half_float") != nullptr;
    }

    static bool canUseHalfFloatingPointLinearFiltering() {
        return glGetExtension("OES_texture_half_float_linear") != nullptr;
    }

private:
    GLuint id;
    int width;
    int height;
    int format;
    int type;
    static GLuint framebuffer;
    static GLuint renderbuffer;
};

class Vector {
public:
    Vector(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

    Vector negative() const {
        return Vector(-x, -y, -z);
    }

    Vector add(const Vector& v) const {
        return Vector(x + v.x, y + v.y, z + v.z);
    }

    Vector subtract(const Vector& v) const {
        return Vector(x - v.x, y - v.y, z - v.z);
    }

    Vector multiply(const Vector& v) const {
        return Vector(x * v.x, y * v.y, z * v.z);
    }

    Vector divide(const Vector& v) const {
        return Vector(x / v.x, y / v.y, z / v.z);
    }

    bool equals(const Vector& v) const {
        return x == v.x && y == v.y && z == v.z;
    }

    float dot(const Vector& v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    Vector cross(const Vector& v) const {
        return Vector(
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x
        );
    }

    float length() const {
        return std::sqrt(this->dot(*this));
    }

    Vector unit() const {
        return this->divide(this->length());
    }

    float min() const {
        return std::min(std::min(x, y), z);
    }

    float max() const {
        return std::max(std::max(x, y), z);
    }

    std::tuple<float, float> toAngles() const {
        return { std::atan2(z, x), std::asin(y / this->length()) };
    }

    float angleTo(const Vector& a) const {
        return std::acos(this->dot(a) / (this->length() * a.length()));
    }

    std::vector<float> toArray(int n = 3) const {
        return { x, y, z }.begin(), { x, y, z }.begin() + n;
    }

    Vector clone() const {
        return Vector(x, y, z);
    }

    void init(float x, float y, float z) {
        this->x = x; this->y = y; this->z = z;
    }

    static Vector randomDirection() {
        return fromAngles(static_cast<float>(rand()) / RAND_MAX * M_PI * 2, std::asin(static_cast<float>(rand()) / RAND_MAX * 2 - 1));
    }

    static Vector fromAngles(float theta, float phi) {
        return Vector(std::cos(theta) * std::cos(phi), std::sin(phi), std::sin(theta) * std::cos(phi));
    }

private:
    float x, y, z;
};




