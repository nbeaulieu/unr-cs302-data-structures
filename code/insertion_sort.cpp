/**
 * @file insertion_sort.cpp
 * @brief Implementation of Insertion Sort algorithm with comprehensive documentation
 * @author CS 302 - Data Structures
 * @date 2026
 * 
 * Insertion Sort: Build sorted region from left to right. For each unsorted element,
 * save it, shift sorted elements right to make space, then insert into correct position.
 * Uses SHIFTS, not swaps.
 * 
 * Key Feature: Efficient for nearly-sorted data. Best case O(n) when array is
 * already sorted (only comparisons, no shifts needed).
 * 
 * Key Difference: Uses SHIFTS, not SWAPS!
 * - Shift: Moves one element right to make space (a → b, leaves gap)
 * - Swap: Exchanges two elements (a ↔ b)
 * 
 * Time Complexity: O(n²) worst case, O(n) best case (nearly sorted)
 * Space Complexity: O(1) - in-place sorting
 * 
 * Best for: Teaching purposes, small datasets, nearly-sorted data, online sorting
 */

#include <iostream>
#include <vector>
#include <algorithm>  // for std::swap (C++11+) - not used here, we use shifts instead
#include <cassert>    // for assertions

using namespace std;

/**
 * @brief Shifts an element one position to the right
 * 
 * This function moves an element from position 'from' to position 'to' (where to = from + 1).
 * This is used in Insertion Sort to make space for inserting a new element.
 * 
 * Key Difference from Swap:
 * - Swap: Exchanges two values (a ↔ b)
 * - Shift: Moves one value to make space (a → b, leaving a's position empty)
 * 
 * @param arr The array containing the element to shift
 * @param from Index of element to shift (source position)
 * @param to Index where element should be moved (destination position, typically from + 1)
 * 
 * @note This is different from swap! Shift moves one element, swap exchanges two.
 * @note In Insertion Sort, we shift elements RIGHT to make space for insertion.
 * @note The element at position 'from' is copied to position 'to'
 */
void shiftRight(int arr[], int from, int to) {
    // Move element from 'from' position to 'to' position
    // This leaves the 'from' position available for insertion
    arr[to] = arr[from];
    // Note: The original value at 'to' should have already been moved
    // (or is the gap where we're inserting)
}

/**
 * @brief Overloaded version for vector
 */
void shiftRight(vector<int>& vec, int from, int to) {
    vec[to] = vec[from];
}

/**
 * @brief Sorts an array using Insertion Sort algorithm
 * 
 * Algorithm Overview:
 * 1. Start with sorted region containing first element (index 0)
 * 2. For each unsorted element (starting at index 1):
 *    a. Save the element to be inserted (like holding a card)
 *    b. Compare with elements in sorted region from right to left
 *    c. Shift sorted elements RIGHT to make space
 *    d. Insert the saved element into the correct position
 * 3. Sorted region grows from left to right
 * 4. Repeat until all elements are sorted
 * 
 * Key Difference: Uses SHIFTS, not swaps. More efficient than swapping.
 * 
 * Visual Example:
 * Initial: [29, 10, 14, 37, 13]
 *          Sorted: [29]  Unsorted: [10, 14, 37, 13]
 * 
 * Insert 10:
 *   Save 10, compare with 29: 10 < 29 → shift 29 right → [_, 29, 14, 37, 13]
 *   Compare with nothing (at start) → insert 10 → [10, 29, 14, 37, 13]
 *   Sorted: [10, 29]  Unsorted: [14, 37, 13]
 * 
 * Insert 14:
 *   Save 14, compare with 29: 14 < 29 → shift 29 right → [10, _, 29, 37, 13]
 *   Compare with 10: 14 > 10 → stop, insert 14 → [10, 14, 29, 37, 13]
 *   Sorted: [10, 14, 29]  Unsorted: [37, 13]
 * 
 * Insert 37:
 *   Save 37, compare with 29: 37 > 29 → already in position → [10, 14, 29, 37, 13]
 *   Sorted: [10, 14, 29, 37]  Unsorted: [13]
 * 
 * Insert 13:
 *   Save 13, compare with 37: 13 < 37 → shift 37 right → [10, 14, 29, _, 37]
 *   Compare with 29: 13 < 29 → shift 29 right → [10, 14, _, 29, 37]
 *   Compare with 14: 13 < 14 → shift 14 right → [10, _, 14, 29, 37]
 *   Compare with 10: 13 > 10 → stop, insert 13 → [10, 13, 14, 29, 37] ✓
 * 
 * @param arr The array to be sorted (modified in-place)
 * @param n The number of elements in the array
 * 
 * @note This function modifies the input array directly (in-place sorting)
 * @note Uses SHIFTS (via shiftRight()) instead of SWAPS - key difference!
 * @note Shift: moves element right to make space (a → b)
 * @note Swap: exchanges two elements (a ↔ b)
 * @note Best case O(n) when array is already sorted (only comparisons, no shifts)
 * @note For empty arrays or single-element arrays, function returns immediately
 * 
 * @complexity Time: O(n²) worst case, O(n) best case (nearly sorted)
 * @complexity Space: O(1) - only uses a few variables, no extra memory needed
 */
void insertionSort(int arr[], int n) {
    // Edge case: empty array or single element is already sorted
    if (n <= 1) {
        return;
    }
    
    // Outer loop: process each unsorted element
    // Start at index 1 because index 0 is our initial sorted region
    for (int i = 1; i < n; i++) {
        
        // Save the element we're going to insert (like holding a card)
        // This element is currently "out of place" and needs to be inserted
        int valueToInsert = arr[i];
        
        // Find the correct position in the sorted region (indices 0 to i-1)
        // We'll shift elements right to make space, then insert
        
        // Start from the rightmost element of the sorted region
        int j = i - 1;
        
        // Shift elements RIGHT while they're greater than valueToInsert
        // We're looking for the correct insertion point
        // Continue as long as:
        //   1. We haven't reached the beginning (j >= 0)
        //   2. The current element is greater than what we're inserting
        while (j >= 0 && arr[j] > valueToInsert) {
            // Shift the element right to make space
            shiftRight(arr, j, j + 1);
            // Alternative direct assignment: arr[j + 1] = arr[j];
            j--;  // Move left to check the next element
        }
        
        // At this point, we've found the correct insertion position
        // j+1 is where we should insert valueToInsert
        // (j might be -1 if we need to insert at the beginning)
        arr[j + 1] = valueToInsert;
        
        // At this point, the sorted region has grown by one element (from left to right)
        // Elements from index 0 to i are now sorted
    }
    
    // After processing all elements, the array is completely sorted
}

/**
 * @brief Overloaded version for std::vector (more C++-idiomatic)
 * 
 * This version uses vector's [] operator and is more flexible.
 * 
 * @param vec The vector to be sorted (modified in-place)
 */
void insertionSort(vector<int>& vec) {
    int n = vec.size();
    
    // Edge case handling
    if (n <= 1) {
        return;
    }
    
    // Same algorithm, but using vector's [] operator
    for (int i = 1; i < n; i++) {
        int valueToInsert = vec[i];
        int j = i - 1;
        
        while (j >= 0 && vec[j] > valueToInsert) {
            shiftRight(vec, j, j + 1);
            // Alternative direct assignment: vec[j + 1] = vec[j];
            j--;
        }
        
        vec[j + 1] = valueToInsert;
    }
}

/**
 * @brief Helper function to print an array (for demonstration purposes)
 */
void printArray(const int arr[], int n) {
    cout << "[";
    for (int i = 0; i < n; i++) {
        cout << arr[i];
        if (i < n - 1) {
            cout << ", ";
        }
    }
    cout << "]" << endl;
}

/**
 * @brief Helper function to print a vector
 */
void printVector(const vector<int>& vec) {
    cout << "[";
    for (size_t i = 0; i < vec.size(); i++) {
        cout << vec[i];
        if (i < vec.size() - 1) {
            cout << ", ";
        }
    }
    cout << "]" << endl;
}

/**
 * @brief Demonstrates Insertion Sort with detailed step-by-step output
 * 
 * This version prints the array after each insertion to help students
 * visualize how the algorithm works, showing the shifts and insertions.
 */
void insertionSortWithSteps(int arr[], int n) {
    if (n <= 1) {
        return;
    }
    
    cout << "\n=== Insertion Sort Step-by-Step ===" << endl;
    cout << "Initial array: ";
    printArray(arr, n);
    cout << "Sorted region: [" << arr[0] << "]" << endl;
    cout << endl;
    
    for (int i = 1; i < n; i++) {
        int valueToInsert = arr[i];
        
        cout << "Insert element " << valueToInsert << " (index " << i << "):" << endl;
        cout << "  Current state: ";
        printArray(arr, n);
        
        int j = i - 1;
        int shifts = 0;
        
        // Show the shifting process
        while (j >= 0 && arr[j] > valueToInsert) {
            cout << "    " << valueToInsert << " < " << arr[j] 
                 << " → shift " << arr[j] << " right" << endl;
            shiftRight(arr, j, j + 1);
            // Alternative direct assignment: arr[j + 1] = arr[j];
            j--;
            shifts++;
        }
        
        if (shifts == 0) {
            cout << "    " << valueToInsert << " ≥ all elements in sorted region → already in position" << endl;
        } else {
            cout << "    " << valueToInsert << " ≥ " << (j >= 0 ? to_string(arr[j]) : "start") 
                 << " → stop shifting" << endl;
        }
        
        arr[j + 1] = valueToInsert;
        
        cout << "  After insertion: ";
        printArray(arr, n);
        cout << "  Sorted region: [";
        for (int k = 0; k <= i; k++) {
            cout << arr[k];
            if (k < i) cout << ", ";
        }
        cout << "]" << endl;
        cout << endl;
    }
    
    cout << "Final sorted array: ";
    printArray(arr, n);
    cout << endl;
}

// ============================================================================
// MAIN FUNCTION - Demonstration and Testing
// ============================================================================

int main() {
    cout << "========================================" << endl;
    cout << "  Insertion Sort - Teaching Example" << endl;
    cout << "========================================\n" << endl;
    
    // Example 1: Basic sorting
    cout << "Example 1: Basic Insertion Sort" << endl;
    cout << "-------------------------------" << endl;
    int arr1[] = {29, 10, 14, 37, 13};
    int n1 = sizeof(arr1) / sizeof(arr1[0]);
    
    cout << "Before sorting: ";
    printArray(arr1, n1);
    
    insertionSort(arr1, n1);
    
    cout << "After sorting:  ";
    printArray(arr1, n1);
    cout << endl;
    
    // Example 2: Step-by-step visualization
    cout << "Example 2: Step-by-Step Visualization" << endl;
    cout << "--------------------------------------" << endl;
    int arr2[] = {64, 34, 25, 12, 22, 11, 90};
    int n2 = sizeof(arr2) / sizeof(arr2[0]);
    insertionSortWithSteps(arr2, n2);
    
    // Example 3: Using vector
    cout << "Example 3: Using std::vector" << endl;
    cout << "----------------------------" << endl;
    vector<int> vec = {5, 2, 8, 1, 9, 3};
    cout << "Before sorting: ";
    printVector(vec);
    
    insertionSort(vec);
    
    cout << "After sorting:  ";
    printVector(vec);
    cout << endl;
    
    // Example 4: Best case (already sorted)
    cout << "Example 4: Best Case (Already Sorted)" << endl;
    cout << "------------------------------------" << endl;
    int arr3[] = {1, 2, 3, 4, 5};
    int n3 = sizeof(arr3) / sizeof(arr3[0]);
    cout << "Already sorted array: ";
    printArray(arr3, n3);
    cout << "Demonstrating best case (O(n) - only comparisons, no shifts):" << endl;
    insertionSortWithSteps(arr3, n3);
    
    // Example 5: Reverse sorted array (worst case)
    cout << "Example 5: Worst Case (Reverse Sorted)" << endl;
    cout << "--------------------------------------" << endl;
    int arr4[] = {5, 4, 3, 2, 1};
    int n4 = sizeof(arr4) / sizeof(arr4[0]);
    cout << "Reverse sorted array: ";
    printArray(arr4, n4);
    insertionSort(arr4, n4);
    cout << "After sorting: ";
    printArray(arr4, n4);
    cout << endl;
    
    // Example 6: Single element
    cout << "Example 6: Single Element" << endl;
    cout << "------------------------" << endl;
    int arr5[] = {42};
    int n5 = 1;
    cout << "Single element: ";
    printArray(arr5, n5);
    insertionSort(arr5, n5);
    cout << "After sorting: ";
    printArray(arr5, n5);
    cout << endl;
    
    // Example 7: Duplicate values
    cout << "Example 7: Array with Duplicates" << endl;
    cout << "--------------------------------" << endl;
    int arr6[] = {3, 1, 3, 2, 1, 2};
    int n6 = sizeof(arr6) / sizeof(arr6[0]);
    cout << "Array with duplicates: ";
    printArray(arr6, n6);
    insertionSort(arr6, n6);
    cout << "After sorting: ";
    printArray(arr6, n6);
    cout << endl;
    
    // Example 8: Nearly sorted (demonstrates efficiency)
    cout << "Example 8: Nearly Sorted Array" << endl;
    cout << "------------------------------" << endl;
    int arr7[] = {1, 2, 3, 5, 4, 6, 7};
    int n7 = sizeof(arr7) / sizeof(arr7[0]);
    cout << "Nearly sorted array (only 4 and 5 swapped): ";
    printArray(arr7, n7);
    cout << "Demonstrating efficiency on nearly-sorted data:" << endl;
    insertionSortWithSteps(arr7, n7);
    
    cout << "========================================" << endl;
    cout << "  All examples completed successfully!" << endl;
    cout << "========================================" << endl;
    
    return 0;
}

// ============================================================================
// KEY TEACHING POINTS
// ============================================================================
/*
 * 1. ALGORITHM STRATEGY:
 *    - Insertion Sort uses an "insertion" strategy: build sorted region left-to-right
 *    - For each unsorted element, find its correct position and insert it
 *    - Uses SHIFTS, not swaps - more efficient than swapping
 *    - Sorted region grows from LEFT TO RIGHT (opposite of Selection/Bubble Sort)
 * 
 * 2. TIME COMPLEXITY ANALYSIS:
 *    - Worst case: O(n²) - array is reverse sorted
 *      - Each element needs to shift all previous elements
 *      - Total shifts: 1 + 2 + 3 + ... + (n-1) = n(n-1)/2 = O(n²)
 *    - Best case: O(n) - array is already sorted
 *      - Only comparisons needed, no shifts
 *      - Total comparisons: (n-1) = O(n)
 *    - Average case: O(n²) - random data
 *    - Nearly sorted: O(n) - very efficient!
 * 
 * 3. SPACE COMPLEXITY:
 *    - O(1) - only uses a few variables (i, j, valueToInsert)
 *    - Sorts "in-place" - doesn't need extra memory
 * 
 * 4. STABILITY:
 *    - Insertion Sort IS stable
 *    - Equal elements maintain their relative order
 *    - Example: [3₁, 3₂, 1] → after insertion: [1, 3₁, 3₂]
 *    - The relative order of 3₁ and 3₂ is preserved
 * 
 * 5. SHIFTS VS SWAPS (KEY TEACHING POINT):
 *    - Insertion Sort uses SHIFTS (moving elements right)
 *    - Selection/Bubble Sort use SWAPS (exchanging pairs)
 *    
 *    SHIFT (Insertion Sort):
 *      arr[j+1] = arr[j]  // Move element right, leaves gap
 *      Example: [10, 20, _, 30] → shift 20 right → [10, _, 20, 30]
 *      Purpose: Make space for insertion
 *    
 *    SWAP (Selection/Bubble Sort):
 *      temp = a; a = b; b = temp  // Exchange two values
 *      Example: [10, 20] → swap → [20, 10]
 *      Purpose: Exchange positions of two elements
 *    
 *    - Shifts are more efficient for nearly-sorted data
 *    - Each shift is O(1), but we may need many shifts
 *    - Key insight: We're not swapping pairs, we're making space for insertion
 * 
 * 6. WHEN TO USE:
 *    - Teaching/learning purposes (intuitive algorithm)
 *    - Small datasets (often fastest for n < 10-20)
 *    - Nearly-sorted data (excellent performance)
 *    - Online sorting (sorting data as it arrives)
 *    - Hybrid algorithms (used in Timsort, Introsort)
 *    - NOT recommended for large, random datasets
 * 
 * 7. COMPARISON WITH OTHER SORTS:
 *    - vs Selection Sort: Both O(n²), but Insertion Sort is faster in practice
 *      Insertion Sort has better constants and is O(n) best case
 *    - vs Bubble Sort: Insertion Sort is generally faster
 *      Both stable, both O(n) best case, but Insertion Sort has better average case
 *    - vs Quick Sort/Merge Sort: Slower for large n, but simpler and often
 *      faster for small datasets (used in hybrid algorithms)
 * 
 * 8. ANALOGY:
 *    - Like sorting a hand of playing cards
 *    - Pick up one card at a time
 *    - Insert it into the correct position in your sorted hand
 *    - Shift other cards to make space
 * 
 * 9. COMMON MISTAKES TO AVOID:
 *    - Forgetting to save valueToInsert before shifting (loses the value)
 *    - Incorrect loop bounds (should start at i=1, not i=0)
 *    - Wrong comparison direction (should be arr[j] > valueToInsert)
 *    - Not handling j = -1 case correctly (insertion at beginning)
 *    - Confusing shifts with swaps (they're different operations)
 */
