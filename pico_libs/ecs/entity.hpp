#pragma once
#include <bitset>
#include <memory>

namespace xac::ecs {

template <typename TSettings>
class World;

template <typename TSettings>
class Entity {
 public:
  friend class World<TSettings>;
  using ComponentList = typename TSettings::ComponentList;
  using ThisWorld = World<TSettings>;
  struct Id {
    uint64_t index;
    uint64_t version;
  };
  Entity() = default;
  // Entity(const Entity& e) noexcept = default;
  // Entity(Entity&& e) noexcept = default;
  // Entity& operator=(const Entity& e) = default;
  // Entity& operator=(Entity&& e) noexcept = default;
  Entity(Id id, ThisWorld* world);

 public:
  auto GetId() -> Id {
    return id_;
  }
  auto GetWorld() -> ThisWorld* {
    return world_;
  }
  auto GetComponentsMask() {
    return components_mask_;
  }

 private:
  Id id_;
  ThisWorld* world_ = nullptr;
  std::bitset<TSettings::ComponentList::size> components_mask_;
};

template <typename TSettings>
Entity<TSettings>::Entity(Id id, ThisWorld* world) : id_(id), world_(world) {}
}  // namespace xac::ecs