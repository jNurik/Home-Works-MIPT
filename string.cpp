#include "string.hpp"

#include <cstddef>
#include <cstdlib>
#include <cstring>

String::String() : size_(0), capacity_(0), string_(nullptr) {}

String::String(size_t size, char character) : capacity_(size), size_(size) {
  string_ = new char[capacity_ + 1];
  memset(string_, character, size_);
  string_[size_] = '\0';
}

String::String(const char* string) : size_(strlen(string)), capacity_(strlen(string)), string_(new char[strlen(string) + 1]) {
  if (string == nullptr) {
    string_[0] = '\0';
  } else {
    memcpy(string_, string, size_);
    string_[size_] = '\0';
  }
}

String::String(const String& other) : size_(other.size_), capacity_(other.capacity_), string_(new char[capacity_ + 1]) {
  if (nullptr == other.string_) {
    string_[0] = '\0';
  } else {
    memcpy(string_, other.string_, size_);
    string_[size_] = '\0';
  }
}

String& String::operator=(const String& other) {
  if (string_ == other.string_) {
    return *this;
  }
  delete[] string_;
  size_ = other.size_;
  capacity_ = other.capacity_;
  string_ = new char[capacity_ + 1];
  memcpy(string_, other.string_, size_);
  string_[size_] = '\0';
  return *this;
}

void String::Clear() {
  size_ = 0;
  string_[0] = '\0';
}

void String::PushBack(char character) {
  if (capacity_ == 0) {
    capacity_ = 1;
    string_ = new char[capacity_ + 1];
    string_[0] = character;
    string_[1] = '\0';
    size_ = 1;
    return;
  }
  if (size_ == capacity_) {
    String new_string = *this;
    delete[] string_;
    capacity_ *= 2;
    string_ = new char[capacity_ + 1];
    memcpy(string_, new_string.string_, size_);
  }
  string_[size_] = character;
  ++size_;
  string_[size_] = '\0';
}

void String::PopBack() {
  if (size_ != 0) {
    --size_;
    string_[size_] = '\0';
  }
}

void String::Resize(size_t new_size) {
  if (capacity_ < new_size) {
    String new_string = *this;
    delete[] string_;
    capacity_ = new_size;
    string_ = new char[capacity_ + 1];
    memcpy(string_, new_string.string_, size_);
  }
  size_ = new_size;
  string_[size_] = '\0';
}

void String::Resize(size_t new_size, char character) {
  if (new_size < size_) {
    Resize(new_size);
    return;
  }
  size_t tmp = size_;
  if (capacity_ < new_size) {
    Resize(new_size);
  }
  size_ = new_size;
  memset(string_ + tmp, character, new_size - tmp);
  string_[size_] = '\0';
}

void String::Reserve(size_t new_cap) {
  if (capacity_ >= new_cap) {
    return;
  }
  String new_string = *this;
  delete[] string_;
  capacity_ = new_cap;
  string_ = new char[capacity_ + 1];
  memcpy(string_, new_string.string_, size_);
  string_[size_] = '\0';
}

void String::ShrinkToFit() {
  if (capacity_ > size_) {
    String new_str = *this;
    delete[] string_;
    capacity_ = size_;
    string_ = new char[capacity_ + 1];
    memcpy(string_, new_str.string_, size_);
    string_[size_] = '\0';
  }
}

void String::Swap(String& other) {
  std::swap(other.string_, string_);
  std::swap(other.capacity_, capacity_);
  std::swap(other.size_, size_);
}

char& String::operator[](size_t i) { return string_[i]; }

const char& String::operator[](size_t i) const { return string_[i]; }

char& String::Front() { return string_[0]; }

const char& String::Front() const { return string_[0]; }

char& String::Back() { return string_[size_ - 1]; }

const char& String::Back() const { return string_[size_ - 1]; }

bool String::Empty() const { return size_ == 0; }

size_t String::Size() const { return size_; }

size_t String::Capacity() const { return capacity_; }

char* String::Data() { return string_; }

const char* String::Data() const { return string_; }

bool operator>(const String& str1, const String& str2) {
  for (size_t i = 0; i < std::min(str1.Size(), str2.Size()); ++i) {
    if (str1[i] > str2[i]) {
      return true;
    }
    if (str1[i] < str2[i]) {
      return false;
    }
  }
  return str1.Size() > str2.Size();
}

bool operator<(const String& str1, const String& str2) { return str2 > str1; }

bool operator>=(const String& str1, const String& str2) {
  return !(str1 < str2);
}

bool operator<=(const String& str1, const String& str2) { return str2 >= str1; }

bool operator==(const String& str1, const String& str2) {
  return !(str1 > str2) && !(str1 < str2);
}

bool operator!=(const String& str1, const String& str2) {
  return !(str1 == str2);
}

String& String::operator+=(const char& character) {
  PushBack(character);
  return *this;
}

String& String::operator+=(const String& other) {
  for (size_t i = 0; i < other.size_; ++i) {
    PushBack(other[i]);
  }
  return *this;
}

String operator+(const String& str1, const String& str2) {
  if (str1.Data() == nullptr && str2.Data() != nullptr) {
    return str2;
  }
  if (str1.Data() == nullptr && str2.Data() == nullptr) {
    String new_str(1, 'e');
    new_str.PopBack();
    return new_str;
  }
  String new_str = str1;
  new_str += str2;
  return new_str;
}

String& String::operator*=(size_t num) {
  if (num == 0) {
    *this = "";
    return *this;
  }
  String tmp = *this;
  for (size_t i = 0; i + 1 < num; ++i) {
    *this += tmp;
  }
  return *this;
}

String operator*(const String& str, size_t num) {
  String new_str = str;
  new_str *= num;
  return new_str;
}

std::ostream& operator<<(std::ostream& out, const String& str) {
  out << str.Data();
  return out;
}

std::istream& operator>>(std::istream& in, String& str) {
  if (str.size_ != 0) {
    str.Clear();
  }
  char tmp = ' ';
  while (in.get(tmp) && tmp != '\n' && tmp != '\0' && tmp != '\t') {
    str.PushBack(tmp);
  }
  return in;
}

size_t Find(size_t beg, const String& str, const String& delim) {
  if (str.Empty()) {
    return 0;
  }
  for (size_t i = beg; i < str.Size(); ++i) {
    for (size_t j = 0; j < delim.Size(); ++j) {
      if (str[i + j] != delim[j]) {
        break;
      }
      if (j + 1 == delim.Size()) {
        return i;
      }
    }
  }
  return str.Size();
}

String Substr(size_t beg, size_t end, const String& str) {
  String ans(1, 'e');
  ans.PopBack();
  if (beg == end) {
    return ans;
  }
  for (size_t i = beg; i < end; ++i) {
    ans.PushBack(str[i]);
  }
  return ans;
}

std::vector<String> String::Split(const String& delim) const {
  std::vector<String> strings{};
  if (size_ < delim.size_ || string_ == nullptr) {
    strings = {""};
    return strings;
  }
  size_t beg = 0;
  size_t end = Find(beg, *this, delim);
  while (end != size_) {
    strings.push_back(Substr(beg, end, *this));
    beg = end + delim.size_;
    end = Find(beg, *this, delim);
  }
  strings.push_back(Substr(beg, size_, *this));
  return strings;
}

String String::Join(const std::vector<String>& strings) const {
  String new_str(1, 'e');
  new_str.PopBack();
  if (strings.empty()) {
    return new_str;
  }
  for (size_t i = 0; i + 1 < strings.size(); ++i) {
    new_str += strings[i];
    new_str += *this;
  }
  new_str += strings[strings.size() - 1];
  new_str[new_str.size_] = '\0';
  return new_str;
}

String::~String() { delete[] string_; }