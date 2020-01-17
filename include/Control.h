#ifndef Control_h_
#define Control_h_

#include "Model.h"

class Control{
public:
  Control();
  ~Control(){};
  void get_from_model(Model &model);
  void update_model(Model &model);
  int get_model_no() const;
  float get_speed() const;
  glm::vec3 get_cameraFront() const;
  void handleKeys(GLFWwindow* window, int key, int code, int action, int mode, Model &model);
  void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods, Model &model);
  void mouse_callback(GLFWwindow* window, double xpos, double ypos);
  float project_to_sphere(float r, float x, float y);
  void trackball();
  void build_rot_matrix();
  void circleAround(Model &model1, Model &model2, float time);

private:
  float scale, x, y, z, speed=0.01;
  float yaw   = -90.0f, pitch =  0.0f, lastX =  800.0f / 2.0, lastY =  600.0 / 2.0;
  int select_model = 0, select_mapping = 0, select_texture = 0;
  float radius = 0.25;
  glm::vec3 press_pos, release_pos;
  glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::quat quaternion;
  glm::mat4 rot_matrix;
  bool firstMouse = true;
};



#endif
