#pragma once

#include <GL/glew.h>

#include <string>

namespace SHV::OpenGl {
class ShaderProgram {
   public:
    ShaderProgram(const std::string& programName);

    void SetUp();
    void TearDown();

    void Use();

    GLuint GetUniformLocation(const std::string& uniformName) const;

   private:
    GLuint program = 0;

    std::string vertexShaderPath;
    std::string fragmentShaderPath;
};
}  // namespace SHV::OpenGl
