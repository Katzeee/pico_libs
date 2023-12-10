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
  using World = World<TSettings>;
  struct Id {
    uint32_t id;
    uint32_t version;
  };
  Entity() = default;
  Entity(Id id, World* world);

 public:
  auto GetId() -> Id {
    return id_;
  }
  auto GetWorld() -> World* {
    return world_;
  }
  auto GetComponentsMask() {
    return components_mask_;
  }

 private:
  Id id_;
  World* world_ = nullptr;
  std::bitset<TSettings::ComponentList::size> components_mask_;
};

template <typename TSettings>
Entity<TSettings>::Entity(Id id, World* world) : id_(id), world_(world) {}
}  // namespace xac::ecs