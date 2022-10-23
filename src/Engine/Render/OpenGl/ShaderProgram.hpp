#pragma once

#include <GL/glew.h>

#include <glm/mat4x4.hpp>

#include <string>

namespace SHV::OpenGl {
class ShaderProgram {
   public:
    ShaderProgram(const std::string& programName);
    ~ShaderProgram();

    void Use();

    void SetMat4(const std::string& uniformName, const glm::mat4& value);
    void SetInt(const std::string& uniformName, int value);

   private:
    GLuint GetUniformLocation(const std::string& uniformName) const;

    GLuint program = 0;

    std::string vertexShaderPath;
    std::string fragmentShaderPath;
};
}  // namespace SHV::OpenGl
