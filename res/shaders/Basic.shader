#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;
out float fogAmount;

//uniform vec3 u_CameraPosition;

//out vec3 v_CameraPosition;

void main()
{
	gl_Position = u_MVP * vec4(position.xyz, 1.0);
    v_TexCoord = texCoord;

	const float LOG2 = -1.442695;
	const float density = 0.02;
	float d = density * length(gl_Position.xyz);
	fogAmount = 1.0 - clamp(exp2(d * d * LOG2), 0.0, 1.0);
};




#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in float fogAmount;
//in vec3 v_CameraPosition;
uniform sampler2D u_Texture;
//uniform vec3 u_CameraPosition;

void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoord);
	//float fogCoordinate = abs(v_CameraPosition.z);

	color = texColor;
	color = mix(color, vec4(0.53, 0.81, 0.92, 1.0), fogAmount);
};