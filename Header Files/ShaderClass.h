#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>
#include <filesystem>

std::string get_file_contains(const char* filename);

class Shader{
public:
	GLuint ID;
	Shader(const char* vertexFile, const char* fragmentFile);
	Shader(const char* File);

	void Activate();
	void Delete();

	void compileErrors(unsigned int shader, const char* type);
};