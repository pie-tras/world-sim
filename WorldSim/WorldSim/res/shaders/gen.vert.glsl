#version 330 core

layout(location = 0) in vec2 position;

out vec2 textureCoords;

uniform int u_textureType;
uniform vec2 u_textureOffset;
uniform vec2 u_textureScale;

uniform mat4 u_mvp;

void main(){
    gl_Position = u_mvp * vec4(position.xy, 0.0, 1.0);

    textureCoords = vec2((position.x + 1.0) / 2.0,
			1 - (position.y + 1.0) / 2.0);

    if (u_textureType == 2) {
			textureCoords = vec2(
					(textureCoords.x + u_textureOffset.x) / u_textureScale.x - 0.0000001 * position.x ,
					(textureCoords.y + u_textureOffset.y) / u_textureScale.y + 0.0000001 * position.y);
	}
}