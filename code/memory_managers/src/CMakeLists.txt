cmake_minimum_required(VERSION 2.8)
project(libmemory_manager)

#
# build library
#
set(FIRST_FIT_FREE_LIST_MEMORY_ALLOCATOR_SRC first_fit_free_list_memory_allocator.cc)
set(MEMORY_MANAGER_SRC memory_manager.cc)
set(UTILS_SRC utils.cc)
set(BINARY_BUDDY_MEMORY_ALLOCATOR_SRC binary_buddy_memory_allocator.cc)
set(POOL_MEMORY_ALLOCATOR_SRC pool_memory_allocator.cc)
set(LIFO_MEMORY_ALLOCATOR_SRC lifo_memory_allocator.cc)
set(MONITOR_MEMORY_ALLOCATOR_SRC monitor_memory_allocator.cc)


add_library(memory_manager STATIC
    ${MEMORY_MANAGER_SRC} ${FIRST_FIT_FREE_LIST_MEMORY_ALLOCATOR_SRC}
    ${UTILS_SRC} ${BINARY_BUDDY_MEMORY_ALLOCATOR_SRC}
    ${POOL_MEMORY_ALLOCATOR_SRC} ${LIFO_MEMORY_ALLOCATOR_SRC}
    ${MONITOR_MEMORY_ALLOCATOR_SRC})
