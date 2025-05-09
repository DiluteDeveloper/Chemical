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

	}

}

void main()
{                          
	switch(faceType) {
	case 0:
		fragColor = texture(GetTexture(), texCoord);
		break;
	case 1:
		fragColor = texture(GetTexture(), texCoord);
		break;
	case 2:
		fragColor = vec4(vec3(texture(GetTexture(), texCoord).xyz * 0.6), 1.0);
		break;
	case 3:
		fragColor = vec4(vec3(texture(GetTexture(), texCoord).xyz * 0.6), 1.0);
		break;
	case 4:
		fragColor = vec4(vec3(texture(GetTexture(), texCoord).xyz * 0.8), 1.0);
		break;
	case 5:
		fragColor = vec4(vec3(texture(GetTexture(), texCoord).xyz * 0.8), 1.0);
		break;

	}

}