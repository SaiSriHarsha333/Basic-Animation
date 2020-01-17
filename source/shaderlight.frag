#version 330

uniform vec4 lightColor;

out vec4 color;

void main()
{
	//color = vec4(1.0f, 1.0f, 0.0f, 1.0f);
	color = lightColor;
}
