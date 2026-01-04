#pragma once

#include <glad/glad.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <string>
#include <fstream>


GLFWwindow* gl_initWindow();

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow *window);

std::string readShaderFile(const std::string& filePath);

GLuint gl_createVertShader(std::string filename);

GLuint gl_createFragShader(std::string filename);
