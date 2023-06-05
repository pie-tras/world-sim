#pragma once

#include "Global.h"

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader {
private:
    unsigned int vertexShaderID = 0;
    unsigned int fragmentShaderID = 0;

    ShaderProgramSource parseShaders(const std::string& vertPath, const std::string& fragPath);
    unsigned int compileShader(unsigned int type, const std::string& source);
    unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);

protected:
    unsigned int programID = 0;

    virtual void getAllUniformLocations() = 0;
public:
    Shader(const std::string& key);
    ~Shader();

    void start();
    void stop();
    void cleanUp();

    void loadInt(unsigned int location, const int);
    void loadFloat(unsigned int location, const float);
    void loadDouble(unsigned int location, const double value);
    void loadBool(unsigned int location, const bool value);
    void loadVec2f(unsigned int location, const glm::vec2& vector);
    void loadVec3f(unsigned int location, const glm::vec3& vector);
    void loadVec4f(unsigned int location, const glm::vec4& vector);
    void loadMat4f(unsigned int location, const glm::mat4& matrix);
};