#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include <GL/glew.h>

using namespace std;

void shaderInit();
string loadFileToString(const char * filename);
unsigned int loadShader(const char * filename, unsigned int shaderType);
unsigned int loadProgram(const char * vertexShader, const char * fragmentShader);

#endif
