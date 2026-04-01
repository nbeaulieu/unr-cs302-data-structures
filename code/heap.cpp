/**
 * @file heap.cpp
 * @brief Implementation of a Max-Heap data structure with comprehensive documentation
 * @author CS 302 - Data Structures
 * @date 2026
 *
 * Max-Heap: A complete binary tree stored in an array where every parent node
 * is greater than or equal to its children. The largest element is always at
 * the root (index 0).
 *
 * Key Feature: Efficient insertion and extraction of the maximum element.
 * Supports O(log n) insert and extract-max, O(1) peek, and O(n) build-heap.
 *
 * Array Representation:
 * - Parent of node at index i:      (i - 1) / 2
 * - Left child of node at index i:  2 * i + 1
 * - Right child of node at index i: 2 * i + 2
 *
 * Time Complexity: Insert O(log n), Extract-Max O(log n), Build-Heap O(n), Peek O(1)
 * Space Complexity: O(1) extra space - stored in-place in an array
 *
 * Best for: Priority queues, top-k problems, scheduling, heap sort, median maintenance
 */

#include <iostream>
#include <vector>
#include <algorithm>  // for std::swap
#include <cassert>    // for assertions

using namespace std;

/**
 * @brief Restores the max-heap property by bubbling an element UP toward the root
 *
 * After inserting a new element at the end of the heap, it may be larger than
 * its parent, violating the max-heap property. Sift-up repeatedly compares the
 * element with its parent and swaps if the element is larger, moving it up
 * until the heap property is restored.
 *
 * Parent/Child Index Formulas (0-based):
 * - Parent of node i:      (i - 1) / 2
 * - Left child of node i:  2 * i + 1
 * - Right child of node i: 2 * i + 2
 *
 * Visual Example:
 *   Insert 50 into heap [40, 30, 20, 10, 15]:
 *
 *   Before sift-up (50 added at index 5):
 *          40
 *         /  \
 *       30    20
 *      / \   /
 *    10  15 50    ← 50 > parent 20, violates heap property
 *
 *   Step 1: swap 50 and 20 (index 5 and 2):
 *          40
 *         /  \
 *       30    50  ← 50 > parent 40, still violates
 *      / \   /
 *    10  15 20
 *
 *   Step 2: swap 50 and 40 (index 2 and 0):
 *          50     ← heap property restored
 *         /  \
 *       30    40
 *      / \   /
 *    10  15 20
 *
 * @param heap The vector representing the heap
 * @param index The index of the element to sift up (typically the last element)
 *
 * @note Used after insertion to restore the max-heap property
 * @note Moves the element UPWARD toward the root
 * @note Stops when the element is less than or equal to its parent, or reaches root
 *
 * @complexity Time: O(log n) - at most the height of the tree
 * @complexity Space: O(1) - only uses index variables
 */
void siftUp(vector<int>& heap, int index) {
    // Continue while we haven't reached the root (index 0)
    while (index > 0) {
        // Calculate parent index using the formula: parent = (i - 1) / 2
        int parent = (index - 1) / 2;

        // If current element is greater than its parent, swap them
        // This moves the larger element up toward the root
        if (heap[index] > heap[parent]) {
            swap(heap[index], heap[parent]);
            // Move up to the parent's position and continue checking
            index = parent;
        } else {
            // Heap property is satisfied, we're done
            break;
        }
    }
}

/**
 * @brief Restores the max-heap property by pushing an element DOWN toward the leaves
 *
 * After replacing the root (during extract-max) or during build-heap, an element
 * may be smaller than one or both of its children. Sift-down compares the element
 * with both children, swaps with the LARGER child, and repeats until the heap
 * property is restored.
 *
 * Why swap with the LARGER child?
 * - We need the largest value to move up to be the parent
 * - If we swapped with the smaller child, the larger child would have a
 *   smaller parent, violating the heap property
 *
 * Visual Example:
 *   Sift down 10 from the root of [10, 40, 30, 20, 15]:
 *
 *   Before sift-down:
 *          10     ← 10 < children 40 and 30, violates heap property
 *         /  \
 *       40    30
 *      / \
 *    20  15
 *
 *   Step 1: compare children 40 and 30, larger is 40; swap 10 and 40:
 *          40
 *         /  \
 *       10    30  ← 10 < children 20 and 15, still violates
 *      / \
 *    20  15
 *
 *   Step 2: compare children 20 and 15, larger is 20; swap 10 and 20:
 *          40
 *         /  \
 *       20    30
 *      / \
 *    10  15           ← 10 is a leaf, heap property restored
 *
 * @param heap The vector representing the heap
 * @param n The number of elements to consider (may be less than heap.size())
 * @param index The index of the element to sift down (typically the root, index 0)
 *
 * @note Used after extract-max and during build-heap
 * @note Moves the element DOWNWARD toward the leaves
 * @note Always swaps with the LARGER child to maintain max-heap property
 * @note The parameter n allows us to use a subset of the array (useful for heap sort)
 *
 * @complexity Time: O(log n) - at most the height of the tree
 * @complexity Space: O(1) - only uses index variables
 */
void siftDown(vector<int>& heap, int n, int index) {
    while (true) {
        // Calculate left and right child indices
        int left = 2 * index + 1;   // Left child index
        int right = 2 * index + 2;  // Right child index
        int largest = index;         // Assume current node is largest

        // Check if left child exists and is larger than current node
        if (left < n && heap[left] > heap[largest]) {
            largest = left;
        }

        // Check if right child exists and is larger than current largest
        if (right < n && heap[right] > heap[largest]) {
            largest = right;
        }

        // If the largest is not the current node, swap and continue
        if (largest != index) {
            swap(heap[index], heap[largest]);
            // Move down to the child's position and continue checking
            index = largest;
        } else {
            // Heap property is satisfied, we're done
            break;
        }
    }
}

/**
 * @brief Inserts a new value into the max-heap
 *
 * Algorithm:
 * 1. Add the new value at the end of the array (maintains complete tree shape)
 * 2. Call siftUp to bubble it into the correct position
 *
 * Visual Example:
 *   Insert 35 into heap [50, 30, 40, 10, 15, 20]:
 *
 *   Step 1: Add 35 at end → [50, 30, 40, 10, 15, 20, 35]
 *          50
 *         /  \
 *       30    40
 *      / \   / \
 *    10  15 20  35  ← new element added here
 *
 *   Step 2: siftUp(35) — 35 < 40 (parent), stop
 *   Result: [50, 30, 40, 10, 15, 20, 35] (no swap needed)
 *
 * @param heap The vector representing the heap
 * @param value The value to insert
 *
 * @note Adds element at end then sifts up to restore heap property
 * @note The heap grows by one element
 *
 * @complexity Time: O(log n) - siftUp traverses at most the height of the tree
 * @complexity Space: O(1) - amortized for vector push_back
 */
void heapInsert(vector<int>& heap, int value) {
    // Step 1: Add the new value at the end of the heap
    // This maintains the complete binary tree structure
    heap.push_back(value);

    // Step 2: Sift up the new element to restore the max-heap property
    // The new element is at the last index: heap.size() - 1
    siftUp(heap, heap.size() - 1);
}

/**
 * @brief Removes and returns the maximum element from the max-heap
 *
 * Algorithm:
 * 1. Save the root element (the maximum)
 * 2. Move the last element to the root position
 * 3. Remove the last element (shrink the heap)
 * 4. Sift down the new root to restore the heap property
 * 5. Return the saved maximum
 *
 * Visual Example:
 *   Extract max from heap [50, 30, 40, 10, 15, 20]:
 *
 *   Step 1: Save max = 50
 *   Step 2: Move last element (20) to root → [20, 30, 40, 10, 15]
 *          20     ← moved from last position
 *         /  \
 *       30    40
 *      / \
 *    10  15
 *
 *   Step 3: siftDown(20) — compare children 30 and 40, swap with 40:
 *          40
 *         /  \
 *       30    20  ← 20 has no children below, done
 *      / \
 *    10  15
 *
 *   Result: [40, 30, 20, 10, 15], return 50
 *
 * @param heap The vector representing the heap (must not be empty)
 * @return The maximum value that was at the root
 *
 * @note The heap shrinks by one element
 * @note Asserts that the heap is not empty
 * @note After extraction, the next-largest element becomes the new root
 *
 * @complexity Time: O(log n) - siftDown traverses at most the height of the tree
 * @complexity Space: O(1) - only uses a temporary variable
 */
int heapExtractMax(vector<int>& heap) {
    // Ensure the heap is not empty
    assert(!heap.empty() && "Cannot extract from an empty heap");

    // Step 1: Save the maximum value (always at the root, index 0)
    int maxValue = heap[0];

    // Step 2: Move the last element to the root
    heap[0] = heap.back();

    // Step 3: Remove the last element (shrink the heap by one)
    heap.pop_back();

    // Step 4: Sift down the new root to restore the max-heap property
    // Only sift down if the heap is not empty after removal
    if (!heap.empty()) {
        siftDown(heap, heap.size(), 0);
    }

    // Step 5: Return the saved maximum
    return maxValue;
}

/**
 * @brief Builds a max-heap from an unsorted array using bottom-up heapify
 *
 * Algorithm (Bottom-Up Heapify):
 * 1. Start from the last non-leaf node: index (n/2 - 1)
 * 2. Call siftDown on each node, moving from right to left, bottom to top
 * 3. After processing all nodes, the array satisfies the max-heap property
 *
 * Why start at n/2 - 1?
 * - Leaf nodes (indices n/2 to n-1) are trivially valid heaps (no children)
 * - We only need to fix internal nodes that might violate the heap property
 * - The last non-leaf node is at index (n/2 - 1)
 *
 * Why is this O(n) and not O(n log n)?
 * - Most nodes are near the bottom and need very few swaps
 * - Leaf nodes (n/2 of them) need 0 swaps
 * - Nodes one level up (n/4 of them) need at most 1 swap
 * - Nodes two levels up (n/8 of them) need at most 2 swaps
 * - Sum: n/4 * 1 + n/8 * 2 + n/16 * 3 + ... = O(n)
 *
 * Visual Example:
 *   Build heap from [4, 10, 3, 5, 1]:
 *
 *   Initial (unsorted):
 *          4
 *         / \
 *       10    3
 *      / \
 *     5   1
 *
 *   Last non-leaf = (5/2 - 1) = index 1
 *
 *   siftDown(index 1): node 10, children 5 and 1 → 10 > both, no swap
 *          4
 *         / \
 *       10    3
 *      / \
 *     5   1
 *
 *   siftDown(index 0): node 4, children 10 and 3 → swap with 10
 *          10
 *         /  \
 *        4     3
 *       / \
 *      5   1
 *     then node 4, children 5 and 1 → swap with 5
 *          10
 *         /  \
 *        5     3
 *       / \
 *      4   1
 *
 *   Result: [10, 5, 3, 4, 1] — valid max-heap!
 *
 * @param vec The vector to convert into a max-heap (modified in-place)
 *
 * @note Modifies the vector in-place — no extra memory needed
 * @note Bottom-up approach is O(n), much faster than n individual insertions O(n log n)
 * @note Leaf nodes are already valid heaps, so we start from the last internal node
 *
 * @complexity Time: O(n) - bottom-up heapify is linear (NOT O(n log n))
 * @complexity Space: O(1) - in-place, no extra memory
 */
void buildHeap(vector<int>& vec) {
    int n = vec.size();

    // Edge case: empty or single-element array is already a valid heap
    if (n <= 1) {
        return;
    }

    // Start from the last non-leaf node and work backwards to the root
    // Last non-leaf node index = (n / 2) - 1
    // All nodes after this index are leaves (they have no children)
    for (int i = n / 2 - 1; i >= 0; i--) {
        siftDown(vec, n, i);
    }
}

/**
 * @brief Prints the heap in array representation [a, b, c, ...]
 *
 * @param heap The vector representing the heap
 *
 * @note Displays the heap as a flat array, which is the underlying storage format
 */
void printHeap(const vector<int>& heap) {
    cout << "[";
    for (size_t i = 0; i < heap.size(); i++) {
        cout << heap[i];
        if (i < heap.size() - 1) {
            cout << ", ";
        }
    }
    cout << "]" << endl;
}

/**
 * @brief Prints the heap as a tree-like structure with indentation showing levels
 *
 * Uses indentation to represent the depth of each node in the binary tree.
 * This helps visualize the parent-child relationships in the heap.
 *
 * Visual Example for heap [50, 30, 40, 10, 15, 20]:
 *   Level 0:         50
 *   Level 1:     30      40
 *   Level 2:   10  15  20
 *
 * @param heap The vector representing the heap
 *
 * @note The output is a simplified tree view using indentation
 * @note Empty heaps print a message indicating they are empty
 */
void printHeapTree(const vector<int>& heap) {
    if (heap.empty()) {
        cout << "(empty heap)" << endl;
        return;
    }

    int n = heap.size();
    int level = 0;
    int levelSize = 1;  // Number of nodes at the current level
    int index = 0;

    while (index < n) {
        // Calculate indentation for this level
        // Deeper levels get less indentation (tree widens toward bottom)
        int maxLevel = 0;
        int temp = n;
        while (temp > 1) {
            maxLevel++;
            temp /= 2;
        }

        // Print leading spaces for this level
        int spaces = (1 << (maxLevel - level)) - 1;
        for (int s = 0; s < spaces; s++) {
            cout << "  ";
        }

        // Print all nodes at this level
        int gap = (1 << (maxLevel - level + 1)) - 1;
        for (int i = 0; i < levelSize && index < n; i++, index++) {
            if (i > 0) {
                for (int g = 0; g < gap; g++) {
                    cout << "  ";
                }
            }
            cout << heap[index];
            if (heap[index] < 10) cout << " ";  // Alignment for single digits
        }
        cout << endl;

        // Move to the next level
        level++;
        levelSize *= 2;
    }
}

/**
 * @brief Inserts a value into the heap with step-by-step output showing each swap
 *
 * This version prints each comparison and swap during the sift-up process
 * to help students visualize how insertion works.
 *
 * @param heap The vector representing the heap
 * @param value The value to insert
 *
 * @note Prints detailed trace of comparisons and swaps during sift-up
 * @note Useful for teaching — shows exactly how the element bubbles up
 *
 * @complexity Time: O(log n) - same as regular insert, plus O(n) for printing
 * @complexity Space: O(1) - only uses index variables
 */
void heapInsertWithSteps(vector<int>& heap, int value) {
    cout << "  Inserting " << value << "..." << endl;

    // Step 1: Add the value at the end
    heap.push_back(value);
    int index = heap.size() - 1;

    cout << "    Added " << value << " at index " << index << ": ";
    printHeap(heap);

    // Step 2: Sift up with detailed output
    int swaps = 0;
    while (index > 0) {
        int parent = (index - 1) / 2;

        if (heap[index] > heap[parent]) {
            cout << "    " << heap[index] << " > " << heap[parent]
                 << " (parent at index " << parent << ") → swap" << endl;
            swap(heap[index], heap[parent]);
            index = parent;
            swaps++;
            cout << "    After swap: ";
            printHeap(heap);
        } else {
            cout << "    " << heap[index] << " <= " << heap[parent]
                 << " (parent at index " << parent << ") → stop" << endl;
            break;
        }
    }

    if (index == 0 && swaps > 0) {
        cout << "    Reached the root — sift-up complete" << endl;
    }
    if (swaps == 0) {
        cout << "    No swaps needed — already in correct position" << endl;
    }

    cout << "    Heap after insert: ";
    printHeap(heap);
}

/**
 * @brief Extracts the maximum with step-by-step output showing each swap
 *
 * This version prints each comparison and swap during the sift-down process
 * to help students visualize how extract-max works.
 *
 * @param heap The vector representing the heap (must not be empty)
 *
 * @note Prints detailed trace of comparisons and swaps during sift-down
 * @note Useful for teaching — shows exactly how the replacement element sinks down
 *
 * @complexity Time: O(log n) - same as regular extract, plus O(n) for printing
 * @complexity Space: O(1) - only uses index variables
 */
void heapExtractMaxWithSteps(vector<int>& heap) {
    assert(!heap.empty() && "Cannot extract from an empty heap");

    int maxValue = heap[0];
    cout << "  Extracting max value: " << maxValue << endl;

    // Move last element to root
    heap[0] = heap.back();
    heap.pop_back();

    if (heap.empty()) {
        cout << "    Heap is now empty" << endl;
        cout << "    Extracted: " << maxValue << endl;
        return;
    }

    cout << "    Moved " << heap[0] << " (last element) to root: ";
    printHeap(heap);

    // Sift down with detailed output
    int index = 0;
    int n = heap.size();

    while (true) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int largest = index;

        // Determine the larger child
        if (left < n && right < n) {
            cout << "    Node " << heap[index] << " at index " << index
                 << ": left child = " << heap[left] << ", right child = " << heap[right];
            if (heap[left] >= heap[right]) {
                cout << " → larger child is " << heap[left] << " (left)" << endl;
            } else {
                cout << " → larger child is " << heap[right] << " (right)" << endl;
            }
        } else if (left < n) {
            cout << "    Node " << heap[index] << " at index " << index
                 << ": only left child = " << heap[left] << endl;
        } else {
            cout << "    Node " << heap[index] << " at index " << index
                 << ": no children → stop (leaf node)" << endl;
            break;
        }

        if (left < n && heap[left] > heap[largest]) {
            largest = left;
        }
        if (right < n && heap[right] > heap[largest]) {
            largest = right;
        }

        if (largest != index) {
            cout << "    " << heap[index] << " < " << heap[largest]
                 << " → swap" << endl;
            swap(heap[index], heap[largest]);
            index = largest;
            cout << "    After swap: ";
            printHeap(heap);
        } else {
            cout << "    " << heap[index] << " >= both children → stop" << endl;
            break;
        }
    }

    cout << "    Extracted: " << maxValue << ", Heap: ";
    printHeap(heap);
}

/**
 * @brief Builds a heap with step-by-step output showing each sift-down
 *
 * This version prints the state of the array after each sift-down operation
 * during the bottom-up heapify process.
 *
 * @param vec The vector to convert into a max-heap (modified in-place)
 *
 * @note Prints detailed trace of each sift-down during build-heap
 * @note Shows why we start from the last non-leaf node
 * @note Useful for teaching — demonstrates the O(n) bottom-up approach
 *
 * @complexity Time: O(n) - same as regular buildHeap, plus O(n) for printing
 * @complexity Space: O(1) - in-place
 */
void buildHeapWithSteps(vector<int>& vec) {
    int n = vec.size();

    cout << "\n=== Build Heap (Bottom-Up Heapify) Step-by-Step ===" << endl;
    cout << "Initial array: ";
    printHeap(vec);

    if (n <= 1) {
        cout << "Array has " << n << " element(s) — already a valid heap" << endl;
        return;
    }

    int lastNonLeaf = n / 2 - 1;
    cout << "Array size: " << n << endl;
    cout << "Last non-leaf node: index " << lastNonLeaf
         << " (value " << vec[lastNonLeaf] << ")" << endl;
    cout << "Leaf nodes (indices " << lastNonLeaf + 1 << " to " << n - 1
         << ") are already valid heaps" << endl;
    cout << endl;

    for (int i = lastNonLeaf; i >= 0; i--) {
        cout << "  siftDown(index " << i << ", value " << vec[i] << "):" << endl;

        // Perform sift-down with tracing
        int index = i;
        bool swapped = false;
        while (true) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int largest = index;

            if (left < n && vec[left] > vec[largest]) {
                largest = left;
            }
            if (right < n && vec[right] > vec[largest]) {
                largest = right;
            }

            if (largest != index) {
                cout << "    swap " << vec[index] << " with " << vec[largest] << endl;
                swap(vec[index], vec[largest]);
                index = largest;
                swapped = true;
            } else {
                break;
            }
        }

        if (!swapped) {
            cout << "    no swap needed — already satisfies heap property" << endl;
        }

        cout << "    Array: ";
        printHeap(vec);
    }

    cout << "\nFinal heap: ";
    printHeap(vec);
    cout << endl;
}

// ============================================================================
// MAIN FUNCTION - Demonstration and Testing
// ============================================================================

int main() {
    cout << "========================================" << endl;
    cout << "  Max-Heap - Teaching Example" << endl;
    cout << "========================================\n" << endl;

    // Example 1: Basic Heap Operations
    cout << "Example 1: Basic Heap Operations" << endl;
    cout << "--------------------------------" << endl;
    vector<int> heap1;

    int values1[] = {10, 20, 5, 30, 15};
    int n1 = sizeof(values1) / sizeof(values1[0]);

    for (int i = 0; i < n1; i++) {
        heapInsert(heap1, values1[i]);
        cout << "After inserting " << values1[i] << ": ";
        printHeap(heap1);
    }
    cout << "\nTree representation:" << endl;
    printHeapTree(heap1);
    cout << endl;

    // Example 2: Step-by-Step Insert
    cout << "Example 2: Step-by-Step Insert" << endl;
    cout << "------------------------------" << endl;
    vector<int> heap2;

    int values2[] = {10, 20, 5, 40, 30, 50};
    int n2 = sizeof(values2) / sizeof(values2[0]);

    for (int i = 0; i < n2; i++) {
        heapInsertWithSteps(heap2, values2[i]);
        cout << endl;
    }

    // Example 3: Step-by-Step Extract Max
    cout << "Example 3: Step-by-Step Extract Max" << endl;
    cout << "-----------------------------------" << endl;
    vector<int> heap3 = {50, 30, 40, 10, 20, 5};
    cout << "Starting heap: ";
    printHeap(heap3);
    cout << endl;

    heapExtractMaxWithSteps(heap3);
    cout << endl;
    heapExtractMaxWithSteps(heap3);
    cout << endl;

    // Example 4: Build Heap (Heapify)
    cout << "Example 4: Build Heap (Heapify)" << endl;
    cout << "-------------------------------" << endl;
    vector<int> arr4 = {4, 10, 3, 5, 1, 8, 7};
    buildHeapWithSteps(arr4);

    // Example 5: Extract All (Heap Sort behavior)
    cout << "Example 5: Extract All (Heap Sort Behavior)" << endl;
    cout << "--------------------------------------------" << endl;
    vector<int> heap5 = {4, 10, 3, 5, 1, 8, 7};
    buildHeap(heap5);
    cout << "Heap: ";
    printHeap(heap5);
    cout << "Extracting all elements (produces sorted order, largest first):" << endl;
    cout << "Sorted output: ";
    while (!heap5.empty()) {
        int val = heapExtractMax(heap5);
        cout << val;
        if (!heap5.empty()) {
            cout << ", ";
        }
    }
    cout << endl;
    cout << endl;

    // Example 6: Single Element
    cout << "Example 6: Single Element" << endl;
    cout << "-------------------------" << endl;
    vector<int> heap6;
    heapInsert(heap6, 42);
    cout << "After inserting 42: ";
    printHeap(heap6);
    int extracted = heapExtractMax(heap6);
    cout << "Extracted max: " << extracted << endl;
    cout << "Heap after extraction: ";
    printHeap(heap6);
    cout << endl;

    // Example 7: Duplicates
    cout << "Example 7: Duplicates" << endl;
    cout << "---------------------" << endl;
    vector<int> arr7 = {5, 3, 5, 3, 5, 1, 1};
    cout << "Array with duplicates: ";
    printHeap(arr7);
    buildHeap(arr7);
    cout << "After buildHeap: ";
    printHeap(arr7);
    cout << "Extracting all: ";
    while (!arr7.empty()) {
        cout << heapExtractMax(arr7);
        if (!arr7.empty()) {
            cout << ", ";
        }
    }
    cout << endl;
    cout << endl;

    // Example 8: Already a Heap
    cout << "Example 8: Already a Heap" << endl;
    cout << "-------------------------" << endl;
    vector<int> arr8 = {50, 30, 40, 10, 20, 35, 25};
    cout << "Input (already a valid max-heap): ";
    printHeap(arr8);
    buildHeapWithSteps(arr8);

    cout << "========================================" << endl;
    cout << "  All examples completed successfully!" << endl;
    cout << "========================================" << endl;

    return 0;
}

// ============================================================================
// KEY TEACHING POINTS
// ============================================================================
/*
 * 1. HEAP PROPERTY:
 *    - Max-heap: Every parent node is GREATER THAN OR EQUAL to its children
 *      → The maximum element is always at the root (index 0)
 *    - Min-heap: Every parent node is LESS THAN OR EQUAL to its children
 *      → The minimum element is always at the root (index 0)
 *    - This is a LOCAL property: each parent only needs to be compared with
 *      its direct children, not all descendants
 *    - A heap is NOT fully sorted — siblings have no ordering relationship
 *
 * 2. ARRAY REPRESENTATION:
 *    - A heap is a COMPLETE binary tree stored as an array (no gaps)
 *    - For a node at index i (0-based):
 *      → Parent:      (i - 1) / 2
 *      → Left child:  2 * i + 1
 *      → Right child: 2 * i + 2
 *    - Example: [50, 30, 40, 10, 15, 20]
 *      Index:    0   1   2   3   4   5
 *      Tree:           50
 *                     /  \
 *                   30    40
 *                  / \   /
 *                10  15 20
 *    - No pointers needed — parent/child relationships are computed from indices
 *
 * 3. SIFT UP VS SIFT DOWN:
 *    - Sift Up: Moves an element UPWARD toward the root
 *      → Used after INSERTION (new element may be too large for its position)
 *      → Compare with parent, swap if larger, repeat
 *    - Sift Down: Moves an element DOWNWARD toward the leaves
 *      → Used after EXTRACTION (replacement element may be too small)
 *      → Used during BUILD HEAP (fix subtrees from bottom up)
 *      → Compare with BOTH children, swap with the LARGER child, repeat
 *    - Key insight: sift down must choose the LARGER child to maintain the
 *      heap property (the largest must be the parent)
 *
 * 4. TIME COMPLEXITY:
 *    - Insert (heapInsert):     O(log n) — sift up at most height of tree
 *    - Extract-Max:             O(log n) — sift down at most height of tree
 *    - Peek (look at root):     O(1)     — maximum is always at index 0
 *    - Build Heap (heapify):    O(n)     — bottom-up approach is linear
 *    - Search:                  O(n)     — heap is NOT sorted, must scan all
 *    - Height of complete tree: floor(log₂ n)
 *
 * 5. BUILD HEAP IS O(n), NOT O(n log n):
 *    - Naive approach: insert n elements one by one → O(n log n)
 *    - Bottom-up heapify: start from leaves, sift down → O(n)
 *    - Why? Most nodes are near the bottom and need very few swaps:
 *      → n/2 leaf nodes need 0 swaps
 *      → n/4 nodes at level 1 need at most 1 swap
 *      → n/8 nodes at level 2 need at most 2 swaps
 *      → ...
 *      → 1 root node needs at most log(n) swaps
 *    - Total work: Σ (n / 2^(k+1)) * k for k = 0 to log(n)
 *      = n * Σ k / 2^(k+1) = n * 2 = O(n)
 *    - This is a key insight often tested in exams!
 *
 * 6. SPACE COMPLEXITY:
 *    - O(1) extra space — the heap is stored in-place in the array
 *    - No pointers, no linked list nodes, no recursion stack needed
 *    - The array IS the heap — index arithmetic gives parent/child relations
 *    - This makes heaps very cache-friendly compared to tree-based structures
 *
 * 7. HEAP SORT CONNECTION:
 *    - Heap Sort = Build Heap + Repeated Extract-Max
 *    - Step 1: Build a max-heap from the array — O(n)
 *    - Step 2: Repeatedly extract the max and place it at the end — O(n log n)
 *    - Total: O(n) + O(n log n) = O(n log n)
 *    - In-place (O(1) extra space), but NOT stable
 *    - Comparison: Merge Sort is O(n log n) but needs O(n) extra space
 *    - Comparison: Quick Sort is O(n log n) average but O(n²) worst case
 *
 * 8. PRIORITY QUEUES:
 *    - A priority queue is an abstract data type: insert, extract-max/min, peek
 *    - A heap is the standard implementation of a priority queue
 *    - C++ STL: std::priority_queue (uses a max-heap by default)
 *    - Java: PriorityQueue (uses a min-heap by default)
 *    - Python: heapq module (uses a min-heap)
 *    - Priority queues are used in: Dijkstra's algorithm, Huffman coding,
 *      task scheduling, event-driven simulation, A* search
 *
 * 9. WHEN TO USE HEAPS:
 *    - Priority queues: when you need fast insert and extract-max/min
 *    - Top-k problems: find the k largest/smallest elements in a stream
 *    - Scheduling: OS process scheduling, job scheduling with priorities
 *    - Median maintenance: use two heaps (max-heap + min-heap) for running median
 *    - Graph algorithms: Dijkstra's shortest path, Prim's minimum spanning tree
 *    - Merge k sorted lists: use a min-heap to efficiently merge
 *    - NOT for: searching for arbitrary elements, sorted traversal, random access
 *
 * 10. COMMON MISTAKES:
 *    - Forgetting 0-based indexing: parent = (i-1)/2, NOT i/2
 *    - Wrong child formulas: left = 2i+1, right = 2i+2 (NOT 2i, 2i+1)
 *    - Confusing sift up vs sift down:
 *      → Sift UP after INSERT (new element bubbles up)
 *      → Sift DOWN after EXTRACT (replacement sinks down)
 *    - Sifting down with the SMALLER child instead of the LARGER child
 *    - Forgetting to check if children exist before comparing (index out of bounds)
 *    - Assuming the heap is fully sorted (it is NOT — only parent ≥ children)
 *    - Using O(n log n) naive build instead of O(n) bottom-up heapify
 *    - Off-by-one errors in last non-leaf calculation: it is (n/2 - 1), not n/2
 */
