#version 330 core

in vec2 TexCoord;

out vec4 color;

uniform sampler2D Texture0;
uniform sampler2D Texture1;

void main()
{
    //color = vec4(0.5f, 0.5f, 0.5f, 0.0f);
    //color = vec4(TexCoord.x, TexCoord.y, 0.0f, 1.0f);
    color = texture(Texture0, TexCoord);
}
