#include "enemy_object.h"
#include <iostream>
using namespace std;

EnemyObject::EnemyObject()
    : GameObject() {};

EnemyObject::EnemyObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, bool vertical)
    : GameObject(pos, size, sprite, color)
{
    this->Vertical = vertical;
    this->randomDirection();
};

glm::vec2 EnemyObject::Move(float dt)
{
    // move the object
    if (this->Direction == 0) { //LEFT
        Position.x -= dt;
    }
    if (this->Direction == 1) { //RIGHT
        Position.x += dt;
    }
    if (this->Direction == 2) { //UP
        Position.y += dt;
    }
    if (this->Direction == 3) { //DOWN
        Position.y -= dt;
    }
    return this->Position;
}

void EnemyObject::randomDirection() {
    if (this->Vertical) { //enemy can move any direction initally
        this->Direction = (rand() % 4);
    }
    else { //enemy can only move left to right
        this->Direction = (rand() % 2);
    }
}
void EnemyObject::changeDirection() {
    //reverse directions
    if (this->Direction == 0) { //LEFT
        this->Direction = 1;
    }
    else if (this->Direction == 1) { //RIGHT
        this->Direction = 0;
    }
    else if (this->Direction == 2) { //UP
        this->Direction = 3;
    }
    else if (this->Direction == 3) { //DOWN
        this->Direction = 2;
    }
}