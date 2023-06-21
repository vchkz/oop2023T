#include <arrayd/arrayd.hpp>
#include <stdexcept>

ArrayD::ArrayD(const std::ptrdiff_t size) {
    if (size < 0) {
        throw std::invalid_argument("Size must be non-negative");
    }
    if (size > 0) {
        capacity_ = size;
        data_ = new double[capacity_];
    }
    size_ = size;
}

ArrayD::~ArrayD() {
    delete[] data_;
}

ArrayD::ArrayD(const ArrayD& other) {
    size_ = other.size_;
    capacity_ = other.capacity_;
    data_ = new double[capacity_];
    std::copy(other.data_, other.data_ + size_, data_);
}

ArrayD& ArrayD::operator=(const ArrayD& other) {
    if (this != &other) {
        delete[] data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        data_ = new double[capacity_];
        std::copy(other.data_, other.data_ + size_, data_);
    }
    return *this;
}

double& ArrayD::operator[](const std::ptrdiff_t i) {
    if (i < 0 || i >= size_) {
        throw std::out_of_range("Index out of range");
    }
    return data_[i];
}

const double& ArrayD::operator[](const std::ptrdiff_t i) const {
    if (i < 0 || i >= size_) {
        throw std::out_of_range("Index out of range");
    }
    return data_[i];
}

std::ptrdiff_t ArrayD::ssize() const noexcept { return size_; }

void ArrayD::resize(const std::ptrdiff_t new_size) {
    if (new_size <= 0) {
        throw std::invalid_argument("New size must be non-negative");
    }
    if (new_size > capacity_) {
        capacity_ = new_size * 2;
        double* new_data = new double[capacity_];
        std::fill(new_data + size_, new_data + capacity_, 0); // fill with zeros
        std::copy(data_, data_ + size_, new_data);
        delete[] data_;
        data_ = new_data;
    }
    size_ = new_size;
}

void ArrayD::insert(const std::ptrdiff_t i, const double value) {
    if (i < 0 || i > size_) {
        throw std::out_of_range("Index out of range");
    }
    resize(size_ + 1);
    for (std::ptrdiff_t j = size_; j > i; --j) {
        data_[j] = data_[j - 1];
    }
    data_[i] = value;
}
void ArrayD::remove(const std::ptrdiff_t i) {
    if (i < 0 || i >= size_) {
        throw std::out_of_range("Index out of range");
    }
    for (std::ptrdiff_t j = i; j < size_ - 1; ++j) {
        data_[j] = data_[j + 1];
    }
    resize(size_ - 1);
}