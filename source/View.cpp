#include "../include/View.h"

View::View(){

}

void View::addShader(const char* shaderIDCode, GLenum shaderIDType, GLuint &ShaderID_){
  GLuint theShader = glCreateShader(shaderIDType);
  // std::cout << glGetError() << std::endl;

	const GLchar* theCode[1];
	theCode[0] = shaderIDCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderIDCode);

	glShaderSource(theShader, 1, theCode, codeLength);
  // std::cout << glGetError() << std::endl;
	glCompileShader(theShader);
  // std::cout << glGetError() << std::endl;

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d shaderID: '%s'\n", shaderIDType, eLog);
		return;
	}

	glAttachShader(ShaderID_, theShader);
  // std::cout << glGetError() << std::endl;
}

void View::compileShader(const char* vertexCode, const char* fragmentCode, GLuint &ShaderID_){
  ShaderID_ = glCreateProgram();
  // std::cout << shaderID << '\n';

  if(!ShaderID_)
  {
    std::cout << "Error creating shaderID." << std::endl;
    return;
  }

  addShader(vertexCode, GL_VERTEX_SHADER, ShaderID_);
  addShader(fragmentCode, GL_FRAGMENT_SHADER, ShaderID_);

  GLint result = 0;
  GLchar eLog[1024] = { 0 };

  glLinkProgram(ShaderID_);
  // std::cout << glGetError() << std::endl;
  glGetProgramiv(ShaderID_, GL_LINK_STATUS, &result);
  if (!result)
  {
    glGetProgramInfoLog(ShaderID_, sizeof(eLog), NULL, eLog);
    printf("Error linking program: '%s'\n", eLog);
    return;
  }

  glValidateProgram(ShaderID_);
  // std::cout << glGetError() << std::endl;
  glGetProgramiv(ShaderID_, GL_VALIDATE_STATUS, &result);
  if (!result)
  {
    glGetProgramInfoLog(ShaderID_, sizeof(eLog), NULL, eLog);
    printf("Error validating program: '%s'\n", eLog);
    return;
  }
}

std::string View::readFile(const char* fileLocation){
  std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);

	if (!fileStream.is_open()) {
		printf("Failed to read %s! File doesn't exist.", fileLocation);
		return "";
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

void View::createShader(const char* vertexLocation, const char* fragmentLocation){
  std::string vertexString = readFile(vertexLocation);
	std::string fragmentString = readFile(fragmentLocation);
	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	compileShader(vertexCode, fragmentCode, shaderID);
}

void View::createLightShader(const char* vertexLocation, const char* fragmentLocation){
  std::string vertexString = readFile(vertexLocation);
	std::string fragmentString = readFile(fragmentLocation);
	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	compileShader(vertexCode, fragmentCode, lightShaderID);
}

unsigned int View::get_shaderID() const{
  return shaderID;
}

unsigned int View::get_LightShaderID() const{
  return lightShaderID;
}

void View::set_cameraFront(glm::vec3 front){
  cameraFront = front;
}

void View::createFloor(){
  float vertices[] = {
    -1.25, -0.5, 1.25,
    1.25, -0.5, 1.25,
    1.25, -0.5, -1.25,
    -1.25, -0.5, -1.25
  };
  unsigned int indices[] = {
      0, 1, 3,
      1, 2, 3
  };
  glGenVertexArrays(1, &floorVAO);
  glGenBuffers(1, &floorVBO);
  glGenBuffers(1, &floorEBO);
  glBindVertexArray(floorVAO);
  glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floorEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void View::drawFloor(){
  glUseProgram(get_shaderID());
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glm::mat4 view = glm::mat4(1.0f);
  view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
  glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
  glUseProgram(get_shaderID());
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  GLuint uniformModel = glGetUniformLocation(get_shaderID(), "modelMAT");
  GLuint uniformView = glGetUniformLocation(get_shaderID(), "view");
  GLuint uniformProjection = glGetUniformLocation(get_shaderID(), "projection");
  GLuint uniformModelColor = glGetUniformLocation(get_shaderID(), "lightColor");
  GLuint uniformLightPos = glGetUniformLocation(get_shaderID(), "lightPos");
  glm::mat4 modelMAT = glm::mat4(1.0);
  glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMAT));
  glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
  glUniform4f(uniformModelColor, 1.0, 1.0, 1.0f, 1.0f);
  glUniform3f(uniformLightPos, 0.0, 1.0, 0.0);

  glBindVertexArray(floorVAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

  glUseProgram(0);
}


void View::drawModel(Model &m){
  glm::vec3 LightColor = m.get_lightColor();
  if(LightColor == glm::vec3(1, 1, 1)){

    glUseProgram(get_LightShaderID());
    GLuint uniformLightModel = glGetUniformLocation(get_LightShaderID(), "light_model");
    GLuint uniformLightView = glGetUniformLocation(get_LightShaderID(), "view");
    GLuint uniformLightProjection = glGetUniformLocation(get_LightShaderID(), "projection");
    GLuint uniformLightColor = glGetUniformLocation(get_LightShaderID(), "lightColor");
    glm::mat4 Lightmodel = glm::mat4(1);
    glm::vec3 LightPos = m.getLightPos();
    float radius = 4.0f;
    // float camX   = sin(glfwGetTime()) * radius;
    // float camZ   = cos(glfwGetTime()) * radius;
    // std::cout << camX << " " << camZ << '\n';
    // float camX = 0.0f;
    // float camY = 0.0f;
    // float camZ = 3.0f;
    // glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
    // glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    // glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
    glUniformMatrix4fv(uniformLightModel, 1, GL_FALSE, glm::value_ptr(Lightmodel));
    glUniformMatrix4fv(uniformLightView, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(uniformLightProjection, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform4f(uniformLightColor, LightColor.x, LightColor.y, LightColor.z, 1.0f);

    glBindVertexArray(m.get_lightVAO());
    glDrawArrays(GL_POINTS, 0, 1);
    glBindVertexArray(0);
    glUseProgram(0);

    glUseProgram(get_shaderID());
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    GLuint uniformModel = glGetUniformLocation(get_shaderID(), "modelMAT");
    GLuint uniformView = glGetUniformLocation(get_shaderID(), "view");
    GLuint uniformProjection = glGetUniformLocation(get_shaderID(), "projection");
    GLuint uniformModelColor = glGetUniformLocation(get_shaderID(), "lightColor");
    GLuint uniformLightPos = glGetUniformLocation(get_shaderID(), "lightPos");
    glm::mat4 modelMAT = m.get_modelMat();
    modelMAT = glm::translate(modelMAT, glm::vec3(m.get_x(), m.get_y(), m.get_z()));
    modelMAT = modelMAT * m.get_rot_matrix();
    modelMAT = glm::rotate(modelMAT, m.get_rotAngle(), glm::vec3(0.0, 1.0, 0.0));
    modelMAT = glm::scale(modelMAT, glm::vec3(m.get_scale(), m.get_scale(), m.get_scale()));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMAT));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform4f(uniformModelColor, LightColor.x, LightColor.y, LightColor.z, 1.0f);
    glUniform3f(uniformLightPos, LightPos.x, LightPos.y, LightPos.z);
    // std::cout << glGetError() << 6 << std::endl;

    glBindVertexArray(m.get_VAO());
    glDrawElements(GL_TRIANGLES, 3*m.get_no_of_indices(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glUseProgram(0);
  }
}
