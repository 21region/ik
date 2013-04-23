#version 130

uniform vec3 lightColor;
uniform vec3 dirToLight;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec3 normal;
in vec3 position;
out vec3 norm;

void main()
{
    mat4 trans = projection * view * model;
    gl_Position = trans * vec4(position, 1.0f);
    norm = normal;
}
