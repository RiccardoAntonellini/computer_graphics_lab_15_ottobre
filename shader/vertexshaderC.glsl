// ============================================================================
// shader/vertexshaderC.glsl - GUIDA DIDATTICA
// Vertex shader: riceve attributi di posizione e colore e produce la posizione di clip.
// ============================================================================

#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aColor;

uniform mat4 u_Model;
uniform mat4 u_Projection;

out vec4 vColor;

void main()
{
    gl_Position = u_Projection * u_Model * vec4(aPosition, 1.0);
    vColor = aColor;
}
