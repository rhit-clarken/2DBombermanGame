#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "game_object.h"
#include "bomb_object.h"
#include "player_object.h"
#include "explosion_object.h"
#include "text_renderer.h"
#include <iostream>
using namespace std;


// Game-related State data
SpriteRenderer* Renderer;
PlayerObject* MapPlayer;
std::vector<BombObject*> BombList;
std::vector<ExplosionObject*> ExplosionList;
std::vector<EnemyObject*> EnemyList;
GameLevel* Map;
TextRenderer* Text;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_MENU), Keys(), KeysProcessed(), Width(width), Height(height)
{

}

Game::~Game()
{
    delete Renderer;
    for (BombObject* bomb : BombList) {
        delete bomb;
    }
    for (ExplosionObject* explosion : ExplosionList) {
        delete explosion;
    }
    for (EnemyObject* enemy : EnemyList) {
        delete enemy;
    }
    delete MapPlayer;
    delete Map;
    delete Text;
}

void Game::Init()
{
    // load shaders
    ResourceManager::LoadShader("C:/Users/clarken/Desktop/winter 22-23/computer graphics/sprite.vs", "C:/Users/clarken/Desktop/winter 22-23/computer graphics/sprite.fs", nullptr, "sprite");
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
        static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // set render-specific controls
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    Text = new TextRenderer(this->Width, this->Height);
    Text->Load("C:/Users/clarken/Desktop/winter 22-23/computer graphics/OCRAEXT.TTF", 24);
    // load textures
    ResourceManager::LoadTexture("C:/Users/clarken/Desktop/winter 22-23/computer graphics/background.jpg", false, "background");
    ResourceManager::LoadTexture("C:/Users/clarken/Desktop/winter 22-23/computer graphics/awesomeface.png", true, "face");
    ResourceManager::LoadTexture("C:/Users/clarken/Desktop/winter 22-23/computer graphics/block.png", false, "block");
    ResourceManager::LoadTexture("C:/Users/clarken/Desktop/winter 22-23/computer graphics/block_solid.png", false, "block_solid");
    ResourceManager::LoadTexture("C:/Users/clarken/Desktop/winter 22-23/computer graphics/paddle.png", true, "paddle");
    // load level
    GameLevel one;
    Map = &one;
    one.Load("C:/Users/clarken/Desktop/winter 22-23/computer graphics/levels/one.lvl", this->Width, this->Height);
    this->Levels.push_back(one);
    this->Level = 0;
    // configure game objects
    glm::vec2 playerPos = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y - 0.001f);
    MapPlayer = new PlayerObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("block_solid"));
    ResetLevel();
}

void Game::Update(float dt)
{
    // update objects
    // move enemies
    for (EnemyObject* enemy : this->Levels[this->Level].Enemies) {
        float velocity = (PLAYER_VELOCITY * dt)/10;
        int num_steps = 20;
        float step_speed = velocity / num_steps;
        for (int i = 0; i < num_steps; i++) {
            enemy->Move(step_speed);
            this->DoCollisions();
        }
    }
    // check if bombs should explode
    for (BombObject* bomb : BombList) {
        if (bomb->CheckExplode()) {
            if (!bomb->Exploded) {
                glm::vec2 expSize(MapPlayer->Size.x, MapPlayer->Size.y*3);
                glm::vec2 expHorzSize(MapPlayer->Size.y*3, MapPlayer->Size.x);
                glm::vec2 expUpPos = glm::vec2(bomb->Position.x, bomb->Position.y - ((expSize.y * 2)/3));
                ExplosionObject* expUp = new ExplosionObject(expUpPos, expSize, ResourceManager::GetTexture("paddle"));
                glm::vec2 expDownPos = glm::vec2(bomb->Position.x, bomb->Position.y);
                ExplosionObject* expDown = new ExplosionObject(expDownPos, expSize, ResourceManager::GetTexture("paddle"));
                glm::vec2 expRightPos = glm::vec2(bomb->Position.x, bomb->Position.y);
                ExplosionObject* expRight = new ExplosionObject(expRightPos, expHorzSize, ResourceManager::GetTexture("paddle"));
                glm::vec2 expLeftPos = glm::vec2(bomb->Position.x - ((expHorzSize.x * 2) / 3), bomb->Position.y);
                ExplosionObject* expLeft = new ExplosionObject(expLeftPos, expHorzSize, ResourceManager::GetTexture("paddle"));
                ExplosionList.push_back(expUp);
                ExplosionList.push_back(expDown);
                ExplosionList.push_back(expRight);
                ExplosionList.push_back(expLeft);
            }
            bomb->Exploded = true;
        }
        if (bomb->InitialDT >= 0.0f) {
            bomb->InitialDT -= dt;
        }
    }
    //check if explosion should dissapear
    for (ExplosionObject* exp : ExplosionList) {
        if (exp->CheckDone()) {
            exp->Done = true;
        }
        if (exp->InitialDT >= 0.0f) {
            exp->InitialDT -= dt;
        }
    }
    // check for collisions
    this->DoCollisions();
    // check loss condition
    if (this->State == GAME_ACTIVE && this->Levels[this->Level].IsCompleted())
    {
        this->ResetLevel();
        this->State = GAME_WIN;
    }
}

void Game::ProcessInput(float dt)
{
    if (this->State == GAME_MENU)
    {
        if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
        {
            this->State = GAME_ACTIVE;
            this->KeysProcessed[GLFW_KEY_ENTER] = true;
        }
    }
    if (this->State == GAME_WIN)
    {
        if (this->Keys[GLFW_KEY_ENTER])
        {
            this->KeysProcessed[GLFW_KEY_ENTER] = true;
            this->State = GAME_MENU;
        }
    }
    if (this->State == GAME_ACTIVE)
    {
        float velocity = PLAYER_VELOCITY * dt;
        // move playerboard
        if (this->Keys[GLFW_KEY_LEFT])
        {
            if (MapPlayer->Position.x >= 0.0f)
            {
                MapPlayer->OldPosition = MapPlayer->Position;
                int num_steps = 10;
                float step_speed = velocity / num_steps;
                for (int i = 0; i < num_steps; i++) {
                    MapPlayer->Position.x -= step_speed;
                    this->DoCollisions();
                }
            }
        }
        if (this->Keys[GLFW_KEY_RIGHT])
        {
            if (MapPlayer->Position.x <= this->Width - MapPlayer->Size.x)
            {
                MapPlayer->OldPosition = MapPlayer->Position;
                int num_steps = 10;
                float step_speed = velocity / num_steps;
                for (int i = 0; i < num_steps; i++) {
                    MapPlayer->Position.x += step_speed;
                    this->DoCollisions();
                }
            }
        }
        if (this->Keys[GLFW_KEY_UP])
        {
            if (MapPlayer->Position.y >= 0.0f)
            {
                MapPlayer->OldPosition = MapPlayer->Position;
                int num_steps = 10;
                float step_speed = velocity / num_steps;
                for (int i = 0; i < num_steps; i++) {
                    MapPlayer->Position.y -= step_speed;
                    this->DoCollisions();
                }
            }
        }
        if (this->Keys[GLFW_KEY_DOWN])
        {
            if (MapPlayer->Position.y <= this->Height);
            {
                MapPlayer->OldPosition = MapPlayer->Position;
                int num_steps = 10;
                float step_speed = velocity / num_steps;
                for (int i = 0; i < num_steps; i++) {
                    MapPlayer->Position.y += step_speed;
                    this->DoCollisions();
                }
            }
        }
        if (this->Keys[GLFW_KEY_SPACE] && !this->KeysProcessed[GLFW_KEY_SPACE])
        {
            glm::vec2 bombPos = MapPlayer->Position;
            BombObject* Bomb = new BombObject(bombPos, (this->Levels[0].TileSize.x * 2) / 6, ResourceManager::GetTexture("face"));
            BombList.push_back(Bomb);
            this->KeysProcessed[GLFW_KEY_SPACE] = true;
        }
        if (this->Keys[GLFW_KEY_Q]) 
        {
            this->ResetLevel();
        }
    }
}

void Game::Render()
{
    if (this->State == GAME_ACTIVE || this->State == GAME_MENU || this->State == GAME_WIN || this->State == GAME_LOSE)
    {
        // draw background
        Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);
        // draw level
        this->Levels[0].Draw(*Renderer);
        //drawMapPlayer
        MapPlayer->Draw(*Renderer);
        // draw bombs
        for (BombObject* bomb : BombList)
            if (!bomb->Exploded)
                bomb->Draw(*Renderer);
        //draw explosions
        for (ExplosionObject* exp : ExplosionList)
            if (!exp->Done)
                exp->Draw(*Renderer);
    }
    if (this->State == GAME_MENU)
    {
        Text->RenderText("Press ENTER to start", 250.0f, this->Height / 2.0f, 1.0f);
    }
    if (this->State == GAME_WIN)
    {
        Text->RenderText("You WON!!!", 320.0f, this->Height / 2.0f - 20.0f, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        Text->RenderText("Press ENTER to retry or ESC to quit", 130.0f, this->Height / 2.0f, 1.0f, glm::vec3(1.0f, 1.0f, 0.0f));
    }
    if (this->State == GAME_LOSE)
    {
        Text->RenderText("You LOSE!!!", 320.0f, this->Height / 2.0f - 20.0f, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        Text->RenderText("Press ENTER to retry or ESC to quit", 130.0f, this->Height / 2.0f, 1.0f, glm::vec3(1.0f, 1.0f, 0.0f));
    }
}


void Game::ResetLevel()
{
    this->Levels[0].Load("C:/Users/clarken/Desktop/winter 22-23/computer graphics/one.lvl", this->Width, this->Height);
    MapPlayer->Position = this->Levels[0].PlayerPos;
    MapPlayer->OldPosition = this->Levels[0].PlayerPos;
    MapPlayer->Size = glm::vec2((this->Levels[0].TileSize.x*2)/3, (this->Levels[0].TileSize.y*2)/3);
}

// collision detection
bool CheckCollision(GameObject& one, GameObject& two);
//Collision CheckCollision(BallObject& one, GameObject& two);
Direction VectorDirection(glm::vec2 closest);

void Game::DoCollisions()
{
    for (GameObject& box : this->Levels[this->Level].Bricks)
    {
        if (!box.Destroyed)
        {
            bool collision = CheckCollision(*MapPlayer, box);
            if (collision) // if collision is true
            {
                MapPlayer->Position = MapPlayer->OldPosition;
            }
            for (ExplosionObject* exp : ExplosionList) {
                if (CheckCollision(*exp, box)) {
                    if (!box.IsSolid) {
                       box.Destroyed = true;
                    }
                    else {
                       exp->Done = true;
                    }
                }
            }
            for (EnemyObject* enemy : this->Levels[0].Enemies) {
                if (CheckCollision(*enemy, box)) {
                    enemy->changeDirection();
                }
            }
        }
    }

    for (EnemyObject* enemy : this->Levels[0].Enemies) {
        if (CheckCollision(*enemy, *MapPlayer)) {
            if (!enemy->Destroyed) {
                this->ResetLevel();
                this->State = GAME_LOSE;
            }
        }
        for (ExplosionObject* exp : ExplosionList) {
            if (CheckCollision(*exp, *enemy)) {
                if (!exp->Done) {
                    enemy->Destroyed = true;
                }
            }
        }
    }
    
}

bool CheckCollision(GameObject& one, GameObject& two) // AABB - AABB collision
{
    // collision x-axis?
    bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
        two.Position.x + two.Size.x >= one.Position.x;
    // collision y-axis?
    bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
        two.Position.y + two.Size.y >= one.Position.y;
    // collision only if on both axes
    return collisionX && collisionY;
}