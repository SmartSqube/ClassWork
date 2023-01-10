
#pragma once

#include <glad/glad.h>
#include <string>
#include <glm/mat4x4.hpp>
using namespace std;
using namespace glm;

namespace Renderer {
    class ShaderProgram {
    public:
        ShaderProgram(const string& vertexShader, const string& fragmentShader);
        ~ShaderProgram();
        bool isCompiled() const { return m_isCompiled; }
        void use() const;
        void setInt(const string& name, const GLint value);
        void setMatrix4(const string& name, const mat4& matrix);

        ShaderProgram() = delete;
        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;
        ShaderProgram& operator=(ShaderProgram&& shaderProgram) noexcept;
        ShaderProgram(ShaderProgram&& shaderProgram) noexcept;

    private:
        bool createShader(const string& source, const GLenum shaderType, GLuint& shaderID);

        bool m_isCompiled = false;
        GLuint m_ID = 0;
    };
}