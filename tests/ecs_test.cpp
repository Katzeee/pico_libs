#include <iostream>
#include <random>
#include <world.hpp>

#include "gtest/gtest.h"
using namespace xac;

struct Position {
  int x;
  int y;
  int z;
};

struct Acc {
  int x;
  int y;
  int z;
};

struct Rotation {
  int x;
  int y;
  int z;
};

using CList = mpl::type_list<Position, Acc, Rotation>;
using Settings = ecs::Settings<CList>;

TEST(ECS_TEST, ENTITY_CREATE) {
  uint16_t entity_count = std::numeric_limits<uint16_t>::max();
  ecs::World<Settings> world;
  std::vector<ecs::Entity<Settings>::Id> entities;
  for (uint16_t i = 0; i < entity_count; i++) {
    auto e = world.create();
    entities.push_back(e);
    ASSERT_EQ(e.id, i);
    EXPECT_EQ(e.version, 1);
  }
}

TEST(ECS_TEST, COMPONENT_ASSIGN) {
  // FIXME: Should use another setting
  ecs::World<Settings> world;
  std::random_device rd;
  std::mt19937 seed(rd());
  std::uniform_int_distribution<uint32_t> rand_int(500, 500000);
  uint32_t entity_count = rand_int(seed);
  std::cout << entity_count << std::endl;
  std::vector<ecs::Entity<Settings>::Id> entities;
  for (auto i = 0; i < entity_count; i++) {
    auto e = world.create();
    entities.push_back(e);
    ASSERT_EQ(e.id, i);
    EXPECT_EQ(e.version, 1);
  }
  world.each([&world](auto &&e, uint32_t i) {
    EXPECT_EQ(e.GetId().id, i);
    EXPECT_EQ(e.GetId().version, 1);
    EXPECT_EQ(e.GetWorld(), &world);
    EXPECT_EQ(e.GetComponentsMask().to_string(), "000");
  });
  for (auto i = 0; i < entity_count; i++) {
    auto &e = entities[i];
    world.assign<Position>(e, 1, 2, 3);
    EXPECT_EQ(e.id, i);
    EXPECT_EQ(e.version, 2);
  }
  world.each([&world](auto &&e, uint32_t i) {
    EXPECT_EQ(e.GetId().id, i);
    EXPECT_EQ(e.GetId().version, 2);
    EXPECT_EQ(e.GetWorld(), &world);
    EXPECT_EQ(e.GetComponentsMask().to_string(), "001");
  });
  {
    auto view = world.view<const Position>();
    auto it = view.begin();
    static_assert(std::is_same_v<decltype(*it), std::tuple<const Position &>>);
    int count = 0;
    for (; it != view.end(); it++) {
      count++;
      auto &&[position] = *it;
      EXPECT_EQ(position.x, 1);
      EXPECT_EQ(position.y, 2);
      EXPECT_EQ(position.z, 3);
    }
    EXPECT_EQ(count, entity_count);
  }
  {
    auto view = world.view<Position>();
    auto it = view.begin();
    static_assert(std::is_same_v<decltype(*it), std::tuple<Position &>>);
  }
  {
    auto view = world.view<Rotation, Position>();
    auto it = view.begin();
    static_assert(std::is_same_v<decltype(*it), std::tuple<Rotation &, Position &>>);
    EXPECT_EQ(it, view.end());
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