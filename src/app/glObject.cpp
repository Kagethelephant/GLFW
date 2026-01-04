#include "glObject.hpp"

#include "utils/matrix.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

gl_vao::gl_vao(){
   // This is the VAO that is used to bind the VBO
   glGenVertexArrays(1, &vao);  
}


unsigned int gl_vao::createVBO (std::vector<GLfloat> _vertices){
   // Create and generate an ID for the VBO (Vertex Buffer Object)
   GLuint vbo;
   glGenBuffers(1,&vbo);

   gl_object newObject;
   newObject.vbo = vbo;
   newObject.vertices = _vertices;

   unsigned int index = objects.size();
   objects.push_back(newObject);

   return index;
}


unsigned int gl_vao::createVBO (std::vector<GLfloat> _vertices, std::vector<GLint> _indices){

   // Create and generate an ID for the VBO (Vertex Buffer Object)
   GLuint vbo;
   glGenBuffers(1,&vbo);
   // Create an element buffer object, this contains the indexes for the vertices for a triangle
   GLuint ebo;
   glGenBuffers(1, &ebo);

   gl_object newObject;
   newObject.vbo = vbo;
   newObject.ebo = ebo;
   newObject.isEbo = true;
   newObject.vertices = _vertices;
   newObject.indices = _indices;

   unsigned int index = objects.size();
   objects.push_back(newObject);

   return index;
}


void gl_vao::addAttribute(unsigned int vbo, unsigned int id, unsigned int count, int type, int normalized, std::size_t stride, void* offset){

   gl_object::attribute attrib;
   attrib.id = id;
   attrib.count = count;
   attrib.type = type;
   attrib.normalized = normalized;
   attrib.stride = stride;
   attrib.offset = offset;

   objects[vbo].isAttribute = true;

   objects[vbo].attributes.push_back(attrib);
}


void gl_vao::bindObjects(){

   // Bind Vertex Array Object
   glBindVertexArray(vao);

   std::cout << "BIND START" << std::endl;
   for(gl_object object : objects){
      
      // Setup the VBO using the VAO
      glBindBuffer(GL_ARRAY_BUFFER, object.vbo);
      glBufferData(GL_ARRAY_BUFFER, object.vertices.size() * sizeof(GLfloat), object.vertices.data(), GL_STATIC_DRAW);

      // Set up the attributes for the vertices (how is the data aranged)
      // 1) Shader layout location, 2) Qty of vert attributes, 3) Size of attribute, 4) normaliize btwn -1 to 1, 5)span btwn verts in bytes, 6) start of buffer
      if (object.isAttribute){
         for (gl_object::attribute attribute : object.attributes){
            glVertexAttribPointer(attribute.id, attribute.count, attribute.type, attribute.normalized, attribute.stride, attribute.offset);
            // This tells GL to use the vertex attributes defined above (it does not do this by default)
            glEnableVertexAttribArray(attribute.id);  
         }
      }
      // Setup the EBO using the VAO
      if (object.isEbo){
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object.ebo);
         glBufferData(GL_ELEMENT_ARRAY_BUFFER, object.indices.size() * sizeof(GLint), object.indices.data(), GL_STATIC_DRAW);
      }
   }
}


unsigned int gl_vao::load(std::string filename) {

   // Create and generate an ID for the VBO (Vertex Buffer Object)
   GLuint vbo;
   glGenBuffers(1,&vbo);
   // Create an element buffer object, this contains the indexes for the vertices for a triangle
   GLuint ebo;
   glGenBuffers(1, &ebo);

   gl_object newObject;
   newObject.vbo = vbo;
   newObject.ebo = ebo;
   newObject.isEbo = true;
   
   // for (int i=0; i < vertices.size()/3; i++){
   //    newObject.vertices.push_back(vertices[i*3]);
   //    newObject.vertices.push_back(vertices[i*3+1]);
   //    newObject.vertices.push_back(vertices[i*3+2]);
   //    std::cout << "x: " << vertices[i*3] <<" y: " << vertices[i*3+1] << " z: " << vertices[i*3+2] << std::endl;

   //    newObject.vertices.push_back(0);
   //    newObject.vertices.push_back(0);
   //    newObject.vertices.push_back(0);
   // }

   // Try to open the file
   std::ifstream obj(filename);

   // Create an array to hold the chars of each line
   // cycle through all the lines in the file until we are at the end
   while(!obj.eof()) {
      // Create a char array to store the line from the file
      char line[128];
      obj.getline(line,128);
      // Pass the line from the file "stream" into the line
      std::stringstream stream;
      stream << line;
      // Check if the line is a vertice or a triangle
      char junk;
      if(line[0] == 'v') {
         // If it is a vertice then pull the xyz values from the string and put it in the vert array
         GLfloat x,y,z;
         stream >> junk >> x >> y >> z;
         newObject.vertices.push_back(x);
         newObject.vertices.push_back(y);
         newObject.vertices.push_back(z);

         // newObject.vertices.push_back(0);
         // newObject.vertices.push_back(0);
         // newObject.vertices.push_back(0);
      }
      if(line[0] == 'f') {
         // If it is a triangle then get the corosponding vertices and load it into the mesh
         GLint v0,v1,v2;
         stream >> junk >> v0 >> v1 >> v2;
         newObject.indices.push_back(v0-1);
         newObject.indices.push_back(v1-1);
         newObject.indices.push_back(v2-1);
      }
   }

   // for (int i=0; i<newObject.indices.size()/3; i++){
   //    int index0 = newObject.indices[i*3];
   //    std::cout << "index0: " << newObject.indices[i*3] << std::endl;
   //    int index1 = newObject.indices[i*3+1];
   //    std::cout << "index1: " << newObject.indices[i*3+1] << std::endl;
   //    int index2 = newObject.indices[i*3+2];
   //    std::cout << "index2: " << newObject.indices[i*3+2] << std::endl;

   //    vec3 v0(newObject.vertices[index0*6],newObject.vertices[index0*6+1],newObject.vertices[index0*6+2]);
   //    std::cout << "x: " << v0.x <<" y: " << v0.y << " z: " << v0.z << std::endl;
   //    vec3 v1(newObject.vertices[index1*6],newObject.vertices[index1*6+1],newObject.vertices[index1*6+2]);
   //    std::cout << "x: " << v1.x <<" y: " << v1.y << " z: " << v1.z << std::endl;
   //    vec3 v2(newObject.vertices[index2*6],newObject.vertices[index2*6+1],newObject.vertices[index2*6+2]);
   //    std::cout << "x: " << v2.x <<" y: " << v2.y << " z: " << v2.z << std::endl;

   //    vec3 normal((v1-v0).cross(v2-v0));
   //    std::cout << "x: " << normal.x <<" y: " << normal.y << " z: " << normal.z << std::endl;
   //    newObject.triNormals.push_back(normal);

   //    newObject.vertices[index0*6+3] += normal.x; newObject.vertices[index0*6+4] += normal.y; newObject.vertices[index0*6+5] += normal.z;
   //    newObject.vertices[index1*6+3] += normal.x; newObject.vertices[index1*6+4] += normal.y; newObject.vertices[index1*6+5] += normal.z;
   //    newObject.vertices[index2*6+3] += normal.x; newObject.vertices[index2*6+4] += normal.y; newObject.vertices[index2*6+5] += normal.z;
   // }

   // for (int i=0; i<newObject.vertices.size()/6; i++){
   //    vec3 normalized(newObject.vertices[i*6+3],newObject.vertices[i*6+4],newObject.vertices[i*6+5]);
   //    normalized.normalize();
   //    newObject.vertices[i*6+3] = normalized.x;
   //    newObject.vertices[i*6+4] = normalized.y;
   //    newObject.vertices[i*6+5] = normalized.z;
   //    std::cout << "vertice-----------------" << std::endl;
   //    std::cout << "x: " << newObject.vertices[i*6] <<" y: " << newObject.vertices[i*6+1] << " z: " << newObject.vertices[i*6+2] << std::endl;
   //    std::cout << "normal" << std::endl;
   //    std::cout << "x: " << newObject.vertices[i*6+3] <<" y: " << newObject.vertices[i*6+4] << " z: " << newObject.vertices[i*6+5] << std::endl;
   // }



   unsigned int index = objects.size();
   objects.push_back(newObject);

   return index;
}


void gl_vao::bind(){
   glBindVertexArray(vao);
}
