#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * vec4(position.xyz, 1.0);
};




#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
uniform vec4 u_Color;

void main()
{
	color = u_Color;
};