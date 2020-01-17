#ifndef Model_h_
#define Model_h_

#define PI 3.141592654
#define TWOPI 6.283185308

#include <bits/stdc++.h>
#include "../include/stb_image.h"
#include <GL/glew.h>
#include <GL/glu.h>
#include <cstdlib>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Model{
public:
  Model();
  ~Model(){};
  void createModel(int object_id);
  void createTextures(std::string filename, unsigned int &textureID);
  void SphericalMapping();
  float vrtx_to_sphree_scalingFactor(float vrtx_x, float vrtx_y, float vrtx_z, float radius);
  void linkSphericalMapping();
  void CylindricalMapping();
  void linkCylindricalMapping();
  void PlanarMapping();
  void linkPlanarMapping();
  void set_mapping(int map);
  int get_mapping() const;
  void link_mapping(int map);
  void set_texture(int tex);
  int get_texture() const;
  void link_texture(int tex);
  unsigned int get_VAO() const;
  // void createCircle();
  // void BindColor(unsigned int face);
  void LightModel();
  unsigned int get_lightVAO() const;
  glm::vec3 get_lightColor() const;
  void set_lightColor(glm::vec3 newColor);
  void setLightPos(glm::vec3 pos);
  glm::vec3 getLightPos() const;
  // unsigned int get_CVAO() const;
  int get_no_of_vertices() const;
  int get_no_of_indices() const;
  // int get_no_of_segments() const;
  void set_x(float x_val);
  void set_y(float y_val);
  void set_z(float z_val);
  void set_scale(float scale_val);
  void set_modelMat(glm::mat4 modMat);
  void set_angle(float ang);
  void set_rotAngle();
  void set_boolRot();
  float get_x() const;
  float get_y() const;
  float get_z() const;
  float get_scale() const;
  float get_y_max() const;
  float get_y_min() const;
  glm::mat4 get_modelMat() const;
  float get_angle() const;
  float get_rotAngle() const;
  void set_rot_matrix(glm::mat4 matrix);
  glm::mat4 get_rot_matrix() const;
  void normalize();
  void compute_normals();
  glm::vec3 triangleNormal(glm::vec3 vrtx1, glm::vec3 vrtx2, glm::vec3 vrtx3);
  // glm::vec3 triangleNormal(unsigned int face);
  glm::vec3 vec_subtract(glm::vec3 v1, glm::vec3 v2);
  // float angleBetween(glm::vec3 a, glm::vec3 b);
  // glm::vec3 get_val_translate(unsigned int face);
  // float get_val_rotate(unsigned int face);
  // float get_val_scale(unsigned int face);

  // Model(const Model& m);
  friend std :: istream& operator>>(std :: istream &is, Model &m);

private:
  float *vertices, max, *normals, *sperical_coords, *cylindrical_coods, *planar_coords, y_min, y_max, angle, vertical_angle;
  // float *circlepoints;
  unsigned int *indices;
  bool rotation;
  // unsigned int *circleindices;
  unsigned int VBO, VAO, EBO, VBO1, LVAO, LVBO, SVBO, CVBO, PVBO, textureIDs[4];
  // unsigned int CVAO, CVBO, CEBO, CVBO1;
  int no_of_vertices, no_of_indices, select_mapping, select_texture;// no_of_segments = 15;
  // float *circleColor = new float[3*(no_of_segments+1)];
  float scale, x, y, z;
  glm::mat4 rot_matrix, modelMAT;
  glm::vec3 lightColor, lightPos;

};

#endif
