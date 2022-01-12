#pragma once
#include "GameEngine/EntitySystem/Component.h"
#include "SFML/System/Vector2.hpp"

namespace Game {
class LinkedEntityComponent : public GameEngine::Component {
public:
  LinkedEntityComponent();
  ~LinkedEntityComponent();

  void SetFollowedEntity(GameEngine::Entity *in) { m_followedEntity = in; }
  void SetFollowedOffset(sf::Vector2f in) { m_offset = in; }
  virtual void Update() override;

private:
  GameEngine::Entity *m_followedEntity;
  sf::Vector2f m_offset;
};
} // namespace Game
