#version 330 core
out vec4 FragColor;

in vec3 OurColor;
in vec2 TexCoord;

uniform sampler2D texture1;
//uniform sampler2D texture2;

void main() {
	FragColor = vec4(OurColor, 1.0) * texture(texture1, TexCoord);
}