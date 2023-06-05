#version 330 core

in vec2 textureCoords;

out vec4 color;

uniform sampler2D entityTexture;
uniform int u_textureType;
uniform vec4 u_color;

void main(){
   if(u_textureType != 0) {
        color = texture(entityTexture, textureCoords) * u_color;
   } else {
        color = u_color;
   }
}