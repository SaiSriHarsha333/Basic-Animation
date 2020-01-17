#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 TextureCoord;

uniform mat4 modelMAT;
uniform mat4 view;
uniform mat4 projection;

out vec4 vCol;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

void main()
{
	gl_Position = projection * view * modelMAT * vec4(pos.x, pos.y, pos.z, 1.0f);
	//vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);
	vCol = vec4(color.x, color.y, color.z, 1.0f);
	TexCoord = TextureCoord;
	Normal = mat3(transpose(inverse(modelMAT)))*color;
	FragPos = vec3(modelMAT*vec4(pos, 1.0));
}
