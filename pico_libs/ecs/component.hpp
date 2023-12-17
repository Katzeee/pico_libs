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
  using ThisWorld = World<TSettings>;

  auto operator*() -> T& {
    return world_->template get<T>(id_);
  }
  auto operator->() -> T* {
    return &world_->template get<T>(id_);
  }
  auto get() -> T* {
    return &world_->template get<T>(id_);
  }

  ComponentHandle(EntityId id, ThisWorld* world) : id_(id), world_(world) {}

 private:
  EntityId id_;
  ThisWorld* world_ = nullptr;
};
}  // namespace xac::ecs