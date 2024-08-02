#version 330 core

in vec3 fcolor;

uniform int canviColor;

out vec4 fragColor;

vec3 roig = vec3(1.0, 0.0, 0.0);

vec3 blau = vec3(0.0, 0.0, 1.0);

vec3 verd = vec3(0.0, 1.0, 0.0);

void main() {
    fragColor = vec4(fcolor, 1);
}

