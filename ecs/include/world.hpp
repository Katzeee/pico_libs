#pragma once
#include <assert.h>

#include <algorithm>
#include <functional>
#include <memory>
#include <type_list.hpp>
#include <vector>

#include "component.hpp"
#include "entity.hpp"
#include "settings.hpp"
namespace xac::ecs {
template <typename TSettings>
class Entity;

constexpr static uint32_t kInitSize = 200;

template <typename TSettings>
class World {
 public:
  using ComponentList = typename TSettings::ComponentList;
  template <typename... Args>
  using TupleOfVectors = std::tuple<std::vector<Args>...>;
  using Entity = Entity<TSettings>;
  using EntityId = typename Entity::Id;

 public:
  World();
  auto create() -> EntityId;
  template <typename T, typename... Args>
  auto assign(EntityId &id, Args &&...args) -> std::shared_ptr<ComponentHandle>;
  template <typename F>
  auto each(F &&f);

 private:
  auto prepare_entity_create() -> void;
  template <typename T>
  auto prepare_component_create(const EntityId &id) -> void;

 private:
  mpl::rename<TupleOfVectors, ComponentList> components_pool_;
  std::vector<Entity> entities_;
  std::vector<uint32_t> entity_version_;
  static uint32_t entity_count_;
};

template <typename TSettings>
inline uint32_t World<TSettings>::entity_count_ = 0;

template <typename TSettings>
World<TSettings>::World() {
  entities_.resize(kInitSize);
  entity_version_.resize(kInitSize);
  // for (uint32_t i = 0; i < ComponentList::size; i++) {
  //   std::get<0>(components_pool_).resize(kInitSize);
  // }
}

// TODO: reuse id
template <typename TSettings>
auto World<TSettings>::create() -> EntityId {
  prepare_entity_create();
  auto id = EntityId{};
  id.id = entity_count_;
  id.version = ++entity_version_[entity_count_];
  entities_[entity_count_] = std::move(Entity{id, this});
  entity_count_++;
  return id;
}

template <typename TSettings>
template <typename T, typename... Args>
auto World<TSettings>::assign(EntityId &id, Args &&...args) -> std::shared_ptr<ComponentHandle> {
  static_assert(TSettings::template has_component<T>(), "type is not in component list");
  prepare_component_create<T>(id);
  auto &entity = entities_[id.id];
  auto &real_version = entity_version_[id.id];
  real_version++;
  id.version = real_version;
  entity.id_ = id;
  entity.components_mask_.set(mpl::index_of_v<T, ComponentList>);
  auto &pool = std::get<mpl::index_of_v<T, ComponentList>>(components_pool_);
  pool[id.id] = T{std::forward<Args>(args)...};
  return nullptr;
}

template <typename TSettings>
template <typename F>
auto World<TSettings>::each(F &&f) {
  for (uint32_t i = 0; i < entity_count_; i++) {
    std::invoke(f, entities_[i], i);
  }
}

template <typename TSettings>
template <typename T>
auto World<TSettings>::prepare_component_create(const EntityId &id) -> void {
  auto entity = entities_[id.id];
  assert(entity.id_.version == id.version && "This id is out of date");
  assert(entity_version_[id.id] == id.version && "This entity is dead");
  assert(!entity.components_mask_.test(mpl::index_of_v<T, ComponentList>));
  auto &pool = std::get<mpl::index_of_v<T, ComponentList>>(components_pool_);
  if (entity_count_ >= pool.size()) {
    // FIX: may out of bound
    pool.resize(entity_count_ * 2);
  }
}

template <typename TSettings>
auto World<TSettings>::prepare_entity_create() -> void {
  assert(entities_.size() == entity_version_.size());
  if (entity_count_ >= entities_.size()) {
    // FIX: may out of bound
    entities_.resize(entity_count_ * 2);  // HINT: must be resize, because you will use index
    entity_version_.resize(entity_count_ * 2);
  }
}

}  // namespace xac::ecs