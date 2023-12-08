#pragma once
#include <assert.h>

#include <algorithm>
#include <functional>
#include <memory>
#include <numeric>
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

  template <typename...>
  class component_view;

  template <typename... Args>
  class component_iterator {  // HINT: after c++17, std::iterator is deperated
   public:
    using value_type = typename component_view<Args...>::value_type;
    using type = component_iterator<Args...>;
    component_iterator(World *world, uint32_t i) : world_(world), i_(i) {
      next();
    }
    auto operator*() -> value_type {
      return {std::get<mpl::index_of_v<std::decay_t<Args>, ComponentList>>(world_->components_pool_).at(i_)...};
    }
    auto operator++(int) -> type {
      auto temp = this;
      ++(*temp);
      return *temp;
    }
    auto operator++() -> type & {
      i_++;
      next();
      return *this;
    }
    friend auto operator==(const component_iterator<Args...> &lhs, const component_iterator<Args...> &rhs) -> bool {
      assert(lhs.world_ == rhs.world_);
      return lhs.i_ == rhs.i_;
    }

   private:
    auto next() -> void {
      for (; i_ < entity_count_; i_++) {
        if ((component_view<Args...>::mask_ & world_->entities_.at(i_).components_mask_) ==
            component_view<Args...>::mask_) {
          break;
        }
      }
    }

   private:
    World<TSettings> *world_;
    uint32_t i_;
  };

  template <typename... Args>
  class component_view {
    friend class component_iterator<Args...>;

   public:
    using value_type = std::tuple<std::remove_reference_t<Args> &...>;
    // using value_type = std::tuple<Args &&...>;
    component_view(World<TSettings> *world) : world_(world) {}
    auto begin() -> component_iterator<Args...> {
      return component_iterator<Args...>{world_, 0};
    }
    auto end() -> component_iterator<Args...> {
      return component_iterator<Args...>(world_, entity_count_);
    }

   private:
   private:
    World<TSettings> *world_;
    inline constexpr static uint32_t mask_value_ =
        mpl::index_sequence_v<mpl::index_of_v<std::decay_t<Args>, ComponentList>...>;
    inline static std::bitset<ComponentList::size> mask_ = std::bitset<ComponentList::size>(mask_value_);
  };

 public:
  World();
  auto create() -> EntityId;
  template <typename T, typename... Args>
  auto assign(EntityId &id, Args &&...args) -> std::shared_ptr<ComponentHandle>;
  template <typename F>
  auto each(F &&f);
  template <typename... Args>
  auto view() -> component_view<Args...>;

 private:
  auto prepare_entity_create() -> void;
  template <typename T>
  auto prepare_component_create(const EntityId &id) -> void;
  // generate view mask in compile time

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
}

// TODO: reuse id
template <typename TSettings>
auto World<TSettings>::create() -> EntityId {
  prepare_entity_create();
  auto id = EntityId{};
  id.id = entity_count_;
  id.version = ++entity_version_.at(entity_count_);
  entities_.at(entity_count_) = std::move(Entity{id, this});
  entity_count_++;
  return id;
}

template <typename TSettings>
template <typename T, typename... Args>
auto World<TSettings>::assign(EntityId &id, Args &&...args) -> std::shared_ptr<ComponentHandle> {
  static_assert(TSettings::template has_component<T>(), "type is not in component list");
  prepare_component_create<T>(id);
  auto &entity = entities_.at(id.id);
  auto &real_version = entity_version_.at(id.id);
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
template <typename... Args>
auto World<TSettings>::view() -> component_view<Args...> {
  return component_view<Args...>{this};
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
    assert(entity_count_ * 2 <= std::numeric_limits<uint32_t>::max());
    entities_.resize(entity_count_ * 2, {});  // HINT: must be resize, because you will use index
    entity_version_.resize(entity_count_ * 2);
  }
}

}  // namespace xac::ecs