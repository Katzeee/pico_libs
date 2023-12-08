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
    ASSERT_EQ(e.version, 1);
  }
}

TEST(ECS_TEST, COMPONENT_ASSIGN) {
  using CListB = mpl::type_list<Position, Acc, Rotation>;
  using SettingsB = ecs::Settings<CListB>;
  ecs::World<SettingsB> world;
  std::random_device rd;
  std::mt19937 seed(rd());
  std::uniform_int_distribution<uint32_t> rand_int(500, 500000);
  uint32_t entity_count = rand_int(seed);
  std::vector<ecs::Entity<SettingsB>::Id> entities;
  for (uint32_t i = 0; i < entity_count; i++) {
    auto e = world.create();
    entities.push_back(e);
    ASSERT_EQ(e.id, i);
    ASSERT_EQ(e.version, 1);
  }
  world.each([&world](auto &&e, uint32_t i) {
    ASSERT_EQ(e.GetId().id, i);
    ASSERT_EQ(e.GetId().version, 1);
    ASSERT_EQ(e.GetWorld(), &world);
    ASSERT_EQ(e.GetComponentsMask().to_string(), "000");
  });
  for (uint32_t i = 0; i < entity_count; i++) {
    auto &e = entities[i];
    world.assign<Position>(e, 1, 2, 3);
    ASSERT_EQ(e.id, i);
    ASSERT_EQ(e.version, 2);
  }
  world.each([&world](auto &&e, uint32_t i) {
    ASSERT_EQ(e.GetId().id, i);
    ASSERT_EQ(e.GetId().version, 2);
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
    world.assign<Rotation>(e, 7, 8, 9);
    ASSERT_EQ(e.id, i);
    ASSERT_EQ(e.version, 3);
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