#version 330 core
out vec4 FragColor;

in vec3 our_color;
in vec2 tex_coord;
flat in int instance_id;

// texture samplers
uniform sampler2D texture_back;
uniform sampler2D texture_front;

// image info
uniform vec2 u_resolution;
uniform vec2 u_tex_resolution;

//uniform mat4 transform_front;
//uniform mat4 transform_back;

uniform float x_pos;

void main() {
    float aspect = u_resolution.x/u_resolution.y;
    //     st.y *= aspect;
    //    vec4 front = texture2D(texture_front, vec2(tex_coord.x - x_pos, tex_coord.y));
    vec4 front = texture2D(texture_front, vec2(tex_coord.x - x_pos, tex_coord.y));
    vec4 back = texture2D(texture_back, vec2(tex_coord.x - x_pos, tex_coord.y));
    //    vec4 back = texture2D(texture_back, tex_coord);

    // over operation
    vec3 color = (front.a * front.rgb) + (1 - front.a) * (back.a * back.rgb);
    float alpha = (1 - front.a) * back.a;
    if (instance_id == 0){
        FragColor = texture(texture_back, tex_coord);
    } else {
        //        FragColor = vec4(color, alpha);
        FragColor = texture(texture_front, tex_coord);
    }

    //     FragColor = vec4(FragColor.xy, 0.0, alpha);
    //    FragColor = mix(texture(texture_back, tex_coord), texture(texture_front, tex_coord), 0.5);
}