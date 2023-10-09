#pragma once

#include <GL/glew.h>

#include <glm/mat4x4.hpp>

#include <string>

namespace SHV::OpenGl {
class ShaderProgram {
   public:
    ShaderProgram(const std::string& programName);
    ShaderProgram(const std::string& vertexShaderName, const std::string& fragmentShaderName);
    ~ShaderProgram();

    void Use();

    bool SetMat4(const std::string& uniformName, const glm::mat4& value);
    bool SetInt(const std::string& uniformName, int value);

   private:
    GLint GetUniformLocation(const std::string& uniformName) const;

    GLuint program = 0;

    std::string vertexShaderPath;
    std::string fragmentShaderPath;
};
}  // namespace SHV::OpenGl
