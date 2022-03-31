#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>
#include <type_traits>
#include <vector>

template <typename Iterator, typename Sentinel = Iterator> struct range_t {

  Iterator beg;
  Sentinel e;
  Iterator begin() const { return beg; }
  Sentinel end() const { return e; }

  bool empty() const { return begin() == end(); }

  range_t without_front(std::size_t count = 1) const {
    if constexpr (std::is_same<std::random_access_iterator_tag,
                               typename std::iterator_traits<
                                   Iterator>::iterator_category>{}) {

      count = (std::min)(std::size_t(std::distance(beg, e)), count);
    }

    return {std::next(beg, count), e};
  }

  range_t without_back(std::size_t count = 1) const {
    if constexpr (std::is_same<std::random_access_iterator_tag,
                               typename std::iterator_traits<
                                   Iterator>::iterator_category>{}) {

      count = (std::min)(std::size_t(std::distance(beg, e)), count);
    }

    return {beg, std::prev(e, count)};
  }
};

template <typename Iterator, typename Sentinel>
range_t<Iterator, Sentinel> range(Iterator beg, Sentinel e) {
  return {beg, e};
}

template <typename Iterable> auto range(Iterable &r) {
  return range(std::begin(r), std::end(r));
}

// iterate over all elements except first one
template <typename C> constexpr auto except_first(C &c) {
  auto r = range(c);
  if (r.empty())
    return r;

  return r.without_front();
}

// iterate over all elements except last one
template <typename C> constexpr auto except_last(C &c) {
  auto r = range(c);
  if (r.empty())
    return r;

  return r.without_back();
}

/* only works with std::vector and std::deque
  iterate over elements except
*/
template <typename Vec_Deq>
constexpr auto except_index(Vec_Deq &container,
                            std::initializer_list<std::size_t> index) {

  if (container.empty())
    return container;

  auto arr = container;
  std::size_t count{0};

  for (auto i : index) {
    arr.erase(std::remove(arr.begin(), arr.end(), arr.at(i - count)));

    count++;
  }

  return arr;
}

// iterate in range from, to
auto in_range(int from, int to, int how = 1) {

  std::vector<int> vec;

  for (int i{from}; i <= to; i += how) {
    vec.push_back(i);
  }

  return vec;
}

// call non void functions multiple times
template <typename Fn, typename... Args>
constexpr auto MULTI_FUNC_CALL_V(std::size_t number, Fn &&function,
                                 Args &&...args) noexcept {

  if constexpr (!std::is_same<decltype(std::invoke(function, args...)),
                              void>{}) {
    try {
      throw std::bad_function_call();

    } catch (std::exception const &e) {
      std::cerr << e.what()
                << " : your function has a non void return type ! ! use "
                   "MULTI_FUNC_CALL \n";
      std::exit(-1);
    }

  } else {

    for ([[maybe_unused]] auto &&i : in_range(0, number)) {
      std::invoke(function, args...);
    }
  }
}

// call void functions multiple times
template <typename Fn, typename... Args>
constexpr auto MULTI_FUNC_CALL(std::size_t number, Fn &&function,
                               Args &&...args) noexcept {

  if constexpr (std::is_same<decltype(std::invoke(function, args...)),
                             void>{}) {
    try {
      throw std::bad_function_call();

    } catch (std::exception const &e) {
      std::cerr << e.what()
                << " : your function has a void return type ! use "
                   "MULTI_FUNC_CALL_V\n";
      std::exit(-1);
    }

  } else {

    decltype(std::invoke(function, args...)) result;
    for ([[maybe_unused]] auto &&i : in_range(0, number)) {
      result = std::invoke(function, args...);
    }
    return result;
  }
}