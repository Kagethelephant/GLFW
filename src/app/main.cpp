#include <bits/types/struct_timeval.h>
#include <glad/glad.h>
#include <GL/glext.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <string>
#include <sstream>
#include <fstream>



// Resize the viewport on window resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
   // When the window is resized, resize the viewport
   glViewport(0, 0, width, height);
} 

// Get keyboard inputs
void processInput(GLFWwindow *window){
   // See if the current key pressed "GLFW_PRESS" is the escape key
   if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}




std::string readShaderFile(const std::string& filePath) {
    std::ifstream shaderFile(filePath);
    if (!shaderFile.is_open()) {
        std::cerr << "Could not open shader file: " << filePath << std::endl;
        return "";
    }
    std::string line;
    std::string shaderCode;
    while (std::getline(shaderFile, line)) {
        shaderCode += line + '\n';
    }
    shaderFile.close();
    return shaderCode;
}


// Main function
int main(){

   // ----------------------------- CREATE WINDOW AND OpenGL CONEXT -------------------------------
   // Create a window and 
   GLFWwindow* window;

   // Initialize GLFW
   if(!glfwInit()){return 1;}      

   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // The 4 in opengl v4.5
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5); // The 5 in opengl v4.5
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Ensure that we use the core profile

   // Get the aspect ratio from the primary monitor
   const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
   int window_width = mode->width;
   int window_height = mode->height;

   // Create window and catch errors
   window = glfwCreateWindow(window_width, window_height, "The Game", glfwGetPrimaryMonitor(), NULL);
   if (window == NULL){
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      return -1;
   }

   // Make the created window the current context
   glfwMakeContextCurrent(window);

   // Load glad
   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
      std::cout << "Failed to initialize GLAD" << std::endl;
      return -1;
   }

   // Tell OpenGL the viewport is equal to the window coordinates
   glViewport(0, 0, window_width, window_height);

   // Set the callback function for the window being resized
   glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 

   

   // ------------------------------ INITIALIZE VERTEX BUFFER -------------------------------
   float vertices[] = {
      0.5f,  0.5f, 0.0f,  // top right
      0.5f, -0.5f, 0.0f,  // bottom right
      -0.5f, -0.5f, 0.0f,  // bottom left
      -0.5f,  0.5f, 0.0f   // top left 
   };
   unsigned int indices[] = {  // note that we start from 0!
      0, 1, 3,   // first triangle
      1, 2, 3    // second triangle
   }; 


   // Create and generate an ID for the VBO (Vertex Buffer Object)
   unsigned int VBO;
   glGenBuffers(1,&VBO);

   // Bind the ID with the GL_ARRAY_BUFFER (this reflects the type of a VBO)
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   // Assign vertex data to the VBO with the size of the array, the data and how to draw it
   // GL_STATIC_DRAW is for objects that do not update often. GL_DYNAMIC_DRAW is for objects that do
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   
   // Create an element buffer object, this contains the indexes for the vertices for a triangle
   unsigned int EBO;
   glGenBuffers(1, &EBO);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 



   // ------------------------------ CREATE VERTEX SHADER -------------------------------
   // Work around string vertex shader that just adds a 4th value to the vertex
   std::string vertexSource = readShaderFile("../src/shaders/vertex.glsl").c_str();
   const char *vertexShaderSource = vertexSource.c_str();

   // Create a shder object for the vertex shader
   unsigned int vertexShader;
   vertexShader = glCreateShader(GL_VERTEX_SHADER);
   // This attatches the shader string to the shader, 2nd parameter is how many strings
   glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
   glCompileShader(vertexShader);

   // Check if the compilation of the shader was successfull
   int  success;
   glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

   // If the compilation was not successful then retrieve the error message
   char infoLog[512];
   if(!success){
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
   }


   // ------------------------------ CREATE FRAGMENT SHADER -------------------------------
   // Work around string vertex shader that just adds a 4th value to the vertex
   std::string fragmentSource = readShaderFile("../src/shaders/fragment.glsl").c_str();
   const char *fragmentShaderSource = fragmentSource.c_str();
   // const char *fragmentShaderSource = FileToString("../src/shaders/fragment.glsl");
   // Create a shder object for the vertex shader
   unsigned int fragmentShader;
   fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
   // This attatches the shader string to the shader, 2nd parameter is how many strings
   glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
   glCompileShader(fragmentShader);

   // Check if the compilation of the shader was successfull
   glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

   // If the compilation was not successful then retrieve the error message
   if(!success){
      glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
   }

   // ------------------------------ CREATE SHADER PROGRAM -------------------------------
   // Create the shader program to link all of the shders together for excecution
   unsigned int shaderProgram;
   shaderProgram = glCreateProgram();

   // Attatch both of the shaders to the program and link the program
   glAttachShader(shaderProgram, vertexShader);
   glAttachShader(shaderProgram, fragmentShader);
   glLinkProgram(shaderProgram); // This links all the previously attatched shaders with outputs and inputs

   // Check to see if the compilation of the program was successful
   glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
   if(!success) {
      glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
   }

   // Tell OpenGL to use the shader program (every shader and rendering call after this will use this program)
   glUseProgram(shaderProgram);
   // After the shaders are linked in the program we no longer need them
   glDeleteShader(vertexShader);
   glDeleteShader(fragmentShader);  

   // This difines how the shaders should interperate the vertex data
   // First parameter: the start position of the vertex data
   // Second: The size of the data eg. float
   // Third: The size of the vertex attribute eg. 3 for a 3D vertex
   // Fourth: This specifies if we want to normalize the data (between -1 and 1)
   // Fifth: The span between data in bytes
   // Last: This is the offset of where the position data begins in the buffer of type void*
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
   // glEnableVertexAttribArray(0);  

   // This is the VAO that is used to bind the VBO
   unsigned int VAO;
   glGenVertexArrays(1, &VAO);  


   // ..:: Initialization code :: ..
   // 1. bind Vertex Array Object
   glBindVertexArray(VAO);
   // 2. copy our vertices array in a vertex buffer for OpenGL to use
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
   // 3. copy our index array in a element buffer for OpenGL to use
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
   // 4. then set the vertex attributes pointers
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);  


   // ------------------------------ MAIN WINDOW LOOP ---------------------------------
   // Main loop for the window
   while(!glfwWindowShouldClose(window)){

      // Get keyboard inputs
      processInput(window);

      // render
      // clear the colorbuffer
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      // be sure to activate the shader
      glUseProgram(shaderProgram);

      // update the uniform color
      float timeValue = glfwGetTime();
      float greenValue = sin(timeValue) / 2.0f + 0.5f;
      int vertexColorLocation = glGetUniformLocation(shaderProgram, "vertexColor");
      glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);


      glBindVertexArray(VAO);
      // Change the draw mode to only draw lines
      // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      // Since we are using an EBO use draw elements instead of array
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
      // glBindVertexArray(0);
      //


      glfwSwapBuffers(window);
      // glfwWaitEvents(); // This will wait until there is an event to restart the loop
      glfwPollEvents(); // This restarts the loop regardless of an event
   }

   std::cout << "-------- PROGRAM END ----------" << '\n';


   // Clear all of the GLFW assets
   glfwTerminate();
   return 0;
}
