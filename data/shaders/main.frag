#version 130

uniform vec3 lightColor;
uniform vec3 dirToLight;
uniform mat4 model;
uniform mat4 view;
//uniform bool has_light;

in vec3 norm;
out vec4 outColor;

void main()
{
//    if (has_light)
//    {
        vec3 adjNormal = normalize(model * view * vec4(norm, 0.0f)).xyz;
        float cosAngle = clamp (dot(adjNormal, dirToLight), 0, 1);
        outColor = cosAngle * vec4(lightColor, 1.0f);
//    }
//    else outColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
