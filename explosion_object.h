#ifndef EXPLOSIONOBJECT_H
#define EXPLOSIONOBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "texture.h"


// ExplosionObject holds the state of the Explosion object inheriting
// relevant state data from GameObject. Contains some extra
// functionality specific to Explosion object that
// were too specific for within GameObject alone.
class ExplosionObject : public GameObject
{
public:
    // ball state	
    bool    Done;
    float   InitialDT;
    // constructor(s)
    ExplosionObject();
    ExplosionObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite);
    //check if explosion object should dissapear
    bool ExplosionObject::CheckDone();
};

#endif
