#pragma once

#include "utils/matrix.hpp"
#include <cstddef>
#include <glad/glad.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <string>
#include <sys/types.h>
#include <vector>


class gl_vao {

public:

   gl_vao();

   unsigned int createVBO (std::vector<GLfloat>);
   unsigned int createVBO (std::vector<GLfloat>, std::vector<GLint>);

   unsigned int load (std::string filename);

   void addAttribute(unsigned int object, unsigned int id, unsigned int count, int type, int normalized, std::size_t stride, void* offset);

   void bindObjects();
   void bind();

   GLuint vao;

   struct gl_object {
      
      GLuint vbo;
      GLuint ebo;

      bool isEbo = false;
      bool isAttribute = false;
      
      std::vector<GLfloat> vertices; 
      std::vector<GLint> indices; 
      // std::vector<vec3> triNormals; 

      struct attribute {

         unsigned int id;
         unsigned int count;
         int type;
         int normalized;
         std::size_t  stride;
         void* offset;
      };

      std::vector<attribute> attributes;
   };

   std::vector<gl_object> objects;

private:

};
