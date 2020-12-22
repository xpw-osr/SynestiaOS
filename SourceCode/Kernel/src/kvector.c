//
// Created by XingfengYang on 2020/7/10.
//
#include "kernel/kheap.h"
#include "kernel/kvector.h"
#include "kernel/log.h"
#include "libc/stdlib.h"

extern Heap kernelHeap;

KernelStatus kvector_operation_default_resize(struct KernelVector *vector, uint32_t newSize) {
    vector = kernelHeap.operations.realloc(&kernelHeap, vector, newSize);
    if (vector == nullptr) {
        LogError("[KVector] kVector resize failed.\n");
        return ERROR;
    }
    return OK;
}

KernelStatus kvector_operation_default_free(struct KernelVector *vector) {
    KernelStatus status = kernelHeap.operations.free(&kernelHeap, vector);
    if (status != OK) {
        LogError("[KVector] kVector free failed.\n");
        return status;
    }
    return OK;
}

KernelStatus kvector_operation_default_add(struct KernelVector *vector, ListNode *node) {
    if (vector->size >= vector->capacity) {
        vector->capacity += DEFAULT_VECTOR_SIZE;
        KernelStatus status = vector->operations.resize(vector, vector->capacity * sizeof(ListNode *) + sizeof(KernelVector));
        if (status != OK) {
            LogError("[KVector] kVector resize failed.\n");
            return status;
        }
    }
    vector->data = (ListNode **) (vector + sizeof(KernelVector));
    vector->data[vector->size] = node;
    vector->size++;
    return OK;
}

ListNode *kvector_operation_default_get(struct KernelVector *vector, uint32_t index) {
    if (index >= vector->size) {
        return nullptr;
    }
    return vector->data[index];
}

uint32_t kvector_operation_default_size(struct KernelVector *vector) {
    return vector->size;
}

uint32_t kvector_operation_default_capacity(struct KernelVector *vector) {
    return vector->capacity;
}

bool kvector_operation_default_isEmpty(struct KernelVector *vector) {
    return vector->size == 0;
}

bool kvector_operation_default_isFull(struct KernelVector *vector) {
    return vector->size == vector->capacity;
}

KernelStatus kvector_operation_default_clear(struct KernelVector *vector) {
    vector->size = 0;
    for (uint32_t i = 0; i < vector->size; i++) {
        vector->data = nullptr;
    }
    return OK;
}

KernelVector *kvector_allocate() {
    // 1. allocate vector memory block from virtual memory (heap), and align.
    KernelVector *vector = (KernelVector *) kernelHeap.operations.alloc(
            &kernelHeap, DEFAULT_VECTOR_SIZE * sizeof(ListNode *) + sizeof(KernelVector));
    if (vector == nullptr) {
        LogError("[KVector] kVector allocate failed.\n");
        return nullptr;
    }
    vector->capacity = DEFAULT_VECTOR_SIZE;
    vector->size = 0;
    vector->data = (ListNode **) (vector + sizeof(KernelVector));

    vector->operations.resize = kvector_operation_default_resize;
    vector->operations.free = kvector_operation_default_free;
    vector->operations.add = kvector_operation_default_add;
    vector->operations.get = kvector_operation_default_get;
    vector->operations.size = kvector_operation_default_size;
    vector->operations.capacity = kvector_operation_default_capacity;
    vector->operations.isEmpty = kvector_operation_default_isEmpty;
    vector->operations.isFull = kvector_operation_default_isFull;
    vector->operations.clear = kvector_operation_default_clear;

    return vector;
}
