//                      Invisible Maze
//  Montgomery Young          ProjectID: Final Project
//
#include "gameobject.h"

int GameObject::next_id_ = 1;

//bool GameObject::operator ==(const GameObject* rhs) {
    //return (id_ == rhs.id_ );
//    return (this->id_ == rhs->id_);
//}


GameObject::GameObject() {
  id_ = next_id_;
  next_id_++;
  listener_to_notify_ = NULL;
}

GameObject::GameObject(int id) {
  id_ = id;
  listener_to_notify_ = NULL;
}

int GameObject::id() {
  return id_;
}

void GameObject::NotifyListener() {
  if (listener_to_notify_ != NULL) listener_to_notify_(this);
}




