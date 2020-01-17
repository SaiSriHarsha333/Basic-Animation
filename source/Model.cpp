#include "../include/Model.h"

Model::Model(){
  max = 0, y_min = 1, angle = 0.0, y_max = -1, vertical_angle = 0.0;
  scale = 1.0, x = 0.0, y = 0.0, z = 0.0;
  rot_matrix = glm::mat4(1);
  select_mapping = 0, select_texture = 0;
  modelMAT = glm::mat4(1.0f);
  rotation = false;
}

void Model::createModel(int object_id){
  x = -0.75 + 0.5*object_id;
  normalize();
  y = -0.5 - y_min;
  // set_modelMat(modelMAT);
  // std::cout << y_min << '\n';
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &VBO1);
  glGenBuffers(1, &EBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, 3*sizeof(float[no_of_vertices]), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3*sizeof(unsigned int[no_of_indices]), indices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  glBindBuffer(GL_ARRAY_BUFFER, VBO1);
  glBufferData(GL_ARRAY_BUFFER, 3*sizeof(float[no_of_vertices]), normals, GL_STATIC_DRAW);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  LightModel();
  createTextures("./data/wall.jpg", textureIDs[0]);
  createTextures("./data/chess.jpg", textureIDs[1]);
  createTextures("./data/face-04.jpg", textureIDs[2]);
  createTextures("./data/world.jpeg", textureIDs[3]);
  SphericalMapping();
  CylindricalMapping();
  PlanarMapping();
  linkCylindricalMapping();
  glBindVertexArray(VAO);
  glBindTexture(GL_TEXTURE_2D, textureIDs[0]);
  glBindVertexArray(0);
}

void Model::createTextures(std::string filename, unsigned int &textureID){
  glBindVertexArray(VAO);
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);
   // set the texture wrapping parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load image, create texture and generate mipmaps
  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
  unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
  if (data)
  {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
      std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);
  glBindVertexArray(0);
}

void Model::SphericalMapping(){
  float a;
  float radius = 2;
  float onSphereX, onSphereY, onSphereZ;
  sperical_coords = new float[2*no_of_vertices];
  for(int i = 0; i < no_of_vertices; i++){
    a = vrtx_to_sphree_scalingFactor(vertices[3*i], vertices[3*i+1], vertices[3*i+2], radius);
    onSphereX = a*vertices[3*i];
    onSphereY = a*vertices[3*i+1];
    onSphereZ = a*vertices[3*i+2];

    sperical_coords[2*i+1] = acos(onSphereY/radius) / PI;
    if (onSphereZ >= 0)
      sperical_coords[2*i] = acos(onSphereX/(radius * sin(PI*(sperical_coords[2*i+1])))) / TWOPI;
   else
      sperical_coords[2*i] = (PI + acos(onSphereX/(radius * sin(PI*(sperical_coords[2*i+1]))))) / TWOPI;
  }
  glBindVertexArray(VAO);
  glGenBuffers(1, &SVBO);
  glBindBuffer(GL_ARRAY_BUFFER, SVBO);
  glBufferData(GL_ARRAY_BUFFER, 2*sizeof(float[no_of_vertices]), sperical_coords, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

float Model::vrtx_to_sphree_scalingFactor(float vrtx_x, float vrtx_y, float vrtx_z, float radius){
  return sqrt((radius*radius)/((vrtx_x*vrtx_x)+(vrtx_y*vrtx_y)+(vrtx_z*vrtx_z)));
}

void Model::linkSphericalMapping(){
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, SVBO);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(2);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Model::CylindricalMapping(){
  cylindrical_coods = new float[2*no_of_vertices];
  float a;
  float onCylinderX, onCylinderY, onCylinderZ;
  float radius = 2.0, height = 2.0;
  for(int i = 0; i < no_of_vertices; i++){
    a = sqrt((radius*radius)/((vertices[3*i]*vertices[3*i])+(vertices[3*i+2]*vertices[3*i+2])));
    onCylinderX = a*vertices[3*i];
    onCylinderY = 4.0*vertices[3*i+1];
    onCylinderZ = a*vertices[3*i+2];

    cylindrical_coods[2*i] = (atan(onCylinderX/onCylinderZ)+(PI/2))/PI;
    cylindrical_coods[2*i+1] = (onCylinderY + (height/2))/height;
  }
  glBindVertexArray(VAO);
  glGenBuffers(1, &CVBO);
  glBindBuffer(GL_ARRAY_BUFFER, CVBO);
  glBufferData(GL_ARRAY_BUFFER, 2*sizeof(float[no_of_vertices]), cylindrical_coods, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Model::linkCylindricalMapping(){
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, CVBO);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(2);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Model::PlanarMapping(){
  planar_coords = new float[2*no_of_vertices];
  for(int i = 0; i < no_of_vertices; i++){
    planar_coords[2*i] = 4.0*vertices[3*i];
    planar_coords[2*i+1] = 4.0*vertices[3*i+1];
  }
  glBindVertexArray(VAO);
  glGenBuffers(1, &PVBO);
  glBindBuffer(GL_ARRAY_BUFFER, PVBO);
  glBufferData(GL_ARRAY_BUFFER, 2*sizeof(float[no_of_vertices]), planar_coords, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Model::linkPlanarMapping(){
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, PVBO);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(2);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Model::set_mapping(int map){
  select_mapping = map;
  link_mapping(select_mapping);
}

void Model::link_mapping(int map){
  if (map == 0) {
    linkCylindricalMapping();
  }
  else if(map == 1){
    linkSphericalMapping();
  }
  else{
    linkPlanarMapping();
  }
}

int Model::get_mapping() const{
  return select_mapping;
}

void Model::set_texture(int tex){
  select_texture = tex;
  link_texture(select_texture);
}

int Model::get_texture() const{
  return select_texture;
}

void Model::link_texture(int tex){
  glBindVertexArray(VAO);
  glBindTexture(GL_TEXTURE_2D, textureIDs[select_texture]);
  glBindVertexArray(0);
}

unsigned int Model::get_VAO() const{
  return VAO;
}

void Model::LightModel(){
  lightPos = glm::vec3(x, (0.5), z);
	float vertices[] = {lightPos.x, lightPos.y, lightPos.z};
  lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
  glGenVertexArrays(1, &LVAO);
	glBindVertexArray(LVAO);

	glGenBuffers(1, &LVBO);
	glBindBuffer(GL_ARRAY_BUFFER, LVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

unsigned int Model::get_lightVAO() const{
  return LVAO;
}

glm::vec3 Model::get_lightColor() const{
  return lightColor;
}

void Model::set_lightColor(glm::vec3 newColor){
  lightColor = newColor;
}

void Model::setLightPos(glm::vec3 pos){
  lightPos = pos;
}

glm::vec3 Model::getLightPos() const{
  return lightPos;
}

int Model::get_no_of_vertices() const{
  return no_of_vertices;
}

void Model::set_x(float x_val){
  x = x_val;
}

void Model::set_y(float y_val){
  y = y_val;
}

void Model::set_z(float z_val){
  z = z_val;
}

void Model::set_scale(float scale_val){
  scale = scale_val;
}

void Model::set_modelMat(glm::mat4 modMat){
  modelMAT = glm::translate(modMat, glm::vec3(x, y, z));
}

void Model::set_angle(float ang){
  angle = ang;
}

void Model::set_rotAngle(){
  if(rotation){
    vertical_angle += 0.1;
  }
}

void Model::set_boolRot(){
  if(rotation){
    rotation = false;
  }
  else{
    rotation = true;
  }
}

void Model::set_rot_matrix(glm::mat4 matrix){
  rot_matrix = matrix;
}

float Model::get_x() const{
  return x;
}

float Model::get_y() const{
  return y;
}

float Model::get_z() const{
  return z;
}

float Model::get_scale() const{
  return scale;
}

float Model::get_y_max() const{
  return y_max;
}

float Model::get_y_min() const{
  return y_min;
}

glm::mat4 Model::get_modelMat() const{
  return modelMAT;
}

float Model::get_angle() const{
  return angle;
}

float Model::get_rotAngle() const{
  return vertical_angle;
}

int Model::get_no_of_indices() const{
  return no_of_indices;
}


glm::mat4 Model::get_rot_matrix() const{
  return rot_matrix;
}

void Model::normalize(){
  for (int i = 0; i < (3*no_of_vertices); i++) {
    vertices[i] = vertices[i] / (max*4.0);
    // vertices[i] = vertices[i] / 4.0;
    if(i%3 == 1){
      if(vertices[i] < y_min){
        y_min = vertices[i];
      }
    }

    if(i%3 == 1){
      if(vertices[i] > y_max){
        y_max = vertices[i];
      }
    }
  }
}

void Model::compute_normals(){
  glm::vec3 vrtx1, vrtx2, vrtx3, N;
  normals = new float[3*no_of_vertices];
  float count;
  for (int i = 0; i < no_of_vertices; i++) {
    count = 0.0;
    N = glm::vec3(0,0,0);
    for(int j = 0; j < no_of_indices; j++){
      if (indices[3*j] == i || indices[3*j+1] == i || indices[3*j+2] == i) {
        vrtx1 = glm::vec3(vertices[3*(indices[3*j])], vertices[3*(indices[3*j])+1], vertices[3*(indices[3*j])+2]);
        vrtx2 = glm::vec3(vertices[3*(indices[3*j+1])], vertices[3*(indices[3*j+1])+1], vertices[3*(indices[3*j+1])+2]);
        vrtx3 = glm::vec3(vertices[3*(indices[3*j+2])], vertices[3*(indices[3*j+2])+1], vertices[3*(indices[3*j+2])+2]);
        N = N + triangleNormal(vrtx1, vrtx2, vrtx3);
        count = count + 1.0;
      }
    }

    N = N/count;
    N = glm::normalize(N);
    normals[3*i] = N.x;
    normals[3*i+1] = N.y;
    normals[3*i+2] = N.z;
  }
}

glm::vec3 Model::vec_subtract(glm::vec3 v1, glm::vec3 v2){
  return glm::vec3(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z);
}

glm::vec3 Model::triangleNormal(glm::vec3 vrtx1, glm::vec3 vrtx2, glm::vec3 vrtx3){
  return glm::cross(vec_subtract(vrtx2, vrtx1), vec_subtract(vrtx3, vrtx1));
}

std :: istream& operator>>(std :: istream &is, Model &m){
  std::string str;
  unsigned int index1, index2, index3;
  float cood, tmp;
  is >> str;
  is >> str >> str >> tmp;
  is >> str >> str >> m.no_of_vertices;
  is >> str >> str >> str;
  is >> str >> str >> str;
  is >> str >> str >> str;
  is >> str >> str >> m.no_of_indices;
  is >> str >> str >> str >> str >> str;
  is >> str;
  m.vertices = new float[3*m.no_of_vertices];
  m.indices = new unsigned int[3*m.no_of_indices];
  for (int i = 0; i < (3*m.no_of_vertices); i++) {
    is >> cood;
    m.vertices[i] = cood;
    if (abs(cood) > m.max) {
      m.max = abs(cood);
    }
  }
  for (int i = 0; i < m.no_of_indices; i++) {
    is >> tmp >> index1 >> index2 >> index3;
    m.indices[(3*i)] = index1;
    m.indices[(3*i+1)] = index2;
    m.indices[(3*i+2)] = index3;
  }
  m.compute_normals();
}

// void Model::createCircle(){
//   circlepoints = new float[3*no_of_segments+3];
//   circleindices = new unsigned int[3*no_of_segments];
//   float theta;
//   circlepoints[0] = 0.0;
//   circlepoints[1] = 0.0;
//   circlepoints[2] = 0.0;
//   for(int i = 1; i <=no_of_segments; i++){
//     theta = 2.0f * 3.1415926f * float(i) / float(no_of_segments);
//     circlepoints[3*i] = glm::cos(theta);
//     circlepoints[3*i+1] = glm::sin(theta);
//     circlepoints[3*i+2] = 0.0;
//   }
//   for(int i = 0; i < no_of_segments; i++){
//     circleindices[3*i] = 0;
//     circleindices[3*i+1] = i+1;
//     circleindices[3*i+2] = i+2;
//   }
//   circleindices[3*no_of_segments-1] = 1;
//
//   glGenVertexArrays(1, &CVAO);
//   glGenBuffers(1, &CVBO);
//   glGenBuffers(1, &VBO1);
//   glGenBuffers(1, &CEBO);
//   glBindVertexArray(CVAO);
//   glBindBuffer(GL_ARRAY_BUFFER, CVBO);
//   glBufferData(GL_ARRAY_BUFFER, 3*sizeof(float[no_of_segments+1]), circlepoints, GL_STATIC_DRAW);
//   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CEBO);
//   // std::cout << glGetError() << std::endl;
//   glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3*sizeof(unsigned int[no_of_segments]), circleindices, GL_STATIC_DRAW);
//
//   glEnableVertexAttribArray(0);
//   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//
//   glBindBuffer(GL_ARRAY_BUFFER, 0);
//   // std::cout << glGetError() << std::endl;
//   glBindVertexArray(0);
// }

// void Model::BindColor(unsigned int face){
//   glm::vec3 vrtx1, vrtx2, vrtx3, N, NT;
//   vrtx1 = glm::vec3(vertices[3*(indices[3*face])], vertices[3*(indices[3*face])+1], vertices[3*(indices[3*face])+2]);
//   vrtx2 = glm::vec3(vertices[3*(indices[3*face+1])], vertices[3*(indices[3*face+1])+1], vertices[3*(indices[3*face+1])+2]);
//   vrtx3 = glm::vec3(vertices[3*(indices[3*face+2])], vertices[3*(indices[3*face+2])+1], vertices[3*(indices[3*face+2])+2]);
//   N = triangleNormal(vrtx1, vrtx2, vrtx3);
//   NT = glm::normalize(N);
//   for(int i = 0; i <= no_of_segments; i++){
//     circleColor[3*i] = abs(NT.x);
//     circleColor[3*i+1] = abs(NT.y);
//     circleColor[3*i+2] = abs(NT.z);
//     // std::cout << circleColor[3*i] << '\n';
//   }
//   glBindVertexArray(CVAO);
//   glBindBuffer(GL_ARRAY_BUFFER, CVBO1);
//   // std::cout << glGetError() << std::endl;
//   glBufferData(GL_ARRAY_BUFFER, 3*sizeof(float[no_of_segments+1]), circleColor, GL_STATIC_DRAW);
//
//   glEnableVertexAttribArray(1);
//   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//   // std::cout << glGetError() << std::endl;
//   glBindBuffer(GL_ARRAY_BUFFER, 0);
//   // std::cout << glGetError() << std::endl;
//   glBindVertexArray(0);
// }

// unsigned int Model::get_CVAO() const{
//   return CVAO;
// }

// int Model::get_no_of_segments() const{
//   return no_of_segments;
// }

// glm::vec3 Model::triangleNormal(unsigned int face){
//   glm::vec3 vrtx1, vrtx2, vrtx3;
//   vrtx1 = glm::vec3(vertices[3*(indices[3*face])], vertices[3*(indices[3*face])+1], vertices[3*(indices[3*face])+2]);
//   vrtx2 = glm::vec3(vertices[3*(indices[3*face+1])], vertices[3*(indices[3*face+1])+1], vertices[3*(indices[3*face+1])+2]);
//   vrtx3 = glm::vec3(vertices[3*(indices[3*face+2])], vertices[3*(indices[3*face+2])+1], vertices[3*(indices[3*face+2])+2]);
//   return glm::cross(vec_subtract(vrtx2, vrtx1), vec_subtract(vrtx3, vrtx1));
// }

// float Model::angleBetween(glm::vec3 a, glm::vec3 b){
//  glm::vec3 da=glm::normalize(a);
//  glm::vec3 db=glm::normalize(b);
//  return glm::acos(glm::dot(da, db));
// }

// glm::vec3 Model::get_val_translate(unsigned int face){
//   float x, y, z, length12, length23, length31, perimeter;
//   glm::vec3 vrtx1, vrtx2, vrtx3;
//   vrtx1 = glm::vec3(vertices[3*(indices[3*face])], vertices[3*(indices[3*face])+1], vertices[3*(indices[3*face])+2]);
//   vrtx2 = glm::vec3(vertices[3*(indices[3*face+1])], vertices[3*(indices[3*face+1])+1], vertices[3*(indices[3*face+1])+2]);
//   vrtx3 = glm::vec3(vertices[3*(indices[3*face+2])], vertices[3*(indices[3*face+2])+1], vertices[3*(indices[3*face+2])+2]);
//   length12 = glm::distance(vrtx1, vrtx2);
//   length23 = glm::distance(vrtx2, vrtx3);
//   length31 = glm::distance(vrtx3, vrtx1);
//   perimeter = length12 + length23 + length31;
//   x = (((length23*vrtx1.x) + (length31*vrtx2.x) + (length12*vrtx3.x))/perimeter)*scale;
//   y = (((length23*vrtx1.y) + (length31*vrtx2.y) + (length12*vrtx3.y))/perimeter)*scale;
//   z = (((length23*vrtx1.z) + (length31*vrtx2.z) + (length12*vrtx3.z))/perimeter)*scale;
//   return glm::vec3(x, y, z);
// }

// float Model::get_val_rotate(unsigned int face){
//   glm::vec3 vrtx1, vrtx2, vrtx3, incentre;
//   vrtx1 = glm::vec3(vertices[3*(indices[3*face])], vertices[3*(indices[3*face])+1], vertices[3*(indices[3*face])+2]);
//   vrtx2 = glm::vec3(vertices[3*(indices[3*face+1])], vertices[3*(indices[3*face+1])+1], vertices[3*(indices[3*face+1])+2]);
//   vrtx3 = glm::vec3(vertices[3*(indices[3*face+2])], vertices[3*(indices[3*face+2])+1], vertices[3*(indices[3*face+2])+2]);
//   return angleBetween(triangleNormal(vrtx1, vrtx2, vrtx3), glm::vec3(0, 0, 1));
// }

// float Model::get_val_scale(unsigned int face){
//   float k, length12, length23, length31, semiperimeter;
//   glm::vec3 vrtx1, vrtx2, vrtx3;
//   vrtx1 = glm::vec3(vertices[3*(indices[3*face])], vertices[3*(indices[3*face])+1], vertices[3*(indices[3*face])+2]);
//   vrtx2 = glm::vec3(vertices[3*(indices[3*face+1])], vertices[3*(indices[3*face+1])+1], vertices[3*(indices[3*face+1])+2]);
//   vrtx3 = glm::vec3(vertices[3*(indices[3*face+2])], vertices[3*(indices[3*face+2])+1], vertices[3*(indices[3*face+2])+2]);
//   length12 = glm::distance(vrtx1, vrtx2);
//   length23 = glm::distance(vrtx2, vrtx3);
//   length31 = glm::distance(vrtx3, vrtx1);
//   semiperimeter = (length12 + length23 + length31)/2;
//   k = sqrt(semiperimeter*(semiperimeter-length12)*(semiperimeter-length23)*(semiperimeter-length31));
//   return (k/semiperimeter);
// }
