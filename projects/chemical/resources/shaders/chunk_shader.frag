#version 460 core

out vec4 fragColor;

in vec2 texCoord;

in flat unsigned int texID;

layout(binding=0)uniform sampler2D dirtTexture;
layout(binding=1)uniform sampler2D grassTexture;
layout(binding=2)uniform sampler2D stoneTexture;
layout(binding=3)uniform sampler2D bedrockTexture;

sampler2D GetTexture() {

	switch(texID) {
	case 0:
		return dirtTexture;
		break;
	case 1:
		return grassTexture;
		break;
	case 2:
		return stoneTexture;
		break;
	case 3:
		return bedrockTexture;
		break;

	}

}

void main()
{                          

	fragColor = texture(GetTexture(), texCoord);
}