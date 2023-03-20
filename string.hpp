#pragma once
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <vector>

class String {
 public:
  String();

  String(size_t size, char character);

  String(const char*);

  String(const String& other);

  ~String();

  String& operator=(const String& other);

  void Clear();

  void PushBack(char character);

  void PopBack();

  void Resize(size_t new_size);

  void Resize(size_t new_size, char character);

  void Reserve(size_t new_cap);

  void ShrinkToFit();

  void Swap(String& other);

  char& operator[](size_t i);

  const char& operator[](size_t i) const;

  char& Front();

  const char& Front() const;

  char& Back();

  const char& Back() const;

  bool Empty() const;

  size_t Size() const;

  size_t Capacity() const;

  char* Data();

  const char* Data() const;

  String& operator+=(const char& character);

  String& operator+=(const String& other);

  String& operator*=(size_t num);

  friend std::istream& operator>>(std::istream& in, String& str);

  std::vector<String> Split(const String& delim = " ") const;

  String Join(const std::vector<String>& strings) const;

  friend String Substr(size_t beg, size_t end, const String& str);

 private:
  size_t size_ = 0;
  size_t capacity_ = 0;
  char* string_ = nullptr;
};

bool operator>(const String& str1, const String& str2);

bool operator<(const String& str1, const String& str2);

bool operator>=(const String& str1, const String& str2);

bool operator<=(const String& str1, const String& str2);

bool operator==(const String& str1, const String& str2);

bool operator!=(const String& str1, const String& str2);

String operator+(const String& str1, const String& str2);

String operator*(const String& str, size_t num);

std::ostream& operator<<(std::ostream& out, const String& str);

size_t Find(size_t beg, const String& str, const String& delim);