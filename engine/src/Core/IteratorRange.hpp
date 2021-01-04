#pragma once

template <typename It> class IteratorRange {
  public:
    IteratorRange(It first, It last) : first(first), last(last) {}

    It begin() const { return first; }

    It end() const { return last; }

  private:
    It first, last;
};