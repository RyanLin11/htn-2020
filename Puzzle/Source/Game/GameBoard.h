#pragma once
#include "GameEngine/EntitySystem/Components/LightEntity.h"
#include "GameEngine/EntitySystem/Entity.h"

#include <SFML/System/Vector2.hpp>
#include <vector>

namespace Game {
// Used for storing and controlling all game related entities, as well as
// providing an entry point for the "game" side of application
class PlayerEntity;

class GameBoard {
public:
  GameBoard();
  virtual ~GameBoard();

  void Update();
  bool IsGameOver() const { return m_isGameOver; }

protected:
  void CreatePlayer();
  void LoadMap(const std::string &filename, const size_t tile_size);
  void CreateBackground();

private:
  GameEngine::Entity *m_player;

  void CreateCountDown();
  void UpdateCountDown();
  void CreateBlocker();
  void UpdateBlocker();
  void UpdateLight();
  void Reset();
  void CreateObstacle(float x, float y, float wid, float len);

  GameEngine::Entity *m_BlockerLeft;
  GameEngine::Entity *m_BlockerRight;
  GameEngine::Entity *m_CountDown;
  GameEngine::Entity *m_light;

  bool m_isGameOver;
  size_t m_width, m_height;
  std::vector<GameEngine::Entity *> m_tiles;
};
} // namespace Game
