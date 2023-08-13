#include "ShaderClass.h"

// Получаем путь к рабочей директории единожды при запуске программы
const std::filesystem::path currentPath = std::filesystem::current_path();

std::string get_file_contents(const char* filename)
{

	// Открываем файл по абсолютному пути
	std::ifstream in(currentPath / "Resource Files" / "Shaders" / filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

// Конструктор класса Shader принимает имена файлов с вершинным и фрагментным шейдерами.
Shader::Shader(const char* vertexFile, const char* fragmentFile) {
	// Загрузка содержимого файлов с кодом шейдеров в строковые переменные.
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	// Получение указателей на сырые строки (const char*) из строковых переменных,
	// которые необходимы для функций OpenGL.
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	// Создание объекта вершинного шейдера, компиляция исходного кода и проверка ошибок.
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	// Создание объекта фрагментного шейдера, компиляция исходного кода и проверка ошибок.
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	compileErrors(fragmentShader, "FRAGMENT");

	// Создание программного объекта, прикрепление вершинного и фрагментного шейдеров,
	// и связывание их вместе.
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);

	// Удаление объектов шейдеров после того, как они были прикреплены к программе,
	// так как они больше не нужны.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::Shader(const char* File) : Shader((std::string(File) + ".vert").c_str(), (std::string(File) + ".frag").c_str()) {}

// Активация данной программы шейдеров, делая её текущей для использования при отрисовке.
void Shader::Activate() {
	glUseProgram(ID);
}

// Удаление программного объекта шейдеров из памяти OpenGL.
void Shader::Delete() {
	glDeleteProgram(ID);
}

void Shader::compileErrors(unsigned int shader, const char* type)
{
	// Stores status of compilation
	GLint hasCompiled;
	// Character array to store error message in
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
}