#version 460 core

out vec4 fragColor;

in vec2 texCoord;

in flat unsigned int texID;
in flat unsigned int faceType;

layout(binding=0)uniform sampler2D stoneTexture;
layout(binding=1)uniform sampler2D dirtTexture;
layout(binding=2)uniform sampler2D grassTexture;
layout(binding=3)uniform sampler2D bedrockTexture;
layout(binding=4)uniform sampler2D oaklogtopTexture;
layout(binding=5)uniform sampler2D oaklogsideTexture;
layout(binding=6)uniform sampler2D oakleavesTexture;

sampler2D GetTexture() {

	switch(texID) {
	case 0:
		return stoneTexture;
		break;
	case 1:
		return dirtTexture;
		break;
	case 2:
		return grassTexture;
		break;
	case 3:
		return bedrockTexture;
		break;
	case 4:
		return oaklogtopTexture;
		break;
	case 5:
		return oaklogsideTexture;
		break;
	case 6:
		return oakleavesTexture;
		break;

	}

}

void main()
{           
	vec4 color;
	vec3 normal;
	switch(faceType) {
	case 0:
		color = texture(GetTexture(), texCoord);
		normal = vec3(0, 1, 0);
		break;
	case 1:
		color = texture(GetTexture(), texCoord);
		normal = vec3(0, 0, 0);
		break;
	case 2:
		color = texture(GetTexture(), texCoord);
		color = vec4(color.xyz * 0.8, color.w);
		normal = vec3(1, 0, 0);
		break;
	case 3:
		color = texture(GetTexture(), texCoord);
		color = vec4(color.xyz * 0.8, color.w);
		normal = vec3(0, 0, 0);
		break;
	case 4:
		color = texture(GetTexture(), texCoord);
		color = vec4(color.xyz * 0.6, color.w);
		normal = vec3(0, 0, 1);
		break;
	case 5:
		color = texture(GetTexture(), texCoord);
		color = vec4(color.xyz * 0.6, color.w);
		normal = vec3(0, 0, 0);
		break;
	}

	fragColor = color;

}