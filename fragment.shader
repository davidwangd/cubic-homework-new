#version 330 core

in vec3 fragmentColor;
out vec3 Color;

void main(){
	Color = fragmentColor;
}