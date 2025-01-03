#include <vector>
#include <cmath>
#include <GL/gl.h>
#include <GL/glu.h>

const float IOR_AIR = 1.0f;
const float IOR_WATER = 1.333f;
const float abovewaterColor[3] = {0.25f, 1.0f, 1.25f};
const float underwaterColor[3] = {0.4f, 0.9f, 1.0f};
const float poolHeight = 1.0f;

struct Vector3 {
    float x, y, z;

    Vector3 operator-(const Vector3& other) const {
        return {x - other.x, y - other.y, z - other.z};
    }

    Vector3 operator+(const Vector3& other) const {
        return {x + other.x, y + other.y, z + other.z};
    }

    Vector3 operator*(float scalar) const {
        return {x * scalar, y * scalar, z * scalar};
    }

    Vector3 operator/(float scalar) const {
        return {x / scalar, y / scalar, z / scalar};
    }

    float dot(const Vector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    Vector3 normalize() const {
        float length = std::sqrt(x * x + y * y + z * z);
        return {x / length, y / length, z / length};
    }

    Vector3 refract(const Vector3& incident, const Vector3& normal, float eta) const {
        float cosI = -normal.dot(incident);
        float sinT2 = eta * eta * (1.0f - cosI * cosI);
        if (sinT2 > 1.0f) return {0, 0, 0}; // Total internal reflection
        float cosT = std::sqrt(1.0f - sinT2);
        return (incident * eta + normal * (eta * cosI - cosT)).normalize();
    }

    Vector3 reflect(const Vector3& incident, const Vector3& normal) const {
        return incident - normal * (2.0f * normal.dot(incident));
    }
};

struct Renderer {
    GLuint tileTexture;
    Vector3 lightDir;
    GLuint causticTex;
    GLuint waterMesh;
    std::vector<GLuint> waterShaders;
    Vector3 sphereCenter;
    float sphereRadius;

    Renderer() {
        // Initialization code for textures, shaders, etc.
        lightDir = Vector3{2.0f, 2.0f, -1.0f}.normalize();
        // Load textures and create shaders...
    }

    std::pair<float, float> intersectCube(const Vector3& origin, const Vector3& ray, const Vector3& cubeMin, const Vector3& cubeMax) {
        Vector3 tMin = (cubeMin - origin) / ray;
        Vector3 tMax = (cubeMax - origin) / ray;
        Vector3 t1 = {std::min(tMin.x, tMax.x), std::min(tMin.y, tMax.y), std::min(tMin.z, tMax.z)};
        Vector3 t2 = {std::max(tMin.x, tMax.x), std::max(tMin.y, tMax.y), std::max(tMin.z, tMax.z)};
        float tNear = std::max(std::max(t1.x, t1.y), t1.z);
        float tFar = std::min(std::min(t2.x, t2.y), t2.z);
        return {tNear, tFar};
    }

    float intersectSphere(const Vector3& origin, const Vector3& ray, const Vector3& sphereCenter, float sphereRadius) {
        Vector3 toSphere = origin - sphereCenter;
        float a = ray.dot(ray);
        float b = 2.0f * toSphere.dot(ray);
        float c = toSphere.dot(toSphere) - sphereRadius * sphereRadius;
        float discriminant = b * b - 4.0f * a * c;
        if (discriminant > 0.0f) {
            float t = (-b - std::sqrt(discriminant)) / (2.0f * a);
            if (t > 0.0f) return t;
        }
        return 1.0e6f;
    }

    // Other methods for rendering, updating caustics, etc.
};