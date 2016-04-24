#include "funcit.h"
#include <stdexcept>
#include <iostream>

// Example structures and functions for the tests
struct SimpleStruct
{
  int constMemberFn(int x) const
  {
    return x * 1;
  }

  int nonConstMemberFn(int x)
  {
    mem = x * 2;
    return mem;
  }

  int mem;
};

struct Functor
{
  int operator()(int x)
  {
    return x * 3;
  }
};

struct ConstFunctor
{
  int operator()(int x) const
  {
    return x * 4;
  }
};

int test_fn(int x)
{
  return x * 5;
}

template <class T1, class T2>
constexpr bool same_fn_t(const std::function<T2>&)
{
  return std::is_same<T1, T2>::value;
}

template <class T>
void runtime_assert(T v1, T v2, const std::string& s)
{
  if(v1 != v2) throw std::logic_error("Assertion fail:" + s);
}

void run_tests()
{
  // Function pointers
  auto f1 = funcit::wrap(&test_fn);
  static_assert(same_fn_t<int(int)>(f1), "");
  runtime_assert(f1(10), 50, "f1");

  // Function (non-pointer)
  auto f2 = funcit::wrap(test_fn);
  static_assert(same_fn_t<int(int)>(f2), "");
  runtime_assert(f2(20), 100, "f2");

  // Pointers to member functions
  auto f3 = funcit::wrap(&SimpleStruct::constMemberFn);
  static_assert(same_fn_t<int(const SimpleStruct&, int)>(f3), "");
  SimpleStruct s;
  runtime_assert(f3(s, 10), 10, "f3");

  auto f4 = funcit::wrap(&SimpleStruct::nonConstMemberFn);
  static_assert(same_fn_t<int(SimpleStruct&, int)>(f4), "");
  runtime_assert(f4(s, 10), 20, "f4");

  // Pointers to member
  //auto f5 = funcit::wrap(&SimpleStruct::x);

  // Functors
  auto f6 = funcit::wrap(Functor());
  static_assert(same_fn_t<int(int)>(f6), "");
  runtime_assert(f6(10), 30, "f6");

  auto f7 = funcit::wrap(ConstFunctor());
  static_assert(same_fn_t<int(int)>(f7), "");
  runtime_assert(f7(10), 40, "f7");

  // Lambdas
  auto f8 = funcit::wrap([](int x) { return x + 1; });
  static_assert(same_fn_t<int(int)>(f8), "");
  runtime_assert(f8(10), 11, "f8");

  int local = 10;
  auto f9 = funcit::wrap([&local](int x) { return local + x; });
  static_assert(same_fn_t<int(int)>(f9), "");
  runtime_assert(f9(10), 20, "f9");

  auto f10 = funcit::wrap([local](int x) { return local + x; });
  static_assert(same_fn_t<int(int)>(f10), "");
  runtime_assert(f10(20), 30, "f10");
}


int main()
{
  try
  {
    run_tests();
  }
  catch(std::logic_error& e)
  {
    std::cerr << "Test Fail: " << e.what() << std::endl;
    return 1;
  }

  std::cerr << "All Tests Pass" << std::endl;
  return 0;
}
