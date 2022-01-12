#include "LinkedEntityComponent.h"
#include "GameEngine/EntitySystem/Component.h"
#include "GameEngine/GameEngineMain.h"
#include "SFML/System/Vector2.hpp"

using namespace Game;

LinkedEntityComponent::LinkedEntityComponent()
    : m_followedEntity(nullptr), m_offset(sf::Vector2f(0.f, 0.f)) {}

LinkedEntityComponent::~LinkedEntityComponent() {}

void LinkedEntityComponent::Update() {
  if (m_followedEntity) {
    GetEntity()->SetPos(m_followedEntity->GetPos() + m_offset);
  }
}
