#include "../include/Control.h"

Control::Control(){

}

void Control::get_from_model(Model &model){
  x = model.get_x();
  y = model.get_y();
  z = model.get_z();
  scale = model.get_scale();
  rot_matrix = model.get_rot_matrix();
  select_mapping = model.get_mapping();
  select_texture = model.get_texture();
}

void Control::update_model(Model &model){
  model.set_x(x);
  model.set_y(y);
  model.set_z(z);
  model.set_scale(scale);
  model.set_rot_matrix(rot_matrix);
  model.set_mapping(select_mapping);
  model.set_texture(select_texture);
}

int Control::get_model_no() const{
  return select_model;
}

float Control::get_speed() const{
  return speed;
}

glm::vec3 Control::get_cameraFront() const{
  return cameraFront;
}

void Control::handleKeys(GLFWwindow* window, int key, int code, int action, int mode, Model &model){
  get_from_model(model);
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key == GLFW_KEY_EQUAL && action == GLFW_PRESS)
	{
		scale += 0.05;
	}

	if (key == GLFW_KEY_MINUS && action == GLFW_PRESS)
	{
		scale -= 0.05;
	}

	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
		x -= 0.05;
	}

	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		x += 0.05;
	}

	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		y += 0.05;
	}

	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		y -= 0.05;
	}

  if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		select_model = 0;
	}

  if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		select_model = 1;
	}

  if (key == GLFW_KEY_3 && action == GLFW_PRESS)
	{
		select_model = 2;
	}

  if (key == GLFW_KEY_4 && action == GLFW_PRESS)
	{
		select_model = 3;
	}

  if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		model.set_boolRot();
	}

  if (key == GLFW_KEY_M && action == GLFW_PRESS)
	{
		if(select_mapping < 2){
      select_mapping++;
    }
    else{
      select_mapping = 0;
    }
	}

  if (key == GLFW_KEY_T && action == GLFW_PRESS)
	{
		if(select_texture < 3){
      select_texture++;
    }
    else{
      select_texture = 0;
    }
	}

  if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if(model.get_lightColor().x == 1){
      model.set_lightColor(glm::vec3(0, 0, 0));
    }
    else{
      model.set_lightColor(glm::vec3(1, 1, 1));
    }
	}

  if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		speed += 0.01;
	}

  if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
    if(speed >= 0.01){
		    speed -= 0.01;
    }
    else{
      speed = 0.00;
    }
	}
  update_model(model);
}

void Control::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods, Model &model){
	double modelview[16], projection[16];
	double xpos, ypos, xfinal, yfinal, zfinal;
	int viewport[4];
  get_from_model(model);


	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		glfwGetCursorPos(window, &xpos, &ypos);
		ypos = 600 - ypos;
		glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
		glGetDoublev(GL_PROJECTION_MATRIX, projection);
		glGetIntegerv(GL_VIEWPORT, viewport);
		gluUnProject(xpos, ypos, 0.0, modelview, projection, viewport, &xfinal, &yfinal, &zfinal);
		// xfinal = xfinal*scale + 3;
		// yfinal = yfinal*scale + 3;
		// std::cout << "left button click Position at (" << xfinal << " : " << yfinal << " : " << zfinal << std::endl;

	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		glfwGetCursorPos(window, &xpos, &ypos);
		ypos = 600 - ypos;
		glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
		glGetDoublev(GL_PROJECTION_MATRIX, projection);
		glGetIntegerv(GL_VIEWPORT, viewport);
		gluUnProject(xpos, ypos, 0.0, modelview, projection, viewport, &xfinal, &yfinal, &zfinal);
		// xfinal = xfinal*scale + 3;
		// yfinal = yfinal*scale + 3;
		// std::cout << "right button click Position at (" << xfinal << " : " << yfinal << std::endl;
    zfinal = project_to_sphere(radius, xfinal, yfinal);
    press_pos = glm::vec3(xfinal, yfinal, zfinal);
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
		glfwGetCursorPos(window, &xpos, &ypos);
		ypos = 600 - ypos;
		glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
		glGetDoublev(GL_PROJECTION_MATRIX, projection);
		glGetIntegerv(GL_VIEWPORT, viewport);
		gluUnProject(xpos, ypos, 0.0, modelview, projection, viewport, &xfinal, &yfinal, &zfinal);
		// xfinal = xfinal*scale;
		// yfinal = yfinal*scale;
		// std::cout << "right button release Position at (" << xfinal << " : " << yfinal << std::endl;
    zfinal = project_to_sphere(radius, xfinal, yfinal);
    release_pos = glm::vec3(xfinal, yfinal, zfinal);
    trackball();
	}
  update_model(model);
}

void Control::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

float Control::project_to_sphere(float r, float x, float y){
  float d, t, z;
    d = sqrt(x*x + y*y);
    if (d < r * 0.70710678118654752440) {    /* Inside sphere */
      z = sqrt(r*r - d*d);
    } else {           /* On hyperbola */
      t = r / 1.41421356237309504880;
      z = t*t / d;
    }
    return z;
}

void Control::trackball(){
  if (press_pos.x == release_pos.x && press_pos.y == release_pos.y) {
  /* Zero rotation */
    quaternion.w = 1;
    quaternion.x = 0;
    quaternion.y = 0;
    quaternion.z = 0;
    return;
  }
  glm::vec3 cross_product = glm::cross(press_pos, release_pos);
  float dist = glm::distance(press_pos, release_pos)/2;
  if (dist > 1.0) {dist = 1.0;}
  if (dist < -1.0) {dist = -1.0;}
  float angle = 2.0 * glm::asin(dist);
  cross_product = glm::normalize(cross_product);
  quaternion.x = cross_product.x;
  quaternion.y = cross_product.y;
  quaternion.z = cross_product.z;
  quaternion.w = glm::cos(angle/2.0);
  float v_norm = sqrt(quaternion.x*quaternion.x+quaternion.y*quaternion.y+quaternion.z*quaternion.z+quaternion.w*quaternion.w);
  quaternion.x /= v_norm ;
  quaternion.y /= v_norm ;
  quaternion.z /= v_norm ;
  quaternion.w /= v_norm ;
  //glm::normalize(quaternion);
  build_rot_matrix();
}

void Control::build_rot_matrix()
{
    rot_matrix[0][0] = 1.0 - 2.0 * (quaternion.y * quaternion.y + quaternion.z * quaternion.z);
    rot_matrix[0][1] = 2.0 * (quaternion.x * quaternion.y - quaternion.z * quaternion.w);
    rot_matrix[0][2] = 2.0 * (quaternion.z * quaternion.x + quaternion.y * quaternion.w);
    rot_matrix[0][3] = 0.0;

    rot_matrix[1][0] = 2.0 * (quaternion.x * quaternion.y + quaternion.z * quaternion.w);
    rot_matrix[1][1]= 1.0 - 2.0 * (quaternion.z * quaternion.z + quaternion.x * quaternion.x);
    rot_matrix[1][2] = 2.0 * (quaternion.y * quaternion.z - quaternion.x * quaternion.w);
    rot_matrix[1][3] = 0.0;

    rot_matrix[2][0] = 2.0 * (quaternion.z * quaternion.x - quaternion.y * quaternion.w);
    rot_matrix[2][1] = 2.0 * (quaternion.y * quaternion.z + quaternion.x * quaternion.w);
    rot_matrix[2][2] = 1.0 - 2.0 * (quaternion.y * quaternion.y + quaternion.x * quaternion.x);
    rot_matrix[2][3] = 0.0;

    rot_matrix[3][0] = 0.0;
    rot_matrix[3][1] = 0.0;
    rot_matrix[3][2] = 0.0;
    rot_matrix[3][3] = 1.0;
}

// void Control::circleAround(Model &model1, Model &model2, float time){
//   get_from_model(model2);
//
//   x = model1.get_x() + (1.0 * sin(speed));
//   z = model1.get_z() + (1.0 * cos(speed));
//   update_model(model2);
//   speed += 0.01;
// }
