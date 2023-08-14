#pragma once

#include<glad/glad.h>
#include<stb/stb_image.h>
#define STB_IMAGE_IMPLEMENTATION

#include"shaderClass.h"

class Texture
{
public:
	GLuint ID;
	GLuint unit;
	const char* type;

	//							enum?
	Texture(const char* image, const char* texType, GLuint slot);

	// Assigns a texture unit to a texture
	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	// Binds a texture
	void Bind();
	// Unbinds a texture
	void Unbind();
	// Deletes a texture
	void Delete();
};