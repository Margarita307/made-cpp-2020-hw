#pragma once
#include <cstddef>
#include <new>
#include <iostream>

namespace task {

    const size_t CHUNK_SIZE = 8;

    struct Chunk {
        Chunk() {
            bytes = new char[CHUNK_SIZE];
            start = bytes;
        }
        Chunk* next = nullptr;
        char* bytes;
        char* start;
        ~Chunk() {
            delete[] bytes;
        }
    };

    template<class T>
    class Allocator {
    public:
        using value_type = T;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;
        using size_type = size_t;
        using difference_type = ptrdiff_t;

        template<class U>
        struct rebind {
            typedef Allocator<U> other;
        };

        Allocator() {}

        Allocator(Allocator& other) {
            if ((*num_of_allocators) == 1) {
                destroy_chunks();
            }
            num_of_allocators = other.num_of_allocators;
            ++(*num_of_allocators);
            first_chunk = other.first_chunk;
        }

        ~Allocator() {
            if (--(*num_of_allocators) == 0)
                destroy_chunks();
        }

        Allocator& operator=(const Allocator& other) {
            if ((*num_of_allocators) == 1) {
                destroy_chunks();
            }
            num_of_allocators = other.num_of_allocators;
            ++(*num_of_allocators);
            first_chunk = other.first_chunk;
            return *this;
        }

        pointer allocate(size_type n) {
            size_t requested_bytes = n * sizeof(T);
            if (requested_bytes > CHUNK_SIZE)
                return nullptr;

            Chunk* chunk = (*first_chunk);
            Chunk* prev = nullptr;
            while (chunk != nullptr && static_cast<size_t>(chunk->bytes + CHUNK_SIZE - chunk->start) < requested_bytes) {
                prev = chunk;
                chunk = chunk->next;
            }
            if (chunk == nullptr) {
                chunk = new Chunk;
                if (prev == nullptr)
                    (*first_chunk) = chunk;
                else
                    prev->next = (*chunk);
            }
            char* res = chunk->start;
            chunk->start += requested_bytes;
            return reinterpret_cast<T*>(res);
        }

        void deallocate(T* p, size_t n) {
            // Do nothing    
        }

        template<class ... Args>
        void construct(T* p, Args&&... args) {
            new(p) T(std::forward<Args>(args)...);
        }

        void destroy(pointer p) {
            p->~T();
        }

    private:
        // Store pointer to the pointer so that allocators originated as copies of each other
        // have the same pointer to the first chunk, so that any of them could destroy all chunks
        Chunk** first_chunk = new Chunk* { nullptr };
        size_t* num_of_allocators = new size_t{ 1 };

        void destroy_chunks() {
            Chunk* chunk = (*first_chunk);
            Chunk* temp;
            while (chunk != nullptr) {
                temp = chunk->next;
                delete chunk;
                chunk = temp;
            }
        }
    };
} // namespace task

