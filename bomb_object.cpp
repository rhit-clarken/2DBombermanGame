#include "bomb_object.h"


BombObject::BombObject()
    : GameObject(), Radius(12.5f), Exploded(false) { }

BombObject::BombObject(glm::vec2 pos, float radius, Texture2D sprite)
    : GameObject(pos, glm::vec2(radius * 2.0f, radius * 2.0f), sprite, glm::vec3(1.0f), glm::vec2(0.0f)), Radius(radius), InitialDT(3.0f), Exploded(false) { }

bool BombObject::CheckExplode() {
    return (this->InitialDT <= 0.0f);
}