#pragma once
namespace xac::ecs {
template <typename TSettings>
class Entity;
template <typename TSettings>
class World;

template <typename TSettings, typename T>
class ComponentHandle {
 public:
  using EntityId = typename Entity<TSettings>::Id;
  using World = World<TSettings>;

  auto operator*() -> T& {}

  ComponentHandle(EntityId id, World* world) : id_(id), world_(world) {}

 private:
  EntityId id_;
  World* world_ = nullptr;
};
}  // namespace xac::ecs