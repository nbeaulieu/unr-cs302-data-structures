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
