/**
 * @file selection_sort.cpp
 * @brief Implementation of Selection Sort algorithm with comprehensive documentation
 * @author CS 302 - Data Structures
 * @date 2026
 * 
 * Selection Sort (Maximum-to-End Variant):
 * - Find the LARGEST element in the unsorted region
 * - Swap it to the END (rightmost position) of the unsorted region
 * - Sorted region grows from RIGHT TO LEFT
 * 
 * Note: This is the "maximum to end" variant, NOT "minimum to front"
 * 
 * Time Complexity: O(n²) in all cases
 * Space Complexity: O(1) - in-place sorting
 * 
 * Best for: Teaching purposes, small datasets, when simplicity is more important than speed
 */

#include <iostream>
#include <vector>
#include <algorithm>  // for std::swap (C++11+)
#include <cassert>    // for assertions

using namespace std;

/**
 * @brief Sorts an array using Selection Sort algorithm (MAXIMUM-TO-END variant)
 * 
 * Algorithm Overview (Maximum-to-End):
 * 1. The array is divided into two regions: sorted (right) and unsorted (left)
 * 2. In each pass, find the LARGEST element in the unsorted region
 * 3. Swap it with the rightmost element of the unsorted region (moving it to the END)
 * 4. The sorted region grows from right to left
 * 5. Repeat until entire array is sorted
 * 
 * Key Point: We find MAXIMUM and place at END, NOT minimum at front!
 * 
 * Visual Example (finding maximum, swapping to end):
 * Pass 1: [29, 10, 14, 37, 13] → find MAX (37), swap with end → [29, 10, 14, 13, 37]
 *         Unsorted: [29, 10, 14, 37, 13]  Sorted: []
 * Pass 2: [29, 10, 14, 13] | [37] → find MAX (29), swap with end → [13, 10, 14, 29, 37]
 *         Unsorted: [29, 10, 14, 13]      Sorted: [37]
 * Pass 3: [13, 10, 14] | [29, 37] → find MAX (14), already at end → [13, 10, 14, 29, 37]
 *         Unsorted: [13, 10, 14]          Sorted: [29, 37]
 * Pass 4: [13, 10] | [14, 29, 37] → find MAX (13), swap with end → [10, 13, 14, 29, 37]
 *         Unsorted: [13, 10]              Sorted: [14, 29, 37]
 * Final:  [10, 13, 14, 29, 37] ✓
 * 
 * @param arr The array/vector to be sorted (modified in-place)
 * @param n The number of elements in the array
 * 
 * @note This function modifies the input array directly (in-place sorting)
 * @note For empty arrays or single-element arrays, function returns immediately
 * @note This is the MAXIMUM-TO-END variant (not minimum-to-front)
 * 
 * @complexity Time: O(n²) - nested loops, always performs n(n-1)/2 comparisons
 * @complexity Space: O(1) - only uses a few variables, no extra memory needed
 */
void selectionSort(int arr[], int n) {
    // Edge case: empty array or single element is already sorted
    if (n <= 1) {
        return;
    }
    
    // Outer loop: controls how many passes we make
    // We need (n-1) passes because after (n-1) elements are sorted,
    // the last element is automatically in the correct position
    for (int pass = 0; pass < n - 1; pass++) {
        
        // Find the index of the LARGEST element in the unsorted region
        // IMPORTANT: We find MAXIMUM, not minimum!
        // Unsorted region: indices [0] to [n - 1 - pass]
        // (pass represents how many elements are already sorted on the RIGHT)
        int indexOfLargest = 0;  // Start by assuming first element is largest
        
        // Inner loop: scan through the unsorted region to find the largest
        // We start at index 1 because we've already assumed index 0 is largest
        for (int i = 1; i < n - pass; i++) {
            
            // Compare current element with our current "largest"
            // If we find a larger element, update our record
            if (arr[i] > arr[indexOfLargest]) {
                indexOfLargest = i;
            }
        }
        
        // After finding the LARGEST element in the unsorted region,
        // swap it with the rightmost element of the unsorted region
        // This moves the maximum to the END (right side)
        // The rightmost unsorted position is: n - 1 - pass
        int rightmostUnsortedIndex = n - 1 - pass;
        
        // Only swap if the largest element isn't already at the end
        // This optimization avoids unnecessary swaps (though it doesn't change O(n²))
        if (indexOfLargest != rightmostUnsortedIndex) {
            swap(arr[indexOfLargest], arr[rightmostUnsortedIndex]);
        }
        
        // At this point, arr[rightmostUnsortedIndex] contains the maximum
        // and is in its final sorted position at the END
        // The sorted region has grown by one element (from RIGHT TO LEFT)
    }
    
    // After all passes, the array is completely sorted
}

/**
 * @brief Overloaded version for std::vector (more C++-idiomatic)
 * 
 * This version uses iterators and is more flexible. It works with any
 * container that supports random access iterators.
 * 
 * @param vec The vector to be sorted (modified in-place)
 */
void selectionSort(vector<int>& vec) {
    int n = vec.size();
    
    // Edge case handling
    if (n <= 1) {
        return;
    }
    
    // Same algorithm, but using vector's [] operator
    for (int pass = 0; pass < n - 1; pass++) {
        int indexOfLargest = 0;
        
        for (int i = 1; i < n - pass; i++) {
            if (vec[i] > vec[indexOfLargest]) {
                indexOfLargest = i;
            }
        }
        
        int rightmostUnsortedIndex = n - 1 - pass;
        if (indexOfLargest != rightmostUnsortedIndex) {
            swap(vec[indexOfLargest], vec[rightmostUnsortedIndex]);
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
 * @brief Demonstrates Selection Sort with detailed step-by-step output
 * 
 * This version prints the array after each pass to help students visualize
 * how the algorithm works.
 */
void selectionSortWithSteps(int arr[], int n) {
    if (n <= 1) {
        return;
    }
    
    cout << "\n=== Selection Sort Step-by-Step ===" << endl;
    cout << "Initial array: ";
    printArray(arr, n);
    cout << endl;
    
    for (int pass = 0; pass < n - 1; pass++) {
        int indexOfLargest = 0;
        
        // Find largest
        for (int i = 1; i < n - pass; i++) {
            if (arr[i] > arr[indexOfLargest]) {
                indexOfLargest = i;
            }
        }
        
        int rightmostUnsortedIndex = n - 1 - pass;
        
        cout << "Pass " << (pass + 1) << ": ";
        cout << "Largest in unsorted region is " << arr[indexOfLargest] 
             << " at index " << indexOfLargest;
        
        if (indexOfLargest != rightmostUnsortedIndex) {
            cout << " → swapping to END (index " << rightmostUnsortedIndex << ")";
            swap(arr[indexOfLargest], arr[rightmostUnsortedIndex]);
        } else {
            cout << " → already at END (correct position)";
        }
        
        cout << "\nAfter pass " << (pass + 1) << ": ";
        printArray(arr, n);
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
    cout << "  Selection Sort - Teaching Example" << endl;
    cout << "========================================\n" << endl;
    
    // Example 1: Basic sorting
    cout << "Example 1: Basic Selection Sort" << endl;
    cout << "--------------------------------" << endl;
    int arr1[] = {29, 10, 14, 37, 13};
    int n1 = sizeof(arr1) / sizeof(arr1[0]);
    
    cout << "Before sorting: ";
    printArray(arr1, n1);
    
    selectionSort(arr1, n1);
    
    cout << "After sorting:  ";
    printArray(arr1, n1);
    cout << endl;
    
    // Example 2: Step-by-step visualization
    cout << "Example 2: Step-by-Step Visualization" << endl;
    cout << "--------------------------------------" << endl;
    int arr2[] = {64, 34, 25, 12, 22, 11, 90};
    int n2 = sizeof(arr2) / sizeof(arr2[0]);
    selectionSortWithSteps(arr2, n2);
    
    // Example 3: Using vector
    cout << "Example 3: Using std::vector" << endl;
    cout << "----------------------------" << endl;
    vector<int> vec = {5, 2, 8, 1, 9, 3};
    cout << "Before sorting: ";
    printVector(vec);
    
    selectionSort(vec);
    
    cout << "After sorting:  ";
    printVector(vec);
    cout << endl;
    
    // Example 4: Edge cases
    cout << "Example 4: Edge Cases" << endl;
    cout << "---------------------" << endl;
    
    // Already sorted array
    int arr3[] = {1, 2, 3, 4, 5};
    int n3 = sizeof(arr3) / sizeof(arr3[0]);
    cout << "Already sorted array: ";
    printArray(arr3, n3);
    selectionSort(arr3, n3);
    cout << "After sorting: ";
    printArray(arr3, n3);
    cout << endl;
    
    // Reverse sorted array
    int arr4[] = {5, 4, 3, 2, 1};
    int n4 = sizeof(arr4) / sizeof(arr4[0]);
    cout << "Reverse sorted array: ";
    printArray(arr4, n4);
    selectionSort(arr4, n4);
    cout << "After sorting: ";
    printArray(arr4, n4);
    cout << endl;
    
    // Single element
    int arr5[] = {42};
    int n5 = 1;
    cout << "Single element: ";
    printArray(arr5, n5);
    selectionSort(arr5, n5);
    cout << "After sorting: ";
    printArray(arr5, n5);
    cout << endl;
    
    // Duplicate values
    int arr6[] = {3, 1, 3, 2, 1, 2};
    int n6 = sizeof(arr6) / sizeof(arr6[0]);
    cout << "Array with duplicates: ";
    printArray(arr6, n6);
    selectionSort(arr6, n6);
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
 * 1. ALGORITHM STRATEGY (MAXIMUM-TO-END):
 *    - Selection Sort uses a "selection" strategy: find the MAXIMUM element,
 *      then place it at the END (rightmost position)
 *    - Unlike Insertion Sort (which builds sorted region left-to-right),
 *      Selection Sort builds sorted region RIGHT-TO-LEFT
 *    - Key: We find MAXIMUM and swap to END, NOT minimum to front!
 * 
 * 2. TIME COMPLEXITY ANALYSIS:
 *    - Outer loop: (n-1) iterations
 *    - Inner loop: (n-1), (n-2), (n-3), ..., 1 iterations
 *    - Total comparisons: (n-1) + (n-2) + ... + 1 = n(n-1)/2 = O(n²)
 *    - This is true for BEST, AVERAGE, and WORST cases
 *    - Selection Sort always performs the same number of comparisons
 * 
 * 3. SPACE COMPLEXITY:
 *    - O(1) - only uses a few variables (pass, i, indexOfLargest)
 *    - Sorts "in-place" - doesn't need extra memory
 * 
 * 4. STABILITY:
 *    - Selection Sort is NOT stable
 *    - Example: [3₁, 3₂, 1] → after pass 1: [1, 3₂, 3₁]
 *    - The relative order of equal elements (3₁ and 3₂) changed
 * 
 * 5. WHEN TO USE:
 *    - Teaching/learning purposes (simple to understand)
 *    - Small datasets where O(n²) is acceptable
 *    - When minimizing writes is important (only n swaps maximum)
 *    - NOT recommended for large datasets or production code
 * 
 * 6. COMPARISON WITH OTHER SORTS:
 *    - vs Bubble Sort: Both O(n²), but Selection Sort typically faster
 *      because it does fewer swaps (at most n swaps vs potentially n² swaps)
 *    - vs Insertion Sort: Insertion Sort is better for nearly-sorted data
 *      (can be O(n)), Selection Sort is always O(n²)
 *    - vs Quick Sort/Merge Sort: Much slower for large n, but simpler to understand
 */
