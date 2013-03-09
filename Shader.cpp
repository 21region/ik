#include <stdexcept>
#include "Shader.h"

using namespace std;
using namespace Math;

namespace ST
{
    //-------------- Activate shader program --------------//
    void Shader::Activate() const
    {
        if (m_program == 0)
            throw runtime_error("Can't use uninitialized shader program.");

        glUseProgram(m_program);
    }

    //-------------- Get shader attribute index --------------//
    GLint Shader::GetAttribLocation(string attribName) const
    {
        if (m_program == 0)
            throw runtime_error("Can't use uninitialized shader program.");

        return glGetAttribLocation(m_program, attribName.c_str());
    }
    //-------------- Get shader uniform index --------------//
    GLint Shader::GetUniformLocation(std::string uniformName) const
    {
        if (m_program == 0)
            throw runtime_error("Can't use uninitialized shader program.");

        return glGetUniformLocation(m_program, uniformName.c_str());
    }

    //-------------- Set bool as a uniform --------------//
    void Shader::SetUniformBool(const std::string& uname, bool b) const
    {
        GLint location = GetUniformLocation(uname.c_str());
        glUniform1i(location, b);
    }

    //-------------- Set Matrix4D as a uniform --------------//
    void Shader::SetUniformMatrix(const string& uname, const Matrix4D& m) const
    {
        GLint location = GetUniformLocation(uname.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, &m[0]);
    }

    //-------------- Compile shader from file --------------//
    void Shader::CreateShader(GLenum type, string fileName)
    {
        // Create, load and compile shader.
        GLuint shader = glCreateShader(type);

        const char* shaderData = loadShader(fileName).c_str();
        glShaderSource(shader, 1, &shaderData, 0);

        glCompileShader(shader);

        // Check for compile errors.
        GLint status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE)
        {
            GLint infoLogLength;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

            GLchar infoLog[infoLogLength + 1];
            glGetShaderInfoLog(shader, infoLogLength, 0, infoLog);

            string shaderTypeName;
            (type == GL_VERTEX_SHADER) ? shaderTypeName = "Vertex shader"
                                       : shaderTypeName = "Fragment shader";

            string errorMessage = shaderTypeName +
                                  " compile failure:\n" + infoLog;
            throw runtime_error(errorMessage);
        }

        // Compiled shader will be used for linking later.
        m_shaders.push_back(shader);
    }

    //----------- Create program from compiled shaders -----------//
    void Shader::CreateProgram()
    {
        // Link all shaders into one program.
        m_program = glCreateProgram();

        for (size_t shader = 0; shader < m_shaders.size(); shader++)
            glAttachShader(m_program, m_shaders[shader]);

        glLinkProgram(m_program);

        // Check for linking errors.
        GLint status;
        glGetProgramiv(m_program, GL_LINK_STATUS, &status);
        if (status == GL_FALSE)
        {
            GLint infoLogLength;
            glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &infoLogLength);

            GLchar infoLog[infoLogLength + 1];
            glGetProgramInfoLog(m_program, infoLogLength, 0, infoLog);

            string errorMessage = "Link failure:\n";
            throw runtime_error(errorMessage + infoLog);
        }
    }

    //-------------- Load shader from file --------------//
    string Shader::loadShader(string fileName) const
    {
        ifstream ifs(fileName.c_str());

        if (!ifs)
            throw runtime_error("Cannot open shader file: " + fileName);

        // Read the whole file in a string.
        string shaderSource((istreambuf_iterator<char>(ifs)),
                             istreambuf_iterator<char>());

        return shaderSource;
    }
}
