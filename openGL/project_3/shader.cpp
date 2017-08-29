#include "shader.h"

void shaderInit()
{
	glewInit();
}

string loadFileToString(const char * filename)
{
	ifstream file(filename);
	stringstream ss;
	ss << file.rdbuf();
	return ss.str();
}

unsigned int loadShader(const char * filename, unsigned int shaderType)
{
	string source = loadFileToString(filename);
	const char * sources[1] = { source.c_str() };
	
	// Create and compile GLSL shader
	unsigned int shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, sources, nullptr);
	glCompileShader(shader);

	// Check GLSL shader compile status
	int status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		int lenght = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &lenght);
		
		char * log = new char[lenght];
		glGetShaderInfoLog(shader, lenght, nullptr, log);
		
		cout << log << endl;
		delete log;
		exit(1);
	}

	return shader;
}

unsigned int loadProgram(const char * vertexShader, const char * fragmentShader)
{
	// Load frament and vertex shaders (GLSL)
	unsigned int vs = loadShader(vertexShader, GL_VERTEX_SHADER);
	unsigned int fs = loadShader(fragmentShader, GL_FRAGMENT_SHADER);

	// Create GLSL program, attach shaders and compile it
	unsigned int program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

	// Check GLSL program link status
	int status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		int lenght = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &lenght);
		
		char * log = new char[lenght];
		glGetProgramInfoLog(program, lenght, nullptr, log);
		
		cout << log << endl;
		delete log;
		exit(1);
	}

	return program;
}

