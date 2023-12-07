#pragma once
#include <type_list.hpp>

namespace xac::ecs {
template <typename TComponentList>
struct Settings {
  using ComponentList = TComponentList;
  template <typename T>
  constexpr static auto has_component() -> bool {
    return mpl::contains<T, ComponentList>::value;
  }
};

}  // namespace xac::ecs