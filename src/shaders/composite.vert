#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_color;
layout (location = 2) in vec3 a_tex_coord;

out vec3 our_color;
out vec2 tex_coord;
flat out int instance_id;
uniform mat4 model;
//uniform mat4 transform_back;

//uniform mat4 transforms[2];

uniform vec2 offsets[10];


void main() {
    vec2 offset = offsets[gl_InstanceID];
    gl_Position = model * vec4(vec3(a_pos.xy + offset, 0.0), 1.0);
    //    gl_Position = model * vec4(a_pos, 1.0);
    //    gl_Position = vec4(a_pos, 1.0);
    our_color = a_color;
    tex_coord = vec2(a_tex_coord.x, a_tex_coord.y);
    instance_id = gl_InstanceID;
}