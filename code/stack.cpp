/**
 * @file stack.cpp
 * @brief Implementation of Stack data structure — Array-backed and Linked-list-backed
 * @author CS 302 - Data Structures
 * @date 2026
 *
 * Stack: A linear data structure that follows the LIFO (Last In, First Out)
 * principle. Elements are added (pushed) and removed (popped) from the same
 * end, called the "top" of the stack.
 *
 * Two Implementations:
 * - Array-backed:  Fixed-size array, O(1) push/pop, simple but bounded capacity
 * - Linked-list:   Dynamic nodes, O(1) push/pop, unbounded but uses more memory
 *
 * Key Operations:
 * - push(val):  Add element to top        O(1)
 * - pop():      Remove and return top     O(1)
 * - peek():     View top without removal  O(1)
 * - isEmpty():  Check if stack is empty   O(1)
 *
 * Time Complexity: All operations O(1)
 * Space Complexity: O(n) for n elements stored
 *
 * Best for: Undo systems, expression parsing, DFS, call stack simulation,
 *           backtracking, balanced parentheses checking
 */

#include <iostream>
#include <string>
#include <cassert>

using namespace std;

// ============================================================================
// ARRAY-BACKED STACK
// ============================================================================

/**
 * @brief Stack implementation using a fixed-size array
 *
 * The array stores elements from index 0 upward. The variable topIdx tracks
 * the index of the current top element. When the stack is empty, topIdx = -1.
 *
 * Visual Example:
 *   After pushing 10, 20, 30:
 *
 *   arr:  [10, 20, 30, _, _, ...]
 *   index: 0   1   2   3  4
 *                   ^
 *                topIdx = 2
 *
 *   Top of stack is arr[topIdx] = 30
 *
 * Advantages: Simple, cache-friendly, no dynamic allocation
 * Disadvantages: Fixed capacity, wastes space if not full, overflow possible
 */
class ArrayStack {
    static const int MAX_SIZE = 100;
    int arr[MAX_SIZE];
    int topIdx;

public:
    /**
     * @brief Constructs an empty array-backed stack
     * @note topIdx = -1 indicates the stack is empty
     */
    ArrayStack() : topIdx(-1) {}

    /**
     * @brief Pushes a value onto the top of the stack
     *
     * Algorithm:
     * 1. Check if stack is full (topIdx == MAX_SIZE - 1)
     * 2. Increment topIdx
     * 3. Store value at arr[topIdx]
     *
     * Visual Example:
     *   Before: [10, 20, _, _]  topIdx = 1
     *   push(30):
     *     topIdx becomes 2
     *     arr[2] = 30
     *   After:  [10, 20, 30, _]  topIdx = 2
     *
     * @param val The value to push
     *
     * @note Prints error if stack is full (stack overflow)
     *
     * @complexity Time: O(1)
     * @complexity Space: O(1)
     */
    void push(int val) {
        if (isFull()) {
            cout << "Stack Overflow! Cannot push " << val << endl;
            return;
        }
        // Increment top index, then store the value
        topIdx++;
        arr[topIdx] = val;
    }

    /**
     * @brief Removes and returns the top element
     *
     * Algorithm:
     * 1. Check if stack is empty (topIdx == -1)
     * 2. Save the value at arr[topIdx]
     * 3. Decrement topIdx
     * 4. Return saved value
     *
     * Visual Example:
     *   Before: [10, 20, 30, _]  topIdx = 2
     *   pop():
     *     Save val = arr[2] = 30
     *     topIdx becomes 1
     *   After:  [10, 20, 30, _]  topIdx = 1
     *           (30 is still in array but logically removed)
     *
     * @return The value that was on top
     *
     * @note Prints error and returns -1 if stack is empty (stack underflow)
     *
     * @complexity Time: O(1)
     * @complexity Space: O(1)
     */
    int pop() {
        if (isEmpty()) {
            cout << "Stack Underflow! Cannot pop from empty stack" << endl;
            return -1;
        }
        // Save top value, then decrement index
        int val = arr[topIdx];
        topIdx--;
        return val;
    }

    /**
     * @brief Returns the top element without removing it
     *
     * @return The value at the top of the stack
     *
     * @note Prints error and returns -1 if stack is empty
     *
     * @complexity Time: O(1)
     * @complexity Space: O(1)
     */
    int peek() {
        if (isEmpty()) {
            cout << "Stack is empty! Nothing to peek" << endl;
            return -1;
        }
        return arr[topIdx];
    }

    /**
     * @brief Checks if the stack is empty
     * @return true if topIdx == -1 (no elements), false otherwise
     * @complexity Time: O(1)
     */
    bool isEmpty() {
        return topIdx == -1;
    }

    /**
     * @brief Checks if the stack is full (array capacity reached)
     * @return true if topIdx == MAX_SIZE - 1, false otherwise
     * @complexity Time: O(1)
     */
    bool isFull() {
        return topIdx == MAX_SIZE - 1;
    }

    /**
     * @brief Returns the number of elements in the stack
     * @return Number of elements (topIdx + 1)
     * @complexity Time: O(1)
     */
    int size() {
        return topIdx + 1;
    }

    /**
     * @brief Prints the stack contents from bottom to top
     *
     * @note Shows bottom and top labels for clarity
     *
     * @complexity Time: O(n)
     * @complexity Space: O(1)
     */
    void print() {
        if (isEmpty()) {
            cout << "(empty stack)" << endl;
            return;
        }
        cout << "Bottom [ ";
        for (int i = 0; i <= topIdx; i++) {
            cout << arr[i];
            if (i < topIdx) cout << ", ";
        }
        cout << " ] Top" << endl;
    }
};

// ============================================================================
// LINKED-LIST-BACKED STACK
// ============================================================================

/**
 * @brief Node for the linked-list-backed stack
 *
 * Each node stores a data value and a pointer to the next node below it
 * in the stack.
 */
struct StackNode {
    int data;
    StackNode* next;
};

/**
 * @brief Stack implementation using a singly linked list
 *
 * New elements are inserted at the HEAD of the list (the top of the stack).
 * Pop removes the head node. This gives O(1) for both push and pop.
 *
 * Visual Example:
 *   After pushing 10, 20, 30:
 *
 *   top → [30] → [20] → [10] → nullptr
 *          ^
 *        (top of stack)
 *
 *   pop() removes 30:
 *   top → [20] → [10] → nullptr
 *
 * Advantages: No fixed capacity, grows/shrinks dynamically, no wasted space
 * Disadvantages: Extra memory per node (pointer overhead), not cache-friendly
 */
class LinkedStack {
    StackNode* top;
    int count;

public:
    /**
     * @brief Constructs an empty linked-list stack
     * @note top = nullptr and count = 0 indicate empty stack
     */
    LinkedStack() : top(nullptr), count(0) {}

    /**
     * @brief Destructor — frees all remaining nodes
     *
     * Walks the list from top to bottom, deleting each node.
     * Prevents memory leaks when the stack goes out of scope.
     *
     * @complexity Time: O(n)
     * @complexity Space: O(1)
     */
    ~LinkedStack() {
        while (top != nullptr) {
            StackNode* temp = top;
            top = top->next;
            delete temp;
        }
    }

    /**
     * @brief Pushes a value onto the top of the stack
     *
     * Algorithm:
     * 1. Create a new node with the given value
     * 2. Set new node's next pointer to current top
     * 3. Update top to point to the new node
     * 4. Increment count
     *
     * Visual Example:
     *   Before: top → [20] → [10] → nullptr
     *   push(30):
     *     Create node [30]
     *     [30]->next = top (which points to [20])
     *     top = [30]
     *   After:  top → [30] → [20] → [10] → nullptr
     *
     * @param val The value to push
     *
     * @complexity Time: O(1)
     * @complexity Space: O(1) — one new node allocated
     */
    void push(int val) {
        // Create new node and link it to the current top
        StackNode* newNode = new StackNode();
        newNode->data = val;
        newNode->next = top;

        // New node becomes the top
        top = newNode;
        count++;
    }

    /**
     * @brief Removes and returns the top element
     *
     * Algorithm:
     * 1. Check if stack is empty
     * 2. Save the top node's data
     * 3. Move top to the next node
     * 4. Delete the old top node
     * 5. Decrement count and return saved value
     *
     * Visual Example:
     *   Before: top → [30] → [20] → [10] → nullptr
     *   pop():
     *     Save val = 30, temp = [30]
     *     top = [30]->next = [20]
     *     delete [30]
     *   After:  top → [20] → [10] → nullptr
     *   Return: 30
     *
     * @return The value that was on top
     *
     * @note Prints error and returns -1 if stack is empty
     *
     * @complexity Time: O(1)
     * @complexity Space: O(1)
     */
    int pop() {
        if (isEmpty()) {
            cout << "Stack Underflow! Cannot pop from empty stack" << endl;
            return -1;
        }
        // Save the top node and its value
        StackNode* temp = top;
        int val = top->data;

        // Move top to the next node
        top = top->next;

        // Free the old top node
        delete temp;
        count--;

        return val;
    }

    /**
     * @brief Returns the top element without removing it
     *
     * @return The value at the top of the stack
     *
     * @note Prints error and returns -1 if stack is empty
     *
     * @complexity Time: O(1)
     * @complexity Space: O(1)
     */
    int peek() {
        if (isEmpty()) {
            cout << "Stack is empty! Nothing to peek" << endl;
            return -1;
        }
        return top->data;
    }

    /**
     * @brief Checks if the stack is empty
     * @return true if top is nullptr (no elements), false otherwise
     * @complexity Time: O(1)
     */
    bool isEmpty() {
        return top == nullptr;
    }

    /**
     * @brief Returns the number of elements in the stack
     * @return Number of elements currently in the stack
     * @complexity Time: O(1)
     */
    int size() {
        return count;
    }

    /**
     * @brief Prints the stack contents from bottom to top
     *
     * Since the linked list goes from top to bottom, we need to traverse
     * the entire list and print in reverse order for a bottom-to-top view.
     *
     * @complexity Time: O(n)
     * @complexity Space: O(n) — uses temporary array to reverse order
     */
    void print() {
        if (isEmpty()) {
            cout << "(empty stack)" << endl;
            return;
        }
        // Collect values in an array to print bottom-to-top
        int vals[1000];
        int idx = 0;
        StackNode* curr = top;
        while (curr != nullptr) {
            vals[idx++] = curr->data;
            curr = curr->next;
        }
        cout << "Bottom [ ";
        for (int i = idx - 1; i >= 0; i--) {
            cout << vals[i];
            if (i > 0) cout << ", ";
        }
        cout << " ] Top" << endl;
    }
};

// ============================================================================
// STEP-BY-STEP PUSH (Array-backed, with trace output)
// ============================================================================

/**
 * @brief Pushes a value onto an ArrayStack with detailed step-by-step output
 *
 * Prints each step of the push operation to help students visualize
 * how the array and topIdx change.
 *
 * @param stack The ArrayStack to push onto
 * @param val The value to push
 *
 * @note For teaching purposes — shows index changes and array state
 *
 * @complexity Time: O(n) for printing, O(1) for the push itself
 * @complexity Space: O(1)
 */
void pushWithSteps(ArrayStack& stack, int val) {
    cout << "  Pushing " << val << "..." << endl;
    cout << "    Before: size = " << stack.size() << ", ";
    stack.print();

    if (stack.isFull()) {
        cout << "    Stack is FULL — cannot push (stack overflow)" << endl;
        return;
    }

    stack.push(val);
    cout << "    topIdx incremented to " << (stack.size() - 1) << endl;
    cout << "    Stored " << val << " at arr[" << (stack.size() - 1) << "]" << endl;
    cout << "    After:  size = " << stack.size() << ", ";
    stack.print();
}

// ============================================================================
// STEP-BY-STEP POP (Array-backed, with trace output)
// ============================================================================

/**
 * @brief Pops a value from an ArrayStack with detailed step-by-step output
 *
 * Prints each step of the pop operation to help students visualize
 * how the array and topIdx change.
 *
 * @param stack The ArrayStack to pop from
 *
 * @note For teaching purposes — shows index changes and value returned
 *
 * @complexity Time: O(n) for printing, O(1) for the pop itself
 * @complexity Space: O(1)
 */
void popWithSteps(ArrayStack& stack) {
    cout << "  Popping..." << endl;
    cout << "    Before: size = " << stack.size() << ", ";
    stack.print();

    if (stack.isEmpty()) {
        cout << "    Stack is EMPTY — cannot pop (stack underflow)" << endl;
        return;
    }

    int topBefore = stack.peek();
    int val = stack.pop();
    cout << "    Saved top value: " << topBefore << endl;
    cout << "    topIdx decremented to " << (stack.size() - 1) << endl;
    cout << "    Returned: " << val << endl;
    cout << "    After:  size = " << stack.size() << ", ";
    stack.print();
}

// ============================================================================
// APPLICATION: BALANCED PARENTHESES CHECKER
// ============================================================================

/**
 * @brief Checks if parentheses/brackets/braces in an expression are balanced
 *
 * Algorithm:
 * 1. Scan the expression left to right
 * 2. For each opening bracket ( [ { : push onto stack
 * 3. For each closing bracket ) ] } :
 *    a. If stack is empty → unbalanced (no matching opener)
 *    b. Pop the top; if it doesn't match the closer → unbalanced
 * 4. After scanning all characters, if stack is empty → balanced
 *
 * Visual Example:
 *   Expression: "{[()]}"
 *
 *   Char '{': push → stack: [{]
 *   Char '[': push → stack: [{, []
 *   Char '(': push → stack: [{, [, (]
 *   Char ')': pop '(' matches ')' → stack: [{, []
 *   Char ']': pop '[' matches ']' → stack: [{]
 *   Char '}': pop '{' matches '}' → stack: []
 *   Stack empty → BALANCED
 *
 * @param expr The string expression to check
 * @return true if all brackets are properly matched and nested, false otherwise
 *
 * @note Uses an ArrayStack internally
 * @note Only checks (, ), [, ], {, } — ignores other characters
 *
 * @complexity Time: O(n) — single pass through the string
 * @complexity Space: O(n) — worst case all characters are opening brackets
 */
bool checkParentheses(const string& expr) {
    ArrayStack stack;

    for (int i = 0; i < (int)expr.length(); i++) {
        char ch = expr[i];

        // Push opening brackets onto the stack
        if (ch == '(' || ch == '[' || ch == '{') {
            stack.push(ch);
        }
        // For closing brackets, check for matching opener
        else if (ch == ')' || ch == ']' || ch == '}') {
            // No matching opener — stack is empty
            if (stack.isEmpty()) {
                return false;
            }

            // Pop the top and check if it matches the current closer
            int top = stack.pop();
            bool match = (ch == ')' && top == '(') ||
                         (ch == ']' && top == '[') ||
                         (ch == '}' && top == '{');

            if (!match) {
                return false;
            }
        }
        // Other characters are ignored
    }

    // If stack is empty, all openers had matching closers
    return stack.isEmpty();
}

/**
 * @brief Checks parentheses with step-by-step output showing stack state
 *
 * Same algorithm as checkParentheses, but prints each step for teaching.
 *
 * @param expr The string expression to check
 * @return true if balanced, false otherwise
 *
 * @complexity Time: O(n)
 * @complexity Space: O(n)
 */
bool checkParenthesesWithSteps(const string& expr) {
    ArrayStack stack;

    cout << "  Checking: \"" << expr << "\"" << endl;

    for (int i = 0; i < (int)expr.length(); i++) {
        char ch = expr[i];

        if (ch == '(' || ch == '[' || ch == '{') {
            stack.push(ch);
            cout << "    Char '" << ch << "': push → stack: ";
            stack.print();
        } else if (ch == ')' || ch == ']' || ch == '}') {
            if (stack.isEmpty()) {
                cout << "    Char '" << ch << "': stack empty — no matching opener!" << endl;
                cout << "    Result: UNBALANCED" << endl;
                return false;
            }

            int top = stack.pop();
            bool match = (ch == ')' && top == '(') ||
                         (ch == ']' && top == '[') ||
                         (ch == '}' && top == '{');

            if (!match) {
                cout << "    Char '" << ch << "': popped '" << (char)top
                     << "' — MISMATCH!" << endl;
                cout << "    Result: UNBALANCED" << endl;
                return false;
            }

            cout << "    Char '" << ch << "': pop '" << (char)top
                 << "' matches → stack: ";
            stack.print();
        }
    }

    bool balanced = stack.isEmpty();
    if (balanced) {
        cout << "    Stack empty → BALANCED" << endl;
    } else {
        cout << "    Stack not empty — unmatched openers remain!" << endl;
        cout << "    Remaining: ";
        stack.print();
        cout << "    Result: UNBALANCED" << endl;
    }
    return balanced;
}

// ============================================================================
// MAIN FUNCTION - Demonstration and Testing
// ============================================================================

int main() {
    cout << "========================================" << endl;
    cout << "  Stack - Teaching Example" << endl;
    cout << "========================================\n" << endl;

    // Example 1: Array Stack — Push and Pop
    cout << "Example 1: Array Stack — Push and Pop" << endl;
    cout << "--------------------------------------" << endl;
    ArrayStack as;

    int values1[] = {10, 20, 30, 40, 50};
    int n1 = sizeof(values1) / sizeof(values1[0]);

    for (int i = 0; i < n1; i++) {
        as.push(values1[i]);
        cout << "After push(" << values1[i] << "): ";
        as.print();
    }
    cout << "Peek: " << as.peek() << endl;
    cout << "Size: " << as.size() << endl;

    cout << "\nPopping all elements (LIFO order):" << endl;
    while (!as.isEmpty()) {
        cout << "  pop() = " << as.pop() << "  →  ";
        as.print();
    }
    cout << endl;

    // Example 2: Linked Stack — Push and Pop
    cout << "Example 2: Linked Stack — Push and Pop" << endl;
    cout << "---------------------------------------" << endl;
    LinkedStack ls;

    int values2[] = {10, 20, 30, 40, 50};
    int n2 = sizeof(values2) / sizeof(values2[0]);

    for (int i = 0; i < n2; i++) {
        ls.push(values2[i]);
        cout << "After push(" << values2[i] << "): ";
        ls.print();
    }
    cout << "Peek: " << ls.peek() << endl;
    cout << "Size: " << ls.size() << endl;

    cout << "\nPopping all elements (LIFO order):" << endl;
    while (!ls.isEmpty()) {
        cout << "  pop() = " << ls.pop() << "  →  ";
        ls.print();
    }
    cout << endl;

    // Example 3: Step-by-Step Push (Array Stack)
    cout << "Example 3: Step-by-Step Push (Array Stack)" << endl;
    cout << "-------------------------------------------" << endl;
    ArrayStack as3;
    pushWithSteps(as3, 10);
    cout << endl;
    pushWithSteps(as3, 20);
    cout << endl;
    pushWithSteps(as3, 30);
    cout << endl;

    // Example 4: Step-by-Step Pop (Array Stack)
    cout << "Example 4: Step-by-Step Pop (Array Stack)" << endl;
    cout << "------------------------------------------" << endl;
    popWithSteps(as3);
    cout << endl;
    popWithSteps(as3);
    cout << endl;
    popWithSteps(as3);
    cout << endl;

    // Example 5: Balanced Parentheses (Valid)
    cout << "Example 5: Balanced Parentheses (Valid)" << endl;
    cout << "----------------------------------------" << endl;
    checkParenthesesWithSteps("{[()]}");
    cout << endl;
    checkParenthesesWithSteps("((a+b)*(c-d))");
    cout << endl;

    // Example 6: Unbalanced Parentheses
    cout << "Example 6: Unbalanced Parentheses" << endl;
    cout << "----------------------------------" << endl;
    checkParenthesesWithSteps("{[(])}");
    cout << endl;
    checkParenthesesWithSteps("((())");
    cout << endl;
    checkParenthesesWithSteps("))(");
    cout << endl;

    // Example 7: Comparison of Both Implementations
    cout << "Example 7: Comparison of Both Implementations" << endl;
    cout << "----------------------------------------------" << endl;
    ArrayStack arrStack;
    LinkedStack lnkStack;

    cout << "Pushing 100, 200, 300 onto both stacks:" << endl;
    int vals[] = {100, 200, 300};
    for (int i = 0; i < 3; i++) {
        arrStack.push(vals[i]);
        lnkStack.push(vals[i]);
    }
    cout << "  Array Stack:  ";
    arrStack.print();
    cout << "  Linked Stack: ";
    lnkStack.print();

    cout << "\nPopping from both:" << endl;
    cout << "  Array pop:  " << arrStack.pop() << endl;
    cout << "  Linked pop: " << lnkStack.pop() << endl;
    cout << "  Array Stack:  ";
    arrStack.print();
    cout << "  Linked Stack: ";
    lnkStack.print();
    cout << endl;

    // Example 8: Edge Cases
    cout << "Example 8: Edge Cases" << endl;
    cout << "---------------------" << endl;

    // Empty pop
    cout << "Popping from empty stack:" << endl;
    ArrayStack emptyStack;
    cout << "  Array:  pop() = " << emptyStack.pop() << endl;
    LinkedStack emptyLinked;
    cout << "  Linked: pop() = " << emptyLinked.pop() << endl;
    cout << endl;

    // Peeking empty stack
    cout << "Peeking at empty stack:" << endl;
    cout << "  Array:  peek() = " << emptyStack.peek() << endl;
    cout << "  Linked: peek() = " << emptyLinked.peek() << endl;
    cout << endl;

    // Push-pop-push pattern
    cout << "Push-pop-push pattern:" << endl;
    ArrayStack edgeStack;
    edgeStack.push(1);
    cout << "  push(1): ";
    edgeStack.print();
    cout << "  pop() = " << edgeStack.pop() << ": ";
    edgeStack.print();
    edgeStack.push(2);
    cout << "  push(2): ";
    edgeStack.print();
    edgeStack.push(3);
    cout << "  push(3): ";
    edgeStack.print();
    cout << endl;

    // Single element
    cout << "Single element stack:" << endl;
    ArrayStack singleStack;
    singleStack.push(42);
    cout << "  peek() = " << singleStack.peek() << endl;
    cout << "  pop() = " << singleStack.pop() << endl;
    cout << "  isEmpty() = " << (singleStack.isEmpty() ? "true" : "false") << endl;
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
 * 1. LIFO PRINCIPLE:
 *    - Last In, First Out — the most recently pushed element is popped first
 *    - Think of a stack of plates: you add and remove from the top only
 *    - This is the fundamental difference from a queue (FIFO)
 *    - All operations happen at ONE end (the top), unlike arrays where you
 *      can access any index
 *
 * 2. ARRAY VS LINKED-LIST IMPLEMENTATIONS:
 *    - Array-backed:
 *      → Fixed capacity (MAX_SIZE), simple indexing
 *      → topIdx tracks the top element's position
 *      → push: increment topIdx, store value
 *      → pop: read value, decrement topIdx
 *      → Cache-friendly (contiguous memory)
 *    - Linked-list-backed:
 *      → Dynamic size (no overflow unless out of memory)
 *      → top pointer points to the head node
 *      → push: create node, insert at head
 *      → pop: remove head node, return its data
 *      → Extra memory per element (pointer overhead)
 *
 * 3. AMORTIZED VS WORST-CASE:
 *    - Array stack with fixed size: all operations are O(1) worst case
 *    - Dynamic array (like std::vector): push is O(1) amortized
 *      → Occasionally needs to resize (copy all elements): O(n)
 *      → But this happens rarely enough that average cost is O(1)
 *    - Linked-list stack: all operations are O(1) worst case
 *      → But each push allocates memory (slower constant factor)
 *    - For exam purposes: both are O(1) per operation
 *
 * 4. STACK OVERFLOW AND UNDERFLOW:
 *    - Stack Overflow: pushing onto a full stack
 *      → Array stack: topIdx reaches MAX_SIZE - 1
 *      → Linked stack: only if system runs out of memory (rare)
 *      → The term "stack overflow" in programming often refers to the
 *        call stack exceeding its limit (e.g., infinite recursion)
 *    - Stack Underflow: popping from an empty stack
 *      → topIdx is already -1 (array) or top is nullptr (linked)
 *      → Must always check isEmpty() before popping
 *
 * 5. CONNECTION TO THE CALL STACK:
 *    - When a function is called, a stack frame is pushed onto the call stack
 *    - The frame contains: local variables, return address, parameters
 *    - When the function returns, its frame is popped
 *    - Recursion adds one frame per recursive call
 *    - Stack overflow in recursion = too many frames (too deep)
 *    - This is why the data structure is called a "stack" — same concept
 *
 * 6. APPLICATIONS:
 *    - Undo/Redo: push actions onto stack, pop to undo
 *    - Expression parsing: balanced brackets, infix-to-postfix conversion
 *    - DFS (Depth-First Search): use a stack instead of recursion
 *    - Browser back button: pages visited are pushed, back pops
 *    - Syntax checking in compilers: matching delimiters
 *    - Backtracking algorithms: maze solving, N-queens
 *
 * 7. POSTFIX EVALUATION:
 *    - Postfix (Reverse Polish): "3 4 + 5 *" means (3 + 4) * 5 = 35
 *    - Algorithm: scan left to right
 *      → Number: push onto stack
 *      → Operator: pop two operands, apply operator, push result
 *    - No parentheses needed — order of operations is implicit
 *    - Calculators (HP) and stack-based languages (Forth) use this
 *    - Related: infix-to-postfix conversion also uses a stack
 *
 * 8. ARRAY VS LINKED — WHEN TO CHOOSE:
 *    - Use array stack when:
 *      → Maximum size is known in advance
 *      → You need cache-friendly performance
 *      → Memory overhead per element matters
 *    - Use linked-list stack when:
 *      → Size is unpredictable or highly variable
 *      → You cannot afford a failed push (no overflow)
 *      → Memory fragmentation is not a concern
 *    - In practice: std::stack (C++ STL) uses deque by default
 *
 * 9. WHEN TO USE A STACK:
 *    - Any problem involving "last in, first out" ordering
 *    - Any problem where you process things in reverse order
 *    - Matching/nesting problems (parentheses, HTML tags)
 *    - Converting recursion to iteration (simulate the call stack)
 *    - Expression evaluation and conversion
 *    - NOT for: random access, searching, FIFO processing
 *
 * 10. COMMON MISTAKES:
 *    - Forgetting to check isEmpty() before pop/peek → underflow crash
 *    - Off-by-one with topIdx: it starts at -1, not 0
 *    - Memory leaks in linked stack: forgetting to delete nodes
 *    - Confusing stack (LIFO) with queue (FIFO)
 *    - Using a stack when a queue is needed (BFS needs a queue, not stack)
 *    - Forgetting that pop() REMOVES the element — use peek() to just look
 *    - In parentheses checking: forgetting to check if stack is empty at the END
 *      (catches case like "(()" where openers remain)
 */
