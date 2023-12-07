#pragma once
#include <bitset>
#include <memory>

namespace xac::ecs {

template <typename TSettings>
class World;

template <typename TSettings>
class Entity {
 public:
  struct Id {
    uint32_t id;
    uint32_t version;
  };
  Entity() = default;
  Entity(Id id, World<TSettings>* world);

//  private:
  Id id_;
  World<TSettings>* world_ = nullptr;
  std::bitset<TSettings::ComponentList::size> components_mask_;
};

template <typename TSettings>
Entity<TSettings>::Entity(Id id, World<TSettings>* world) : id_(id), world_(world) {}
}  // namespace xac::ecs