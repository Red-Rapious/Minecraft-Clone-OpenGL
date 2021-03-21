#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;
uniform vec3 u_CameraPosition;

out vec3 v_CameraPosition;

void main()
{
	gl_Position = u_MVP * vec4(position.xyz, 1.0);
    v_TexCoord = texCoord;
	vec3 p = u_CameraPosition;
	v_CameraPosition = u_CameraPosition;
};




#shader fragment
#version 330 core

struct FogParameters
{
	vec3 m_color;
	float m_density;
};

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 v_CameraPosition;
uniform sampler2D u_Texture;
uniform FogParameters fogParameter;

void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoord);
	float fogCoordinate = abs(v_CameraPosition.z);

	color = texColor;
	//color = mix(color, vec4(fogParameter.m_color, 1.0), exp(-pow(fogParameter.m_density * fogCoordinate, 2.0)));
};