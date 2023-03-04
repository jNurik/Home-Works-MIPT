#include <vector>
#include <iostream>
#include <cmath>
#include <iterator>

struct test {
  test() { std::cout << "T() " << std::endl; }
  ~test() { std::cout << "~T() " << std::endl; }
};

struct test1 {
  int a = 0;
  test1(int a) : a(a) { if (a == 0) throw "no"; }
  ~test1() { std::cout << "~Test1()" << std::endl; }
};


template <typename T>
class Deque {
  private:
  //size of internal array
  static const size_t sz_internal_arr_= 8;
  //size of external array
  size_t size_ = 0;
  //real size of external array
  size_t real_size_ = 0;
  //container size for users
  size_t size_deque_ = 0;
  T** external_array_ = nullptr;

  public:
  class iterator;
  class const_iterator;
  class reverse_iterator;

  Deque() noexcept;
  Deque(size_t count) noexcept;
  Deque(size_t count, const T& value) noexcept;
  Deque(const Deque& object);
  ~Deque() noexcept;
  Deque& operator=(const Deque& object) noexcept;

  bool empty() const noexcept;
  size_t size() const noexcept;
  T& operator[](size_t i) noexcept;
  T operator[](size_t i) const noexcept;
  T& at(size_t i);
  T at(size_t i) const;

  void push_back(const T& value) noexcept;
  void pop_back(const T& value) noexcept;
  void push_front(const T& value) noexcept;
  void pop_front(const T& value) noexcept;

  iterator begin() const noexcept;
  const_iterator cbegin() const noexcept;
  iterator end() const noexcept;
  const_iterator cend() const noexcept;
  reverse_iterator rbegin() const noexcept;
  reverse_iterator rcbegin() const noexcept;
  reverse_iterator rend() const noexcept;
  reverse_iterator rcend() const noexcept;

  void insert(iterator, const T&) noexcept;
  void erase(iterator) noexcept;

};

//------------definitions_Deque--------------//

template <typename T>
Deque<T>::Deque() noexcept = default;

template <typename T>
Deque<T>::Deque(size_t count) noexcept : size_(ceil((double)count / (double)sz_internal_arr_)), real_size_(size_), size_deque_(count) {
  external_array_ = new T*[real_size_]; //вроде тут ничего не ломается
  for (size_t i = 0; i < size_; ++i) {
    external_array_[i] = reinterpret_cast<T*>(new uint8_t[sizeof(T) * sz_internal_arr_]); // точно (arr + i) =?
  }
}

template <typename T>
Deque<T>::Deque(size_t count, const T& value) noexcept : Deque(count)  {
  for (size_t i = 0; i < count; ++i) {
    size_t pos_external_array = i / sz_internal_arr_;
    size_t pos_internal_array = i % sz_internal_arr_;
    try {
      new(external_array_[pos_external_array] + pos_internal_array) T(value);
    } catch(...) {
      for (size_t j = 0; j < i; ++j) {
        for (size_t k = 0; k < sz_internal_arr_; ++k) {
          (external_array_[j] + k) -> ~T();
        }
        delete[] reinterpret_cast<uint8_t*>(external_array_[j]);
      }
      delete[] external_array_;
    }
  }
}

template <typename T>
Deque<T>::~Deque() noexcept {
  for (size_t i = 0; i < size_; ++i) {
    for (size_t j = 0; j < sz_internal_arr_; ++j) {
      (external_array_[i] + j) -> ~T();
    }
    delete[] reinterpret_cast<uint8_t*>(external_array_[i]);
  }
  delete[] external_array_;
}

// template <typename T>
// Deque<T>::Deque(const Deque& object) noexcept : size_(object.size_), real_size_(object.real_size_), size_deque_(object.size_deque_) { //при создании итераторов, будет беда
//   T** new_external_array_ = new[]
// }

template <typename T>
bool Deque<T>::empty() const noexcept { return (size_ == 0); } // может size_deque
template <typename T>
size_t Deque<T>::size() const noexcept {
  return size_deque_;
}
template <typename T>
T& Deque<T>::operator[](size_t i) noexcept {
  return external_array_[i / sz_internal_arr_][i % sz_internal_arr_];
}
template <typename T>
T Deque<T>::operator[](size_t i) const noexcept {
  return external_array_[i / sz_internal_arr_][i % sz_internal_arr_];
}
template <typename T>
T& Deque<T>::at(size_t i) {
  if (i < size_deque_) return external_array_[i / sz_internal_arr_][i % sz_internal_arr_];
  throw std::out_of_range("oops");
}
template <typename T>
T Deque<T>::at(size_t i) const {
  if (i < size_deque_) return external_array_[i / sz_internal_arr_][i % sz_internal_arr_];
  throw std::out_of_range("oops");
}


template <typename T>
typename Deque<T>::iterator Deque<T>::begin() const noexcept { return iterator(0); }
template <typename T>
typename Deque<T>::iterator Deque<T>::end() const noexcept { return iterator(size_deque_); }


//------------iter--------------//
template <typename T>
class Deque<T>::iterator {
  private:
  //Pointer to an external array
  T* ptr_;
  //Position in the internal array
  size_t pos_;
  using itraotor_category = std::random_access_iterator_tag;
  using value_type = T;
  using Distance = size_t;
  using Pointer = T*;
  using Reference	= T&;
  public:
  iterator() noexcept;
  iterator(const Distance& index) noexcept;
  iterator(const T*& ptr, size_t pos) noexcept;
  iterator(const iterator& obj) noexcept;
  ~iterator() noexcept;
  iterator& operator=(const iterator& obj) noexcept;
  iterator operator++() noexcept;
  // iter operator++(int);
  iterator operator--() noexcept;
  // iter operqtor--(int);
  iterator operator+=(const Distance& number) noexcept;
  iterator operator-=(const Distance& number) noexcept;
  value_type& operator*() const noexcept;
  value_type* operator->() const noexcept;
  friend Distance operator-(const iterator& obj1, const iterator& obj2) noexcept;
  friend bool operator<(const iterator& obj1, const iterator& obj2) noexcept;
};
template <typename T>
typename Deque<T>::iterator operator+(const typename Deque<T>::iterator& it, const typename Deque<T>::iterator::Distance& number) noexcept;
template <typename T>
typename Deque<T>::iterator operator+(const typename Deque<T>::iterator::Distance& number, const typename Deque<T>::iterator& it) noexcept;
template <typename T>
typename Deque<T>::iterator operator-(const typename Deque<T>::iterator& it, const typename Deque<T>::iterator::Distance& number) noexcept;
template <typename T>
typename Deque<T>::iterator operator-(const typename Deque<T>::iterator::Distance& number, const typename Deque<T>::iterator& it) noexcept;
template <typename T>
bool operator>(const typename Deque<T>::iterator& obj1, const typename Deque<T>::iterator& obj2) noexcept;
template <typename T>
bool operator==(const typename Deque<T>::iterator& obj1, const typename Deque<T>::iterator& obj2) noexcept;
template <typename T>
bool operator<=(const typename Deque<T>::iterator& obj1, const typename Deque<T>::iterator& obj2) noexcept;
template <typename T>
bool operator>=(const typename Deque<T>::iterator& obj1, const typename Deque<T>::iterator& obj2) noexcept;
template <typename T>
bool operator!=(const typename Deque<T>::iterator& obj1, const typename Deque<T>::iterator& obj2) noexcept;

//------------definitions_iter--------------//
template <typename T>
Deque<T>::iterator::iterator() noexcept = default;
template <typename T>
Deque<T>::iterator::iterator(const Distance& index) noexcept : ptr_(external_array_[index / sz_internal_arr_]), pos_(index % sz_internal_arr_) {}
template <typename T>
Deque<T>::iterator::iterator(const T*& ptr, size_t pos) noexcept : ptr_(ptr), pos_(pos) {}
template <typename T>
Deque<T>::iterator::iterator(const typename Deque<T>::iterator& obj) noexcept = default;
template <typename T>
Deque<T>::iterator::~iterator() noexcept {
  delete ptr_;
}
template <typename T>
typename Deque<T>::iterator& Deque<T>::iterator::operator=(const typename Deque<T>::iterator& obj) noexcept = default;

template <typename T>
typename Deque<T>::iterator Deque<T>::iterator::operator++() noexcept {
  if (pos_ == sz_internal_arr_ - 1) return iterator((ptr_ + 1), 0);
  return iter(ptr_, pos_ + 1);
}
template <typename T>
typename Deque<T>::iterator Deque<T>::iterator::operator--() noexcept {
  if (pos_ == 0) return iterator((ptr_ - 1), sz_internal_arr_ - 1);
  return iterator(ptr_, pos_ - 1);
}

template <typename T>
typename Deque<T>::iterator Deque<T>::iterator::operator+=(const typename Deque<T>::iterator::Distance& number) noexcept {
  return iterator(ptr_ + (number + pos_) / sz_internal_arr_, (number + pos_) % sz_internal_arr_);
}
template <typename T>
typename Deque<T>::iterator Deque<T>::iterator::operator-=(const typename Deque<T>::iterator::Distance& number) noexcept {
  return iterator(ptr_ + (number - pos_) / sz_internal_arr_, (number - pos_) % sz_internal_arr_);
}

template <typename T>
typename Deque<T>::iterator operator+(const typename Deque<T>::iterator& it, const typename Deque<T>::iterator::Distance& number) noexcept {
  it += number;
  return it;
}
template <typename T>
typename Deque<T>::iterator operator+(const typename Deque<T>::iterator::Distance& number, const typename Deque<T>::iterator& it) noexcept {
  it += number;
  return it;
}
template <typename T>
typename Deque<T>::iterator operator-(const typename Deque<T>::iterator& it, const typename Deque<T>::iterator::Distance& number) noexcept {
  it -= number;
  return it;
}
template <typename T>
typename Deque<T>::iterator operator-(const typename Deque<T>::iterator::Distance& number, const typename Deque<T>::iterator& it) noexcept {
  it -= number;
  return it;
}

template <typename T>
typename Deque<T>::iterator::value_type& Deque<T>::iterator::operator*() const noexcept {
  return ptr_[pos_];
}
template <typename T>
typename Deque<T>::iterator::value_type* Deque<T>::iterator::operator->() const noexcept {
  return (ptr_ + pos_);
}

template <typename T>
typename Deque<T>::iterator::Distance operator-(const typename Deque<T>::iterator& obj1, const typename Deque<T>::iterator& obj2) noexcept {
  return abs((obj1.ptr_ - obj2.ptr_) + (obj1.pos_ - obj2.pos_));
}

template <typename T>
bool operator<(const typename Deque<T>::iterator& obj1, const typename Deque<T>::iterator& obj2) noexcept {
  return ((obj1.ptr_ - obj2.ptr_) + (obj1.pos_ - obj2.pos_)) < 0;
}
template <typename T>
bool operator>(const typename Deque<T>::iterator& obj1, const typename Deque<T>::iterator& obj2) noexcept {
  return obj2 < obj1;
}
template <typename T>
bool operator==(const typename Deque<T>::iterator& obj1, const typename Deque<T>::iterator& obj2) noexcept {
  return !(obj1 < obj2) && !(obj2 < obj1);
}
template <typename T>
bool operator<=(const typename Deque<T>::iterator& obj1, const typename Deque<T>::iterator& obj2) noexcept {
  return (obj1 < obj2) || (obj1 == obj2);
}
template <typename T>
bool operator>=(const typename Deque<T>::iterator& obj1, const typename Deque<T>::iterator& obj2) noexcept {
  return (obj1 > obj2) || (obj1 == obj2);
}
template <typename T>
bool operator!=(const typename Deque<T>::iterator& obj1, const typename Deque<T>::iterator& obj2) noexcept {
  return !(obj1 == obj2);
}

//------------const_iter------------/
template <typename T>
class Deque<T>::const_iterator {
  private:
  T* ptr_;
  size_t pos_;
  using iterator_category = std::random_access_iterator_tag;
  using value_type = T;
  using Distance = size_t;
  using Pointer = T*;
  using Reference	= T&;
  public:
};
//------------definitions_const_iter--------------//



int main() {
  // {
  //   test q;
  //   Deque<test> a(5, q);
  // }
  // try {
  //   for (size_t i = 0; i < 10; ++i)  {
  //     test1 d(i);
  //     Deque<test1> a(10, d); 
  //   }
  // } catch(const char* obj) {
  //   std::cout << obj << "\n";
  // } catch(...) {
  //   std::cout << "obj" << "\n";
  // }
  // std::vector<Deque<int>> a2(150, Deque<int>(500, 3));
  // std::vector<Deque<int>> a3(150, Deque<int>(500, 3));


  size_t n;
  std::cin >> n;
  Deque<int> arr(n, 10);
  auto it = arr.begin();
  for (size_t i = 0; i < n; ++i)  {
    arr[i] = i;
  }
  while (it != arr.end()) {
    std::cout << *it << " ";
    ++it;
  }
  std::cout << "\n";
  std::cout << std::endl;
  std::cout << arr.size() << " " << arr.empty() << std::endl;

  return 0;

}