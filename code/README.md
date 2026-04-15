# Code Examples

This directory contains C++ implementations of algorithms and data structures used in CS 302.

## Available Examples

### Sorting Algorithms

All sorting algorithms include:
- Complete implementation with comprehensive documentation
- Step-by-step visualization functions
- Multiple examples and edge cases
- Educational comments explaining the algorithm
- Best/worst case demonstrations

#### Selection Sort (`selection_sort.cpp`)
- **Algorithm**: Maximum-to-End variant
- **Strategy**: Find largest element, swap to end. Sorted region grows right-to-left.
- **Complexity**: O(n²) all cases, O(n) swaps
- **Compile**: `g++ -std=c++11 -o demo selection_sort.cpp`

#### Bubble Sort (`bubble_sort.cpp`)
- **Algorithm**: Adjacent pair comparison with early termination
- **Strategy**: Compare adjacent pairs, swap if out of order. Largest "bubbles" to end.
- **Complexity**: O(n²) worst case, O(n) best case (with early termination)
- **Key Feature**: Early termination optimization (stops if no swaps occur)
- **Compile**: `g++ -std=c++11 -o demo bubble_sort.cpp`

#### Insertion Sort (`insertion_sort.cpp`)
- **Algorithm**: Build sorted region left-to-right using shifts
- **Strategy**: Save element, shift sorted elements right, insert into gap. Uses shifts, not swaps.
- **Complexity**: O(n²) worst case, O(n) best case (nearly sorted)
- **Key Feature**: Very efficient for nearly-sorted data
- **Compile**: `g++ -std=c++11 -o demo insertion_sort.cpp`

### Foundational Data Structures

#### Arrays (`array.cpp`)
- **Algorithm**: Fixed-capacity array with insert, delete, search, access
- **Strategy**: Contiguous memory, O(1) random access, shifting for insert/delete
- **Complexity**: Access O(1), Insert/Delete O(n), Search O(n)
- **Key Feature**: Step-by-step shift visualization
- **Compile**: `g++ -std=c++11 -o demo array.cpp`

#### Linked Lists (`linked_list.cpp`)
- **Algorithm**: Singly and doubly linked list operations
- **Strategy**: Node-based storage with pointer manipulation. No shifting, no random access.
- **Complexity**: Head insert O(1), Access O(n), Search O(n)
- **Key Feature**: Both singly and doubly linked implementations with reverse
- **Compile**: `g++ -std=c++11 -o demo linked_list.cpp`

#### Stacks (`stack.cpp`)
- **Algorithm**: LIFO stack with array-backed and linked-list-backed implementations
- **Strategy**: Push/pop at top only. Includes balanced parentheses checker.
- **Complexity**: Push O(1), Pop O(1), Peek O(1)
- **Key Feature**: Dual implementation comparison, parentheses checking application
- **Compile**: `g++ -std=c++11 -o demo stack.cpp`

#### Queues (`queue.cpp`)
- **Algorithm**: FIFO queue with circular array and linked-list implementations
- **Strategy**: Enqueue at rear, dequeue from front. Circular array uses modular arithmetic.
- **Complexity**: Enqueue O(1), Dequeue O(1), Peek O(1)
- **Key Feature**: Circular array wrap-around visualization
- **Compile**: `g++ -std=c++11 -o demo queue.cpp`

### Recursion

#### Recursion (`recursion.cpp`)
- **Algorithm**: Factorial, Fibonacci (naive and memoized), recursive binary search
- **Strategy**: Demonstrates call stack behavior, redundant computation, and memoization
- **Complexity**: Factorial O(n), Fibonacci O(2^n) naive / O(n) memoized, Binary Search O(log n)
- **Key Feature**: Call tracing with indentation, naive vs memoized comparison
- **Compile**: `g++ -std=c++11 -o demo recursion.cpp`

### Trees & Heaps

#### Max-Heap (`heap.cpp`)
- **Algorithm**: Max-Heap with insert, extract max, and build heap
- **Strategy**: Complete binary tree stored in array. Parent >= both children. Sift up for insert, sift down for extract.
- **Complexity**: Insert O(log n), Extract Max O(log n), Build Heap O(n)
- **Key Feature**: Dual tree and array representation, step-by-step visualizations
- **Compile**: `g++ -std=c++11 -o demo heap.cpp`

#### AVL Tree (`avl_tree.cpp`)
- **Algorithm**: Self-balancing BST with height-tracked nodes and rotations
- **Strategy**: After every insert, walk back up updating heights; if |balance factor| > 1, apply one of four rotation cases (LL, RR, LR, RL) to restore balance
- **Complexity**: Search / Insert / Delete all O(log n) worst case
- **Key Feature**: Step-by-step trace of rotations as the tree grows
- **Compile**: `g++ -std=c++11 -o demo avl_tree.cpp`

#### Red-Black Tree (`red_black_tree.cpp`)
- **Algorithm**: Self-balancing BST using node colors and a sentinel NIL
- **Strategy**: Insert red nodes then fix up using three cases (recolor / zig-zag rotate / straight-line rotate) based on the uncle's color
- **Complexity**: Search / Insert / Delete all O(log n) worst case
- **Key Feature**: Colored pretty-print, CLRS-style sentinel implementation
- **Compile**: `g++ -std=c++11 -o demo red_black_tree.cpp`

#### 2-3 Tree (`two_three_tree.cpp`)
- **Algorithm**: Multi-way search tree with 2-nodes and 3-nodes, bottom-up splitting
- **Strategy**: Insert into leaf; if a node holds 3 keys, split and promote the middle key to the parent; splits may cascade; root splits grow the tree upward
- **Complexity**: Search / Insert O(log n)
- **Key Feature**: Perfect height balance (all leaves at same depth), conceptual ancestor of B-trees
- **Compile**: `g++ -std=c++11 -o demo two_three_tree.cpp`

#### 2-3-4 Tree (`two_three_four_tree.cpp`)
- **Algorithm**: Multi-way search tree with 2-, 3-, and 4-nodes, top-down preemptive splitting
- **Strategy**: Descend the tree pre-splitting any 4-node before stepping into it; guarantees the parent always has room and insert is a single downward pass
- **Complexity**: Search / Insert O(log n)
- **Key Feature**: Directly isomorphic to red-black trees — the clearest path to understanding how red-black fix-up works
- **Compile**: `g++ -std=c++11 -o demo two_three_four_tree.cpp`

## Compiling and Running

All code examples use C++11 standard and can be compiled with:

```bash
g++ -std=c++11 -Wall -Wextra -o <output_name> <source_file>.cpp
./<output_name>
```

### Examples

**Selection Sort:**
```bash
cd code
g++ -std=c++11 -o selection_sort_demo selection_sort.cpp
./selection_sort_demo
```

**Bubble Sort:**
```bash
g++ -std=c++11 -o bubble_sort_demo bubble_sort.cpp
./bubble_sort_demo
```

**Insertion Sort:**
```bash
g++ -std=c++11 -o insertion_sort_demo insertion_sort.cpp
./insertion_sort_demo
```

**Arrays:**
```bash
g++ -std=c++11 -o array_demo array.cpp
./array_demo
```

**Linked Lists:**
```bash
g++ -std=c++11 -o linked_list_demo linked_list.cpp
./linked_list_demo
```

**Stacks:**
```bash
g++ -std=c++11 -o stack_demo stack.cpp
./stack_demo
```

**Queues:**
```bash
g++ -std=c++11 -o queue_demo queue.cpp
./queue_demo
```

**Recursion:**
```bash
g++ -std=c++11 -o recursion_demo recursion.cpp
./recursion_demo
```

**Max-Heap:**
```bash
g++ -std=c++11 -o heap_demo heap.cpp
./heap_demo
```

**AVL Tree:**
```bash
g++ -std=c++11 -o avl_tree_demo avl_tree.cpp
./avl_tree_demo
```

**Red-Black Tree:**
```bash
g++ -std=c++11 -o red_black_tree_demo red_black_tree.cpp
./red_black_tree_demo
```

**2-3 Tree:**
```bash
g++ -std=c++11 -o two_three_tree_demo two_three_tree.cpp
./two_three_tree_demo
```

**2-3-4 Tree:**
```bash
g++ -std=c++11 -o two_three_four_tree_demo two_three_four_tree.cpp
./two_three_four_tree_demo
```

## Code Style

- Well-documented with Doxygen-style comments
- Educational focus with inline explanations
- Handles edge cases (empty arrays, single elements, etc.)
- Includes examples and demonstrations
- Follows C++ best practices

## Adding New Examples

When adding new code examples:
1. Place the `.cpp` file in this directory
2. Follow the same documentation style
3. Include a `main()` function with examples
4. Update this README
5. Ensure it compiles with `-Wall -Wextra` (no warnings)
