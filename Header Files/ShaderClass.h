#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>
#include <filesystem>

// Получаем путь к рабочей директории единожды при запуске программы
const std::filesystem::path currentPath = std::filesystem::current_path();

std::string get_file_contents(std::filesystem::path filename);

class Shader{
public:
	GLuint ID;
	Shader(const char* vertexFile, const char* fragmentFile);
	Shader(const char* File);

	void Activate();
	void Delete();

	void compileErrors(unsigned int shader, const char* type);
};