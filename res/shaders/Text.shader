#shader vertex
#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform int u_windowSizeX;
uniform int u_windowSizeY;

void main()
{
	vec2 positionHomo = (position - vec2(u_windowSizeX/2, u_windowSizeY/2)) / vec2(u_windowSizeX / 2, u_windowSizeY / 2);
	gl_Position = vec4(positionHomo.xy, 0.0, 1.0);
    v_TexCoord = texCoord;
};




#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
in vec2 v_TexCoord;
uniform sampler2D u_Texture;

void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoord);
	color = texColor;
};