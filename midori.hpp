#include <algorithm>
#include <numeric>
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

template <typename C> constexpr auto except_first(C &c) {
  auto r = range(c);
  if (r.empty())
    return r;

  return r.without_front();
}

template <typename C> constexpr auto except_last(C &c) {
  auto r = range(c);
  if (r.empty())
    return r;

  return r.without_back();
}

// only works with std::vector and std::deque
constexpr auto except_index(auto &container,
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

auto from(int one, int two) {

  std::vector<int> vec(two - one + 1);

  std::iota(vec.begin(), vec.end(), one);

  return vec;
}
