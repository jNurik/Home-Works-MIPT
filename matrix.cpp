#pragma once

#include <algorithm>
#include <iostream>
#include <vector>

template <size_t N, size_t M, typename T = int64_t>
class Matrix {
public:
	Matrix() {
		arr_.resize(N);
		for (size_t i = 0; i < N; ++i) {
			arr_[i].resize(M);
		}
	}
	Matrix(std::vector<std::vector<T>> matrix) : arr_(matrix) {}
	Matrix(T element) : Matrix() {
		for (size_t i = 0; i < N; ++i) {
			for (size_t j = 0; j < M; ++j) {
				arr_[i][j] = element;
			}
		}
	}
	Matrix(const Matrix& matrix) : arr_(matrix.arr_) {}  // = default
	Matrix& operator=(const Matrix& matrix) {
		arr_ = matrix.arr_;
		return *this;
	}
	~Matrix() = default;

	T& operator()(size_t i, size_t j) { return arr_[i][j]; }
	T operator()(size_t i, size_t j) const { return arr_[i][j]; }

	Matrix<M, N, T> Transposed() const {
		Matrix<M, N, T> ans;
		for (size_t i = 0; i < N; ++i) {
			for (size_t j = 0; j < M; ++j) {
				ans(j, i) = arr_[i][j];
			}
		}
		return ans;
	}
	Matrix& operator+=(const Matrix& matrix) {
		for (size_t i = 0; i < N; ++i) {
			for (size_t j = 0; j < M; ++j) {
				arr_[i][j] += matrix.arr_[i][j];
			}
		}
		return *this;
	}
	Matrix& operator-=(const Matrix& matrix) {
		for (size_t i = 0; i < N; ++i) {
			for (size_t j = 0; j < M; ++j) {
				arr_[i][j] -= matrix.arr_[i][j];
			}
		}
		return *this;
	}

	Matrix& RowSwitching(size_t i, size_t j) {
		swap(arr_[i], arr_[j]);
		return *this;
	}
	Matrix& RowMultiplication(size_t i, double a) {
		for (size_t j = 0; j < M; ++j) {
			arr_[i][j] = static_cast<T>(a * arr_[i][j]);
		}
		return *this;
	}
	Matrix& RowAddition(size_t i, size_t j, double a) {
		for (size_t k = 0; k < M; ++k) {
			arr_[i][k] += static_cast<T>(a * arr_[j][k]);
		}
		return *this;
	}

	Matrix& GetStepView() {
		size_t iter = 0;
		for (size_t j = 0; j < M; ++j) {
			for (size_t i = iter; i < N; ++i) {
				if (arr_[i][j] != 0) {
					RowMultiplication(i, 1 / arr_[i][j]);
					//for (size_t j1 = 0; j1 < M; ++j1) {
					//	arr_[i][j1] = static_cast<T>((1 / arr_[i][j1]) * arr_[i][j1]);
					//}
					if (i != iter) {
						RowSwitching(i, iter);
					}
					for (size_t k = 0; k < iter; ++k) {
						RowAddition(k, iter, -arr_[k][j]);
					}
					for (size_t k = i; k < N; ++k) {
						if (k != iter) {
							RowAddition(k, iter, -arr_[k][j]);
						}
					}
					++iter;
					break;
				}
			}
			if (iter >= N) { break; }
		}
		return *this;
	}
	size_t Rank() {
		size_t iter = 0;
		for (size_t j = 0; j < M; ++j) {
			for (size_t i = iter; i < N; ++i) {
				if (arr_[i][j] != 0) {
					RowMultiplication(i, 1 / arr_[i][j]);
					if (i != iter) {
						RowSwitching(i, iter);
					}
					for (size_t k = 0; k < iter; ++k) {
						RowAddition(k, iter, -arr_[k][j]);
					}
					for (size_t k = i; k < N; ++k) {
						if (k != iter) {
							RowAddition(k, iter, -arr_[k][j]);
						}
					}
					++iter;
					break;
				}
			}
			if (iter >= N) { break; }
		}
		return iter;
	}
  
	template <size_t N, size_t M, typename T>
	friend size_t Rank(Matrix<N, M, T> object);

private:
	std::vector<std::vector<T>> arr_;
};

template <size_t N, size_t M, typename T>
size_t Rank(Matrix<N, M, T> object) { return object.Rank(); }

template <size_t N, size_t M, typename T>
Matrix<N, M, T> operator+(const Matrix<N, M, T>& matrix1,
	const Matrix<N, M, T>& matrix2) {
	Matrix ans = matrix1;
	ans += matrix2;
	return ans;
}
template <size_t N, size_t M, typename T>
Matrix<N, M, T> operator-(const Matrix<N, M, T>& matrix1,
	const Matrix<N, M, T>& matrix2) {
	Matrix ans = matrix1;
	ans -= matrix2;
	return ans;
}
template <size_t N, size_t M, typename T, size_t N1, size_t M1>
Matrix<N, M1, T> operator*(const Matrix<N, M, T>& matrix1,
	const Matrix<N1, M1, T>& matrix2) = delete;
template <size_t N, size_t M, typename T, size_t M1>
Matrix<N, M1, T> operator*(const Matrix<N, M, T>& matrix1,
	const Matrix<M, M1, T>& matrix2) {
	Matrix<N, M1, T> ans(static_cast<T>(0));
	for (size_t i = 0; i < N; ++i) {
		for (size_t j = 0; j < M1; ++j) {
			for (size_t k = 0; k < M; ++k) {
				ans(i, j) += matrix1(i, k) * matrix2(k, j);
			}
		}
	}
	return ans;
}
template <size_t N, size_t M, typename T>
Matrix<N, M, T> operator*(const Matrix<N, M, T>& matrix, const T& num) {
	Matrix<N, M, T> ans = matrix;
	for (size_t i = 0; i < N; ++i) {
		for (size_t j = 0; j < M; ++j) {
			ans(i, j) *= num;
		}
	}
	return ans;
}
template <size_t N, size_t M>
Matrix<N, M, int64_t> operator*(const Matrix<N, M, int64_t>& matrix,
	int64_t num) {
	Matrix<N, M, int64_t> ans = matrix;
	for (size_t i = 0; i < N; ++i) {
		for (size_t j = 0; j < M; ++j) {
			ans(i, j) *= num;
		}
	}
	return ans;
}
template <size_t N, size_t M, typename T>
Matrix<N, M, T> operator*(const T& num, const Matrix<N, M, T>& matrix) {
	Matrix<N, M, T> ans = matrix;
	for (size_t i = 0; i < N; ++i) {
		for (size_t j = 0; j < M; ++j) {
			ans(i, j) *= num;
		}
	}
	return ans;
}
template <size_t N, size_t M>
Matrix<N, M, int64_t> operator*(int64_t num,
	const Matrix<N, M, int64_t>& matrix) {
	Matrix<N, M, int64_t> ans = matrix;
	for (size_t i = 0; i < N; ++i) {
		for (size_t j = 0; j < M; ++j) {
			ans(i, j) *= num;
		}
	}
	return ans;
}
template <size_t N, size_t M, typename T>
std::ostream& operator<<(std::ostream& out, const Matrix<N, M, T>& matrix) {
	for (size_t i = 0; i < N; ++i) {
		for (size_t j = 0; j < M; ++j) {
			out << matrix(i, j) << ' ';
		}
		out << '\n';
	}
	return out;
}
template <size_t N, size_t M, typename T>
std::istream& operator>>(std::istream& in, Matrix<N, M, T>& matrix) {
	for (size_t i = 0; i < N; ++i) {
		for (size_t j = 0; j < M; ++j) {
			in >> matrix(i, j);
		}
	}
	return in;
}
template <size_t N, size_t M, typename T, size_t N1, size_t M1>
bool operator==(const Matrix<N, M, T>& matrix1,
	const Matrix<N1, M1, T>& matrix2) {
	if (N != N1 || M != M1) {
		return false;
	}
	for (size_t i = 0; i < N; ++i) {
		for (size_t j = 0; j < M; ++j) {
			if (matrix1(i, j) != matrix2(i, j)) {
				return false;
			}
		}
	}
	return true;
}
template <size_t N, typename T>
class Matrix<N, N, T> {
public:
	Matrix() {
		arr_.resize(N);
		for (size_t i = 0; i < N; ++i) {
			arr_[i].resize(N);
		}
	}
	Matrix(std::vector<std::vector<T>> matrix) : arr_(matrix) {}  // = default
	Matrix(T element) : Matrix() {
		for (size_t i = 0; i < N; ++i) {
			for (size_t j = 0; j < N; ++j) {
				arr_[i][j] = element;
			}
		}
	}
	Matrix(const Matrix& matrix) : arr_(matrix.arr_) {}  // = default
	Matrix& operator=(const Matrix& matrix) {
		arr_ = matrix.arr_;
		return *this;
	}
	~Matrix() = default;
	T& operator()(size_t i, size_t j) { return arr_[i][j]; }
	T operator()(size_t i, size_t j) const { return arr_[i][j]; }
	Matrix Transposed() const {
		Matrix ans;
		for (size_t i = 0; i < N; ++i) {
			for (size_t j = 0; j < N; ++j) {
				ans(j, i) = arr_[i][j];
			}
		}
		return ans;
	}
	Matrix& operator+=(const Matrix& matrix) {
		for (size_t i = 0; i < N; ++i) {
			for (size_t j = 0; j < N; ++j) {
				arr_[i][j] += matrix.arr_[i][j];
			}
		}
		return *this;
	}
	Matrix& operator-=(const Matrix& matrix) {
		for (size_t i = 0; i < N; ++i) {
			for (size_t j = 0; j < N; ++j) {
				arr_[i][j] -= matrix.arr_[i][j];
			}
		}
		return *this;
	}
	T Trace() {
		T ans = arr_[0][0];  // error
		for (size_t i = 1; i < N; ++i) {
			ans += arr_[i][i];
		}
		return ans;
	}

	Matrix& RowSwitching(size_t i, size_t j) {
		swap(arr_[i], arr_[j]);
		return *this;
	}
	Matrix& RowMultiplication(size_t i, double a) {
		for (size_t j = 0; j < N; ++j) {
			arr_[i][j] = static_cast<T>(a * arr_[i][j]);
		}
		return *this;
	}
	Matrix& RowAddition(size_t i, size_t j, double a) {
		for (size_t k = 0; k < N; ++k) {
			arr_[i][k] += static_cast<T>(a * arr_[j][k]);
		}
		return *this;
	}

	Matrix& GetStepView() {
		size_t iter = 0;
		for (size_t j = 0; j < N; ++j) {
			for (size_t i = iter; i < N; ++i) {
				if (arr_[i][j] != 0) {
					RowMultiplication(i, 1 / arr_[i][j]);
					if (i != iter) {
						RowSwitching(i, iter);
					}
					for (size_t k = 0; k < iter; ++k) {
						RowAddition(k, iter, -arr_[k][j]);
					}
					for (size_t k = i; k < N; ++k) {
						if (k != iter) {
							RowAddition(k, iter, -arr_[k][j]);
						}
					}
					++iter;
					break;
				}
			}
			if (iter >= N) { break; }
		}
		return *this;
	}
	size_t Rank() {
		size_t iter = 0;
		for (size_t j = 0; j < N; ++j) {
			for (size_t i = iter; i < N; ++i) {
				if (arr_[i][j] != 0) {
					RowMultiplication(i, 1 / arr_[i][j]);
					if (i != iter) {
						RowSwitching(i, iter);
					}
					for (size_t k = 0; k < iter; ++k) {
						RowAddition(k, iter, -arr_[k][j]);
					}
					for (size_t k = i; k < N; ++k) {
						if (k != iter) {
							RowAddition(k, iter, -arr_[k][j]);
						}
					}
					++iter;
					break;
				}
			}
			if (iter >= N) { break; }
		}
		return iter;
	}
	T det() {
		T det = 1;
		size_t iter = 0;
		for (size_t j = 0; j < N; ++j) {
			for (size_t i = iter; i < N; ++i) {
				if (arr_[i][j] != 0) {
					if (i != iter) {
						RowSwitching(i, iter);
						det *= -1;
					}
					for (size_t k = 0; k < iter; ++k) {
						RowAddition(k, iter, -arr_[k][j]);
					}
					for (size_t k = i; k < N; ++k) {
						if (k != iter) {
							RowAddition(k, iter, -arr_[k][j]);
						}
					}
					++iter;
					break;
				}
			}
			if (iter >= N) { break; }
		}
		for (size_t i = 0; i < N; ++i) {
			det *= arr_[i][i];
		}
		return det;
	}
	Matrix InverseMatrix() {
		std::vector<std::vector<T>> e(N, std::vector<T>(N));
		for (size_t i = 0; i < N; ++i) {
			for (size_t j = 0; j < N; ++j) {
				if (i == j) { e[i][j] = 1; }
				else { e[i][j] = 0; }
			}
		}
		Matrix<N, N, double> E(e);

		size_t iter = 0;
		for (size_t j = 0; j < N; ++j) {
			for (size_t i = iter; i < N; ++i) {
				if (arr_[i][j] != 0) {
					E = E.RowMultiplication(i, 1 / arr_[i][j]);
					RowMultiplication(i, 1 / arr_[i][j]);
					if (i != iter) {
						E = E.RowSwitching(i, iter);
						RowSwitching(i, iter);
					}
					for (size_t k = 0; k < iter; ++k) {
						E = E.RowAddition(k, iter, -arr_[k][j]);
						RowAddition(k, iter, -arr_[k][j]);
					}
					for (size_t k = i; k < N; ++k) {
						if (k != iter) {
							E = E.RowAddition(k, iter, -arr_[k][j]);
							RowAddition(k, iter, -arr_[k][j]);
						}
					}
					++iter;
					break;
				}
			}
			if (iter >= N) { break; }
		}
		return E;
	}

	template <size_t N, size_t M, typename T>
	friend size_t Rank(Matrix<N, M, T> object);

	template <size_t N, typename T>
	friend T det(Matrix<N, N, T> object);

private:
	std::vector<std::vector<T>> arr_;
};

template <size_t N, typename T>
T det(Matrix<N, N, T> object) { return object.det(); }

int main() {
	//std::vector<std::vector<double>> a = { {0,6,0,0,0,0}, {0,1,0,0,0,0}, {0,0,1,0,0,0}, {0,0,0,1,0,0}, {8,0,0,0,1,7}};
	////for (size_t i = 0; i < 5; ++i) {
	////	for (size_t j = 0; j < 6; ++j) {
	////		std::cin >> a[i][j];
	////	}
	////}
	//Matrix<5, 6, double> A(a);
	//std::cout << A.GetStepView() << Rank(A) << std::endl;
	{
		std::vector<std::vector<double>> b = { {1,1,0},{1,0,1},{0,1,1} };
		Matrix<3, 3, double> B(b);
		std::cout << "fuck: " << std::endl << B.InverseMatrix() << '\n'; //<< B.InverseMatrix().det() << '\n';
	}
	{
		std::vector<std::vector<double>> b = { {1,0,0,0,0},{0,0,3,0,0},{0,1,0,0,0},{0,0,0,1,0},{0,0,0,0,1} };
		Matrix<5, 5, double> B(b);
		std::cout << B << '\n';
		std::cout << "fuck: " << std::endl << B * B.InverseMatrix() << '\n';
	}
	//{
	//	std::vector<std::vector<double>> b = { {1,1,1,1,1}, {1,2,1,1,1},{1,1,3,1,1},{1,1,1,4,1},{1,1,1,1,5} };
	//	Matrix<5, 5, double> B(b);
	//	std::cout << B << '\n';
	//	std::cout << B.InverseMatrix() << '\n' << B.InverseMatrix().det() << '\n';
	//	std::cout << B.det() << '\n';
	//}
}
