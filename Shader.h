#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

#include "OpenGL.h"
#include "math/Matrix4D.h"

namespace ST
{
    /* Using of shader sequence:
     * 1. Create vertex and fragment shaders,
     *    using CreateShader(...).
     * 2. Create shader program, using CreateProgram(...).
     * 3. Use shader program before drawing anything
     *    by calling Activate().
     *
     * Note: every public method throws runtime_exception().
     * Warning: every public method is valid to use only
     *          after CreateShader(...) call was successful.
     */
    class Shader
    {
    public:
        Shader() : m_program(0) {}

        // Creates shader of type "type" from file named "fileName".
        void CreateShader(GLenum type, std::string fileName);
        // Creates program from vertex & fragment
        // shaders and makes Shader instance valid to use.
        void CreateProgram();

        // Make OpenGL use this shader program to draw anything.
        void Activate() const;

        GLint GetAttribLocation(std::string attribName) const;
        GLint GetUniformLocation(std::string uniformName) const;

        // Set uniforms.
        void SetUniformBool(const std::string&, bool) const;
        void SetUniformMatrix(const std::string&, const Math::Matrix4D&) const;

    private:
        // Loads shader from file and returns it.
        std::string loadShader(std::string fileName) const;

    private:
        std::vector<GLuint> m_shaders; // Compiled vertex and fragment shaders.

        GLuint m_program;
    };
}

#endif // SHADER_H_INCLUDED
