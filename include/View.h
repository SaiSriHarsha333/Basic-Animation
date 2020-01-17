#ifndef View_h_
#define View_h_

// #include <bits/stdc++.h>
// #include <GL/glew.h>
#include "Model.h"
#include "SceneGraph.h"


class View{
public:
  View();
  ~View(){};
  void addShader(const char* shaderIDCode, GLenum shaderIDType, GLuint &ShaderID_);
  void compileShader(const char* vertexCode, const char* fragmentCode, GLuint &ShaderID_);
  std::string readFile(const char* fileLocation);
  void createShader(const char* vertexLocation, const char* fragmentLocation);
  void createLightShader(const char* vertexLocation, const char* fragmentLocation);
  // void LightModel();
  unsigned int get_shaderID() const;
  unsigned int get_LightShaderID() const;
  void set_cameraFront(glm::vec3 front);
  void createFloor();
  void drawFloor();
  void drawModel(Model &m);

private:
  unsigned int shaderID, lightShaderID, floorVAO, floorVBO, floorEBO;
  glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
  glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);
};

#endif
