/**
 * @file bubble_sort.cpp
 * @brief Implementation of Bubble Sort algorithm with comprehensive documentation
 * @author CS 302 - Data Structures
 * @date 2026
 * 
 * Bubble Sort: Compare adjacent pairs left to right. Swap if out of order.
 * Largest element "bubbles" to the end each pass. Sorted region grows from right to left.
 * 
 * Key Feature: Early termination optimization - if a pass completes with zero swaps,
 * the array is already sorted and we can stop early.
 * 
 * Time Complexity: O(n²) worst case, O(n) best case (with early termination)
 * Space Complexity: O(1) - in-place sorting
 * 
 * Best for: Teaching purposes, understanding basic sorting concepts, small datasets
 */

#include <iostream>
#include <vector>
#include <algorithm>  // for std::swap (C++11+)
#include <cassert>    // for assertions

using namespace std;

/**
 * @brief Sorts an array using Bubble Sort algorithm with early termination
 * 
 * Algorithm Overview:
 * 1. Compare adjacent pairs of elements from left to right
 * 2. If elements are out of order (left > right), swap them
 * 3. After each pass, the largest unsorted element "bubbles" to the end
 * 4. Sorted region grows from right to left
 * 5. Early termination: if a pass completes with zero swaps, array is sorted
 * 
 * Visual Example:
 * Pass 1: [29, 10, 14, 37, 13]
 *         29>10? Yes → swap → [10, 29, 14, 37, 13]
 *         29>14? Yes → swap → [10, 14, 29, 37, 13]
 *         29<37? No → no swap → [10, 14, 29, 37, 13]
 *         37>13? Yes → swap → [10, 14, 29, 13, 37] ✓ (37 bubbled to end)
 * 
 * Pass 2: [10, 14, 29, 13, 37]
 *         10<14? No swap → [10, 14, 29, 13, 37]
 *         14<29? No swap → [10, 14, 29, 13, 37]
 *         29>13? Yes → swap → [10, 14, 13, 29, 37] ✓ (29 bubbled to position)
 * 
 * Pass 3: [10, 14, 13, 29, 37]
 *         10<14? No swap → [10, 14, 13, 29, 37]
 *         14>13? Yes → swap → [10, 13, 14, 29, 37] ✓ (14 bubbled to position)
 * 
 * Pass 4: [10, 13, 14, 29, 37]
 *         10<13? No swap → [10, 13, 14, 29, 37]
 *         13<14? No swap → [10, 13, 14, 29, 37]
 *         Zero swaps! → EARLY TERMINATION ✓
 * 
 * @param arr The array to be sorted (modified in-place)
 * @param n The number of elements in the array
 * 
 * @note This function modifies the input array directly (in-place sorting)
 * @note Early termination makes best case O(n) when array is already sorted
 * @note For empty arrays or single-element arrays, function returns immediately
 * 
 * @complexity Time: O(n²) worst case, O(n) best case (with early termination)
 * @complexity Space: O(1) - only uses a few variables, no extra memory needed
 */
void bubbleSort(int arr[], int n) {
    // Edge case: empty array or single element is already sorted
    if (n <= 1) {
        return;
    }
    
    // Outer loop: controls how many passes we make
    // Maximum (n-1) passes needed in worst case
    for (int pass = 0; pass < n - 1; pass++) {
        
        // Flag to track if any swaps occurred in this pass
        // If no swaps occur, the array is already sorted (early termination)
        bool swapped = false;
        
        // Inner loop: compare adjacent pairs in the unsorted region
        // Unsorted region: indices [0] to [n - 1 - pass]
        // We compare up to (n - 1 - pass) because we compare arr[i] with arr[i+1]
        for (int i = 0; i < n - 1 - pass; i++) {
            
            // Compare adjacent elements
            // If left element is greater than right element, they're out of order
            if (arr[i] > arr[i + 1]) {
                // Swap the elements to put them in correct order
                swap(arr[i], arr[i + 1]);
                swapped = true;  // Mark that a swap occurred
            }
        }
        
        // Early termination optimization
        // If no swaps occurred in this pass, the array is already sorted
        // This makes best case O(n) instead of O(n²)
        if (!swapped) {
            // Array is sorted, no need to continue
            break;
        }
        
        // At this point, the largest element in the unsorted region
        // has "bubbled" to the end (rightmost position)
        // The sorted region has grown by one element (from right to left)
    }
    
    // After all passes (or early termination), the array is completely sorted
}

/**
 * @brief Overloaded version for std::vector (more C++-idiomatic)
 * 
 * This version uses vector's [] operator and is more flexible.
 * 
 * @param vec The vector to be sorted (modified in-place)
 */
void bubbleSort(vector<int>& vec) {
    int n = vec.size();
    
    // Edge case handling
    if (n <= 1) {
        return;
    }
    
    // Same algorithm, but using vector's [] operator
    for (int pass = 0; pass < n - 1; pass++) {
        bool swapped = false;
        
        for (int i = 0; i < n - 1 - pass; i++) {
            if (vec[i] > vec[i + 1]) {
                swap(vec[i], vec[i + 1]);
                swapped = true;
            }
        }
        
        if (!swapped) {
            break;  // Early termination
        }
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
 * @brief Demonstrates Bubble Sort with detailed step-by-step output
 * 
 * This version prints the array after each comparison and swap to help
 * students visualize how the algorithm works.
 */
void bubbleSortWithSteps(int arr[], int n) {
    if (n <= 1) {
        return;
    }
    
    cout << "\n=== Bubble Sort Step-by-Step ===" << endl;
    cout << "Initial array: ";
    printArray(arr, n);
    cout << endl;
    
    for (int pass = 0; pass < n - 1; pass++) {
        bool swapped = false;
        
        cout << "Pass " << (pass + 1) << ":" << endl;
        
        for (int i = 0; i < n - 1 - pass; i++) {
            cout << "  Compare " << arr[i] << " and " << arr[i + 1] << ": ";
            
            if (arr[i] > arr[i + 1]) {
                cout << arr[i] << " > " << arr[i + 1] << " → SWAP";
                swap(arr[i], arr[i + 1]);
                swapped = true;
            } else {
                cout << arr[i] << " ≤ " << arr[i + 1] << " → no swap";
            }
            cout << endl;
        }
        
        cout << "After pass " << (pass + 1) << ": ";
        printArray(arr, n);
        
        if (!swapped) {
            cout << "✓ No swaps occurred → EARLY TERMINATION (array is sorted!)" << endl;
            break;
        } else {
            cout << "  (Largest element bubbled to end)" << endl;
        }
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
    cout << "  Bubble Sort - Teaching Example" << endl;
    cout << "========================================\n" << endl;
    
    // Example 1: Basic sorting
    cout << "Example 1: Basic Bubble Sort" << endl;
    cout << "----------------------------" << endl;
    int arr1[] = {29, 10, 14, 37, 13};
    int n1 = sizeof(arr1) / sizeof(arr1[0]);
    
    cout << "Before sorting: ";
    printArray(arr1, n1);
    
    bubbleSort(arr1, n1);
    
    cout << "After sorting:  ";
    printArray(arr1, n1);
    cout << endl;
    
    // Example 2: Step-by-step visualization
    cout << "Example 2: Step-by-Step Visualization" << endl;
    cout << "--------------------------------------" << endl;
    int arr2[] = {64, 34, 25, 12, 22, 11, 90};
    int n2 = sizeof(arr2) / sizeof(arr2[0]);
    bubbleSortWithSteps(arr2, n2);
    
    // Example 3: Using vector
    cout << "Example 3: Using std::vector" << endl;
    cout << "----------------------------" << endl;
    vector<int> vec = {5, 2, 8, 1, 9, 3};
    cout << "Before sorting: ";
    printVector(vec);
    
    bubbleSort(vec);
    
    cout << "After sorting:  ";
    printVector(vec);
    cout << endl;
    
    // Example 4: Early termination (already sorted)
    cout << "Example 4: Early Termination (Already Sorted)" << endl;
    cout << "---------------------------------------------" << endl;
    int arr3[] = {1, 2, 3, 4, 5};
    int n3 = sizeof(arr3) / sizeof(arr3[0]);
    cout << "Already sorted array: ";
    printArray(arr3, n3);
    cout << "Demonstrating early termination:" << endl;
    bubbleSortWithSteps(arr3, n3);
    
    // Example 5: Reverse sorted array
    cout << "Example 5: Reverse Sorted Array" << endl;
    cout << "--------------------------------" << endl;
    int arr4[] = {5, 4, 3, 2, 1};
    int n4 = sizeof(arr4) / sizeof(arr4[0]);
    cout << "Reverse sorted array: ";
    printArray(arr4, n4);
    bubbleSort(arr4, n4);
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
    bubbleSort(arr5, n5);
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
    bubbleSort(arr6, n6);
    cout << "After sorting: ";
    printArray(arr6, n6);
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
 * 1. ALGORITHM STRATEGY:
 *    - Bubble Sort uses a "bubbling" strategy: compare adjacent pairs,
 *      swap if out of order, largest element "bubbles" to the end
 *    - Sorted region grows from RIGHT TO LEFT (like Selection Sort)
 *    - Each pass guarantees the largest unsorted element moves to its final position
 * 
 * 2. TIME COMPLEXITY ANALYSIS:
 *    - Worst case: O(n²) - array is reverse sorted
 *      - Outer loop: (n-1) iterations
 *      - Inner loop: (n-1), (n-2), (n-3), ..., 1 iterations
 *      - Total comparisons: n(n-1)/2 = O(n²)
 *    - Best case: O(n) - array is already sorted
 *      - Early termination after first pass (no swaps)
 *      - Only (n-1) comparisons needed
 *    - Average case: O(n²) - random data
 * 
 * 3. SPACE COMPLEXITY:
 *    - O(1) - only uses a few variables (pass, i, swapped)
 *    - Sorts "in-place" - doesn't need extra memory
 * 
 * 4. STABILITY:
 *    - Bubble Sort IS stable
 *    - Equal elements maintain their relative order
 *    - Example: [3₁, 3₂, 1] → after pass 1: [1, 3₁, 3₂]
 *    - The relative order of 3₁ and 3₂ is preserved
 * 
 * 5. EARLY TERMINATION OPTIMIZATION:
 *    - Boolean flag tracks if any swaps occurred
 *    - If a pass completes with zero swaps, array is sorted
 *    - This optimization makes best case O(n) instead of O(n²)
 *    - Important: This doesn't change worst case complexity
 * 
 * 6. WHEN TO USE:
 *    - Teaching/learning purposes (simple to understand)
 *    - Small datasets where O(n²) is acceptable
 *    - When stability is required (maintains relative order of equal elements)
 *    - NOT recommended for large datasets or production code
 * 
 * 7. COMPARISON WITH OTHER SORTS:
 *    - vs Selection Sort: Both O(n²), but Bubble Sort can be O(n) best case
 *      However, Bubble Sort typically does more swaps (potentially n² swaps)
 *    - vs Insertion Sort: Insertion Sort is generally faster in practice
 *      Both can be O(n) best case, but Insertion Sort has better constants
 *    - vs Quick Sort/Merge Sort: Much slower for large n, but simpler to understand
 * 
 * 8. COMMON MISTAKES TO AVOID:
 *    - Forgetting to implement early termination (misses O(n) best case)
 *    - Incorrect loop bounds (should be n-1-pass, not n-pass)
 *    - Comparing wrong elements (should be arr[i] with arr[i+1], not arr[i] with arr[j])
 */
