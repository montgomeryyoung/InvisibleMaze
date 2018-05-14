//                      Invisible Maze
//  Montgomery Young          ProjectID: Final Project
//
#ifndef INVISIBLEMAZE_GAMEOBJECT_H
#define INVISIBLEMAZE_GAMEOBJECT_H

#include <functional>

class GameData;

// An abstract class for all types of objects that are used to make an
// invisible maze.  This class manages notifying a listener when a GameObject
// changes and the basic functionality for connections between GameObjects.
//
class GameObject {
 public:
  //bool operator ==(const GameObject* rhs);
  friend class GameData;
  GameObject();

  //Creates a GameObject to have a particular id. This is needed if a client wants
  //to use a find function on a collection of GameObjects, since the id is the
  //only identifier and normally, the id is set automatically by a static counter.
  //
  //Param:  (id) The override for the id_ value.
  GameObject(int id);
  virtual ~GameObject() {};
  int id();
 protected:
  //Calls the function identified by listener_to_notify_.
  void NotifyListener();

  GameObject* left_connection_;
  GameObject* up_connection_;
  GameObject* right_connection_;
  GameObject* down_connection_;

 private:
  //Sets the 4 connection member variables to initial values. Derived classes
  //have a better idea of which need to be set to nullptr and which will get the
  //default value of pointing to itself.
  //
  //Setting to nullptr means the connection doesn't logically exist.
  //(i.e. A piece has no connection in that direction, or the piece is against
  //an outer border of the maze.)
  virtual void InitializeConnections() = 0;

  std::function<void(GameObject*)> listener_to_notify_;
  static int next_id_;
  int id_;
};

#endif // INVISIBLEMAZE_GAMEOBJECT_H
