#ifndef FUNCIT_H
#define FUNCIT_H

#include <type_traits>
#include <functional>

namespace funcit {
namespace detail {

template <class T>
struct drop_first_arg {};

template <class R, class F>
struct drop_first_arg<R(F)>
{
  using type = R(void);
};

template <class R, class F, class... Args>
struct drop_first_arg<R(F, Args...)>
{
  using type = R(Args...);
};

// Matches all non-member signature types.
template <typename T>
struct extract_signature
{
  using type = T;
};

template <typename R, typename U, typename... A>
struct extract_signature<R(U::*)(A...)>
{
  using type = R(U&, A...);
};

template <typename R, typename U, typename... A>
struct extract_signature<R(U::*)(A...) const>
{
  using type = R(const U&, A...);
};

// Matches all function types, function pointers, 
// pointers to member functions and pointer to members.
template <typename T, bool IsClass=false>
struct signature_impl
{
  using fn_type = typename std::remove_pointer<T>::type;
  using sig = typename extract_signature<fn_type>::type;
};

// Matches lambdas, and functors.
template <typename T>
struct signature_impl<T, true>
{
  // Get the type of the operator().
  using fn_type = typename std::remove_pointer<decltype(&T::operator())>::type;

  // The 'this' argument is not part of the signature when it is captured inside a
  // std::function - drop it from the signature.
  using sig = typename drop_first_arg<typename extract_signature<fn_type>::type>::type;
};

} // namespace detail

template <typename T>
struct signature
{
  using value = typename detail::signature_impl<T, std::is_class<T>::value>::sig;
};

template<class T>
constexpr auto wrap(T fn)
{
  return std::function<typename signature<T>::value>(fn);
}

} // namespace funcit

#endif