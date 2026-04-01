/**
 * @file array.cpp
 * @brief Implementation of fundamental Array operations with comprehensive documentation
 * @author CS 302 - Data Structures
 * @date 2026
 *
 * Array: A contiguous block of memory storing elements of the same type.
 * Elements are accessed by index in O(1) time using pointer arithmetic.
 *
 * Key Feature: Constant-time random access via index calculation.
 * Address of element i = base_address + i * sizeof(element)
 *
 * Static Array: Fixed capacity determined at compile time.
 * Dynamic Array: Can grow/shrink at runtime (e.g., std::vector).
 *
 * Time Complexity: Access O(1), Search O(n), Insert O(n), Delete O(n), Append O(1)
 * Space Complexity: O(n) for n elements, O(capacity) total allocated
 *
 * Best for: Random access, iteration, cache-friendly traversal, known-size collections
 */

#include <iostream>
#include <cassert>

using namespace std;

/**
 * @brief Accesses an element at the given index with bounds checking
 *
 * Arrays provide O(1) random access because the memory address of any element
 * can be computed directly: address = base + index * sizeof(element).
 * No traversal is needed — this is the primary advantage of arrays.
 *
 * Visual Example:
 *   Array: [10, 20, 30, 40, 50]   capacity = 8, size = 5
 *   Index:   0   1   2   3   4    5   6   7
 *
 *   accessElement(arr, 5, 2) → returns 30 (direct jump to index 2)
 *   accessElement(arr, 5, 7) → prints error (index out of bounds)
 *
 * @param arr The array to access
 * @param n The number of elements currently in the array (size)
 * @param index The index of the element to retrieve
 * @return The element at the given index, or -1 if index is out of bounds
 *
 * @note Bounds checking prevents undefined behavior from out-of-range access
 * @note In C/C++, accessing out-of-bounds memory is undefined behavior (no crash guaranteed)
 *
 * @complexity Time: O(1) — direct address computation
 * @complexity Space: O(1) — no extra memory needed
 */
int accessElement(int arr[], int n, int index) {
    // Check if index is within valid range [0, n-1]
    if (index < 0 || index >= n) {
        cout << "    Error: Index " << index << " is out of bounds (size = " << n << ")" << endl;
        return -1;
    }

    // Direct access — computed as base_address + index * sizeof(int)
    return arr[index];
}

/**
 * @brief Inserts a value at the specified index, shifting elements right
 *
 * To insert at position i, all elements from index i to n-1 must be shifted
 * one position to the right to make room. This is why insertion is O(n) —
 * in the worst case (inserting at index 0), ALL elements must move.
 *
 * Visual Example:
 *   Insert 25 at index 2 in [10, 20, 30, 40, 50]  (capacity = 8)
 *
 *   Before: [10, 20, 30, 40, 50, _, _, _]   size = 5
 *                     ↑ insert here
 *
 *   Step 1: Shift right from end:  arr[5] = arr[4] → [10, 20, 30, 40, 50, 50, _, _]
 *   Step 2: Shift right:           arr[4] = arr[3] → [10, 20, 30, 40, 40, 50, _, _]
 *   Step 3: Shift right:           arr[3] = arr[2] → [10, 20, 30, 30, 40, 50, _, _]
 *   Step 4: Place value:           arr[2] = 25     → [10, 20, 25, 30, 40, 50, _, _]
 *
 *   After:  [10, 20, 25, 30, 40, 50, _, _]   size = 6
 *
 * @param arr The array to insert into
 * @param n The current number of elements (passed by reference, updated on success)
 * @param capacity The maximum number of elements the array can hold
 * @param index The position at which to insert the new value
 * @param value The value to insert
 *
 * @note Shifts elements right starting from the END to avoid overwriting
 * @note Must check capacity before inserting — arrays cannot grow beyond capacity
 * @note n is passed by reference and incremented on successful insertion
 *
 * @complexity Time: O(n) — must shift up to n elements
 * @complexity Space: O(1) — in-place operation
 */
void insertAt(int arr[], int& n, int capacity, int index, int value) {
    // Check if there is room in the array
    if (n >= capacity) {
        cout << "    Error: Array is full (size = " << n << ", capacity = " << capacity << ")" << endl;
        return;
    }

    // Check if index is valid for insertion (can insert at index n to append)
    if (index < 0 || index > n) {
        cout << "    Error: Index " << index << " is out of bounds for insertion (size = " << n << ")" << endl;
        return;
    }

    // Shift elements to the right, starting from the END
    // We must go from right to left to avoid overwriting values
    for (int i = n; i > index; i--) {
        arr[i] = arr[i - 1];
    }

    // Place the new value at the target index
    arr[index] = value;

    // Increase the size
    n++;
}

/**
 * @brief Deletes the element at the specified index, shifting elements left
 *
 * To delete at position i, all elements from index i+1 to n-1 must be shifted
 * one position to the left to fill the gap. This is why deletion is O(n) —
 * in the worst case (deleting at index 0), ALL remaining elements must move.
 *
 * Visual Example:
 *   Delete at index 1 from [10, 20, 30, 40, 50]  (capacity = 8)
 *
 *   Before: [10, 20, 30, 40, 50, _, _, _]   size = 5
 *                 ↑ delete this
 *
 *   Step 1: Shift left: arr[1] = arr[2] → [10, 30, 30, 40, 50, _, _, _]
 *   Step 2: Shift left: arr[2] = arr[3] → [10, 30, 40, 40, 50, _, _, _]
 *   Step 3: Shift left: arr[3] = arr[4] → [10, 30, 40, 50, 50, _, _, _]
 *   Step 4: Reduce size (n = 4)          → [10, 30, 40, 50, _, _, _, _]
 *
 *   After:  [10, 30, 40, 50, _, _, _, _]   size = 4
 *
 * @param arr The array to delete from
 * @param n The current number of elements (passed by reference, decremented on success)
 * @param index The position of the element to remove
 *
 * @note Shifts elements left starting from the deleted position
 * @note n is passed by reference and decremented on successful deletion
 * @note The "removed" element is simply overwritten — no memory is freed
 *
 * @complexity Time: O(n) — must shift up to n-1 elements
 * @complexity Space: O(1) — in-place operation
 */
void deleteAt(int arr[], int& n, int index) {
    // Check if array is empty
    if (n <= 0) {
        cout << "    Error: Array is empty, nothing to delete" << endl;
        return;
    }

    // Check if index is valid
    if (index < 0 || index >= n) {
        cout << "    Error: Index " << index << " is out of bounds (size = " << n << ")" << endl;
        return;
    }

    // Shift elements to the left, starting from the deleted position
    // We go from left to right to avoid overwriting values
    for (int i = index; i < n - 1; i++) {
        arr[i] = arr[i + 1];
    }

    // Decrease the size
    n--;
}

/**
 * @brief Searches for a value in the array using linear search
 *
 * Linear search checks each element one by one from index 0 to n-1.
 * This is the only option for unsorted arrays. For sorted arrays,
 * binary search can achieve O(log n), but linear search works regardless.
 *
 * Visual Example:
 *   Search for 30 in [10, 20, 30, 40, 50]:
 *
 *   Check index 0: arr[0] = 10 ≠ 30 → continue
 *   Check index 1: arr[1] = 20 ≠ 30 → continue
 *   Check index 2: arr[2] = 30 = 30 → FOUND at index 2!
 *
 *   Search for 99 in [10, 20, 30, 40, 50]:
 *   Check all indices 0-4: none match → return -1 (not found)
 *
 * @param arr The array to search
 * @param n The number of elements in the array
 * @param value The value to search for
 * @return The index of the first occurrence, or -1 if not found
 *
 * @note Returns the FIRST occurrence — there may be duplicates
 * @note For sorted arrays, consider binary search O(log n) instead
 *
 * @complexity Time: O(n) — may need to check every element
 * @complexity Space: O(1) — no extra memory needed
 */
int linearSearch(int arr[], int n, int value) {
    // Check each element sequentially
    for (int i = 0; i < n; i++) {
        if (arr[i] == value) {
            return i;  // Found — return the index
        }
    }

    // Value not found in the array
    return -1;
}

/**
 * @brief Appends a value to the end of the array
 *
 * Appending is a special case of insertion at position n (the end).
 * No shifting is required, so it runs in O(1) time. This is the most
 * efficient way to add elements to an array.
 *
 * Visual Example:
 *   Append 60 to [10, 20, 30, 40, 50]  (capacity = 8)
 *
 *   Before: [10, 20, 30, 40, 50, _, _, _]   size = 5
 *                                 ↑ place here
 *   After:  [10, 20, 30, 40, 50, 60, _, _]  size = 6
 *
 * @param arr The array to append to
 * @param n The current number of elements (passed by reference, incremented on success)
 * @param capacity The maximum number of elements the array can hold
 * @param value The value to append
 *
 * @note O(1) because no shifting is needed — just place at index n
 * @note Must check capacity before appending
 *
 * @complexity Time: O(1) — no elements need to be moved
 * @complexity Space: O(1) — in-place operation
 */
void appendElement(int arr[], int& n, int capacity, int value) {
    // Check if there is room
    if (n >= capacity) {
        cout << "    Error: Array is full (size = " << n << ", capacity = " << capacity << ")" << endl;
        return;
    }

    // Place the value at the end and increment size
    arr[n] = value;
    n++;
}

/**
 * @brief Prints the array showing used slots and empty capacity
 *
 * Displays the array contents along with empty slots to visualize
 * the difference between size (used elements) and capacity (total space).
 *
 * Visual Example:
 *   printArray([10, 20, 30, _, _, _], size=3, capacity=6)
 *   Output: [10, 20, 30 | _, _, _]  (size=3, capacity=6)
 *
 * @param arr The array to print
 * @param n The number of elements currently in the array (size)
 * @param capacity The total capacity of the array
 *
 * @note The | separator shows where used elements end and empty slots begin
 */
void printArray(int arr[], int n, int capacity) {
    cout << "[";
    for (int i = 0; i < capacity; i++) {
        if (i < n) {
            cout << arr[i];
        } else {
            cout << "_";
        }
        if (i < capacity - 1) {
            cout << ", ";
        }
    }
    cout << "]  (size=" << n << ", capacity=" << capacity << ")" << endl;
}

/**
 * @brief Inserts at a given index with step-by-step output showing each shift
 *
 * This version prints each shift operation during insertion to help students
 * visualize how elements move to make room for the new value.
 *
 * @param arr The array to insert into
 * @param n The current number of elements (passed by reference, updated on success)
 * @param capacity The maximum number of elements the array can hold
 * @param index The position at which to insert the new value
 * @param value The value to insert
 *
 * @note Prints detailed trace of each shift and the final placement
 * @note Useful for teaching — shows exactly why insertion is O(n)
 *
 * @complexity Time: O(n) — same as regular insert, plus O(n) for printing
 * @complexity Space: O(1) — in-place operation
 */
void insertAtWithSteps(int arr[], int& n, int capacity, int index, int value) {
    cout << "  Inserting " << value << " at index " << index << "..." << endl;

    // Check if there is room
    if (n >= capacity) {
        cout << "    Error: Array is full (size = " << n << ", capacity = " << capacity << ")" << endl;
        return;
    }

    // Check bounds
    if (index < 0 || index > n) {
        cout << "    Error: Index " << index << " is out of bounds for insertion (size = " << n << ")" << endl;
        return;
    }

    cout << "    Before: ";
    printArray(arr, n, capacity);

    // Shift elements to the right with step-by-step output
    int shifts = 0;
    for (int i = n; i > index; i--) {
        arr[i] = arr[i - 1];
        shifts++;
        cout << "    Shift: arr[" << i << "] = arr[" << (i - 1) << "] (moved " << arr[i] << " right)  → ";
        // Temporarily show the array mid-shift
        int tempN = n + 1;
        printArray(arr, tempN, capacity);
    }

    // Place the new value
    arr[index] = value;
    n++;

    if (shifts == 0) {
        cout << "    No shifts needed — inserting at the end" << endl;
    } else {
        cout << "    Total shifts: " << shifts << endl;
    }

    cout << "    Place " << value << " at index " << index << endl;
    cout << "    After:  ";
    printArray(arr, n, capacity);
}

/**
 * @brief Deletes at a given index with step-by-step output showing each shift
 *
 * This version prints each shift operation during deletion to help students
 * visualize how elements move to fill the gap left by the removed element.
 *
 * @param arr The array to delete from
 * @param n The current number of elements (passed by reference, decremented on success)
 * @param index The position of the element to remove
 *
 * @note Prints detailed trace of each shift and the resulting array
 * @note Useful for teaching — shows exactly why deletion is O(n)
 *
 * @complexity Time: O(n) — same as regular delete, plus O(n) for printing
 * @complexity Space: O(1) — in-place operation
 */
void deleteAtWithSteps(int arr[], int& n, int capacity, int index) {
    cout << "  Deleting element at index " << index << " (value " << arr[index] << ")..." << endl;

    // Check if array is empty
    if (n <= 0) {
        cout << "    Error: Array is empty, nothing to delete" << endl;
        return;
    }

    // Check bounds
    if (index < 0 || index >= n) {
        cout << "    Error: Index " << index << " is out of bounds (size = " << n << ")" << endl;
        return;
    }

    cout << "    Before: ";
    printArray(arr, n, capacity);

    int removedValue = arr[index];

    // Shift elements to the left with step-by-step output
    int shifts = 0;
    for (int i = index; i < n - 1; i++) {
        arr[i] = arr[i + 1];
        shifts++;
        cout << "    Shift: arr[" << i << "] = arr[" << (i + 1) << "] (moved " << arr[i] << " left)   → ";
        printArray(arr, n, capacity);
    }

    n--;

    if (shifts == 0) {
        cout << "    No shifts needed — deleted the last element" << endl;
    } else {
        cout << "    Total shifts: " << shifts << endl;
    }

    cout << "    Removed value: " << removedValue << endl;
    cout << "    After:  ";
    printArray(arr, n, capacity);
}

/**
 * @brief Searches for a value with step-by-step output showing each comparison
 *
 * This version prints each comparison during the linear search to help
 * students visualize the sequential scanning process.
 *
 * @param arr The array to search
 * @param n The number of elements in the array
 * @param value The value to search for
 *
 * @note Prints each comparison and whether it matched or not
 * @note Useful for teaching — shows why linear search is O(n) worst case
 *
 * @complexity Time: O(n) — same as regular search, plus O(n) for printing
 * @complexity Space: O(1) — no extra memory needed
 */
void linearSearchWithSteps(int arr[], int n, int value) {
    cout << "  Searching for " << value << "..." << endl;

    int comparisons = 0;
    for (int i = 0; i < n; i++) {
        comparisons++;
        if (arr[i] == value) {
            cout << "    Check index " << i << ": arr[" << i << "] = " << arr[i]
                 << " = " << value << " → FOUND!" << endl;
            cout << "    Result: Found at index " << i << " after " << comparisons
                 << " comparison(s)" << endl;
            return;
        } else {
            cout << "    Check index " << i << ": arr[" << i << "] = " << arr[i]
                 << " ≠ " << value << " → continue" << endl;
        }
    }

    cout << "    Result: Not found after " << comparisons << " comparison(s)" << endl;
}

// ============================================================================
// MAIN FUNCTION - Demonstration and Testing
// ============================================================================

int main() {
    cout << "========================================" << endl;
    cout << "  Array Operations - Teaching Example" << endl;
    cout << "========================================\n" << endl;

    // Example 1: Basic Access
    cout << "Example 1: Basic Access — O(1) Random Access" << endl;
    cout << "---------------------------------------------" << endl;
    const int CAP1 = 8;
    int arr1[CAP1] = {10, 20, 30, 40, 50};
    int size1 = 5;

    cout << "Array: ";
    printArray(arr1, size1, CAP1);

    cout << "Access index 0: " << accessElement(arr1, size1, 0) << endl;
    cout << "Access index 2: " << accessElement(arr1, size1, 2) << endl;
    cout << "Access index 4: " << accessElement(arr1, size1, 4) << endl;
    cout << "Access index 7: ";
    accessElement(arr1, size1, 7);
    cout << "Access index -1: ";
    accessElement(arr1, size1, -1);
    cout << endl;

    // Example 2: Step-by-Step Insert
    cout << "Example 2: Step-by-Step Insert — O(n) Shifting" << endl;
    cout << "-----------------------------------------------" << endl;
    const int CAP2 = 10;
    int arr2[CAP2] = {10, 20, 30, 40, 50};
    int size2 = 5;

    insertAtWithSteps(arr2, size2, CAP2, 2, 25);
    cout << endl;
    insertAtWithSteps(arr2, size2, CAP2, 0, 5);
    cout << endl;
    insertAtWithSteps(arr2, size2, CAP2, size2, 99);
    cout << endl;

    // Example 3: Step-by-Step Delete
    cout << "Example 3: Step-by-Step Delete — O(n) Shifting" << endl;
    cout << "-----------------------------------------------" << endl;
    const int CAP3 = 10;
    int arr3[CAP3] = {10, 20, 30, 40, 50, 60};
    int size3 = 6;

    deleteAtWithSteps(arr3, size3, CAP3, 1);
    cout << endl;
    deleteAtWithSteps(arr3, size3, CAP3, 0);
    cout << endl;
    deleteAtWithSteps(arr3, size3, CAP3, size3 - 1);
    cout << endl;

    // Example 4: Step-by-Step Search
    cout << "Example 4: Step-by-Step Linear Search — O(n)" << endl;
    cout << "---------------------------------------------" << endl;
    const int CAP4 = 8;
    int arr4[CAP4] = {10, 20, 30, 40, 50};
    int size4 = 5;

    cout << "Array: ";
    printArray(arr4, size4, CAP4);
    linearSearchWithSteps(arr4, size4, 30);
    cout << endl;
    linearSearchWithSteps(arr4, size4, 99);
    cout << endl;
    linearSearchWithSteps(arr4, size4, 10);
    cout << endl;

    // Example 5: Append — O(1)
    cout << "Example 5: Append — O(1) No Shifting Needed" << endl;
    cout << "--------------------------------------------" << endl;
    const int CAP5 = 8;
    int arr5[CAP5] = {10, 20, 30};
    int size5 = 3;

    cout << "Before: ";
    printArray(arr5, size5, CAP5);
    appendElement(arr5, size5, CAP5, 40);
    cout << "After append 40: ";
    printArray(arr5, size5, CAP5);
    appendElement(arr5, size5, CAP5, 50);
    cout << "After append 50: ";
    printArray(arr5, size5, CAP5);
    appendElement(arr5, size5, CAP5, 60);
    cout << "After append 60: ";
    printArray(arr5, size5, CAP5);
    cout << endl;

    // Example 6: Full Array — Capacity Limits
    cout << "Example 6: Full Array — Capacity Limits" << endl;
    cout << "----------------------------------------" << endl;
    const int CAP6 = 4;
    int arr6[CAP6] = {10, 20, 30, 40};
    int size6 = 4;

    cout << "Array: ";
    printArray(arr6, size6, CAP6);
    cout << "Attempt to insert 50 at index 2:" << endl;
    insertAt(arr6, size6, CAP6, 2, 50);
    cout << "Attempt to append 50:" << endl;
    appendElement(arr6, size6, CAP6, 50);
    cout << endl;

    // Example 7: Empty Array — Edge Cases
    cout << "Example 7: Empty Array — Edge Cases" << endl;
    cout << "------------------------------------" << endl;
    const int CAP7 = 6;
    int arr7[CAP7] = {};
    int size7 = 0;

    cout << "Empty array: ";
    printArray(arr7, size7, CAP7);
    cout << "Search for 10 in empty array:" << endl;
    linearSearchWithSteps(arr7, size7, 10);
    cout << endl;
    cout << "Insert 10 at index 0 in empty array:" << endl;
    insertAtWithSteps(arr7, size7, CAP7, 0, 10);
    cout << endl;
    cout << "Delete from single-element array:" << endl;
    deleteAtWithSteps(arr7, size7, CAP7, 0);
    cout << endl;

    // Example 8: Mixed Operations and Edge Cases
    cout << "Example 8: Mixed Operations — Duplicates and Boundaries" << endl;
    cout << "--------------------------------------------------------" << endl;
    const int CAP8 = 10;
    int arr8[CAP8] = {5, 10, 10, 20, 10};
    int size8 = 5;

    cout << "Array with duplicates: ";
    printArray(arr8, size8, CAP8);
    cout << "Search for 10 (first occurrence):" << endl;
    linearSearchWithSteps(arr8, size8, 10);
    cout << endl;
    cout << "Insert at beginning (index 0):" << endl;
    insertAtWithSteps(arr8, size8, CAP8, 0, 1);
    cout << endl;
    cout << "Insert at end (index " << size8 << "):" << endl;
    insertAtWithSteps(arr8, size8, CAP8, size8, 99);
    cout << endl;
    cout << "Delete last element:" << endl;
    deleteAtWithSteps(arr8, size8, CAP8, size8 - 1);
    cout << endl;

    cout << "========================================" << endl;
    cout << "  All examples completed successfully!" << endl;
    cout << "========================================" << endl;

    return 0;
}

// ============================================================================
// KEY TEACHING POINTS
// ============================================================================
/*
 * 1. CONTIGUOUS MEMORY:
 *    - Array elements are stored in consecutive memory locations
 *    - Element at index i is at: base_address + i * sizeof(element)
 *    - This means the CPU can jump directly to any element — no traversal
 *    - Contiguity is what makes O(1) random access possible
 *    - Contrast with linked lists: nodes can be scattered anywhere in memory
 *
 * 2. O(1) RANDOM ACCESS:
 *    - The defining advantage of arrays over linked structures
 *    - Given an index, the element is accessed in constant time
 *    - No matter how large the array, accessing arr[0] takes the same time as arr[999999]
 *    - This relies on the contiguous memory layout and pointer arithmetic
 *    - Linked lists require O(n) traversal to reach the nth element
 *
 * 3. O(n) INSERT AND DELETE:
 *    - Inserting or deleting at position i requires shifting all subsequent elements
 *    - Insert: shift elements right (from end toward index) to make room
 *    - Delete: shift elements left (from index toward end) to fill the gap
 *    - Worst case: inserting/deleting at index 0 shifts ALL n elements
 *    - Best case: inserting/deleting at the end requires no shifting — O(1)
 *    - This is the primary disadvantage compared to linked lists (O(1) insert at known position)
 *
 * 4. CACHE FRIENDLINESS:
 *    - Arrays are extremely cache-friendly due to spatial locality
 *    - When one element is loaded into cache, nearby elements come along (cache line)
 *    - Sequential iteration through an array is one of the fastest operations on modern CPUs
 *    - Linked lists suffer cache misses because nodes may be scattered in memory
 *    - In practice, arrays often outperform linked lists even for operations
 *      where linked lists have better theoretical complexity
 *
 * 5. STATIC VS DYNAMIC ARRAYS:
 *    - Static array: fixed size, allocated on the stack (int arr[100])
 *      → Size must be known at compile time
 *      → Cannot grow or shrink
 *      → Very fast allocation and deallocation
 *    - Dynamic array: can resize, allocated on the heap (std::vector, new int[n])
 *      → Size can change at runtime
 *      → Resizing involves allocating new memory and copying
 *      → std::vector in C++, ArrayList in Java, list in Python
 *
 * 6. AMORTIZED RESIZING:
 *    - Dynamic arrays (like std::vector) double their capacity when full
 *    - Individual resize: O(n) — must copy all elements to new, larger array
 *    - Amortized over n insertions: O(1) per insertion
 *    - Growth pattern: capacity 1 → 2 → 4 → 8 → 16 → 32 → ...
 *    - Total copies for n insertions: 1 + 2 + 4 + ... + n = 2n - 1 = O(n)
 *    - Average cost per insertion: O(n) / n = O(1) amortized
 *    - This is why std::vector::push_back is "amortized O(1)"
 *
 * 7. COMPARISON WITH LINKED LISTS:
 *    - Array advantages: O(1) access, cache-friendly, no pointer overhead
 *    - Array disadvantages: O(n) insert/delete, fixed capacity (static), wasted space
 *    - Linked list advantages: O(1) insert/delete at known position, dynamic size
 *    - Linked list disadvantages: O(n) access, cache-unfriendly, pointer overhead
 *    - Rule of thumb: use arrays when you need random access or iteration,
 *      linked lists when you need frequent insertion/deletion at arbitrary positions
 *
 * 8. WHEN TO USE ARRAYS:
 *    - You need fast random access by index
 *    - You iterate through elements frequently (cache-friendly)
 *    - The size is known in advance or changes infrequently
 *    - You want minimal memory overhead (no pointers per element)
 *    - Examples: lookup tables, matrices, buffers, stacks (array-based),
 *      queues (circular array), hash table buckets
 *
 * 9. INDEX BOUNDS:
 *    - Valid indices for an array of size n: 0 to n-1
 *    - Accessing arr[n] or arr[-1] is UNDEFINED BEHAVIOR in C/C++
 *    - Unlike Java/Python, C++ does NOT throw an exception for out-of-bounds access
 *    - This can cause silent data corruption, security vulnerabilities, or crashes
 *    - Always validate indices before accessing array elements
 *    - Common off-by-one errors: using <= n instead of < n in loops
 *
 * 10. CAPACITY VS SIZE:
 *    - Size: the number of elements currently stored (how many are in use)
 *    - Capacity: the total number of elements the array can hold (how much room exists)
 *    - size <= capacity always
 *    - A full array: size == capacity (no room for more elements)
 *    - An empty array: size == 0 (all slots are available)
 *    - In std::vector: size() returns current count, capacity() returns allocated space
 *    - Wasted space = capacity - size (tradeoff for avoiding frequent resizing)
 */
