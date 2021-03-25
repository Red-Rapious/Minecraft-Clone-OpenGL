#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in int faceType;

out vec2 v_TexCoord;
out int v_faceType;
out float fogAmount;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * vec4(position.xyz, 1.0);
    v_TexCoord = texCoord;

	const float LOG2 = -1.442695;
	const float density = 0.013;
	float distance = length(gl_Position.xyz);
	float d = density * distance;

	fogAmount = 1.0 - clamp(exp2(d * d * LOG2), 0.0, 1.0);
	//fogAmount = 1.0 - clamp(exp(-density * distance), 0.0, 1.0);

	v_faceType = faceType;
};




#shader fragment
#version 330 core
#define FRONT 0
#define BACK 1
#define RIGHT 2
#define LEFT 3
#define UP 4
#define DOWN 5

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in int v_faceType;
in float fogAmount;

uniform sampler2D u_Texture;

void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoord);

	color = texColor;
	int faceType = v_faceType;
	//color = vec4(faceType);
	//faceType = 0;
	//if (faceType == 4)
	//{
	//	//color *= 1.0;
	//}
	//else if (v_faceType == RIGHT || v_faceType == LEFT)
	//{
	//	color *= 0.8;
	//}
	//else if (v_faceType == FRONT || v_faceType == BACK)
	//{
	//	color *= 0.6;
	//}
	//else 
	//{
	//	color *= 0.5;
	//}

	color = mix(color, vec4(0.53, 0.81, 0.92, 1.0), fogAmount);
};