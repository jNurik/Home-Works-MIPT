#include <vector>
#include <iostream>
#include <cmath>
#include <iterator>

size_t DivCeil(size_t num1, size_t num2) {
  return static_cast<size_t>(ceil((double)num1 / (double)num2));
}

template <typename T>
class Deque {
  private:
  //size of internal array
  static const size_t kSzInternalArr = 8;
  //size of external array
  size_t size_ = 0;
  //real size of external array
  size_t real_size_ = 0;
  //container size for users
  size_t size_deque_ = 0;
  //beginning of the deque
  size_t begin_deque_ = 0;
  //end of the deque
  size_t end_deque_ = 0;
  //pointer to the beginning of the external array
  //external array != deque
  T** external_array_ = nullptr;

  void swap(Deque& object);
  void copy(T**& new_external_array);

  public:
  
  template <bool IsConst>
  class CommonIterator {
    private:
    //pointer to an external array
    T** ptr_ = nullptr;
    //Position in the internal array
    std::ptrdiff_t pos_  = 0;
    public:

    using iterator_category = std::random_access_iterator_tag;
    using value_type = std::conditional_t<IsConst, const T, T>;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using reference	= value_type&;

    CommonIterator() noexcept = default;
    CommonIterator(T** ptr, size_t pos) noexcept : ptr_(ptr), pos_(pos) {}
    CommonIterator(const CommonIterator& obj) noexcept = default;
    ~CommonIterator() noexcept = default;
    CommonIterator<IsConst>& operator=(const CommonIterator& obj) noexcept = default;

    CommonIterator<IsConst>& operator++() noexcept {
      if (pos_ == kSzInternalArr - 1) {
        ++ptr_;
        pos_ = 0;
        return *this;
      }
      ++pos_;
      return *this;
    }
    CommonIterator<IsConst>& operator--() noexcept {
      if (pos_ == 0) {
        --ptr_;
        pos_ = kSzInternalArr - 1;
        return *this;
      }
      --pos_;
      return *this;
    }
    CommonIterator<IsConst> operator++(int) noexcept {
      CommonIterator<IsConst> tmp = *this;
      
      if (pos_ == kSzInternalArr - 1) {
        ++ptr_;
        pos_ = 0;
        return tmp;
      }
      ++pos_;
      return tmp;
    }

    CommonIterator<IsConst> operator--(int) noexcept {
      CommonIterator<IsConst> tmp = *this;
      if (pos_ == 0) {
        --ptr_; 
        pos_ = kSzInternalArr - 1;
        return tmp;
      }
      --pos_;
      return tmp;
    }

    CommonIterator<IsConst>& operator+=(difference_type number) noexcept {
      ptr_ += (number + pos_) / kSzInternalArr;
      pos_ = (number + pos_) % kSzInternalArr;
      return *this;
    }
    CommonIterator<IsConst>& operator-=(difference_type number) noexcept {
      if (pos_ <= number) {
        ptr_ -= DivCeil((number - pos_), kSzInternalArr);
      }
      pos_ = (pos_ - number) % kSzInternalArr;
      return *this;
    }

    reference operator*() const noexcept {
      return (*ptr_)[pos_];
    }

    pointer operator->() const noexcept {
      return (*ptr_ + pos_);
    }

    template <bool IsBool>
    difference_type operator-(const CommonIterator<IsBool>& iter) const noexcept {
      return std::abs((int)((ptr_ - iter.ptr_) * kSzInternalArr + (pos_ - iter.pos_)));
    }

    template <bool IsBool>
    bool operator<(const CommonIterator<IsBool>& iter) const noexcept {
      if (ptr_ < iter.ptr_) {
        return true;
      }
      if (ptr_ > iter.ptr_) {
        return false;
      }
      return (pos_ < iter.pos_);
    }

    template <bool IsBool>
    bool operator>(const CommonIterator<IsBool>& iter) const noexcept {
      return iter < *this;
    }

    template <bool IsBool>
    bool operator==(const CommonIterator<IsBool>& iter) const noexcept {
      return !(*this < iter) && !(iter < *this);
    }

    template <bool IsBool>
    bool operator!=(const CommonIterator<IsBool>& iter) const noexcept {
       return !(*this == iter);
    }

    template <bool IsBool>
    bool operator>=(const CommonIterator<IsBool>& iter) const noexcept {
      return !(*this < iter);
    }

    template <bool IsBool>
    bool operator<=(const CommonIterator<IsBool>& iter) const noexcept {
      return !(*this > iter);
    }

    CommonIterator<IsConst> operator+(size_t num) const noexcept {
      CommonIterator<IsConst> iter = *this;
      iter += num;
      return iter;
    }

    CommonIterator<IsConst> operator-(size_t num) const noexcept {
      CommonIterator<IsConst> iter = *this;
      iter -= num;
      return iter;
    }

    operator CommonIterator<true>() {
      return CommonIterator<true>(ptr_, pos_);
    }
  };

  using reverse_iterator = std::reverse_iterator<CommonIterator<false>>;
  using const_reverse_iterator = std::reverse_iterator<CommonIterator<true>>;
  using const_iterator = CommonIterator<true>;
  using iterator = CommonIterator<false>;

  Deque() noexcept;
  Deque(size_t count);
  Deque(size_t count, const T& value);
  Deque(const Deque& object);
  ~Deque() noexcept;
  Deque& operator=(const Deque& object);

  bool empty() const noexcept;
  size_t size() const noexcept;

  T& operator[](size_t index) noexcept;
  const T& operator[](size_t index) const noexcept;
  T& at(size_t index);
  const T& at(size_t index) const;

  void push_back(const T& value);
  void pop_back();
  void push_front(const T& value);
  void pop_front();

  iterator begin() noexcept;
  const_iterator begin() const noexcept;

  iterator end() noexcept;
  const_iterator end() const noexcept;

  const_iterator cbegin() const noexcept;
  const_iterator cend() const noexcept;


  reverse_iterator rbegin() noexcept;
  const_reverse_iterator rbegin() const noexcept;

  reverse_iterator rend() noexcept;
  const_reverse_iterator rend() const noexcept;

  const_reverse_iterator rcbegin() const noexcept;
  const_reverse_iterator rcend() const noexcept;

  void insert(iterator iter, const T& value);
  void erase(iterator iter);

};

//------------definitions_Deque--------------//

template <typename T>
Deque<T>::Deque() noexcept = default;

template <typename T>
Deque<T>::Deque(size_t count) : Deque(count, T()) {}

template <typename T>
Deque<T>::Deque(size_t count, const T& value) : size_(DivCeil(count, kSzInternalArr)), real_size_(size_), size_deque_(count), begin_deque_(0), end_deque_(size_deque_) {
  external_array_ = new T*[real_size_];
  for (size_t i = 0; i < size_; ++i) {
    external_array_[i] = reinterpret_cast<T*>(new uint8_t[sizeof(T) * kSzInternalArr]);
  }
  for (size_t i = begin_deque_; i < end_deque_; ++i) {
    size_t pos_external_array = i / kSzInternalArr;
    size_t pos_internal_array = i % kSzInternalArr;
    try {
      new(external_array_[pos_external_array] + pos_internal_array) T(value);
    } catch(...) {
      for (size_t j = begin_deque_; j < i; ++i)  {
        size_t pos_external_array_d = j / kSzInternalArr;
        size_t pos_internal_array_d = j % kSzInternalArr;
        (external_array_[pos_external_array_d] + pos_internal_array_d) -> ~T();
      }
      for (size_t j = 0; j < size_; ++j) {
        delete[] reinterpret_cast<uint8_t*>(external_array_[j]);
      }
      delete[] external_array_;
      throw;
    }
  }
}

template <typename T>
Deque<T>::~Deque() noexcept {
  for (size_t i = begin_deque_; i < end_deque_; ++i)  {
    size_t pos_external_array = i / kSzInternalArr;
    size_t pos_internal_array = i % kSzInternalArr;
    (external_array_[pos_external_array] + pos_internal_array) -> ~T();
  }
  for (size_t i = begin_deque_ / kSzInternalArr; i < DivCeil(end_deque_, kSzInternalArr); ++i)  {
    delete[] reinterpret_cast<uint8_t*>(external_array_[i]);
  }
  delete[] external_array_;
}

template <typename T>
Deque<T>::Deque(const Deque<T>& object) {
  this -> ~Deque();
  size_ = object.size_;
  real_size_ = object.real_size_;
  size_deque_ = object.size_deque_;
  begin_deque_ = object.begin_deque_;
  end_deque_ = object.end_deque_;
  external_array_ = new T*[real_size_];
  for (size_t i = begin_deque_ / kSzInternalArr; i < DivCeil(end_deque_, kSzInternalArr); ++i)  {
    external_array_[i] = reinterpret_cast<T*>(new uint8_t[sizeof(T) * kSzInternalArr]);
  }
  for (size_t i = begin_deque_; i < end_deque_; ++i)  {
    size_t pos_external_array = i / kSzInternalArr;
    size_t pos_internal_array = i % kSzInternalArr;
    try {
      new(external_array_[pos_external_array] + pos_internal_array) T(object.external_array_[pos_external_array][pos_internal_array]);
    } catch(...) {
      for (size_t j = begin_deque_; j < i; ++i)  {
        size_t pos_external_array_d = j / kSzInternalArr;
        size_t pos_internal_array_d = j % kSzInternalArr;
        (external_array_[pos_external_array_d] + pos_internal_array_d) -> ~T();
      }
      for (size_t j = begin_deque_/ kSzInternalArr; j < DivCeil(end_deque_, kSzInternalArr); ++j) {
        delete[] reinterpret_cast<uint8_t*>(external_array_[j]);
      }
      delete[] external_array_;
      throw;
    }
  }
}

template <typename T>
void Deque<T>::swap(Deque<T>& object) {
  std::swap(size_, object.size_);
  std::swap(real_size_, object.real_size_);
  std::swap(size_deque_, object.size_deque_);
  std::swap(begin_deque_, object.begin_deque_);
  std::swap(end_deque_, object.end_deque_);
  std::swap(external_array_, object.external_array_);
}

template <typename T>
Deque<T>& Deque<T>::operator=(const Deque<T>& object) {
  Deque<T> tmp = object;
  swap(tmp);
  return *this;
}

template <typename T>
bool Deque<T>::empty() const noexcept {
  return size_deque_ == 0;
}
template <typename T>
size_t Deque<T>::size() const noexcept {
  return size_deque_;
}
template <typename T>
T& Deque<T>::operator[](size_t index) noexcept {
  return external_array_[(begin_deque_ + index) / kSzInternalArr][(begin_deque_ + index) % kSzInternalArr];
}
template <typename T>
const T& Deque<T>::operator[](size_t index) const noexcept {
  return external_array_[(begin_deque_ + index) / kSzInternalArr][(begin_deque_ + index) % kSzInternalArr];
}
template <typename T>
T& Deque<T>::at(size_t index) {
  if (begin_deque_ + index < end_deque_) {
    return external_array_[(begin_deque_ + index)/ kSzInternalArr][(begin_deque_ + index) % kSzInternalArr];
  }
  throw std::out_of_range("oops");
}
template <typename T>
const T& Deque<T>::at(size_t index) const {
  if (begin_deque_ + index < end_deque_) {
  return external_array_[(begin_deque_ + index)/ kSzInternalArr][(begin_deque_ + index) % kSzInternalArr];
  }
  throw std::out_of_range("oops");
}

template <typename T>
void Deque<T>::copy(T**& new_external_array) {
  size_t begin = 3 * DivCeil(real_size_, 2);
  size_t end =  3 * DivCeil(real_size_, 2) + size_;
  for (size_t i = begin; i < end; ++i)  {
    new_external_array[i] = external_array_[i - begin + begin_deque_ / kSzInternalArr];
  }
}

template <typename T>
void Deque<T>::push_front(const T& value) {
  if (real_size_ == 0) {
    Deque<T> tmp(1, value);
    *this = tmp;
    return;
  }
  if (begin_deque_ == 0) {
    const size_t kSix = 6;
    T** new_external_array = new T*[size_ + kSix * DivCeil(real_size_,2)];
    copy(new_external_array);
    size_t begin = 3 * DivCeil(real_size_, 2) - 1;
    new_external_array[begin] = reinterpret_cast<T*>(new uint8_t[sizeof(T) * kSzInternalArr]);
    try {
      new(new_external_array[begin] + kSzInternalArr - 1) T(value);
    } catch(...) {
      delete[] reinterpret_cast<uint8_t*>(new_external_array[begin]);
      delete[] new_external_array;
      throw;
    }
    delete[] external_array_;
    begin_deque_ = (3 * DivCeil(real_size_, 2) - 1) * kSzInternalArr + kSzInternalArr - 1;
    end_deque_ = (3 * DivCeil(real_size_, 2) + size_ - 1) * kSzInternalArr + (end_deque_ % kSzInternalArr) + kSzInternalArr * (int)(end_deque_ % kSzInternalArr == 0);
    real_size_ =  kSix * DivCeil(real_size_, 2) + size_;
    ++size_;
    external_array_ = new_external_array;
  } else if (begin_deque_ % kSzInternalArr == 0) {
    external_array_[begin_deque_ / kSzInternalArr - 1] = reinterpret_cast<T*>(new uint8_t[sizeof(T) * kSzInternalArr]);
    try {
      new(external_array_[begin_deque_ / kSzInternalArr - 1] + kSzInternalArr - 1) T(value);
    } catch(...) {
      delete[] reinterpret_cast<uint8_t*>(external_array_[begin_deque_ / kSzInternalArr - 1]);
      throw;
    }
    --begin_deque_;
    ++size_;
  } else {
    try {
      new(external_array_[begin_deque_ / kSzInternalArr] + begin_deque_ % kSzInternalArr - 1) T(value);
    } catch(...) {
      throw;
    }
    --begin_deque_;
  }
  ++size_deque_;
}

template <typename T>
void Deque<T>::pop_front() {
  if (size_deque_ == 0) {
    return;
  }
  (external_array_[begin_deque_ / kSzInternalArr] + begin_deque_ % kSzInternalArr) -> ~T();
  if (begin_deque_ % kSzInternalArr == kSzInternalArr - 1) {
    delete[] reinterpret_cast<uint8_t*>(external_array_[begin_deque_ / kSzInternalArr]);
    --size_;
  }
  ++begin_deque_;
  --size_deque_;
}

template <typename T>
void Deque<T>::pop_back() {
  if (empty()) {
    return;
  }
  (external_array_[(end_deque_ - 1) / kSzInternalArr] + (end_deque_ - 1) % kSzInternalArr) -> ~T();
  if ((end_deque_ - 1) % kSzInternalArr == 0) {
    delete[] reinterpret_cast<uint8_t*>(external_array_[(end_deque_ - 1) / kSzInternalArr]);
    --size_;
  }
  --end_deque_;
  --size_deque_;
}

template <typename T>
void Deque<T>::push_back(const T& value) {
  if (real_size_ == 0) {
    Deque<T> tmp(1, value);
    *this = tmp;
    return;
  }
  if (end_deque_ / kSzInternalArr == real_size_) {
    const size_t kSix = 6;
    T** new_external_array = new T*[size_ + kSix * DivCeil(real_size_, 2)];
    copy(new_external_array);
    size_t end =  3 * DivCeil(real_size_, 2) + size_ + 1;
    new_external_array[end - 1] = reinterpret_cast<T*>(new uint8_t[sizeof(T) * kSzInternalArr]);
    try {
      new(new_external_array[end - 1] + 0) T(value);
    } catch(...) {
      delete[] reinterpret_cast<uint8_t*>(new_external_array[end - 1]);
      delete[] new_external_array;
      throw;
    }
    delete[] external_array_;
    begin_deque_ = (3 * DivCeil(real_size_, 2)) * kSzInternalArr + begin_deque_ % kSzInternalArr;
    end_deque_ = (3 * DivCeil(real_size_, 2) + size_) * kSzInternalArr + 1;
    real_size_ =  kSix * DivCeil(real_size_, 2) + size_;
    ++size_;
    external_array_ = new_external_array;
  } else if (end_deque_ % kSzInternalArr == 0) {
    external_array_[end_deque_ / kSzInternalArr] = reinterpret_cast<T*>(new uint8_t[sizeof(T) * kSzInternalArr]);
    try {
      new(external_array_[end_deque_ / kSzInternalArr] + 0) T(value);
    } catch(...) {
      delete[] reinterpret_cast<uint8_t*>(external_array_[end_deque_ / kSzInternalArr]);
      throw;
    }
    ++end_deque_;
    ++size_;
  } else {
    try {
      new(external_array_[end_deque_ / kSzInternalArr] + end_deque_ % kSzInternalArr) T(value);
    } catch(...) {
      throw;
    }
    ++end_deque_;
  }
  ++size_deque_;
}

template <typename T>
typename Deque<T>::iterator Deque<T>::begin() noexcept {
  return CommonIterator<false>(external_array_ + begin_deque_ / kSzInternalArr, begin_deque_ % kSzInternalArr);
}
template <typename T>
typename Deque<T>::const_iterator Deque<T>::begin() const noexcept {
  return CommonIterator<true>(external_array_ + begin_deque_ / kSzInternalArr, begin_deque_ % kSzInternalArr);
}

template <typename T>
typename Deque<T>::const_iterator Deque<T>::end() const noexcept {
  return CommonIterator<true>(external_array_ + end_deque_ / kSzInternalArr, end_deque_ % kSzInternalArr);
}
template <typename T>
typename Deque<T>::iterator Deque<T>::end() noexcept {
  return CommonIterator<false>(external_array_ + end_deque_ / kSzInternalArr, end_deque_ % kSzInternalArr);
}

template <typename T>
typename Deque<T>::const_iterator Deque<T>::cbegin() const noexcept {
 return CommonIterator<true>(external_array_ + begin_deque_ / kSzInternalArr, begin_deque_ % kSzInternalArr);
}
template <typename T>
typename Deque<T>::const_iterator Deque<T>::cend() const noexcept {
  return CommonIterator<true>(external_array_ + end_deque_ / kSzInternalArr, end_deque_ % kSzInternalArr);
}


template <typename T>
typename Deque<T>::reverse_iterator Deque<T>::rbegin() noexcept {
  Deque<T>::iterator iter = this -> end();
  reverse_iterator riter = reverse_iterator(iter);
  return riter;
}
template <typename T>
typename Deque<T>::const_reverse_iterator Deque<T>::rbegin() const noexcept {
  Deque<T>::const_iterator iter = this -> end();
  return const_reverse_iterator(iter);
}

template <typename T>
typename Deque<T>::const_reverse_iterator Deque<T>::rend() const noexcept {
  Deque<T>::const_iterator iter = this -> begin();
  return const_reverse_iterator(iter);
}
template <typename T>
typename Deque<T>::reverse_iterator Deque<T>::rend() noexcept {
  Deque<T>::iterator iter = this -> begin();
  return reverse_iterator(iter);
}

template <typename T>
typename Deque<T>::const_reverse_iterator Deque<T>::rcbegin() const noexcept {
  Deque<T>::const_iterator iter = this -> end();
  return const_reverse_iterator(iter);
}
template <typename T>
typename Deque<T>::const_reverse_iterator Deque<T>::rcend() const noexcept {
  Deque<T>::const_iterator iter = this -> begin();
  return const_reverse_iterator(iter);
}

template <typename T>
void Deque<T>::insert(iterator iter, const T& value) {
  iterator counter = this -> begin();
  T** new_external_array = new T*[real_size_ + (int)(end_deque_ % kSzInternalArr == 0)];
  size_t end = (int)(end_deque_ % kSzInternalArr == 0) + DivCeil(end_deque_, kSzInternalArr);
  for (size_t i = begin_deque_/ kSzInternalArr; i < end; ++i)  {
    new_external_array[i] = reinterpret_cast<T*>(new uint8_t[sizeof(T) * kSzInternalArr]);
  }
  bool is_add = false;
  for (size_t i = begin_deque_; i <= end_deque_; ++i)  {
    size_t pos_external_array = i / kSzInternalArr;
    size_t pos_internal_array = i % kSzInternalArr;
    try {
      if (iter == counter && !is_add) {
        new(new_external_array[pos_external_array] + pos_internal_array) T(value);
        is_add = true;
      } else {
        new(new_external_array[pos_external_array] + pos_internal_array) T(*counter);
        ++counter;
      }
    } catch (...) {
      for (size_t j = begin_deque_; j < i; ++i)  {
        size_t pos_external_array_d = j / kSzInternalArr;
        size_t pos_internal_array_d = j % kSzInternalArr;
        (new_external_array[pos_external_array_d] + pos_internal_array_d) -> ~T();
      }
      for (size_t j = begin_deque_/ kSzInternalArr; j < end; ++j) {
        delete[] reinterpret_cast<uint8_t*>(new_external_array[j]);
      }
      delete[] new_external_array;
      throw;
    }
  }
  std::vector<size_t> tmp = {size_, real_size_, end_deque_, begin_deque_, size_deque_};
  this -> ~Deque();
  size_ = tmp[0] + (int)(end_deque_ % kSzInternalArr == 0);
  real_size_ = tmp[1] + (int)(end_deque_ % kSzInternalArr == 0);
  end_deque_ = tmp[2] + 1;
  begin_deque_ = tmp[3];
  size_deque_ = tmp[4] + 1;
  external_array_ = new_external_array;
}

template <typename T>
void Deque<T>::erase(iterator iter) {
  iterator counter = this -> begin();
  T** new_external_array = new T*[real_size_ - (int)(end_deque_ % kSzInternalArr == 1)];
  size_t end = DivCeil(end_deque_, kSzInternalArr) - (int)(end_deque_ % kSzInternalArr == 1);
  for (size_t i = begin_deque_ / kSzInternalArr; i <end; ++i) {
    new_external_array[i] = reinterpret_cast<T*>(new uint8_t[sizeof(T) * kSzInternalArr]);
  }
  bool is_delete = false;
  for (size_t i = begin_deque_; i < end_deque_; ++i) {
    try {
      size_t pos_external_array = (i - (int)is_delete) / kSzInternalArr;
      size_t pos_internal_array = (i - (int)is_delete) % kSzInternalArr;
      if (iter != counter) {
        new(new_external_array[pos_external_array] + pos_internal_array) T(*counter);
      } else {
        is_delete = true;
      }
      ++counter;
    } catch (...) {
      for (size_t j = begin_deque_; j < i; ++j) {
        size_t pos_external_array_d = j / kSzInternalArr;
        size_t pos_internal_array_d = j % kSzInternalArr;
        (new_external_array[pos_external_array_d] + pos_internal_array_d) -> ~T();
      }
      for (size_t j = begin_deque_ / kSzInternalArr; j < end; ++j) { 
        delete[] reinterpret_cast<uint8_t*>(new_external_array[j]);
      }
      delete[] new_external_array;
      throw;
    }
  }
  std::vector<size_t> tmp = {size_, real_size_, end_deque_, begin_deque_, size_deque_};
  this -> ~Deque();
  size_ = tmp[0] - (int)(end_deque_ % kSzInternalArr == 1);
  real_size_ = tmp[1] - (int)(end_deque_ % kSzInternalArr == 1);
  end_deque_ = tmp[2] - 1;
  begin_deque_ = tmp[3];
  size_deque_ = tmp[4] - 1;
  external_array_ = new_external_array;
}