#include "geometry.hpp"

#include <cmath>

Vector::Vector() = default;

Vector::~Vector() = default;

Vector::Vector(const Vector& vec) = default;

Vector::Vector(int64_t cord_x, int64_t cord_y)
    : cord_x_(cord_x), cord_y_(cord_y) {}

Vector& Vector::operator=(const Vector& vec) = default;

Vector& Vector::operator+=(const Vector& vec) {
  cord_x_ += vec.cord_x_;
  cord_y_ += vec.cord_y_;
  return *this;
}

Vector& Vector::operator-=(const Vector& vec) {
  cord_x_ -= vec.cord_x_;
  cord_y_ -= vec.cord_y_;
  return *this;
}

Vector& Vector::operator*=(int64_t num) {
  cord_x_ *= num;
  cord_y_ *= num;
  return *this;
}

Vector Vector::operator-() const {
  Vector ans;
  ans.cord_x_ = -cord_x_;
  ans.cord_y_ = -cord_y_;
  return ans;
}

int64_t Vector::GetX() const { return cord_x_; }

int64_t Vector::GetY() const { return cord_y_; }

int64_t operator*(const Vector& vec1, const Vector& vec2) {
  return vec1.GetX() * vec2.GetX() + vec1.GetY() * vec2.GetY();
}

int64_t operator^(const Vector& vec1, const Vector& vec2) {
  return vec1.GetX() * vec2.GetY() - vec1.GetY() * vec2.GetX();
}

Vector operator+(const Vector& vec1, const Vector& vec2) {
  Vector ans = vec1;
  ans += vec2;
  return ans;
}

Vector operator-(const Vector& vec1, const Vector& vec2) {
  Vector ans = vec1;
  ans -= vec2;
  return ans;
}

Vector operator*(int64_t num, Vector vec) {
  vec *= num;
  return vec;
}

Vector operator*(Vector vec, int64_t num) {
  vec *= num;
  return vec;
}

Vector operator*(int64_t num, const Vector& vec) {
  Vector ans = vec;
  ans *= num;
  return ans;
}

Vector operator*(const Vector& vec, int64_t num) {
  Vector ans = vec;
  ans *= num;
  return ans;
}

//-----IShape-------//
IShape::~IShape() = default;

//------Point-------//

Point::Point() = default;

Point::Point(int64_t cord_x, int64_t cord_y)
    : cord_x_(cord_x), cord_y_(cord_y) {}

Point::Point(const Vector& vec) : cord_x_(vec.GetX()), cord_y_(vec.GetY()) {}

Point::Point(const Point& point) = default;

Point& Point::operator=(const Point& point) = default;

Point::~Point() = default;

int64_t Point::GetX() const { return cord_x_; }

int64_t Point::GetY() const { return cord_y_; }

void Point::Move(const Vector& vec) {
  cord_x_ += vec.GetX();
  cord_y_ += vec.GetY();
}

bool Point::ContainsPoint(const Point& point) const {
  return cord_x_ == point.cord_x_ && cord_y_ == point.cord_y_;
}

bool Point::CrossSegment(const Segment& segment) const {
  bool are_collinear =
      ((*this - segment.GetA()) ^ (segment.GetB() - segment.GetA())) == 0;
  bool are_directed_same_way =
      ((segment.GetA() - *this) * (segment.GetB() - *this)) <= 0;
  return are_collinear && are_directed_same_way;
}

IShape* Point::Clone() const {
  Point* copy = new Point(*this);
  return copy;
}

Vector operator-(const Point& point1, const Point& point2) {
  Vector ans(point1.GetX() - point2.GetX(), point1.GetY() - point2.GetY());
  return ans;
}

Vector operator+(const Point& point1, const Point& point2) {
  Vector ans(point1.GetX() + point2.GetX(), point1.GetY() + point2.GetY());
  return ans;
}

//------Segment-------//

Segment::Segment() = default;

Segment::Segment(const Point& point1, const Point& point2)
    : point1_(point1), point2_(point2) {}

Segment::Segment(const Segment& segment) = default;

Segment& Segment::operator=(const Segment& segment) = default;

Segment::~Segment() = default;

Point Segment::GetA() const { return point1_; }

Point Segment::GetB() const { return point2_; }

void Segment::Move(const Vector& vec) {
  point1_.Move(vec);
  point2_.Move(vec);
}

bool Segment::ContainsPoint(const Point& point) const {
  return point.CrossSegment(*this);
}

bool Segment::CrossSegment(const Segment& segment) const {
  bool are_collinear =
      ((point2_ - point1_) ^ (segment.point2_ - segment.point1_)) == 0;
  if (are_collinear) {
    if (((segment.point1_ - point1_) ^ (segment.point2_ - point1_)) != 0) {
      return false;
    }
    bool are_point1_in_segment =
        ((segment.point1_ - point1_) * (segment.point2_ - point1_)) <= 0;
    bool are_point2_in_segment =
        ((segment.point1_ - point2_) * (segment.point2_ - point2_)) <= 0;
    bool are_s_point1_in_this =
        ((point1_ - segment.point1_) * (point2_ - segment.point1_)) <= 0;
    bool are_s_point2_in_this =
        ((point1_ - segment.point2_) * (point2_ - segment.point2_)) <= 0;
    return (are_point1_in_segment || are_point2_in_segment) ||
           (are_s_point1_in_this || are_s_point2_in_this);
  }
  int64_t exp1 =
      (segment.point2_ - segment.point1_) ^ (point1_ - segment.point1_);
  int64_t exp2 =
      (segment.point2_ - segment.point1_) ^ (point2_ - segment.point1_);
  int64_t exp3 = (point2_ - point1_) ^ (segment.point1_ - point1_);
  int64_t exp4 = (point2_ - point1_) ^ (segment.point2_ - point1_);
  if (exp1 == 0) {
    return ((point1_ - segment.point1_) * (point1_ - segment.point2_)) <= 0;
  }
  if (exp2 == 0) {
    return ((point2_ - segment.point1_) * (point2_ - segment.point2_)) <= 0;
  }
  if (exp3 == 0) {
    return ((segment.point1_ - point1_) * (segment.point1_ - point2_)) <= 0;
  }
  if (exp4 == 0) {
    return ((segment.point2_ - point1_) * (segment.point2_ - point2_)) <= 0;
  }
  bool condition1 = (exp1 * exp2) < 0;
  bool condition2 = (exp3 * exp4) < 0;
  return condition1 && condition2;
}

IShape* Segment::Clone() const {
  Segment* copy = new Segment(*this);
  return copy;
}

//-------Line--------//

Line::Line() = default;

Line::Line(const Point& point_on_line, const Vector& guide_vector)
    : point_on_line_(point_on_line), guide_vector_(guide_vector) {}

Line::Line(const Point& point1, const Point& point2)
    : point_on_line_(point1), guide_vector_(point2 - point1) {}

Line::Line(const Line& line) = default;

Line& Line::operator=(const Line& line) = default;

Line::~Line() = default;

int64_t Line::GetA() { return guide_vector_.GetY(); }

int64_t Line::GetB() { return -(guide_vector_.GetX()); }

int64_t Line::GetC() {
  Vector normal_to_guide_vector(guide_vector_.GetY(), -(guide_vector_.GetX()));
  Vector vector_on_line(point_on_line_.GetX(), point_on_line_.GetY());
  return -(normal_to_guide_vector * vector_on_line);
}

void Line::Move(const Vector& vec) {
  Point move_point(vec);
  point_on_line_ = Point(move_point + point_on_line_);
}

bool Line::ContainsPoint(const Point& point) const {
  return ((point_on_line_ - point) ^ guide_vector_) == 0;
}

bool Line::CrossSegment(const Segment& segment) const {
  bool ans = ((guide_vector_ ^ (segment.GetA() - point_on_line_)) *
              (guide_vector_ ^ (segment.GetB() - point_on_line_))) <= 0;
  return ans;
}

IShape* Line::Clone() const {
  Line* copy = new Line(*this);
  return copy;
}

//-------Ray--------//
Ray::Ray() = default;

Ray::Ray(const Point& point1, const Point& point2)
    : beg_point_(point1), guide_vector_(point2 - point1) {}

Ray::Ray(const Ray& ray) = default;

Ray& Ray::operator=(const Ray& ray) = default;

Ray::~Ray() = default;

Point Ray::GetA() { return beg_point_; }

Vector Ray::GetVector() { return guide_vector_; }

void Ray::Move(const Vector& vec) {
  Point move_point(vec);
  beg_point_ = Point(move_point + beg_point_);
}

bool Ray::ContainsPoint(const Point& point) const {
  bool are_point_on_line = ((point - beg_point_) ^ guide_vector_) == 0;
  bool are_directed_same_way = ((point - beg_point_) * guide_vector_) >= 0;
  return are_point_on_line && are_directed_same_way;
}

bool Ray::CrossSegment(const Segment& segment) const {
  Vector vec1 = segment.GetA() - beg_point_;
  Vector vec2 = segment.GetB() - beg_point_;
  Vector guide_vec_segment = segment.GetA() - segment.GetB();
  int64_t sign = guide_vec_segment ^ guide_vector_;
  if (sign == 0) {
    bool are_a_in_ray = (guide_vector_ * vec1) >= 0;
    bool are_b_in_ray = (guide_vector_ * vec2) >= 0;
    return are_b_in_ray || are_a_in_ray;
  }
  if ((guide_vec_segment ^ vec1) * sign < 0) {
    return false;
  }
  if ((guide_vec_segment ^ vec1) * sign == 0) {
    return beg_point_.CrossSegment(segment);
  }
  return (guide_vector_ ^ vec1) * (guide_vector_ ^ vec2) <= 0;
}

IShape* Ray::Clone() const {
  Ray* copy = new Ray(*this);
  return copy;
}

//-------Circle------//
Circle::Circle() = default;

Circle::Circle(const Point& centre, int64_t radius)
    : centre_(centre), radius_(radius) {}

Circle::Circle(const Circle& circle) = default;

Circle& Circle::operator=(const Circle& circle) = default;

Circle::~Circle() = default;

Point Circle::GetCentre() const { return centre_; }

int64_t Circle::GetRadius() const { return radius_; }

void Circle::Move(const Vector& vec) { centre_ = Point(centre_ + Point(vec)); }

bool Circle::ContainsPoint(const Point& point) const {
  int64_t tmp =
      (centre_.GetX() - point.GetX()) * (centre_.GetX() - point.GetX()) +
      (centre_.GetY() - point.GetY()) * (centre_.GetY() - point.GetY()) -
      (radius_ * radius_);
  return tmp <= 0;
}

bool Circle::CrossSegment(const Segment& segment) const {
  double dist_a_to_centre = pow((centre_.GetX() - segment.GetA().GetX()), 2) +
                            pow((centre_.GetY() - segment.GetA().GetY()), 2);
  double dist_b_to_centre = pow((centre_.GetX() - segment.GetB().GetX()), 2) +
                            pow((centre_.GetY() - segment.GetB().GetY()), 2);
  if (dist_a_to_centre < pow(radius_, 2) &&
      dist_b_to_centre < pow(radius_, 2)) {
    return false;
  }
  if (((dist_a_to_centre - pow(radius_, 2)) *
       (dist_b_to_centre - pow(radius_, 2))) <= 0) {
    return true;
  }
  Line line(segment.GetA(), segment.GetB());
  double tmp = sqrt(pow(line.GetA(), 2) + pow(line.GetB(), 2));
  double dis_line_to_centre = static_cast<double>(std::abs(
                                  line.GetA() * centre_.GetX() +
                                  line.GetB() * centre_.GetY() + line.GetC())) /
                              tmp;
  Vector guide_vec = segment.GetB() - segment.GetA();
  Vector normal_guide_vec(guide_vec.GetY(), -guide_vec.GetX());
  if (dis_line_to_centre > radius_) {
    return false;
  }
  int64_t sign1 = (segment.GetA() - centre_) ^ normal_guide_vec;
  int64_t sign2 = (segment.GetB() - centre_) ^ normal_guide_vec;
  return sign1 * sign2 <= 0;
}

IShape* Circle::Clone() const {
  Circle* copy = new Circle(*this);
  return copy;
}

