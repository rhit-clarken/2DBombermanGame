#include "game_level.h"

#include <fstream>
#include <sstream>


void GameLevel::Load(const char* file, unsigned int levelWidth, unsigned int levelHeight)
{
    // clear old data
    this->Bricks.clear();
    this->Enemies.clear();
    // load from file
    unsigned int tileCode;
    GameLevel level;
    std::string line;
    std::ifstream fstream(file);
    std::vector<std::vector<unsigned int>> tileData;
    if (fstream)
    {
        while (std::getline(fstream, line)) // read each line from level file
        {
            std::istringstream sstream(line);
            std::vector<unsigned int> row;
            while (sstream >> tileCode) // read each word separated by spaces
                row.push_back(tileCode);
            tileData.push_back(row);
        }
        if (tileData.size() > 0)
            this->init(tileData, levelWidth, levelHeight);
    }
}

void GameLevel::Draw(SpriteRenderer& renderer)
{
    for (GameObject& tile : this->Bricks)
        if (!tile.Destroyed)
            tile.Draw(renderer);
    for (EnemyObject* enemy : this->Enemies)
        if (!enemy->Destroyed)
            enemy->Draw(renderer);
}

bool GameLevel::IsCompleted()
{
    for (EnemyObject* enemy : this->Enemies)
        if (!enemy->Destroyed)
            return false;
    return true;
}

void GameLevel::init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight)
{
    // calculate dimensions
    unsigned int height = tileData.size();
    unsigned int width = tileData[0].size(); // note we can index vector at [0] since this function is only called if height > 0
    float unit_width = levelWidth / static_cast<float>(width), unit_height = levelHeight / height;
    this->TileSize = glm::vec2(unit_width, unit_height);
    // initialize level tiles based on tileData		
    for (unsigned int y = 0; y < height; ++y)
    {
        for (unsigned int x = 0; x < width; ++x)
        {
            // check block type from level data (2D level array)
            if (tileData[y][x] == 1) // solid
            {
                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                GameObject obj(pos, size, ResourceManager::GetTexture("block_solid"), glm::vec3(0.8f, 0.8f, 0.7f));
                obj.IsSolid = true;
                this->Bricks.push_back(obj);
            }
            else if (tileData[y][x] > 1)	// non-solid; now determine its color based on level data
            {
                glm::vec2 pos((unit_width * x) + 0.1f, (unit_height * y)+ 0.1f);
                glm::vec3 color = glm::vec3(1.0f); // original: white
                if (tileData[y][x] == 2)
                {
                    PlayerPos = pos;
                    //color = glm::vec3(0.2f, 0.6f, 1.0f);
                }
                else if (tileData[y][x] == 3) {
                    color = glm::vec3(0.0f, 0.7f, 0.0f);
                    glm::vec2 pos(unit_width * x, unit_height * y);
                    glm::vec2 size(unit_width, unit_height);
                    this->Bricks.push_back(GameObject(pos, size, ResourceManager::GetTexture("block"), color));
                }
                else if (tileData[y][x] == 4) {
                    color = glm::vec3(0.8f, 0.8f, 0.4f);
                    glm::vec2 pos((unit_width)*x + (unit_width/7), (unit_height)*y + (unit_height / 7));
                    glm::vec2 size((unit_width*2)/3, (unit_height*2)/3);
                    if (tileData[y-1][x] == 3) {
                        EnemyObject* temp = new EnemyObject(pos, size, ResourceManager::GetTexture("block"), color, true);
                       this->Enemies.push_back(temp);
                    }
                    else {
                        EnemyObject* temp = new EnemyObject(pos, size, ResourceManager::GetTexture("block"), color, false);
                       this->Enemies.push_back(temp);
                    }
                }
                else if (tileData[y][x] == 5)
                    color = glm::vec3(1.0f, 0.5f, 0.0f);

            }
        }
    }
}