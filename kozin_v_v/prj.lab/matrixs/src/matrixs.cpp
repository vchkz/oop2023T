//
// Created by kozin on 15.03.2023.
//
#include "matrixs/matrixs.hpp"


MatrixS::MatrixS(const SizeType& size) : m_(std::get<0>(size)), n_(std::get<1>(size)), data_(m_ * n_) {}

MatrixS::MatrixS(const std::ptrdiff_t m, const std::ptrdiff_t n) : m_(m), n_(n) {
    if (m <= 0 || n <= 0) {
        throw std::invalid_argument("Invalid matrix size");
    }
    data_.resize(m * n);
}

MatrixS::~MatrixS() = default;

MatrixS::MatrixS(const MatrixS& other) : m_(other.m_), n_(other.n_), data_(other.data_) {}

MatrixS& MatrixS::operator=(const MatrixS& other) {
    if (this != &other) {
        m_ = other.m_;
        n_ = other.n_;
        data_ = other.data_;
    }
    return *this;
}

int& MatrixS::at(const SizeType& elem) {
    if (std::get<0>(elem) < 0 || std::get<0>(elem) >= m_ || std::get<1>(elem) < 0 || std::get<1>(elem) >= n_) {
        throw std::out_of_range("Index out of range");
    }
    return data_[std::get<0>(elem) * n_ + std::get<1>(elem)];
}

const int& MatrixS::at(const SizeType& elem) const {
    if (std::get<0>(elem) < 0 || std::get<0>(elem) >= m_ || std::get<1>(elem) < 0 || std::get<1>(elem) >= n_) {
        throw std::out_of_range("Index out of range");
    }
    return data_[std::get<0>(elem) * n_ + std::get<1>(elem)];
}

int &MatrixS::at(const std::ptrdiff_t i, const std::ptrdiff_t j) {
    if (i < 0 || i >= m_ || j < 0 || j >= n_) {
        throw std::out_of_range("Index out of range");
    }
    return data_[i * n_ + j];
}

const int &MatrixS::at(const std::ptrdiff_t i, const std::ptrdiff_t j) const {
    if (i < 0 || i >= m_ || j < 0 || j >= n_) {
        throw std::out_of_range("Index out of range");
    }
    return data_[i * n_ + j];
}

void MatrixS::resize(const SizeType& new_size) {
    if (std::get<0>(new_size) <= 0 || std::get<1>(new_size) <= 0) {
        throw std::invalid_argument("Invalid matrix size");
    }
    std::vector<int> new_data(std::get<0>(new_size) * std::get<1>(new_size));
    for (std::ptrdiff_t i = 0; i < std::min(m_, std::get<0>(new_size)); ++i) {
        for (std::ptrdiff_t j = 0; j < std::min(n_, std::get<1>(new_size)); ++j) {
            new_data[i * std::get<1>(new_size) + j] = at({i, j});
        }
    }
    m_ = std::get<0>(new_size);
    n_ = std::get<1>(new_size);
    data_ = std::move(new_data);
}

void MatrixS::resize(const std::ptrdiff_t i, const std::ptrdiff_t j) {
    if (i <= 0 || j <= 0) {
        throw std::invalid_argument("Invalid matrix size");
    }
    m_ = i;
    n_ = j;
    data_.resize(m_ * n_);
}

const MatrixS::SizeType& MatrixS::ssize() const noexcept {
    return {m_, n_};
}

ptrdiff_t MatrixS::nRows() const noexcept {
    return m_;
}

ptrdiff_t MatrixS::nCols() const noexcept {
    return n_;
}

