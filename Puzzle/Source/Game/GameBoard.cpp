#include <stdlib.h>

#include "Game/Components/LinkedEntityComponent.h"
#include "Game/Components/PlayerCameraComponent.h"
#include "Game/Components/PlayerMovementComponent.h"
#include "GameBoard.h"
#include "GameEngine/EntitySystem/Components/CollidablePhysicsComponent.h"
#include "GameEngine/EntitySystem/Components/LightEntity.h"
#include "GameEngine/EntitySystem/Components/SpriteRenderComponent.h"
#include "GameEngine/EntitySystem/Components/TextRenderComponent.h"
#include "GameEngine/GameEngineMain.h"
#include <cassert>
#include <fstream>
#include <iostream>

using namespace Game;

GameEngine::TextRenderComponent *CountDownrender;
sf::Time m_countDownTimer;
sf::Clock m_clock;

int countDownTime = 60;
bool isGameOver;
bool cleanGame;

GameBoard::GameBoard()
    : m_player(nullptr), m_CountDown(nullptr), m_BlockerLeft(nullptr),
      m_BlockerRight(nullptr) {
  m_clock.restart();
  isGameOver = false;
  cleanGame = false;

  LoadMap("level1.txt", 80);
  CreatePlayer();
  CreateCountDown();
  CreateBackground();
  CreateBlocker();

  m_light = new GameEngine::LightEntity();
  GameEngine::GameEngineMain::GetInstance()->AddEntity(m_light);
  m_light->SetPos(sf::Vector2f(70.f, 130.f));
  m_light->SetSize(sf::Vector2f(3000.f, 3000.f));
}

GameBoard::~GameBoard() {}

void GameBoard::Update() {
  if (!isGameOver) {
    UpdateCountDown();
  } else if (!cleanGame) {
    GameEngine::GameEngineMain::GetInstance()->RemoveEntity(m_player);
    GameEngine::GameEngineMain::GetInstance()->RemoveEntity(m_CountDown);
  }
  UpdateLight();

  cleanGame = true;
}

void GameBoard::LoadMap(const std::string &filename, const size_t tile_size) {
  std::ifstream file("Resources/Maps/" + filename);
  file >> m_width >> m_height;
  std::string line;
  for (size_t row = 0; row < m_height; ++row) {
    file >> line;
    assert(line.size() == m_width);
    for (size_t col = 0; col < m_width; ++col) {
      switch (line[col]) {
      case 'X':
        CreateObstacle(tile_size * col, tile_size * row, tile_size, tile_size);
        break;
      case '.':
        break;
      default:
        break;
      }
    }
  }
}

void GameBoard::CreatePlayer() {
  m_player = new GameEngine::Entity();
  GameEngine::GameEngineMain::GetInstance()->AddEntity(m_player);

  m_player->SetPos(sf::Vector2f(50.f, 50.f));
  m_player->SetSize(sf::Vector2f(50.f, 50.f));

  GameEngine::SpriteRenderComponent *spriteRender =
      m_player->AddComponent<GameEngine::SpriteRenderComponent>();
  spriteRender->SetFillColor(sf::Color::Transparent);
  spriteRender->SetTexture(GameEngine::eTexture::PlayerDown);
  spriteRender->SetTileIndex(0, 0);

  m_player->AddComponent<PlayerMovementComponent>();
  m_player->AddComponent<GameEngine::CollidablePhysicsComponent>();
  m_player->AddComponent<PlayerCameraComponent>();
}

void GameBoard::CreateBackground() {
  GameEngine::Entity *background = new GameEngine::Entity;
  GameEngine::GameEngineMain::GetInstance()->AddEntity(background);

  background->SetPos(sf::Vector2f(250.f, 250.f));
  background->SetSize(sf::Vector2f(768.f, 756.f));

  GameEngine::SpriteRenderComponent *spriteRender =
      background->AddComponent<GameEngine::SpriteRenderComponent>();
  spriteRender->SetFillColor(sf::Color::Transparent);
  spriteRender->SetTexture(GameEngine::eTexture::Background);
  spriteRender->SetZLevel(-1);

  LinkedEntityComponent *linkedCmp =
      background->AddComponent<LinkedEntityComponent>();
  linkedCmp->SetFollowedEntity(m_player);
  linkedCmp->SetFollowedOffset(sf::Vector2f(-50.f, -50.f));
}

void GameBoard::CreateCountDown() {
  GameEngine::Entity *CountDown = new GameEngine::Entity();
  CountDownrender = CountDown->AddComponent<GameEngine::TextRenderComponent>();
  // get time
  m_countDownTimer = m_clock.getElapsedTime();
  const int time = m_countDownTimer.asSeconds();

  // set text
  CountDownrender->SetFont("Calibri Regular.ttf");
  CountDownrender->SetString("Time: " + std::to_string(time));
  CountDownrender->SetZLevel(1001);
  CountDownrender->SetColor(sf::Color::Red);

  CountDown->SetPos(sf::Vector2f(10.f, 10.f));
  CountDown->SetSize(sf::Vector2f(0.f, 0.f));

  LinkedEntityComponent *linkedCmp =
      CountDown->AddComponent<LinkedEntityComponent>();
  linkedCmp->SetFollowedEntity(m_player);
  linkedCmp->SetFollowedOffset(sf::Vector2f(-470.f, -360.f));

  GameEngine::GameEngineMain::GetInstance()->AddEntity(CountDown);

  m_CountDown = CountDown;
}

void GameBoard::UpdateCountDown() {
  m_countDownTimer = m_clock.getElapsedTime();
  const int time = m_countDownTimer.asSeconds();
  CountDownrender->SetString("Time: " + std::to_string(time));
  if (time >= countDownTime) {
    isGameOver = true;
    Reset();
  }

  if (time % 5 == 0)
    UpdateBlocker();
}

void GameBoard::CreateBlocker() {
  m_BlockerLeft = new GameEngine::Entity();
  m_BlockerRight = new GameEngine::Entity();

  GameEngine::GameEngineMain::GetInstance()->AddEntity(m_BlockerLeft);
  GameEngine::GameEngineMain::GetInstance()->AddEntity(m_BlockerRight);

  m_BlockerLeft->SetPos(sf::Vector2f(0.f, 0.f));
  m_BlockerLeft->SetSize(sf::Vector2f(10.f, 600.f));

  m_BlockerRight->SetPos(sf::Vector2f(400.f, 0.f));
  m_BlockerRight->SetSize(sf::Vector2f(10.f, 600.f));

  GameEngine::RenderComponent *renderLeft =
      m_BlockerLeft->AddComponent<GameEngine::RenderComponent>();
  GameEngine::RenderComponent *renderRight =
      m_BlockerRight->AddComponent<GameEngine::RenderComponent>();

  renderLeft->SetFillColor(sf::Color::Black);
  renderLeft->SetZLevel(1000);
  renderRight->SetFillColor(sf::Color::Black);
  renderRight->SetZLevel(1000);

  LinkedEntityComponent *LinkedLeft =
      m_BlockerLeft->AddComponent<LinkedEntityComponent>();
  LinkedEntityComponent *LinkedRight =
      m_BlockerRight->AddComponent<LinkedEntityComponent>();

  LinkedLeft->SetFollowedEntity(m_player);
  LinkedLeft->SetFollowedOffset(sf::Vector2f(250.f, 0.f));
  LinkedRight->SetFollowedEntity(m_player);
  LinkedRight->SetFollowedOffset(sf::Vector2f(-250.f, 0.f));
}

void GameBoard::UpdateBlocker() {
  m_BlockerLeft->SetSize(sf::Vector2f(m_BlockerLeft->GetSize().x + 0.13,
                                      m_BlockerLeft->GetSize().y));
  m_BlockerRight->SetSize(sf::Vector2f(m_BlockerRight->GetSize().x + 0.13,
                                       m_BlockerRight->GetSize().y));
}

void GameBoard::Reset() {
  m_player->SetPos(sf::Vector2f(50.f, 50.f));
  m_BlockerLeft->SetSize(sf::Vector2f(10.f, 600.f));
  m_BlockerRight->SetSize(sf::Vector2f(10.f, 600.f));
  isGameOver = false;
  m_clock.restart();
}

void GameBoard::CreateObstacle(float x, float y, float wid, float len) {
  GameEngine::Entity *obstacle = new GameEngine::Entity();
  GameEngine::GameEngineMain::GetInstance()->AddEntity(obstacle);

  obstacle->SetPos(sf::Vector2f(x, y));
  obstacle->SetSize(sf::Vector2f(wid, len));

  GameEngine::SpriteRenderComponent *render =
      obstacle->AddComponent<GameEngine::SpriteRenderComponent>();
  render->SetFillColor(sf::Color::Transparent);
  render->SetTexture(GameEngine::eTexture::Wall);
  render->SetZLevel(1);

  obstacle->AddComponent<GameEngine::CollidableComponent>();
}

void GameBoard::UpdateLight() {
  m_light->SetPos(
      sf::Vector2f(m_player->GetPos().x + 5, m_player->GetPos().y + 10));
}
