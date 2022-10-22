#include "Engine/Render/OpenGl/ShaderProgram.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

#include "Engine/Common/Assert.hpp"
#include "Engine/Common/Exception.hpp"
#include "Engine/Common/Logger.hpp"

namespace SHV::OpenGl::SShaderProgram {
std::string ReadFile(const std::string& filePath) {
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if (!fileStream.is_open()) {
        throw SHV::Exception("Could not read file: %s\n. File does not exist.",
                             filePath.c_str());
    }

    std::string line = "";
    while (!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

void CompileShader(GLuint shader, const std::string& shaderPath) {
    std::string shaderStr = ReadFile(shaderPath);
    const char* shaderSrc = shaderStr.c_str();

    GLint result = GL_FALSE;
    int logLength;

    glShaderSource(shader, 1, &shaderSrc, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

    if (logLength > 1) {
        std::vector<char> shaderError(logLength);
        glGetShaderInfoLog(shader, logLength, NULL, shaderError.data());

        throw SHV::Exception("Error during shader compilation occurred: %s\n",
                             shaderError.data());
    }
}

void LinkShaderProgram(GLuint vertShader, GLuint fragShader, GLuint program) {
    GLint result = GL_FALSE;
    int logLength;

    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &result);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

    if (logLength > 1) {
        std::vector<char> programError(logLength);
        glGetProgramInfoLog(program, logLength, NULL, programError.data());

        throw SHV::Exception("Error during shader compilation occurred: %s\n",
                             programError.data());
    }
}

GLuint CreateProgram(const std::string& vertexPath,
                     const std::string& fragmentPath) {
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint program = glCreateProgram();

    // Compile vertex shader
    LogD(eTag::kOpenGl) << "Compiling vertex shader..." << std::endl;
    LogD(eTag::kOpenGl) << "Vertex path: " << vertexPath << std::endl;

    CompileShader(vertShader, vertexPath);

    // Compile fragment shader
    LogD(eTag::kOpenGl) << "Compiling fragment shader..." << std::endl;
    LogD(eTag::kOpenGl) << "Fragment path: " << fragmentPath << std::endl;

    CompileShader(fragShader, fragmentPath);

    // Link program
    LogD(eTag::kOpenGl) << "Linking program..." << std::endl;

    LinkShaderProgram(vertShader, fragShader, program);

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    LogD(eTag::kOpenGl) << "Shader program created successfully!" << std::endl;

    return program;
}

std::string GetShadersDir() {
#if defined(OPEN_GL_SHADERS_DIR)
    return OPEN_GL_SHADERS_DIR;
#else
    throw SHV::Exception("OpenGl shaders directory undefined!");
#endif
}
}  // namespace SHV::OpenGl::SShaderProgram

SHV::OpenGl::ShaderProgram::ShaderProgram(const std::string& programName)
    : vertexShaderPath(SHV::OpenGl::SShaderProgram::GetShadersDir() +
                       programName + ".vert"),
      fragmentShaderPath(SHV::OpenGl::SShaderProgram::GetShadersDir() +
                         programName + ".frag") {
    program = SHV::OpenGl::SShaderProgram::CreateProgram(vertexShaderPath,
                                                         fragmentShaderPath);
}

SHV::OpenGl::ShaderProgram::~ShaderProgram() {
    AssertD(program != 0);
    glDeleteProgram(program);
}

void SHV::OpenGl::ShaderProgram::Use() {
    AssertD(program != 0);
    glUseProgram(program);
}

GLuint SHV::OpenGl::ShaderProgram::GetUniformLocation(
    const std::string& uniformName) const {
    AssertD(program != 0);
    return glGetUniformLocation(program, uniformName.c_str());
}