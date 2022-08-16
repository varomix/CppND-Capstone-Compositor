#version 330 core

out vec4 FragColor;

in vec3 our_color;
in vec2 uvs;

uniform sampler2D texture1;

void main() { 
	FragColor = texture(texture1, uvs);
}
