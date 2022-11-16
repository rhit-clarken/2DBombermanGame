#include "player_object.h"

PlayerObject::PlayerObject()
    : GameObject() {};

PlayerObject::PlayerObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite)
    : GameObject(pos, size, sprite, glm::vec3(1.0f)) 
{
    OldPosition = pos;
};