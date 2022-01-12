#pragma once
#include "GameEngine/EntitySystem/Component.h"

namespace Game {
class PlayerCameraComponent : public GameEngine::Component {
public:
  PlayerCameraComponent();
  ~PlayerCameraComponent();

  virtual void Update() override;
  virtual void OnAddToWorld() override;

private:
};
} // namespace Game
