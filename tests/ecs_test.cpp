#include <iostream>
#include <pico_libs/ecs/world.hpp>
#include <random>

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
  world.each([&](auto &&e, uint32_t i) {
    count++;
    ASSERT_EQ(e.GetId().id, i);
    ASSERT_EQ(e.GetId().version, 0);
    ASSERT_EQ(e.GetWorld(), &world);
    ASSERT_EQ(e.GetComponentsMask().to_string(), "0");
  });
  ASSERT_EQ(count, entity_count - entity_destroy.size());
}

TEST(ECS_TEST, COMPONENT_ASSIGN) {
  using CListD = mpl::type_list<Position, int>;
  using SettingsD = ecs::Settings<CListD>;
  ecs::World<SettingsD> world;
  auto e = world.create();
  ASSERT_EQ(e.id, 0);
  ASSERT_EQ(e.version, 0);
  Position p = {1, 2, 3};
  auto pc = world.assign<Position>(e, p);
  static_assert(std::is_same_v<decltype(*pc), Position &>);
  ASSERT_EQ(p, *pc);
  pc->x = 2;
  p.x = 2;
  ASSERT_EQ(p, *pc);
  reinterpret_cast<Position *>(pc.get())->y = 3;
  p.y = 3;
  ASSERT_EQ(p, *pc);
}

TEST(ECS_TEST, COMPONENT_GET) {
  using CListD = mpl::type_list<Position, float>;
  using SettingsD = ecs::Settings<CListD>;
  ecs::World<SettingsD> world;
  auto e = world.create();
  ASSERT_EQ(e.id, 0);
  ASSERT_EQ(e.version, 0);
  Position p = {1, 2, 3};
  auto pc = world.assign<Position>(e, p);
  ASSERT_EQ(p, *pc);
  pc->x = 2;
  p.x = 2;
  ASSERT_EQ(p, *pc);
  reinterpret_cast<Position *>(pc.get())->y = 3;
  p.y = 3;
  ASSERT_EQ(p, *pc);
  auto fc = world.get<float>(e);
  ASSERT_EQ(fc.get(), nullptr);
}

TEST(ECS_TEST, COMPONENT_TRAVERSE) {
  using CListD = mpl::type_list<Position, Acc, Rotation>;
  using SettingsD = ecs::Settings<CListD>;
  ecs::World<SettingsD> world;

  std::uniform_int_distribution<uint32_t> rand_int(500, 500000);
  uint32_t entity_count = rand_int(seed);
  std::vector<ecs::Entity<SettingsD>::Id> entities;
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
    static_assert(std::is_same_v<decltype(position), ecs::ComponentHandle<SettingsD, Position>>);
    ASSERT_EQ(e.id, i);
    ASSERT_EQ(e.version, 0);
  }
  world.each([&world](auto &&e, uint32_t i) {
    ASSERT_EQ(e.GetId().id, i);
    ASSERT_EQ(e.GetId().version, 0);
    ASSERT_EQ(e.GetWorld(), &world);
    ASSERT_EQ(e.GetComponentsMask().to_string(), "001");
    ASSERT_EQ(world.has<Position>(e.GetId()), true);
    ASSERT_EQ(world.has<Rotation>(e.GetId()), false);
  });
  {
    auto view = world.fuzzy_view<Position>();
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
    auto view = world.fuzzy_view<Position>();
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
    static_assert(std::is_same_v<decltype(rotation), ecs::ComponentHandle<SettingsD, Rotation>>);
    ASSERT_EQ(e.id, i);
    ASSERT_EQ(e.version, 0);
  }
  {
    auto view = world.fuzzy_view<Rotation, Position>();
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
    auto view = world.fuzzy_view<const Position, Acc>();
    auto it = view.begin();
    static_assert(std::is_same_v<decltype(*it), std::tuple<const Position &, Acc &>>);
  }
  {
    auto view = world.fuzzy_view<Acc &&, Rotation &>();
    auto it = view.begin();
    static_assert(std::is_same_v<decltype(*it), std::tuple<Acc &, Rotation &>>);
  }
}

// TODO: test exact view