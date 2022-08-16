#version 330 core
layout (location = 0) in vec3 m_position;
layout (location = 1) in vec3 m_color;
layout (location = 2) in vec2 m_uvs;

out vec3 our_color;
out vec2 uvs;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    uvs = m_uvs;
    our_color = m_color;
    gl_Position = projection * view * model * vec4(m_position, 1.0);
}
