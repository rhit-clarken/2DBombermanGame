#include "explosion_object.h"

ExplosionObject::ExplosionObject()
    : GameObject(), Done(false) { }

ExplosionObject::ExplosionObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite)
    : GameObject(pos, size, sprite, glm::vec3(1.0f), glm::vec2(0.0f)), InitialDT(1.5f), Done(false) { }

bool ExplosionObject::CheckDone() {
    return this->InitialDT <= 0.0f;
}