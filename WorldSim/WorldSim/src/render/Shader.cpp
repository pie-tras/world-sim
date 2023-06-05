#include <fstream>
#include <string>
#include <sstream>

#include "Shader.h"

Shader::Shader(const std::string& key) {
    const std::string shader_path = "res/shaders/";
    ShaderProgramSource source = parseShaders(shader_path + key + ".vert.glsl", shader_path + key + ".frag.glsl");
    createShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader() {
    cleanUp();
}

ShaderProgramSource Shader::parseShaders(const std::string& vertPath, const std::string& fragPath) {
    std::ifstream vertStream(vertPath);
    std::ifstream fragStream(fragPath);

    std::string line;
    std::stringstream ss[2];
    while (getline(vertStream, line)) { ss[0] << line << '\n'; }
    while (getline(fragStream, line)) { ss[1] << line << '\n'; }

    vertStream.close();
    fragStream.close();

    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::compileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader) {
    programID = glCreateProgram();
    vertexShaderID = compileShader(GL_VERTEX_SHADER, vertexShader);
    fragmentShaderID = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);
    glValidateProgram(programID);

    return programID;
}

inline void Shader::getAllUniformLocations() {}

void Shader::start() {
    glUseProgram(programID);
}

void Shader::stop() {
    glUseProgram(0);
}

void Shader::cleanUp() {
    stop();
    glDetachShader(programID, vertexShaderID);
    glDetachShader(programID, fragmentShaderID);
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
    glDeleteProgram(programID);
}

void Shader::loadInt(unsigned int location, const int value) {
    glUniform1i(location, value);
}

void Shader::loadFloat(unsigned int location, const float value) {
    glUniform1f(location, value);
}

void Shader::loadDouble(unsigned int location, const double value) {
    glUniform1d(location, value);
}

void Shader::loadBool(unsigned int location, const bool value) {
    if (value)
        glUniform1i(location, 1);
    else
        glUniform1i(location, 0);
}

void Shader::loadVec2f(unsigned int location, const glm::vec2& vector) {
    glUniform2f(location, vector.x, vector.y);
}

void Shader::loadVec3f(unsigned int location, const glm::vec3& vector) {
    glUniform3f(location, vector.x, vector.y, vector.z);
}

void Shader::loadVec4f(unsigned int location, const glm::vec4& vector) {
    glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
}

void Shader::loadMat4f(unsigned int location, const glm::mat4& matrix) {
    glUniformMatrix4fv(location, 1, false, glm::value_ptr(matrix));
}