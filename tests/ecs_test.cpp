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

using CList = mpl::type_list<Position, Acc>;
using Settings = ecs::Settings<CList>;

static ecs::World<Settings> world;

TEST(ECS_TEST, ENTITY_CREATE) {
  auto e = world.create();
  auto e1 = world.create();
  world.assign<Position>(e, 1, 2, 3);
  world.assign<Acc>(e, 1, 2, 3);
  world.assign<Acc>(e1, 1, 2, 3);
  world.each([](auto &&e) {
    std::cout << "id: " << e.id_.id;
    std::cout << ", version: " << e.id_.version;
    std::cout << ", world: " << e.world_;
    std::cout << ", bitset: " << e.components_mask_.to_string();
    std::cout << std::endl;
  });
}