#pragma once

#include <iostream>

class Vector {
 public:
  Vector();

  ~Vector();

  Vector(const Vector& vec);

  Vector(int64_t cord_x, int64_t cord_y);

  Vector& operator=(const Vector& vec);

  Vector& operator+=(const Vector& vec);

  Vector& operator-=(const Vector& vec);

  Vector& operator*=(int64_t num);

  Vector operator-() const;

  int64_t GetX() const;

  int64_t GetY() const;

 private:
  int64_t cord_x_;
  int64_t cord_y_;
};

int64_t operator*(const Vector& vec1, const Vector& vec2);

int64_t operator^(const Vector& vec1, const Vector& vec2);

Vector operator+(const Vector& vec1, const Vector& vec2);

Vector operator-(const Vector& vec1, const Vector& vec2);

Vector operator*(int64_t num, const Vector& vec);

Vector operator*(const Vector& vec, int64_t num);

Vector operator*(int64_t num, Vector vec);

Vector operator*(Vector vec, int64_t num);

class IShape;

class Point;

class Segment;

class Line;

class Ray;

class Circle;

class IShape {
 public:
  virtual void Move(const Vector&) = 0;

  virtual bool ContainsPoint(const Point&) const = 0;

  virtual bool CrossSegment(const Segment&) const = 0;

  virtual IShape* Clone() const = 0;

  virtual ~IShape();
};

class Point : public IShape {
 public:
  Point();

  Point(int64_t cord_x, int64_t cord_y);

  Point(const Vector& vec);

  Point(const Point& point);

  Point& operator=(const Point& point);

  ~Point();

  int64_t GetX() const;

  int64_t GetY() const;

  void Move(const Vector& vec) override;

  bool ContainsPoint(const Point& point) const override;

  bool CrossSegment(const Segment& segment) const override;

  IShape* Clone() const override;

 private:
  int64_t cord_x_;

  int64_t cord_y_;
};

Vector operator-(const Point& point1, const Point& point2);

class Segment : public IShape {
 public:
  Segment();

  Segment(const Point& point1, const Point& point2);

  Segment(const Segment& segment);

  Segment& operator=(const Segment& segment);

  ~Segment();

  Point GetA() const;

  Point GetB() const;

  void Move(const Vector& vec) override;

  bool ContainsPoint(const Point& point) const override;

  bool CrossSegment(const Segment& segment) const override;  //

  IShape* Clone() const override;

 private:
  Point point1_;

  Point point2_;
};

class Line : public IShape {
 public:
  Line();

  Line(const Point& point_on_line, const Vector& guide_vector);

  Line(const Point& point1, const Point& point2);

  Line(const Line& line);

  Line& operator=(const Line& line);

  ~Line();

  int64_t GetA();

  int64_t GetB();

  int64_t GetC();

  void Move(const Vector& vec) override;

  bool ContainsPoint(const Point& point) const override;

  bool CrossSegment(const Segment& segment) const override;

  IShape* Clone() const override;

 private:
  Point point_on_line_;

  Vector guide_vector_;
};

class Ray : public IShape {
 public:
  Ray();

  Ray(const Point& point1, const Point& point2);

  Ray(const Ray& ray);

  Ray& operator=(const Ray& ray);

  ~Ray();

  Point GetA();

  Vector GetVector();

  void Move(const Vector& vec) override;

  bool ContainsPoint(const Point& point) const override;

  bool CrossSegment(const Segment& segment) const override;

  IShape* Clone() const override;

 private:
  Point beg_point_;

  Vector guide_vector_;
};

class Circle : public IShape {
 public:
  Circle();

  Circle(const Point& centre, int64_t radius);

  Circle(const Circle& circle);

  Circle& operator=(const Circle& circle);

  ~Circle();

  Point GetCentre() const;

  int64_t GetRadius() const;

  void Move(const Vector& vec) override;

  bool ContainsPoint(const Point& point) const override;

  bool CrossSegment(const Segment& segment) const override;

  IShape* Clone() const override;

 private:
  Point centre_;

  int64_t radius_;
};
