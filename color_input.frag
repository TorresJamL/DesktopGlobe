#version 330 core

in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D tex0;

void main()
{
    vec3 color = texture(tex0, texCoord).rgb;
    FragColor = vec4(color, 1.0);
}
