# Swap vs Shift: A Teaching Comparison

This document explains the key difference between **swaps** (used in Selection Sort and Bubble Sort) and **shifts** (used in Insertion Sort).

## Visual Comparison

### SWAP (Selection Sort, Bubble Sort)

**What it does:** Exchanges two values

```
Before:  [a, b]
After:   [b, a]

Example: [10, 20] → swap → [20, 10]
```

**Code:**
```cpp
void swapElements(int& a, int& b) {
    int temp = a;  // Save a
    a = b;         // Copy b to a
    b = temp;      // Copy original a to b
}
```

**When used:**
- Selection Sort: Swap largest element to end
- Bubble Sort: Swap adjacent pairs if out of order

**Characteristics:**
- Exchanges two elements
- Both positions get new values
- Symmetric operation (a ↔ b)

---

### SHIFT (Insertion Sort)

**What it does:** Moves one element to make space

```
Before:  [a, b, _, c]
After:   [a, _, b, c]

Example: [10, 20, _, 30] → shift 20 right → [10, _, 20, 30]
         (gap created at position 1 for insertion)
```

**Code:**
```cpp
void shiftRight(int arr[], int from, int to) {
    arr[to] = arr[from];  // Move element from 'from' to 'to'
    // Leaves 'from' position available (gap)
}
```

**When used:**
- Insertion Sort: Shift sorted elements right to make space for insertion

**Characteristics:**
- Moves one element
- Creates a gap (empty position)
- Asymmetric operation (a → b, leaves gap at a)

---

## Key Differences

| Aspect | SWAP | SHIFT |
|--------|------|-------|
| **Operation** | Exchange two values | Move one value |
| **Result** | Both positions change | One position becomes gap |
| **Purpose** | Reorder elements | Make space for insertion |
| **Used in** | Selection Sort, Bubble Sort | Insertion Sort |
| **Complexity** | O(1) per swap | O(1) per shift |
| **Visual** | a ↔ b | a → b (gap at a) |

## Why Does This Matter?

### Selection Sort & Bubble Sort Use Swaps
- Need to **exchange** elements to put them in correct positions
- Example: Find largest (37), swap with end position
- Both elements change positions

### Insertion Sort Uses Shifts
- Need to **make space** for inserting a new element
- Example: Insert 13, shift larger elements right to create gap
- Only one element moves, creating space for insertion

## Teaching Example

### Selection Sort (Swap):
```
Pass 1: [29, 10, 14, 37, 13]
        Find largest: 37 at index 3
        Swap 37 ↔ 13
        Result: [29, 10, 14, 13, 37]
        Both 37 and 13 changed positions
```

### Insertion Sort (Shift):
```
Insert 13: [10, 14, 29, 37, _]
           Save 13, compare with 37: 13 < 37
           Shift 37 → right: [10, 14, 29, _, 37]
           Compare with 29: 13 < 29
           Shift 29 → right: [10, 14, _, 29, 37]
           Compare with 14: 13 < 14
           Shift 14 → right: [10, _, 14, 29, 37]
           Compare with 10: 13 > 10 → stop
           Insert 13 at gap: [10, 13, 14, 29, 37]
           Elements shifted right, gap created for insertion
```

## Why Shifts Are More Efficient for Nearly-Sorted Data

- **Swaps**: Always exchange two elements, even if one is already correct
- **Shifts**: Only move elements that need to move, stop early when correct position found
- **Result**: Insertion Sort can be O(n) for nearly-sorted data, while Selection Sort is always O(n²)

## Code Comparison

### Selection Sort (Swap):
```cpp
// Find largest, then swap to end
if (indexOfLargest != rightmostUnsortedIndex) {
    swapElements(arr[indexOfLargest], arr[rightmostUnsortedIndex]);
    // Both arr[indexOfLargest] and arr[rightmostUnsortedIndex] change
}
```

### Insertion Sort (Shift):
```cpp
// Shift elements right to make space
while (j >= 0 && arr[j] > valueToInsert) {
    shiftRight(arr, j, j + 1);  // Move arr[j] to arr[j+1]
    j--;  // Check next element
}
// Then insert valueToInsert at the gap
arr[j + 1] = valueToInsert;
```

---

**Key Takeaway:** Swaps exchange, shifts make space. Understanding this difference helps explain why Insertion Sort behaves differently than Selection/Bubble Sort!
