// #include "Model.h"
#include "../include/View.h"
#include "../include/Control.h"
// #include "../include/SceneGraph.h"

Control c;
int no_of_models = 4;
std::vector<Model*> model;
int select_model = 0;

void handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
	select_model = c.get_model_no();
	c.handleKeys(window, key, code, action, mode, *model[select_model]);
	int map = model[select_model]->get_mapping();
	int tex = model[select_model]->get_texture();
	for(int i = 0; i < no_of_models; i++){
		if(select_model != i){
			model[i]->set_mapping(map);
			model[i]->set_texture(tex);
		}
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
	c.mouse_callback(window, xpos, ypos);
}

void createModel(Model &m, int i){
	m.createModel(i);
}

int filerd(std::ifstream &file, Model &m){
	if(!file){
    std::cout << "Unable to open file" << std::endl;	//if an error occurs
    return 0;
  }
  else{
    file >> m;
    file.close();
		return 1;
  }
}

void set_floor(Model &floor_model){
	floor_model.set_x(-0.25);
	floor_model.set_y(-0.5);
	floor_model.set_z(-2.5);
	floor_model.set_scale(8.0);
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods){
  c.MouseButtonCallback(window, button, action, mods, *model[c.get_model_no()]);
}

int main(){
	std::ifstream file1("./data/aeroplane.ply", std::ios::in);
	std::ifstream file2("./data/apple.ply", std::ios::in);
  std::ifstream file3("./data/cube1.ply", std::ios::in);
	std::ifstream file4("./data/canstick.ply", std::ios::in);
	// std::ifstream file5("./data/floor.ply", std::ios::in);
	// std::ifstream file5("./data/floor.ply", std::ios::in);

	// std::ifstream file2("apple.ply", std::ios::in);
  View view;
	for(int i = 0; i < no_of_models; i++){
		Model *tmp = new Model;
		model.push_back(tmp);
	}
	filerd(file1, *model[0]);
	filerd(file2, *model[1]);
	filerd(file3, *model[2]);
	filerd(file4, *model[3]);
	// filerd(file5, &model[4]);
	// filerd(file5, model[4]);

  const GLint WIDTH = 800, HEIGHT = 600;

  if(!glfwInit())
  {
    std::cout << "GLFW initialization failed." << std::endl;
    glfwTerminate();
    return 1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
  if(!mainWindow)
  {
    std::cout << "GLFW window creation failed." << std::endl;
    glfwTerminate();
    return 1;
  }


  const char* vShader = "./source/shader.vert";
  const char* fShader = "./source/shader.frag";
	const char* vLightShader = "./source/shaderlight.vert";
  const char* fLightShader = "./source/shaderlight.frag";
  GLuint uniformModel;
  // Get buffer size information
  int bufferWidth, bufferHeight;
  glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

  glfwMakeContextCurrent(mainWindow);

  glewExperimental = GL_TRUE;

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
  if(glewInit() != GLEW_OK)
  {
    std::cout << "GLEW initialization failed." << std::endl;
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
    return 1;
  }

  glViewport(0, 0, bufferWidth, bufferHeight);

	for(int i = 0; i < no_of_models; i++){
		createModel(*model[i], i);
	}

	SceneGraph *S = new SceneGraph(0, model);
	S->addNode(1, 0);
	S->addNode(2, 1);
	S->addNode(3, 2);
	// S->printNode(S->searchNode(3));

	// createModel(model[4], 2);
	// set_floor(model[4]);
  view.createShader(vShader, fShader);
	view.createLightShader(vLightShader, fLightShader);
	view.createFloor();
	model[0]->set_x(0.0);

	glfwSetCursorPosCallback(mainWindow, mouse_callback);
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  while(!glfwWindowShouldClose(mainWindow))
  {
    glfwPollEvents();
    glfwSetKeyCallback(mainWindow, handleKeys);
    glfwSetMouseButtonCallback(mainWindow, MouseButtonCallback);


    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		S->set_speed(c.get_speed());
		S->LevelOrderTraversal();
		view.set_cameraFront(c.get_cameraFront());
		view.drawFloor();
		for(int i = 0; i < no_of_models; i++){
			view.drawModel(*model[i]);
		}
		// view.drawModel(model[4]);

    glfwSwapBuffers(mainWindow);
  }
  return 0;
}
