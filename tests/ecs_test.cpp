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

TEST(ECS_TEST, ENTITY_CREATE) {
  using CListA = mpl::type_list<>;
  using SettingsA = ecs::Settings<CListA>;
  uint64_t entity_count = std::numeric_limits<uint32_t>::max() >> 8;
  ecs::World<SettingsA> world;
  std::vector<ecs::Entity<SettingsA>::Id> entities;
  for (uint64_t i = 0; i < entity_count; i++) {
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

TEST(ECS_TEST, COMPONENT_ASSIGN1) {
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

TEST(ECS_TEST, COMPONENT_ASSIGN2) {
  struct C1 {};
  struct C2 {};
  struct C3 {};
  struct C4 {};
  struct C5 {};
  struct C6 {};
  struct C7 {};
  struct C8 {};
  struct C9 {};
  struct C10 {};
  struct C11 {};
  struct C12 {};
  struct C13 {};
  struct C14 {};
  struct C15 {};
  struct C16 {};
  struct C17 {};
  struct C18 {};
  struct C19 {};
  struct C20 {};
  struct C21 {};
  struct C22 {};
  struct C23 {};
  struct C24 {};
  struct C25 {};
  struct C26 {};
  struct C27 {};
  struct C28 {};
  struct C29 {};
  struct C30 {};
  struct C31 {};
  struct C32 {};
  struct C33 {};
  struct C34 {};
  struct C35 {};
  struct C36 {};
  struct C37 {};
  struct C38 {};
  struct C39 {};
  struct C40 {};
  struct C41 {};
  struct C42 {};
  struct C43 {};
  struct C44 {};
  struct C45 {};
  struct C46 {};
  struct C47 {};
  struct C48 {};
  struct C49 {};
  struct C50 {};
  struct C51 {};
  struct C52 {};
  struct C53 {};
  struct C54 {};
  struct C55 {};
  struct C56 {};
  struct C57 {};
  struct C58 {};
  struct C59 {};
  struct C60 {};
  struct C61 {};
  struct C62 {};
  struct C63 {};
  struct C64 {};
  struct C65 {};
  struct C66 {};
  struct C67 {};
  struct C68 {};
  struct C69 {};
  struct C70 {};
  struct C71 {};
  struct C72 {};
  struct C73 {};
  struct C74 {};
  struct C75 {};
  struct C76 {};
  struct C77 {};
  struct C78 {};
  struct C79 {};
  struct C80 {};
  struct C81 {};
  struct C82 {};
  struct C83 {};
  struct C84 {};
  struct C85 {};
  struct C86 {};
  struct C87 {};
  struct C88 {};
  struct C89 {};
  struct C90 {};
  struct C91 {};
  struct C92 {};
  struct C93 {};
  struct C94 {};
  struct C95 {};
  struct C96 {};
  struct C97 {};
  struct C98 {};
  struct C99 {};
  struct C100 {};
  struct C101 {};
  struct C102 {};
  struct C103 {};
  struct C104 {};
  struct C105 {};
  struct C106 {};
  struct C107 {};
  struct C108 {};
  struct C109 {};
  struct C110 {};
  struct C111 {};
  struct C112 {};
  struct C113 {};
  struct C114 {};
  struct C115 {};
  struct C116 {};
  struct C117 {};
  struct C118 {};
  struct C119 {};
  struct C120 {};
  struct C121 {};
  struct C122 {};
  struct C123 {};
  struct C124 {};
  struct C125 {};
  struct C126 {};
  struct C127 {};
  struct C128 {};

  {
    using CurSetting = ecs::Settings<mpl::type_list<
        C1, C2, C3, C4, C5, C6, C7, C8, C9, C10, C11, C12, C13, C14, C15, C16, C17, C18, C19, C20, C21, C22, C23, C24,
        C25, C26, C27, C28, C29, C30, C31, C32, C33, C34, C35, C36, C37, C38, C39, C40, C41, C42, C43, C44, C45, C46,
        C47, C48, C49, C50, C51, C52, C53, C54, C55, C56, C57, C58, C59, C60, C61, C62, C63, C64, C65, C66, C67, C68,
        C69, C70, C71, C72, C73, C74, C75, C76, C77, C78, C79, C80, C81, C82, C83, C84, C85, C86, C87, C88, C89, C90,
        C91, C92, C93, C94, C95, C96, C97, C98, C99, C100, C101, C102, C103, C104, C105, C106, C107, C108, C109, C110,
        C111, C112, C113, C114, C115, C116, C117, C118, C119, C120, C121, C122, C123, C124, C125, C126, C127, C128>>;
    ecs::World<CurSetting> world;
    auto e = world.create();
    ASSERT_EQ(e.id, 0);
    ASSERT_EQ(e.version, 0);

    auto p1 = world.assign<C68>(e);
    auto p2 = world.assign<C128>(e);
    auto p3 = world.assign<C1>(e);
    static_assert(std::is_same_v<decltype(*p1), C68 &>);
    ASSERT_EQ(
        world.get(e).GetComponentsMask().to_string(),
        "10000000"
        "00000000"
        "00000000"
        "00000000"
        "00000000"
        "00000000"
        "00000000"
        "00001000"
        "00000000"
        "00000000"
        "00000000"
        "00000000"
        "00000000"
        "00000000"
        "00000000"
        "00000001"
    );
  }
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