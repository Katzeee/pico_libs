#include <iostream>
#include <random>
#include <world.hpp>

#include "gtest/gtest.h"
using namespace xac;

struct Position {
  int x;
  int y;
  int z;

  friend auto operator==(const Position &lhs, const Position &rhs) -> bool {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
  }
};

struct Acc {
  int x;
  int y;
  int z;
  friend auto operator==(const Acc &lhs, const Acc &rhs) -> bool {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
  }
};

struct Rotation {
  int x;
  int y;
  int z;
  friend auto operator==(const Rotation &lhs, const Rotation &rhs) -> bool {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
  }
};

std::random_device rd;
std::mt19937 seed(rd());

TEST(ECS_TEST, DISABLED_ENTITY_CREATE) {
  using CListA = mpl::type_list<>;
  using SettingsA = ecs::Settings<CListA>;
  uint32_t entity_count = std::numeric_limits<uint32_t>::max() >> 8;
  ecs::World<SettingsA> world;
  std::vector<ecs::Entity<SettingsA>::Id> entities;
  for (uint32_t i = 0; i < entity_count; i++) {
    auto e = world.create();
    entities.push_back(e);
    ASSERT_EQ(e.id, i);
    ASSERT_EQ(e.version, 0);
  }
}

TEST(ECS_TEST, ENTITY_DESTROY) {
  using CListB = mpl::type_list<int>;
  using SettingsB = ecs::Settings<CListB>;

  uint32_t entity_count = std::uniform_int_distribution<uint32_t>{500000, 1000000}(seed);
  uint32_t max_destroy_count = std::uniform_int_distribution<uint32_t>{50000, entity_count}(seed);
  std::uniform_int_distribution<uint32_t> rand_int{0, entity_count - 1};

  ecs::World<SettingsB> world;
  std::vector<ecs::Entity<SettingsB>::Id> entities;
  for (uint32_t i = 0; i < entity_count; i++) {
    auto e = world.create();
    entities.push_back(e);
    ASSERT_EQ(e.id, i);
    ASSERT_EQ(e.version, 0);
  }
  std::set<uint32_t> entity_destroy{};
  for (uint32_t i = 0; i < max_destroy_count; i++) {
    entity_destroy.insert(rand_int(seed));
  }
  for (auto &&i : entity_destroy) {
    auto id = ecs::Entity<SettingsB>::Id{i, 0};
    world.destroy(id);
  }
  uint32_t count = 0;
  world.each([&world, &count](auto &&e, uint32_t i) {
    count++;
    ASSERT_EQ(e.GetId().id, i);
    ASSERT_EQ(e.GetId().version, 0);
    ASSERT_EQ(e.GetWorld(), &world);
    ASSERT_EQ(e.GetComponentsMask().to_string(), "0");
  });
  ASSERT_EQ(count, entity_count - entity_destroy.size());
}

TEST(ECS_TEST, COMPONENT_ASSIGN) {
  using CListC = mpl::type_list<Position, Acc, Rotation>;
  using SettingsC = ecs::Settings<CListC>;
  ecs::World<SettingsC> world;

  std::uniform_int_distribution<uint32_t> rand_int(500, 500000);
  uint32_t entity_count = rand_int(seed);
  std::vector<ecs::Entity<SettingsC>::Id> entities;
  for (uint32_t i = 0; i < entity_count; i++) {
    auto e = world.create();
    entities.push_back(e);
    ASSERT_EQ(e.id, i);
    ASSERT_EQ(e.version, 0);
  }
  world.each([&world](auto &&e, uint32_t i) {
    ASSERT_EQ(e.GetId().id, i);
    ASSERT_EQ(e.GetId().version, 0);
    ASSERT_EQ(e.GetWorld(), &world);
    ASSERT_EQ(e.GetComponentsMask().to_string(), "000");
  });
  for (uint32_t i = 0; i < entity_count; i++) {
    auto &e = entities[i];
    auto position = world.assign<Position>(e, 1, 2, 3);
    static_assert(std::is_same_v<decltype(position), ecs::ComponentHandle<SettingsC, Position>>);
    ASSERT_EQ(e.id, i);
    ASSERT_EQ(e.version, 0);
  }
  world.each([&world](auto &&e, uint32_t i) {
    ASSERT_EQ(e.GetId().id, i);
    ASSERT_EQ(e.GetId().version, 0);
    ASSERT_EQ(e.GetWorld(), &world);
    ASSERT_EQ(e.GetComponentsMask().to_string(), "001");
  });
  {
    auto view = world.view<Position>();
    auto it = view.begin();
    static_assert(std::is_same_v<decltype(*it), std::tuple<Position &>>);
    uint32_t count = 0;
    for (; it != view.end(); it++) {
      count++;
      auto &&[position] = *it;
      ASSERT_EQ(position.x, 1);
      ASSERT_EQ(position.y, 2);
      ASSERT_EQ(position.z, 3);
      position = {2, 1, 5};
    }
    ASSERT_EQ(count, entity_count);
  }
  {
    auto view = world.view<Position>();
    auto it = view.begin();
    static_assert(std::is_same_v<decltype(*it), std::tuple<Position &>>);
    uint32_t count = 0;
    for (; it != view.end(); it++) {
      count++;
      auto &&[position] = *it;
      ASSERT_EQ(position.x, 2);
      ASSERT_EQ(position.y, 1);
      ASSERT_EQ(position.z, 5);
    }
    ASSERT_EQ(count, entity_count);
  }
  for (uint32_t i = 0; i < entity_count / 2; i++) {
    auto &e = entities[i];
    auto rotation = world.assign<Rotation>(e, 7, 8, 9);
    static_assert(std::is_same_v<decltype(rotation), ecs::ComponentHandle<SettingsC, Rotation>>);
    ASSERT_EQ(e.id, i);
    ASSERT_EQ(e.version, 0);
  }
  {
    auto view = world.view<Rotation, Position>();
    auto it = view.begin();
    static_assert(std::is_same_v<decltype(*it), std::tuple<Rotation &, Position &>>);
    uint32_t count = 0;
    for (; it != view.end(); it++) {
      count++;
      auto &&[rotation, position] = *it;
      Position p = {2, 1, 5};
      ASSERT_EQ(position, p);
      Rotation r = {7, 8, 9};
      ASSERT_EQ(rotation, r);
    }
    ASSERT_EQ(count, entity_count / 2);
  }
  {
    auto view = world.view<const Position, Acc>();
    auto it = view.begin();
    static_assert(std::is_same_v<decltype(*it), std::tuple<const Position &, Acc &>>);
  }
  {
    auto view = world.view<Acc &&, Rotation &>();
    auto it = view.begin();
    static_assert(std::is_same_v<decltype(*it), std::tuple<Acc &, Rotation &>>);
  }
}