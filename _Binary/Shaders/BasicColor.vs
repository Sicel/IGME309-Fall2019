#version 330
layout (location = 0) in vec3 Position_b;
layout (location = 1) in vec3 Color_b;

out vec3 Color;

uniform mat4 transform = mat4(1.0f);

void main()
{
	gl_Position = transform * vec4(Position_b, 1);
	
	Color = Color_b;
}