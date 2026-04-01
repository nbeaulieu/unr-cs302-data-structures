/**
 * @file recursion.cpp
 * @brief Recursion examples with step-by-step visualization and memoization
 * @author CS 302 - Data Structures
 * @date 2026
 *
 * Recursion: A function that calls itself to solve smaller subproblems.
 * Every recursive function needs a BASE CASE (when to stop) and a
 * RECURSIVE CASE (how to break the problem into smaller pieces).
 *
 * Three classic recursive algorithms are demonstrated:
 * - Factorial:      O(n) time, O(n) stack space
 * - Fibonacci:      O(2^n) naive, O(n) with memoization
 * - Binary Search:  O(log n) time, O(log n) stack space
 *
 * Key Insight: Recursion trades stack space for elegant, readable code.
 * Memoization can dramatically reduce redundant computation.
 *
 * Best for: Tree traversals, divide-and-conquer, backtracking, problems
 * with overlapping subproblems (when combined with memoization).
 */

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>  // for std::setw

using namespace std;

// ============================================================================
// HELPER FUNCTION
// ============================================================================

/**
 * @brief Prints indentation proportional to the recursion depth
 *
 * Used by the "WithSteps" functions to visually show the call stack depth.
 * Each level of depth adds two spaces plus a vertical bar, making it easy
 * to see which calls are nested inside which.
 *
 * @param depth The current recursion depth (0 = top level)
 *
 * @note Depth 0 prints no indentation
 * @note Each depth level adds "| " to show nesting
 */
void printIndent(int depth) {
    for (int i = 0; i < depth; i++) {
        cout << "| ";
    }
}

// ============================================================================
// FACTORIAL
// ============================================================================

/**
 * @brief Computes n! (n factorial) recursively
 *
 * Definition: n! = n * (n-1) * (n-2) * ... * 2 * 1, with 0! = 1
 *
 * Algorithm:
 *   Base case:      n <= 1 → return 1
 *   Recursive case: n > 1  → return n * factorial(n - 1)
 *
 * Call Chain Example (factorial(4)):
 *   factorial(4) = 4 * factorial(3)
 *                = 4 * (3 * factorial(2))
 *                = 4 * (3 * (2 * factorial(1)))
 *                = 4 * (3 * (2 * 1))            ← base case reached
 *                = 4 * (3 * 2)                   ← unwinding begins
 *                = 4 * 6
 *                = 24
 *
 * @param n The non-negative integer to compute factorial of
 * @return n! (n factorial)
 *
 * @note Base case: n <= 1 returns 1
 * @note Each recursive call reduces n by 1, guaranteeing termination
 * @note Stack depth is O(n) — will overflow for very large n
 *
 * @complexity Time: O(n) — exactly n recursive calls
 * @complexity Space: O(n) — n stack frames
 */
int factorial(int n) {
    // Base case: 0! = 1, 1! = 1
    if (n <= 1) {
        return 1;
    }

    // Recursive case: n! = n * (n-1)!
    return n * factorial(n - 1);
}

/**
 * @brief Computes factorial with indented call/return trace
 *
 * Prints each function call as it happens and each return value as the
 * recursion unwinds, with indentation showing the call stack depth.
 *
 * @param n The non-negative integer to compute factorial of
 * @param depth The current recursion depth (default 0 for initial call)
 * @return n! (n factorial)
 *
 * @note The depth parameter controls indentation — do not set manually
 * @note Shows both the "winding" (calls going deeper) and "unwinding" (returns)
 *
 * @complexity Time: O(n) — same as factorial, plus O(n) for printing
 * @complexity Space: O(n) — n stack frames
 */
int factorialWithSteps(int n, int depth = 0) {
    printIndent(depth);
    cout << "factorial(" << n << ")" << endl;

    // Base case
    if (n <= 1) {
        printIndent(depth);
        cout << "→ base case, return 1" << endl;
        return 1;
    }

    // Recursive case
    int result = n * factorialWithSteps(n - 1, depth + 1);

    printIndent(depth);
    cout << "→ " << n << " * factorial(" << n - 1 << ") = "
         << n << " * " << result / n << " = " << result << endl;

    return result;
}

// ============================================================================
// FIBONACCI
// ============================================================================

/**
 * @brief Computes the nth Fibonacci number using naive recursion
 *
 * Definition: F(0) = 0, F(1) = 1, F(n) = F(n-1) + F(n-2) for n >= 2
 * Sequence: 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, ...
 *
 * WARNING: This is intentionally inefficient to demonstrate why memoization
 * matters. The call tree grows exponentially because the same subproblems
 * are recomputed many times.
 *
 * Example call tree for fibonacci(5):
 *                         fib(5)
 *                        /      \
 *                   fib(4)       fib(3)
 *                  /     \       /    \
 *             fib(3)  fib(2)  fib(2)  fib(1)
 *            /    \    /   \   /   \
 *        fib(2) fib(1) f(1) f(0) f(1) f(0)
 *        /   \
 *      f(1)  f(0)
 *
 * Notice: fib(3) is computed 2 times, fib(2) is computed 3 times!
 *
 * @param n The index in the Fibonacci sequence (0-based)
 * @return The nth Fibonacci number
 *
 * @note Base cases: F(0) = 0, F(1) = 1
 * @note EXPONENTIAL time — do not use for large n
 * @note This exists for teaching purposes — use fibMemo for real work
 *
 * @complexity Time: O(2^n) — exponential due to redundant computation
 * @complexity Space: O(n) — maximum stack depth is n
 */
int fibonacci(int n) {
    // Base cases
    if (n <= 0) return 0;
    if (n == 1) return 1;

    // Recursive case: F(n) = F(n-1) + F(n-2)
    return fibonacci(n - 1) + fibonacci(n - 2);
}

/**
 * @brief Computes Fibonacci with indented call tree and call counter
 *
 * Shows every recursive call with indentation to visualize the full call
 * tree. Also counts total function calls to demonstrate the exponential
 * growth of naive recursion.
 *
 * @param n The index in the Fibonacci sequence
 * @param depth The current recursion depth (default 0)
 * @param callCount Reference to a counter tracking total function calls
 * @return The nth Fibonacci number
 *
 * @note callCount must be initialized to 0 before the first call
 * @note The indented output shows redundant calls clearly
 *
 * @complexity Time: O(2^n) — same as naive fibonacci
 * @complexity Space: O(n) — maximum stack depth
 */
int fibonacciWithSteps(int n, int depth, int& callCount) {
    callCount++;
    printIndent(depth);
    cout << "fib(" << n << ")" << endl;

    // Base cases
    if (n <= 0) {
        printIndent(depth);
        cout << "→ return 0" << endl;
        return 0;
    }
    if (n == 1) {
        printIndent(depth);
        cout << "→ return 1" << endl;
        return 1;
    }

    // Recursive case
    int left = fibonacciWithSteps(n - 1, depth + 1, callCount);
    int right = fibonacciWithSteps(n - 2, depth + 1, callCount);
    int result = left + right;

    printIndent(depth);
    cout << "→ fib(" << n - 1 << ") + fib(" << n - 2 << ") = "
         << left << " + " << right << " = " << result << endl;

    return result;
}

/**
 * @brief Computes Fibonacci with memoization — O(n) time
 *
 * Memoization stores previously computed results in a table (memo vector).
 * When a subproblem is encountered again, the cached result is returned
 * immediately instead of recomputing it.
 *
 * How it works:
 *   1. Check if memo[n] already has a value (not -1)
 *   2. If yes, return the cached result immediately — O(1)
 *   3. If no, compute it recursively, store in memo[n], then return
 *
 * Why this is O(n):
 *   - Each value fib(0) through fib(n) is computed exactly ONCE
 *   - All subsequent lookups are O(1) from the cache
 *   - Total work: n computations * O(1) each = O(n)
 *   - Compare to O(2^n) for naive recursion!
 *
 * @param n The index in the Fibonacci sequence
 * @param memo Vector of cached results, initialized to -1 (meaning "not computed")
 * @return The nth Fibonacci number
 *
 * @note memo must be sized to at least n+1 and filled with -1 before first call
 * @note After the call, memo[i] contains fib(i) for all 0 <= i <= n
 * @note Transforms exponential O(2^n) into linear O(n)
 *
 * @complexity Time: O(n) — each subproblem computed at most once
 * @complexity Space: O(n) — memo table + stack depth
 */
int fibMemo(int n, vector<int>& memo) {
    // Base cases
    if (n <= 0) return 0;
    if (n == 1) return 1;

    // Check cache — if already computed, return immediately
    if (memo[n] != -1) {
        return memo[n];
    }

    // Compute, store in cache, and return
    memo[n] = fibMemo(n - 1, memo) + fibMemo(n - 2, memo);
    return memo[n];
}

/**
 * @brief Computes memoized Fibonacci with step-by-step output showing cache hits
 *
 * Shows when a value is computed for the first time vs. retrieved from the
 * cache. This makes the power of memoization visible — most calls return
 * instantly from the cache.
 *
 * @param n The index in the Fibonacci sequence
 * @param memo Vector of cached results, initialized to -1
 * @param depth The current recursion depth (default 0)
 * @param callCount Reference to a counter tracking total function calls
 * @return The nth Fibonacci number
 *
 * @note Prints "CACHE HIT" when a previously computed value is reused
 * @note Compare callCount with naive version to see the dramatic reduction
 *
 * @complexity Time: O(n) — same as fibMemo, plus printing
 * @complexity Space: O(n) — memo table + stack depth
 */
int fibMemoWithSteps(int n, vector<int>& memo, int depth, int& callCount) {
    callCount++;
    printIndent(depth);
    cout << "fib(" << n << ")";

    // Base cases
    if (n <= 0) {
        cout << " → return 0" << endl;
        return 0;
    }
    if (n == 1) {
        cout << " → return 1" << endl;
        return 1;
    }

    // Cache hit — return stored result
    if (memo[n] != -1) {
        cout << " → CACHE HIT, return " << memo[n] << endl;
        return memo[n];
    }

    // Cache miss — compute recursively
    cout << " → computing..." << endl;
    int left = fibMemoWithSteps(n - 1, memo, depth + 1, callCount);
    int right = fibMemoWithSteps(n - 2, memo, depth + 1, callCount);
    memo[n] = left + right;

    printIndent(depth);
    cout << "→ fib(" << n << ") = " << left << " + " << right
         << " = " << memo[n] << " (cached)" << endl;

    return memo[n];
}

// ============================================================================
// BINARY SEARCH (RECURSIVE)
// ============================================================================

/**
 * @brief Searches a sorted array for a target value using recursive binary search
 *
 * Algorithm:
 *   1. Find the middle element of the current range [left, right]
 *   2. If middle element equals target, return its index
 *   3. If target < middle, search the left half
 *   4. If target > middle, search the right half
 *   5. If left > right, the element is not in the array
 *
 * Visual Example:
 *   Array: [2, 5, 8, 12, 16, 23, 38, 45, 67, 91]
 *   Target: 23
 *
 *   Step 1: left=0, right=9, mid=4 → arr[4]=16 < 23 → search right half
 *   Step 2: left=5, right=9, mid=7 → arr[7]=45 > 23 → search left half
 *   Step 3: left=5, right=6, mid=5 → arr[5]=23 = 23 → found at index 5!
 *
 * @param arr Sorted array to search
 * @param left Left boundary of current search range (inclusive)
 * @param right Right boundary of current search range (inclusive)
 * @param target The value to search for
 * @return Index of target if found, -1 if not found
 *
 * @note The array MUST be sorted in ascending order
 * @note Each recursive call halves the search space
 * @note Stack depth is O(log n) — very efficient
 *
 * @complexity Time: O(log n) — search space halves each step
 * @complexity Space: O(log n) — recursion stack depth
 */
int binarySearchRecursive(int arr[], int left, int right, int target) {
    // Base case: search space is empty
    if (left > right) {
        return -1;  // Not found
    }

    // Find the middle index (avoids integer overflow with this formula)
    int mid = left + (right - left) / 2;

    // Found the target
    if (arr[mid] == target) {
        return mid;
    }

    // Target is smaller — search left half
    if (target < arr[mid]) {
        return binarySearchRecursive(arr, left, mid - 1, target);
    }

    // Target is larger — search right half
    return binarySearchRecursive(arr, mid + 1, right, target);
}

/**
 * @brief Recursive binary search with step-by-step output showing range narrowing
 *
 * Prints the current search range, the middle element, and the decision
 * at each step. Makes it easy to see how the search space shrinks by half
 * with each recursive call.
 *
 * @param arr Sorted array to search
 * @param left Left boundary of current search range
 * @param right Right boundary of current search range
 * @param target The value to search for
 * @param depth The current recursion depth (default 0)
 * @return Index of target if found, -1 if not found
 *
 * @note Shows the shrinking range at each level of recursion
 * @note Demonstrates O(log n) behavior visually
 *
 * @complexity Time: O(log n) — same as regular binary search, plus printing
 * @complexity Space: O(log n) — recursion stack depth
 */
int binarySearchWithSteps(int arr[], int left, int right, int target, int depth = 0) {
    printIndent(depth);

    // Base case: search space is empty
    if (left > right) {
        cout << "left=" << left << " > right=" << right
             << " → NOT FOUND" << endl;
        return -1;
    }

    int mid = left + (right - left) / 2;

    // Print current state
    cout << "search [" << left << ".." << right << "], mid=" << mid
         << ", arr[" << mid << "]=" << arr[mid];

    if (arr[mid] == target) {
        cout << " == " << target << " → FOUND at index " << mid << endl;
        return mid;
    } else if (target < arr[mid]) {
        cout << " > " << target << " → search LEFT [" << left << ".." << mid - 1 << "]" << endl;
        return binarySearchWithSteps(arr, left, mid - 1, target, depth + 1);
    } else {
        cout << " < " << target << " → search RIGHT [" << mid + 1 << ".." << right << "]" << endl;
        return binarySearchWithSteps(arr, mid + 1, right, target, depth + 1);
    }
}

// ============================================================================
// MAIN FUNCTION - Demonstration and Testing
// ============================================================================

int main() {
    cout << "========================================" << endl;
    cout << "  Recursion - Teaching Examples" << endl;
    cout << "========================================\n" << endl;

    // ---- Example 1: Factorial Basic ----
    cout << "Example 1: Factorial (Basic)" << endl;
    cout << "----------------------------" << endl;
    cout << "factorial(5) = " << factorial(5) << endl;
    cout << "factorial(0) = " << factorial(0) << endl;
    cout << "factorial(1) = " << factorial(1) << endl;
    cout << "factorial(10) = " << factorial(10) << endl;
    cout << endl;

    // ---- Example 2: Factorial Step-by-Step ----
    cout << "Example 2: Factorial Step-by-Step" << endl;
    cout << "---------------------------------" << endl;
    cout << "Tracing factorial(5):\n" << endl;
    int result2 = factorialWithSteps(5);
    cout << "\nResult: " << result2 << endl;
    cout << endl;

    // ---- Example 3: Fibonacci Naive ----
    cout << "Example 3: Fibonacci (Naive Recursion)" << endl;
    cout << "---------------------------------------" << endl;
    cout << "First 8 Fibonacci numbers:" << endl;
    for (int i = 0; i <= 7; i++) {
        cout << "  fib(" << i << ") = " << fibonacci(i) << endl;
    }
    cout << "\nfib(7) = " << fibonacci(7) << endl;
    cout << "(This required many redundant calls — see Example 4)" << endl;
    cout << endl;

    // ---- Example 4: Fibonacci Step-by-Step ----
    cout << "Example 4: Fibonacci Step-by-Step (showing redundant calls)" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "Tracing fib(5) — watch for repeated subproblems:\n" << endl;
    int callCount4 = 0;
    int result4 = fibonacciWithSteps(5, 0, callCount4);
    cout << "\nResult: fib(5) = " << result4 << endl;
    cout << "Total function calls: " << callCount4 << endl;
    cout << "(Many calls are redundant — fib(2) alone is computed 3 times!)" << endl;
    cout << endl;

    // ---- Example 5: Fibonacci Memoized ----
    cout << "Example 5: Fibonacci with Memoization" << endl;
    cout << "--------------------------------------" << endl;
    int n5 = 10;
    vector<int> memo5(n5 + 1, -1);
    cout << "fibMemo(" << n5 << ") = " << fibMemo(n5, memo5) << endl;
    cout << "\nCached values after computation:" << endl;
    for (int i = 0; i <= n5; i++) {
        if (memo5[i] != -1) {
            cout << "  memo[" << i << "] = " << memo5[i] << endl;
        }
    }
    cout << "(Each value computed exactly once and stored for reuse)" << endl;
    cout << endl;

    // ---- Example 6: Fibonacci Memoized Step-by-Step ----
    cout << "Example 6: Fibonacci Memoized Step-by-Step (showing cache hits)" << endl;
    cout << "---------------------------------------------------------------" << endl;
    int n6 = 7;
    vector<int> memo6(n6 + 1, -1);
    int callCount6 = 0;
    cout << "Tracing fibMemo(" << n6 << "):\n" << endl;
    int result6 = fibMemoWithSteps(n6, memo6, 0, callCount6);
    cout << "\nResult: fib(" << n6 << ") = " << result6 << endl;
    cout << "Total function calls (memoized): " << callCount6 << endl;
    cout << "(Compare this to the exponential calls without memoization!)" << endl;
    cout << endl;

    // ---- Example 7: Binary Search Step-by-Step ----
    cout << "Example 7: Binary Search Step-by-Step" << endl;
    cout << "--------------------------------------" << endl;
    int arr7[] = {2, 5, 8, 12, 16, 23, 38, 45, 67, 91};
    int size7 = sizeof(arr7) / sizeof(arr7[0]);

    cout << "Array: [";
    for (int i = 0; i < size7; i++) {
        cout << arr7[i];
        if (i < size7 - 1) cout << ", ";
    }
    cout << "]" << endl;

    cout << "\nSearching for 23:" << endl;
    int idx1 = binarySearchWithSteps(arr7, 0, size7 - 1, 23);
    cout << "Result: index " << idx1 << endl;

    cout << "\nSearching for 67:" << endl;
    int idx2 = binarySearchWithSteps(arr7, 0, size7 - 1, 67);
    cout << "Result: index " << idx2 << endl;

    cout << "\nSearching for 10 (not in array):" << endl;
    int idx3 = binarySearchWithSteps(arr7, 0, size7 - 1, 10);
    cout << "Result: " << idx3 << " (not found)" << endl;
    cout << endl;

    // ---- Example 8: Naive vs Memoized Call Count Comparison ----
    cout << "Example 8: Naive vs Memoized Fibonacci — Call Count Comparison" << endl;
    cout << "--------------------------------------------------------------" << endl;
    cout << setw(6) << "n" << setw(18) << "Naive Calls" << setw(18) << "Memo Calls"
         << setw(16) << "Speedup" << endl;
    cout << string(58, '-') << endl;

    int testValues[] = {10, 15, 20};
    for (int i = 0; i < 3; i++) {
        int n = testValues[i];

        // Count naive calls using the recurrence:
        // calls(n) = calls(n-1) + calls(n-2) + 1, calls(0) = calls(1) = 1
        // This avoids actually running the exponential recursion for large n
        vector<int> naiveCalls(n + 1, 0);
        naiveCalls[0] = 1;
        if (n >= 1) naiveCalls[1] = 1;
        for (int j = 2; j <= n; j++) {
            naiveCalls[j] = naiveCalls[j - 1] + naiveCalls[j - 2] + 1;
        }
        int naiveCount = naiveCalls[n];

        // Memoized calls: each fib(k) for k=2..n is computed once (n-1 compute calls),
        // each triggers one cache hit on fib(k-2), plus base cases = 2n - 1 total
        int memoCount = (n >= 2) ? (2 * n - 1) : 1;

        cout << setw(6) << n
             << setw(18) << naiveCount
             << setw(18) << memoCount
             << setw(15) << naiveCount / memoCount << "x" << endl;
    }
    cout << "\nThe naive approach grows EXPONENTIALLY while memoized grows LINEARLY." << endl;
    cout << "For n=20, naive makes ~21,891 calls vs ~39 for memoized!" << endl;
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
 * 1. BASE CASE:
 *    - Every recursive function MUST have at least one base case
 *    - The base case is when the function returns WITHOUT calling itself
 *    - Without a base case, recursion never stops → stack overflow
 *    - Examples: factorial(0) = 1, fib(0) = 0, fib(1) = 1, empty search range
 *    - Think of it as the "simplest version" of the problem that you can
 *      answer directly without further decomposition
 *
 * 2. RECURSIVE CASE:
 *    - The recursive case MUST make progress toward the base case
 *    - Each recursive call should work on a SMALLER subproblem
 *    - factorial(n) calls factorial(n-1) — n decreases by 1 each time
 *    - binarySearch halves the range — problem size cuts in half each time
 *    - If you don't make progress, you get infinite recursion
 *
 * 3. CALL STACK:
 *    - Each recursive call adds a new frame to the system call stack
 *    - The frame stores: local variables, parameters, return address
 *    - Frames are removed (popped) when the function returns
 *    - Stack has a limited size — too many frames causes STACK OVERFLOW
 *    - factorial(100000) would likely crash due to stack overflow
 *    - Default stack size is typically 1-8 MB depending on the system
 *    - This is why iterative solutions sometimes preferred for deep recursion
 *
 * 4. TIME COMPLEXITY:
 *    - Factorial: O(n) — one recursive call per level, n levels
 *    - Naive Fibonacci: O(2^n) — two calls per level, branching tree
 *      The exact number of calls is actually fib(n+1)*2 - 1
 *    - Memoized Fibonacci: O(n) — each subproblem computed once
 *    - Binary Search: O(log n) — problem halves each step
 *    - The BRANCHING FACTOR determines growth rate:
 *      → 1 branch (factorial): linear
 *      → 2 branches without reuse (fib): exponential
 *      → 1 branch halving (binary search): logarithmic
 *
 * 5. SPACE COMPLEXITY:
 *    - Stack depth determines space usage for recursion
 *    - Factorial: O(n) stack frames — one per call from n down to 1
 *    - Fibonacci: O(n) stack depth — deepest path is n levels
 *      (even though there are 2^n total calls, at most n are active at once)
 *    - Binary Search: O(log n) stack frames — halving means log n levels
 *    - Memoized Fibonacci: O(n) stack + O(n) memo table = O(n) total
 *
 * 6. MEMOIZATION:
 *    - Technique: cache results of expensive function calls
 *    - Before computing, check if the result is already in the cache
 *    - If yes → return cached result (O(1) lookup)
 *    - If no → compute, store in cache, then return
 *    - Transforms Fibonacci from O(2^n) to O(n) — dramatic improvement
 *    - Trade-off: uses O(n) extra memory for the cache
 *    - This is the core idea behind DYNAMIC PROGRAMMING (bottom-up variant
 *      fills the table iteratively instead of recursively)
 *
 * 7. REDUNDANT COMPUTATION:
 *    - Naive Fibonacci recomputes the same values many, many times
 *    - fib(5) computes fib(3) twice, fib(2) three times, fib(1) five times
 *    - This is called "overlapping subproblems"
 *    - The total number of calls grows like the Fibonacci sequence itself!
 *    - For n=20: naive makes ~21,891 calls; memoized makes ~39
 *    - For n=40: naive makes ~331 million calls; memoized makes ~79
 *    - Recognizing overlapping subproblems is key to optimization
 *
 * 8. TAIL RECURSION:
 *    - A function is tail-recursive if the recursive call is the LAST operation
 *    - Example (tail recursive): return factorial_helper(n-1, n*acc)
 *    - Example (NOT tail recursive): return n * factorial(n-1)
 *      → multiplication happens AFTER the recursive call returns
 *    - Some compilers optimize tail recursion into a loop (no extra stack frames)
 *    - This is called Tail Call Optimization (TCO)
 *    - C++ does NOT guarantee TCO, but some compilers (gcc -O2) may apply it
 *    - Functional languages (Scheme, Haskell) guarantee TCO
 *
 * 9. RECURSION VS ITERATION:
 *    - Every recursive algorithm CAN be converted to an iterative one
 *    - Iteration uses a loop; recursion uses the call stack
 *    - Recursion is often more elegant and easier to reason about
 *    - Iteration avoids stack overflow risk and is often faster in practice
 *    - Some problems are naturally recursive (trees, divide-and-conquer)
 *    - Some are naturally iterative (simple counting, linear scans)
 *    - Example: factorial is trivially iterative; tree traversal is naturally recursive
 *    - Rule of thumb: use recursion when the problem has recursive structure
 *
 * 10. COMMON MISTAKES:
 *    - MISSING BASE CASE: causes infinite recursion → stack overflow
 *    - NOT PROGRESSING: recursive call doesn't reduce the problem
 *      e.g., factorial(n) calling factorial(n) instead of factorial(n-1)
 *    - OFF-BY-ONE: wrong base case value leads to incorrect results
 *      e.g., factorial stopping at n==0 returning 0 instead of 1
 *    - IGNORING STACK LIMITS: deep recursion on large inputs crashes
 *    - NOT CONSIDERING MEMOIZATION: leaving exponential algorithms exponential
 *      when overlapping subproblems exist
 *    - MODIFYING SHARED STATE: accidentally changing a global/reference
 *      variable that other recursive calls depend on
 */
