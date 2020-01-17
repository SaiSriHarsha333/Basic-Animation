#include "../include/SceneGraph.h"

SceneGraph::SceneGraph(int key, std::vector<Model*> m){
  root = newNode(key);
  for(int i = 0; i < m.size(); i++){
    Model *tmp = m[i];
    models.push_back(tmp);
  }
  // speed = 0.01;
}

SceneGraph::Node* SceneGraph::newNode(int key){
  Node *temp = new Node;
  temp->key = key;
  return temp;
}

SceneGraph::Node* SceneGraph::searchNode(int key){
  if (root==NULL){return NULL;}
  std::queue<Node *> q;
  q.push(root);
  while (!q.empty()){
    int n = q.size();
    while (n > 0){
      Node *p = q.front();
      q.pop();
      if(p->key == key){
        // std::cout << p->key << '\n';
        return p;
      }
      for(int i=0; i<p->child.size(); i++){
        q.push(p->child[i]);
      }
      n--;
    }
    // std::cout << std::endl;
  }
  return NULL;
}

void SceneGraph::addNode(int key, int ParentKey){
  Node *parent = searchNode(ParentKey);
  parent->child.push_back(newNode(key));
}

void SceneGraph::set_speed(float spd){
  speed = spd;
}

void SceneGraph::LevelOrderTraversal(){
  if (root==NULL){return;}
  std::queue<Node *> q;
  q.push(root);
  models[root->key]->set_rotAngle();
  int level = 1;
  while (!q.empty()){
    int n = q.size();
    while (n > 0){
      Node *p = q.front();
      q.pop();
      // std::cout << p->key << ' ';
      // printNode(p);
      for (int i=0; i<p->child.size(); i++){
        updateCurrentModel(p->child[i]->key, p->key, level);
        models[p->child[i]->key]->set_rotAngle();
        q.push(p->child[i]);
      }
      n--;
    }
    level++;
  }
}

void SceneGraph::printNode(SceneGraph::Node* node){
  std::cout << node->key << '\n';
}

void SceneGraph::updateCurrentModel(int key, int ParentKey, int level){
  if(level == 1){
    circleAround(key, ParentKey);
  }
  else if(level == 2){
    moveTowards(key, ParentKey);
  }
  else if(level == 3){
    jump(key, ParentKey);
  }
}

void SceneGraph::circleAround(int key, int ParentKey){
  float theta = models[key]->get_angle();
  theta += speed;
  float x = models[ParentKey]->get_x() + (1.0 * sin(theta));
  x += 0.1 * sin(10*theta);
  float z = models[ParentKey]->get_z() + (1.0 * cos(theta));
  // z += 0.05 * sin(10*theta);
  models[key]->set_x(x);
  models[key]->set_y(models[ParentKey]->get_y());
  models[key]->set_z(z);
  models[key]->set_angle(theta);
}

void SceneGraph::moveTowards(int key, int ParentKey){
  float px = models[ParentKey]->get_x();
  float pz = models[ParentKey]->get_z();
  float kx = models[key]->get_x();
  float kz = models[key]->get_z();

  float dist = sqrt(((kx - px)*(kx - px)) + ((kz - pz)*(kz - pz)));
  double alpha = (speed/(2*dist));
  float x = (((1.0 - alpha)*kx) + (alpha * px));
  float z = (((1.0 - alpha)*kz) + (alpha * pz));
  models[key]->set_x(x);
  // models[key]->set_y(models[ParentKey]->get_y());
  models[key]->set_z(z);
}

void SceneGraph::jump(int key, int ParentKey){
  models[key]->set_x(models[ParentKey]->get_x());
  models[key]->set_z(models[ParentKey]->get_z());
  float py_max = models[ParentKey]->get_y_max();
  float py = models[ParentKey]->get_y();
  float ky_min = models[key]->get_y_min();
  float y = models[key]->get_y();
  float theta = models[key]->get_angle();
  y = py_max + py - ky_min + 0.25;
  y += 0.25 * cos(5*theta);
  theta += speed;
  models[key]->set_y(y);
  models[key]->set_angle(theta);
}
