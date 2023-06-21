//
// Created by kozin on 24.03.2023.
//
#include <iostream>
#include <arrayt/arrayt.hpp>


int main() {
    ArrayT<int> arr(3);
    arr[0] = 1;
    arr[1] = 2;
    arr[2] = 3;

    std::cout << "Array size: " << arr.ssize() << std::endl;
    std::cout << "Array elements: ";
    for (std::ptrdiff_t i = 0; i < arr.ssize(); ++i) {
        std::cout << arr[i] << ' ';
    }
    std::cout << std::endl;

    arr.resize(5);
    std::cout << "Array size after resize and insertions: " << arr.ssize() << std::endl;
    arr.insert(3, 4);
    arr.insert(4, 5);

    std::cout << "Array size after resize and insertions: " << arr.ssize() << std::endl;
    std::cout << "Array elements after resize and insertions: ";
    for (std::ptrdiff_t i = 0; i < arr.ssize(); ++i) {
        std::cout << arr[i] << ' ';
    }
    std::cout << std::endl;

    arr.remove(1);
    arr.insert(1, 2);

    std::cout << "Array size after removal: " << arr.ssize() << std::endl;
    std::cout << "Array elements after removal: ";
    for (std::ptrdiff_t i = 0; i < arr.ssize(); ++i) {
        std::cout << arr[i] << ' ';
    }
    std::cout << std::endl;

    return 0;
}

