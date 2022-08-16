#version 330 core
layout (location = 0) in vec2 m_position;
layout (location = 1) in vec3 m_color;
layout (location = 2) in vec2 m_uvs;

out vec3 our_color;
out vec2 uvs;

void main(){
    uvs = m_uvs;
    gl_Position = vec4(m_position.x, m_position.y, 0.0, 1.0);
    our_color = m_color;
}