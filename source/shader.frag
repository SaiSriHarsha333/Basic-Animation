#version 330

in vec4 vCol;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

//uniform mat4 modelMAT;
uniform vec4 lightColor;
uniform sampler2D textureSampler;
uniform vec3 lightPos;

out vec4 color;

void main()
{
	//color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	float ambientStrength = 0.5;
	vec4 ambient = ambientStrength * lightColor;
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec4 diffuse = diff * lightColor;
	color = texture(textureSampler, TexCoord)*(ambient+diffuse);
}
