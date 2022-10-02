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

   private:
    GLuint program;

    std::string vertexShaderPath;
    std::string fragmentShaderPath;
};
}  // namespace SHV::OpenGl