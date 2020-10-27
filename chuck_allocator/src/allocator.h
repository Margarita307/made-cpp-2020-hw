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
        Chunk* prev = nullptr;
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
            last_chunk = other.last_chunk;
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
            last_chunk = other.last_chunk;
            return *this;
        }

        pointer allocate(size_type n) {
            size_t requested_bytes = n * sizeof(T);
            if (requested_bytes > CHUNK_SIZE)
                return nullptr;

            Chunk* chunk = (*last_chunk);
            while (chunk != nullptr && static_cast<size_t>(chunk->bytes + CHUNK_SIZE - chunk->start) < requested_bytes) {
                chunk = chunk->prev;
            }
            if (chunk == nullptr) {
                chunk = new Chunk;
                chunk->prev = (*last_chunk);
                (*last_chunk) = chunk;
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
            new(p) T(args...);
        }

        void destroy(pointer p) {
            p->~T();
        }

    private:
        // Store pointer to the pointer so that allocators originated as copies of each other
        // have the same pointer to last chunk, so that any of them could destroy all chunks
        Chunk** last_chunk = new Chunk* { nullptr };
        size_t* num_of_allocators = new size_t{ 1 };

        void destroy_chunks() {
            Chunk* chunk = (*last_chunk);
            Chunk* temp;
            while (chunk != nullptr) {
                temp = chunk->prev;
                delete chunk;
                chunk = temp;
            }
        }
    };
} // namespace task

