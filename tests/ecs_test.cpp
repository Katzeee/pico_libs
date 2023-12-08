#include <world.hpp>
using namespace xac;

#include "gtest/gtest.h"

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

static ecs::World<Settings> world;

TEST(ECS_TEST, ENTITY_CREATE) {
  std::vector<ecs::Entity<Settings>::Id> entities;
  for (auto i = 0; i < 50000; i++) {
    auto e = world.create();
    entities.push_back(e);
    EXPECT_EQ(e.id, i);
    EXPECT_EQ(e.version, 1);
  }
  world.each([](auto &&e, uint32_t i) {
    EXPECT_EQ(e.GetId().id, i);
    EXPECT_EQ(e.GetId().version, 1);
    EXPECT_EQ(e.GetWorld(), &world);
    EXPECT_EQ(e.GetComponentsMask().to_string(), "000");
  });
  for (auto i = 0; i < 50000; i++) {
    auto &e = entities[i];
    world.assign<Position>(e, 1, 2, 3);
    EXPECT_EQ(e.id, i);
    EXPECT_EQ(e.version, 2);
  }
  world.each([](auto &&e, uint32_t i) {
    EXPECT_EQ(e.GetId().id, i);
    EXPECT_EQ(e.GetId().version, 2);
    EXPECT_EQ(e.GetWorld(), &world);
    EXPECT_EQ(e.GetComponentsMask().to_string(), "001");
  });
}