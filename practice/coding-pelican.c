/**
 * @file    test_shallow_deep_copy.c
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2021-10-10 (date of creation)
 * @updated 2024-10-11 (date of last update)
 * @version v0.1.1
 * @ingroup test
 * @prefix  NONE
 *
 * @brief   shallow copy vs deep copy
 * @details This file is a test code for shallow copy vs deep copy. without using any library.
 *
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 */


#pragma region Shallow_Deep_Copy
#define unused(x) (void)x

struct ArrayWrapper {
    int arr[5];
};

// shallow copy: copy pointer(address), not data(value) -> not allocate new memory
void function1(int arr[5]) {
    // arr is a pointer to the original array
    // Changes here affect the original array
    for (int i = 0; i < 5; ++i) {
        arr[i] = -1;
    }
}

// deep copy: copy data(value) -> allocate new memory
void function2(struct ArrayWrapper wrapper) {
    // wrapper is a complete copy
    // Changes to wrapper.arr do not affect the original
    for (int i = 0; i < 5; ++i) {
        wrapper.arr[i] = -1;
    }
}
#pragma endregion // Shallow_Deep_Copy


#pragma region InternalArray
#define internal_array(_T, _N) \
    struct {                   \
        _T data[_N];           \
    }

struct TestObject {
    union {
        internal_array(int, 3) arr;
        struct {
            int x, y, z;
        };
    };
};
#pragma endregion // InternalArray


#pragma region ArrayTN
#include <assert.h>
#include <stdio.h>

#define PtrT(_Type) _Type*
#define RefT(_Type) _Type* const

#define ArrayTN(_Type, _N, _Alias)                                    \
    struct _Alias {                                                   \
        _Type data_[_N];                                              \
    };                                                                \
    size_t _Alias##_size(const RefT(struct _Alias) self) {            \
        assert(self);                                                 \
        unused(self);                                                 \
        return _N;                                                    \
    }                                                                 \
    PtrT(_Type) _Alias##_at(RefT(struct _Alias) self, size_t index) { \
        assert(self);                                                 \
        assert(0 <= index);                                           \
        assert(index < (_N));                                         \
        return &self->data_[index];                                   \
    }                                                                 \
    void _Alias##_fill(RefT(struct _Alias) self, _Type value) {       \
        assert(self);                                                 \
        for (size_t index = 0; index < (_N); ++index) {               \
            self->data_[index] = value;                               \
        }                                                             \
    }                                                                 \
    typedef struct _Alias _Alias

#define Array(_Alias, ...)   \
    (_Alias) {               \
        .data_ = __VA_ARGS__ \
    }

// NOTE: Couldn't generic be used to store aliases?
// Define array types
ArrayTN(int, 5, ArrayInt5);
ArrayTN(char, 10, ArrayChar10);


// Example usage
int main() {
    ArrayInt5   my_int_array  = Array(ArrayInt5, { 1, 2, 3, 4, 5 });
    ArrayChar10 my_char_array = Array(ArrayChar10, { 0 });

    // Using at() to access elements
    printf("Third element: %d\n", *ArrayInt5_at(&my_int_array, 2));

    // Using size()
    printf("Size of int array: %zu\n", ArrayInt5_size(&my_int_array));

    // Using fill()
    ArrayChar10_fill(&my_char_array, 'A');
    printf("First char: %c\n", *ArrayChar10_at(&my_char_array, 0));

    // Demonstrating value semantics
    ArrayInt5 copy          = my_int_array;
    *ArrayInt5_at(&copy, 0) = 100;
    printf("Original first element: %d\n", *ArrayInt5_at(&my_int_array, 0));
    printf("Copy first element: %d\n", *ArrayInt5_at(&copy, 0));

    return 0;
}
#pragma endregion // ArrayTN
