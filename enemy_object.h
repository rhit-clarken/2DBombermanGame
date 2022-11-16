#ifndef ENEMYOBJECT_H
#define ENEMYOBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "texture.h"


// PlayerObject holds the state of the Player object inheriting
// relevant state data from GameObject. Contains some extra
// functionality specific to Player object that
// were too specific for within GameObject alone.
class EnemyObject : public GameObject
{
public:
    int Direction;
    bool Vertical;
    // constructor(s)
    EnemyObject();
    EnemyObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, bool vertical);
    // moves the ball, keeping it constrained within the window bounds (except bottom edge); returns new position
    glm::vec2 Move(float dt);
    //rendomizes Enemies initial direction
    void randomDirection();
    void changeDirection();
    // resets the ball to original state with given position and velocity
    //void      Reset(glm::vec2 position, glm::vec2 velocity);
};

#endif
