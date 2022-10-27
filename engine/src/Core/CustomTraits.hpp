#include <type_traits>

namespace Engine {

template <typename>
std::false_type is_pointer_like(unsigned long);

template <typename T>
auto is_pointer_like(int) -> decltype(*std::declval<T>(), std::true_type{});

template <typename T>
auto is_pointer_like(long) -> decltype(std::declval<T>().operator->(), std::true_type{});

template <typename T>
using is_pointer_like_arrow_dereferencable = decltype(is_pointer_like<T>(0));

template <typename T>
static auto constexpr is_pointer_like_arrow_dereferencable_v = is_pointer_like_arrow_dereferencable<T>::value;

} // namespace Engine