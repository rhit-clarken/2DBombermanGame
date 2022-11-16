#ifndef BOMBOBJECT_H
#define BOMBOBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "texture.h"


// BallObject holds the state of the Ball object inheriting
// relevant state data from GameObject. Contains some extra
// functionality specific to ball object that
// were too specific for within GameObject alone.
class BombObject : public GameObject
{
public:
    // ball state	
    float   Radius;
    bool    Exploded;
    float   InitialDT;
    // constructor(s)
    BombObject();
    BombObject(glm::vec2 pos, float radius, Texture2D sprite);
    //check if bomb object should explode
    bool BombObject::CheckExplode();
};

#endif
