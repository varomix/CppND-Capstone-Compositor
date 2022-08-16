#version 330 core
out  vec4 FragColor;

in vec3 our_color;
in vec2 uvs;

uniform sampler2D back_layer;
uniform sampler2D front_layer;
uniform int operation;

void main() {
    //    FragColor = texture(back_layer, uvs);
    //    FragColor = texture(front_layer, uvs);
    // mix framebuffers
    //    FragColor = mix(texture(back_layer, uvs), texture(front_layer, uvs), 0.5);

    vec4 front = texture2D(front_layer, uvs);
    vec4 back = texture2D(back_layer, uvs);

    // Image Compositing Operations
    // OVER
    if (operation == 0) {
        vec4 over = front + ((1-front.a) * back);
        FragColor = vec4(over);
    }
    // ADD
    else if (operation == 1) {
        FragColor = vec4(front + back);
    }
    // SUBTRACT
    else if (operation == 2) {
        FragColor = vec4(front - back);
    }
    // MIX
    else if (operation == 3) {
        FragColor = mix(back, front, 0.5);
    }
    // MULTIPLY
    else if (operation == 4) {
        FragColor = vec4(front * back);
    }
    // SCREEN
    else if (operation == 5) {
        FragColor = vec4(1 - ((1 - front) * (1 - back)));
    }
    // MAXIMUM
    else if (operation == 6) {
        FragColor = vec4(max(front, back));
    }
    // MINIMUM
    else if (operation == 7) {
        FragColor = vec4(min(front, back));
    }
    // IN
    else if (operation == 8) {
        FragColor = vec4(back * front.a);
    }
    // OUT
    else if (operation == 9) {
        FragColor = vec4(back * (1 - front.a));
    }
    // ATOP
    else if (operation == 10) {
        vec4 _in = (front * back.a);
        vec4 over = _in + ((1-_in.a) * back);
        FragColor = vec4(over);
    }
}
