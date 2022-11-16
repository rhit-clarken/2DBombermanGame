#ifndef PLAYEROBJECT_H
#define PLAYEROBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "texture.h"


// PlayerObject holds the state of the Player object inheriting
// relevant state data from GameObject. Contains some extra
// functionality specific to Player object that
// were too specific for within GameObject alone.
class PlayerObject : public GameObject
{
public:
    // constructor(s)
    PlayerObject();
    PlayerObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite);
};

#endif
