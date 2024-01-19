#version 460 core

out vec4 fragColor;

in vec2 texCoord;

in flat unsigned int texID;

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

	fragColor = texture(GetTexture(), texCoord);
}