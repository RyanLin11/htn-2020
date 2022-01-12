#include "PlayerMovementComponent.h"
#include "GameEngine/EntitySystem/Components/SpriteRenderComponent.h"
#include "GameEngine/GameEngineMain.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

using namespace Game;

PlayerMovementComponent::PlayerMovementComponent() : frames(0) {}

PlayerMovementComponent::~PlayerMovementComponent() {}

void PlayerMovementComponent::OnAddToWorld() {
  //__super::OnAddToWorld();
}

void PlayerMovementComponent::Update() {
  //__super::Update();
  auto *this_entity = GetEntity();
  GameEngine::SpriteRenderComponent *playerSprite =
      this_entity->GetComponent<GameEngine::SpriteRenderComponent>();
  const float dt = GameEngine::GameEngineMain::GetTimeDelta();

  sf::Vector2f wantedVel = sf::Vector2f(0.f, 0.f);
  const float playerVel = 200.0f;
  bool key_pressed = false;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
    wantedVel.x -= playerVel * dt;
    playerSprite->SetTexture(GameEngine::eTexture::PlayerLeft);
    key_pressed = true;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
    wantedVel.x += playerVel * dt;
    playerSprite->SetTexture(GameEngine::eTexture::PlayerRight);
    key_pressed = true;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
    wantedVel.y -= playerVel * dt;
    playerSprite->SetTexture(GameEngine::eTexture::PlayerUp);
    key_pressed = true;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
    wantedVel.y += playerVel * dt;
    playerSprite->SetTexture(GameEngine::eTexture::PlayerDown);
    key_pressed = true;
  }
  if (key_pressed) {
    const int tile_idx = frames / 250;
    if (tile_idx < 3) {
      playerSprite->SetTileIndex(sf::Vector2i(tile_idx, 0));
      frames++;
    } else {
      frames = 0;
    }
  } else {
    frames = 0;
    playerSprite->SetTileIndex(sf::Vector2i(0, 0));
  }

  this_entity->SetPos(GetEntity()->GetPos() + wantedVel);
}
