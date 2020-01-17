#ifndef SceneGraph_h_
#define SceneGraph_h_

#include "Model.h"

class SceneGraph{
private:
  struct Node
  {
    int key;
    std::vector<Node *>child;
  };

  Node *root;
  std::vector<Model*> models;
  float speed;

  Node *newNode(int key);
  void circleAround(int key, int ParentKey);
  void moveTowards(int key, int ParentKey);
  void jump(int key, int ParentKey);

public:
  SceneGraph(){};
  SceneGraph(int key, std::vector<Model*> m);
  // ~SceneGraph(){};
  Node *searchNode(int key);
  void addNode(int key, int ParentKey);
  void set_speed(float spd);
  void LevelOrderTraversal();
  void printNode(Node* node);
  void updateCurrentModel(int key, int ParentKey, int level);
  // void updateScene();
};

#endif
