//
// Created by ablkdg on 24.08.2023.
//
#pragma once

#ifndef DYNAMICARRAY_VECTOR_H
#define MIN_VECTOR_CAPACITY 8
#define VECTOR_GROW_POWER 2

#include <cstdlib>
#include <algorithm>
#include <initializer_list>

namespace custom {
    // Template class for a custom vector
    template<typename Type>
    class vector {
    public:
        // Adds an element to the end of the vector
        void push_back(Type value);

        // Removes the last element of the vector
        void pop_back();

        // Returns the element at a given index
        Type at(int index);

        // Overloads the [] operator to directly access an element at a given index
        Type &operator[](int index);

        // Returns the number of elements in the vector
        inline int size() { return m_Size; }

        // Returns the total capacity of the vector
        inline int capacity() { return m_Capacity; }

    public:
        // Default constructor
        vector();

        vector(const vector<Type> &other);

        vector(std::initializer_list<Type> initList);

        // Constructor for setting initial vector size
        explicit vector(int size);

        vector(vector<Type> &&other) noexcept;

        // Destructor
        ~vector();

        // Returns a pointer to first element of vector
        inline Type *begin() { return m_Arr; }

        // Returns a pointer to one past the last element of vector
        inline Type *end() { return m_Arr + m_Size; }

        custom::vector<Type> &operator=(custom::vector<Type> &&other) noexcept;

        custom::vector<Type> &operator=(const custom::vector<Type> &other);

    private:
        // Pointer to the underlying dynamically allocated array
        Type *m_Arr;
        // Tracks the number of elements in the vector
        int m_Size;
        // Tracks the total capacity of the vector
        int m_Capacity;

        // Resizes the underlying array as needed when elements are added or removed
        void m_resize();
    };
}

template<typename Type>
custom::vector<Type>::vector() {
    m_Arr = new Type[MIN_VECTOR_CAPACITY];
    m_Capacity = MIN_VECTOR_CAPACITY;
    m_Size = 0;
}

template<typename Type>
custom::vector<Type>::vector(std::initializer_list<Type> initList) {
    {
        m_Size = initList.size();
        m_Capacity = (m_Size > MIN_VECTOR_CAPACITY) ? m_Size : MIN_VECTOR_CAPACITY;

        // Allocate memory based on the capacity
        m_Arr = new Type[m_Capacity];

        // Copy the elements from the initializer_list to the array.
        std::copy(initList.begin(), initList.end(), m_Arr);
    }
}

template<typename Type>
custom::vector<Type>::vector(int size) {
    if (size < MIN_VECTOR_CAPACITY) size = MIN_VECTOR_CAPACITY;
    m_Arr = new Type[size];
    m_Capacity = size;
    m_Size = 0;
}

template<typename Type>
custom::vector<Type>::~vector() {
    delete[] m_Arr;
}

template<typename Type>
void custom::vector<Type>::pop_back() {
    m_Size--;
    m_resize();
}

template<typename Type>
void custom::vector<Type>::push_back(Type value) {
    m_Size++;
    m_resize();
    m_Arr[m_Size - 1] = value;
}

template<typename Type>
void custom::vector<Type>::m_resize() {
    if (m_Size >= m_Capacity) { // Increase the capacity
        size_t newCapacity = m_Capacity * VECTOR_GROW_POWER;
        Type *hold = nullptr;

        try {
            hold = new Type[newCapacity];
        } catch (const std::bad_alloc &e) {
            std::cerr << "Memory allocation failed: " << e.what() << std::endl;
            return;
        }

        for (size_t i = 0; i < m_Size; ++i) {
            hold[i] = std::move(m_Arr[i]);
        }

        Type *temp = m_Arr;
        m_Arr = hold;
        delete[] temp;

        m_Capacity = newCapacity;
    }
        // The size is smaller than half of the current capacity
    else if (m_Size < m_Capacity / VECTOR_GROW_POWER && m_Capacity / VECTOR_GROW_POWER >= MIN_VECTOR_CAPACITY) {

        // Decrease the capacity.
        size_t newCapacity = m_Capacity / VECTOR_GROW_POWER;

        Type *hold = nullptr;
        try {
            hold = new Type[newCapacity];
        } catch (const std::bad_alloc &e) {
            std::cerr << "Memory allocation failed" << e.what() << std::endl;
            return;
        }

        for (size_t i = 0; i < m_Size; ++i) {
            hold[i] = std::move(m_Arr[i]);
        }

        Type *temp = m_Arr;
        m_Arr = hold;
        delete[] temp;

        m_Capacity = newCapacity;
    }
}

template<typename Type>
Type custom::vector<Type>::at(int index) {
    return m_Arr[index];
}

template<typename Type>
Type &custom::vector<Type>::operator[](int index) {
    return m_Arr[index];
}

// Move constructor definition for the vector class
template<typename Type>
custom::vector<Type>::vector(custom::vector<Type> &&other) noexcept
        : m_Arr(other.m_Arr), m_Size(other.m_Size), m_Capacity(other.m_Capacity) {
    // Nullifying/zeroing the data members of the 'other' vector (which is being moved from)
    other.m_Arr = nullptr;
    other.m_Size = 0;
    other.m_Capacity = 0;
}

// Move assignment operator definition for the vector class
template<typename Type>
custom::vector<Type> &custom::vector<Type>::operator=(custom::vector<Type> &&other) noexcept {
    // Checking for self-assignment
    if (this != &other) {
        // Remove the existing data in the current vector
        delete[] m_Arr;

        // Copying the 'other' vector's data into the current vector
        m_Arr = other.m_Arr;
        m_Size = other.m_Size;
        m_Capacity = other.m_Capacity;

        // Nullifying/zeroing the data members of the 'other' vector (which is being moved from)
        other.m_Arr = nullptr;
        other.m_Size = 0;
        other.m_Capacity = 0;
    }

    // Returning the current object to enable operator chaining
    return *this;
}

// Copy assignment operator definition for the vector class
template<typename Type>
custom::vector<Type> &custom::vector<Type>::operator=(const custom::vector<Type> &other) {
    // Checking for self-assignment
    if (this != &other) {
        // Remove the existing data in the current vector
        delete[] m_Arr;

        // Copying the 'other' vector's size and capacity into the current vector
        m_Size = other.m_Size;
        m_Capacity = other.m_Capacity;

        // Creating a new array and copying the 'other' vector's data into the current vector
        m_Arr = new Type[other.m_Capacity];
        std::copy(other.m_Arr, other.m_Arr + other.m_Size, m_Arr);
    }
    // Returning the current object to enable operator chaining
    return *this;
}

// Copy constructor definition for the vector class
template<typename Type>
custom::vector<Type>::vector(const vector<Type> &other)
        :m_Size(other.m_Size), m_Capacity(other.m_Capacity) {
    // Creating a new array and copying the 'other' vector's data into the current vector
    m_Arr = new Type[other.m_Capacity];
    std::copy(other.m_Arr, other.m_Arr + other.m_Size, m_Arr);
}

#define DYNAMICARRAY_VECTOR_H

#endif //DYNAMICARRAY_VECTOR_H
