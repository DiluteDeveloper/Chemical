#version 300 es
precision mediump float;
precision highp int;

out vec4 fragColor;

in vec2 texCoord;
flat in uint texID;
flat in uint faceType;

uniform sampler2D stoneTexture;
uniform sampler2D dirtTexture;
uniform sampler2D grassTexture;
uniform sampler2D bedrockTexture;
uniform sampler2D oaklogtopTexture;
uniform sampler2D oaklogsideTexture;
uniform sampler2D oakleavesTexture;

vec4 SampleTex() {
	switch (texID) {
	case 0u: return texture(stoneTexture, texCoord);
	case 1u: return texture(dirtTexture, texCoord);
	case 2u: return texture(grassTexture, texCoord);
	case 3u: return texture(bedrockTexture, texCoord);
	// case 4u: return texture(oaklogtopTexture, texCoord);
	// case 5u: return texture(oaklogsideTexture, texCoord);
	// case 6u: return texture(oakleavesTexture, texCoord);
	}
	return vec4(1.0, 0.0, 1.0, 1.0); // missing tex
}

void main()
{
	vec4 color = SampleTex();
	float shade = 1.0;                  // top/bottom
	if (faceType == 2u || faceType == 3u) shade = 0.8; // right/left
	else if (faceType >= 4u)              shade = 0.6; // front/back
	fragColor = vec4(color.rgb * shade, color.a);
}
