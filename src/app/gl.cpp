#include "gl.hpp"

#include <glad/glad.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <string>
#include <fstream>

GLFWwindow* gl_initWindow(){
   // Start up GLFW
   glfwInit();

   // Set the GLFW version and use CORE profile (only modern GLFW commands)
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   // Get the aspect ratio from the primary monitor
   const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
   int window_width = mode->width;
   int window_height = mode->height;

   // Create GLFW window
   GLFWwindow* window = glfwCreateWindow(window_width, window_height, "The Game", glfwGetPrimaryMonitor(), NULL);
   // Sets this window as the context so all window functions modify this window
   glfwMakeContextCurrent(window);

   // Load glad. Glad loads GLFW functions and pointers at runtime.
   gladLoadGL();

   // Create viewport. Specifies the area in the window to draw things
   glViewport(0, 0, window_width, window_height);
   // Callback function to resize the viewport when the window resizes during glfwPollEvents
   glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 

   return window;
}

GLuint gl_createVertShader(std::string filename){
   // ------------------------------ CREATE VERTEX SHADER -------------------------------
   // Work around string vertex shader that just adds a 4th value to the vertex
   std::string vertexSource = readShaderFile(filename).c_str();
   const char *vertexShaderSource = vertexSource.c_str();

   // Create a shder object for the vertex shader
   GLuint vertexShader;
   vertexShader = glCreateShader(GL_VERTEX_SHADER);
   // This attatches the shader string to the shader, 2nd parameter is how many strings
   glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
   glCompileShader(vertexShader);

   // Check if the compilation of the shader was successfull
   int  success;
   char infoLog[512];
   glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
   // If the compilation was not successful then retrieve the error message
   if(!success){
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      std::cout << filename << "--- Failed Compilation ---" << infoLog <<  std::endl;
   }

   return vertexShader;
}




GLuint gl_createFragShader(std::string filename){
   // ------------------------------ CREATE FRAGMENT SHADER -------------------------------
   // Work around string vertex shader that just adds a 4th value to the vertex
   std::string fragmentSource = readShaderFile(filename).c_str();
   const char *fragmentShaderSource = fragmentSource.c_str();
   // Create a shder object for the vertex shader
   GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
   // This attatches the shader string to the shader, 2nd parameter is how many strings
   glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
   glCompileShader(fragmentShader);

   int  success;
   char infoLog[512];

   // Check if the compilation of the shader was successfull
   glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
   // If the compilation was not successful then retrieve the error message
   if(!success){
      glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
      std::cout << filename << "--- Failed Compilation ---" << infoLog <<  std::endl;
   }
   return fragmentShader;
}



// Resize the viewport on window resize during the pollEvents
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
   glViewport(0, 0, width, height);
} 


// Get keyboard inputs
void processInput(GLFWwindow *window){
   if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}


// Function to parse shader files
std::string readShaderFile(const std::string& filePath) {
   // Input file stream
   std::ifstream shaderFile(filePath);
   if (!shaderFile.is_open()) {
      std::cerr << "Could not open shader file: " << filePath << std::endl;
      return "";
   }
   // Copy all lines into "shaderCode" as one long string
   std::string line;
   std::string shaderCode;
   while (std::getline(shaderFile, line)) {
      shaderCode += line + '\n';
   }
   shaderFile.close();
   return shaderCode;
}
