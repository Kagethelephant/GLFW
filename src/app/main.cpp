#include <glad/glad.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <string>
#include "gl.hpp"
#include "glObject.hpp"
#include "utils/matrix.hpp"
#include <vector>


int main(){
   std::vector<GLfloat> quadVertices = {
      // pos      // tex
      -1.0f,  1.0f, 0.0f, 1.0f,
      -1.0f, -1.0f, 0.0f, 0.0f,
      1.0f, -1.0f, 1.0f, 0.0f,

      -1.0f,  1.0f, 0.0f, 1.0f,
      1.0f, -1.0f, 1.0f, 0.0f,
      1.0f,  1.0f, 1.0f, 1.0f
   };
   // ----------------------------- CREATE WINDOW AND OpenGL CONEXT -------------------------------
   GLFWwindow* window = gl_initWindow();

   const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
   int window_width = mode->width;
   int window_height = mode->height;


   // ------------------------------ CREATE SHADER PROGRAM -------------------------------
   // Create the shader program to link all of the shders together for excecution
   GLuint shaderProgram = glCreateProgram();
   GLuint vertexShader =  gl_createVertShader("../src/shaders/vertex.glsl");
   GLuint fragmentShader =  gl_createFragShader("../src/shaders/fragment.glsl");

   // Attatch both of the shaders to the program and link the program
   glAttachShader(shaderProgram, vertexShader);
   glAttachShader(shaderProgram, fragmentShader);
   glLinkProgram(shaderProgram); // This links all the previously attatched shaders with outputs and inputs

   int  success;
   char infoLog[512];
   glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
   if(!success) {
      glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
      std::cout << "--- Program 1 Failed Compilation ---" << infoLog <<  std::endl;
   }
   // After the shaders are linked in the program we no longer need them
   glDeleteShader(vertexShader);
   glDeleteShader(fragmentShader);  

   // ------------------------------ CREATE SHADER PROGRAM -------------------------------
   // Create the shader program to link all of the shders together for excecution
   GLuint UIshaderProgram = glCreateProgram();
   GLuint UIvertexShader =  gl_createVertShader("../src/shaders/uiVertex.glsl");
   GLuint UIfragmentShader =  gl_createFragShader("../src/shaders/uiFragment.glsl");

   // Attatch both of the shaders to the program and link the program
   glAttachShader(UIshaderProgram, UIvertexShader);
   glAttachShader(UIshaderProgram, UIfragmentShader);
   glLinkProgram(UIshaderProgram); // This links all the previously attatched shaders with outputs and inputs

   glGetProgramiv(UIshaderProgram, GL_LINK_STATUS, &success);
   if(!success) {
      glGetProgramInfoLog(UIshaderProgram, 512, NULL, infoLog);
      std::cout << "--- Program 2 Failed Compilation ---" << infoLog << std::endl;
   }
   // After the shaders are linked in the program we no longer need them
   glDeleteShader(UIvertexShader);
   glDeleteShader(UIfragmentShader);  


   gl_vao vao;
   
   // unsigned int vbo = vao.createVBO(vertices,indices);
   unsigned int vbo = vao.load("../resources/cow.obj");
   unsigned int verticeCount = vao.objects[vbo].indices.size();
   // 1) Id position of attribute. if you have 3 floats for position and 3 for color, position would be 0 and color 2
   // 2) Number of components in attribute, eg position with 3 floats would be 3
   // 3) data type eg. GL_FLOAT
   // 4) ar the points normalized btwn -1 and 1 (GL_TRUE) or do they need to be normalized (GL_FALSE)
   // 5) Stride. The distance in bytes bwn the start of one attribute and start of the next
   // 6) offset for the first vertex for that atribute in bytes
   // needs to be called for each attribute
   vao.addAttribute(vbo, 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
   // vao.addAttribute(vbo, 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));

   vao.bindObjects();

   gl_vao UIvao;
   
   // unsigned int vbo = vao.createVBO(vertices,indices);
   unsigned int UIvbo = UIvao.createVBO(quadVertices);
   unsigned int UIverticeCount = UIvao.objects[UIvbo].vertices.size();
   // 1) Id position of attribute. if you have 3 floats for position and 3 for color, position would be 0 and color 2
   // 2) Number of components in attribute, eg position with 3 floats would be 3
   // 3) data type eg. GL_FLOAT
   // 4) ar the points normalized btwn -1 and 1 (GL_TRUE) or do they need to be normalized (GL_FALSE)
   // 5) Stride. The distance in bytes bwn the start of one attribute and start of the next
   // 6) offset for the first vertex for that atribute in bytes
   // needs to be called for each attribute
   UIvao.addAttribute(UIvbo, 0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
   UIvao.addAttribute(UIvbo, 1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2*sizeof(GLfloat)));

   UIvao.bindObjects();


   vec3 camPos(0.0f,0.0f,0.0f);
   vec3 camForward(0.0f,0.0f,1.0f);
   vec3 camUp(0.0f,1.0f,0.0f);

   vec3 objPos(0.0f, 0.2f, -5.0f);
   vec3 objRot(0.5f, 0.3f, 0.0f);

   camForward *= matrix_transform(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

   mat4x4 scale = matrix_scale(0.5f, 0.5f, 0.5f);
   mat4x4 transform = matrix_transform(objPos.x, objPos.y, objPos.z, objRot.x, objRot.y, objRot.z);
   mat4x4 lookAt = matrix_pointAt(camPos, camForward, camUp);
   mat4x4 view = matrix_view(lookAt);
   mat4x4 project = matrix_project(70.0f, (float)window_width/(float)window_height, 0.1f, 1000.0f);

   float lightPos[3]{0.0f,0.0f,0.0f};
   float lightColor[3]{1.6f,1.0f,1.8f};
   float objColor[3]{0.6f,0.4f,0.2f};

   GLuint fbheight = 400;
   GLuint fbwidth = 500;

   GLuint fbo;
   glGenFramebuffers(1, &fbo);
   glBindFramebuffer(GL_FRAMEBUFFER, fbo);
   // generate texture
   GLuint textureColorbuffer;
   glGenTextures(1, &textureColorbuffer);
   glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fbwidth, fbheight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   // attach it to currently bound framebuffer object
   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);  

   GLuint depthTex;
   glGenTextures(1, &depthTex);
   glBindTexture(GL_TEXTURE_2D, depthTex);
   glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT24,fbwidth, fbheight,0,GL_DEPTH_COMPONENT,GL_FLOAT,nullptr);
   // REQUIRED settings for depth textures
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   // Attach depth texture
   glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,depthTex,0);

   GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
   glDrawBuffers(1, drawBuffers);
   
   if(!(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)){
      std::cout <<"FRAME BUFFER NOT COMPLETE" << std::endl;
      return -1;
   }
   
   // Reset to default frame buffer and texture
   glBindTexture(GL_TEXTURE_2D, 0);
   glBindFramebuffer(GL_FRAMEBUFFER, 0);

   // ------------------------------ MAIN WINDOW LOOP ---------------------------------
   // Main loop for the window
   while(!glfwWindowShouldClose(window)){

      // Get keyboard inputs
      processInput(window);

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
         objRot.y += 0.01;
         transform = matrix_transform(objPos.x, objPos.y, objPos.z, objRot.x, objRot.y, objRot.z);
      }
      if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
         objRot.y -= 0.01;
         transform = matrix_transform(objPos.x, objPos.y, objPos.z, objRot.x, objRot.y, objRot.z);
      }
      if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
         objRot.x += 0.01;
         transform = matrix_transform(objPos.x, objPos.y, objPos.z, objRot.x, objRot.y, objRot.z);
      }
      if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
         objRot.x -= 0.01;
         transform = matrix_transform(objPos.x, objPos.y, objPos.z, objRot.x, objRot.y, objRot.z);
      }

      glBindFramebuffer(GL_FRAMEBUFFER, fbo);
      glViewport(0, 0, fbwidth, fbheight);
      glUseProgram(shaderProgram);
      vao.bind();
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LESS);
      glDepthMask(GL_TRUE);
      glClearColor(0.0f, 0.5f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // update the uniform color
      int m_scale = glGetUniformLocation(shaderProgram, "scale");
      glUniformMatrix4fv(m_scale,1,GL_FALSE,&scale.m[0][0]);

      int m_trans = glGetUniformLocation(shaderProgram, "transform");
      glUniformMatrix4fv(m_trans,1,GL_FALSE,&transform.m[0][0]);

      int m_view = glGetUniformLocation(shaderProgram, "view");
      glUniformMatrix4fv(m_view,1,GL_FALSE,&view.m[0][0]);

      int m_project = glGetUniformLocation(shaderProgram, "project");
      glUniformMatrix4fv(m_project,1,GL_FALSE,&project.m[0][0]);


      int m_light = glGetUniformLocation(shaderProgram, "light");
      glUniform3fv(m_light,1,&lightPos[0]);

      int m_lightCol = glGetUniformLocation(shaderProgram, "lightCol");
      glUniform3fv(m_lightCol,1,&lightColor[0]);

      int m_objCol = glGetUniformLocation(shaderProgram, "objCol");
      glUniform3fv(m_objCol,1,&objColor[0]);


      glDrawElements(GL_TRIANGLES, verticeCount, GL_UNSIGNED_INT, 0);


      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      // Set viewport to window size
      glViewport(0, 0, window_width, window_height);
      glDisable(GL_DEPTH_TEST);
      glDisable(GL_CULL_FACE);
      // Tell OpenGL to use the shader program (every shader and rendering call after this will use this program)
      glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      glUseProgram(UIshaderProgram);
      UIvao.bind();
      // glBindFramebuffer(GL_FRAMEBUFFER, 0);
      // trigger mipmaps generation explicitly
      // NOTE: If GL_GENERATE_MIPMAP is set to GL_TRUE, then glCopyTexSubImage2D()
      // triggers mipmap generation automatically. However, the texture attached
      // onto a FBO should generate mipmaps manually via glGenerateMipmap().
      
      glBindVertexArray(UIvao.vao);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
      glDrawArrays(GL_TRIANGLES, 0, 6);

      // glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
      // glGenerateMipmap(GL_TEXTURE_2D);
      // glBindTexture(GL_TEXTURE_2D, 0);
      // glUniform1i(glGetUniformLocation(UIshaderProgram, "screenTexture"), 0);
      // glDrawArrays(GL_TRIANGLES, 0, 6);

      glfwSwapBuffers(window);
      // glfwWaitEvents(); // This will wait until there is an event to restart the loop
      glfwPollEvents(); // This restarts the loop regardless of an event
   }

   glDeleteFramebuffers(1, &fbo);  
   // Clear all of the GLFW assets
   glfwTerminate();
   return 0;
}
