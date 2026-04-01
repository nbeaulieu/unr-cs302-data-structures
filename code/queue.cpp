/**
 * @file queue.cpp
 * @brief Implementation of Queue data structure — Circular Array and Linked-list-backed
 * @author CS 302 - Data Structures
 * @date 2026
 *
 * Queue: A linear data structure that follows the FIFO (First In, First Out)
 * principle. Elements are added (enqueued) at the rear and removed (dequeued)
 * from the front.
 *
 * Two Implementations:
 * - Circular array:  Fixed-size array with wrap-around, O(1) enqueue/dequeue
 * - Linked-list:     Dynamic nodes, O(1) enqueue/dequeue, unbounded
 *
 * Key Operations:
 * - enqueue(val):  Add element to rear     O(1)
 * - dequeue():     Remove from front       O(1)
 * - peek():        View front element      O(1)
 * - isEmpty():     Check if queue is empty O(1)
 *
 * Circular Array Trick:
 * - Use modular arithmetic to wrap indices around: nextIndex = (index + 1) % capacity
 * - This avoids the problem of "wasted space" at the front of a linear array
 *
 * Time Complexity: All operations O(1)
 * Space Complexity: O(n) for n elements stored
 *
 * Best for: BFS, scheduling, buffering, producer-consumer, print queues
 */

#include <iostream>
#include <cassert>

using namespace std;

// ============================================================================
// CIRCULAR ARRAY QUEUE
// ============================================================================

/**
 * @brief Queue implementation using a circular (ring) array
 *
 * The array is treated as a circle using modular arithmetic. The front index
 * points to the first element, and the rear index points to the last element.
 * When indices reach the end of the array, they wrap around to index 0.
 *
 * Visual Example (capacity = 8):
 *   After enqueue(10), enqueue(20), enqueue(30):
 *
 *   Index:  0    1    2    3    4    5    6    7
 *   arr:  [10] [20] [30] [ ] [ ] [ ] [ ] [ ]
 *           ^         ^
 *         front     rear
 *         (0)       (2)
 *   count = 3
 *
 *   After dequeue() removes 10, dequeue() removes 20:
 *
 *   Index:  0    1    2    3    4    5    6    7
 *   arr:  [10] [20] [30] [ ] [ ] [ ] [ ] [ ]
 *                     ^
 *                  front=rear
 *                    (2)
 *   count = 1
 *   (indices 0,1 are "logically empty" even though data remains)
 *
 *   After enqueue(40), enqueue(50), ..., enqueue(80) — wrap-around:
 *
 *   Index:  0    1    2    3    4    5    6    7
 *   arr:  [80] [20] [30] [40] [50] [60] [70] [80]
 *           ^    ^
 *         rear  front... wait, let's show a cleaner example below
 *
 * Why circular? Without wrap-around, dequeue would either:
 *   - Shift all elements left: O(n) per dequeue — too slow
 *   - Leave "holes" at the front: wasted space, eventually runs out
 * Circular array solves both problems with O(1) operations.
 *
 * Advantages: Fixed memory, cache-friendly, simple with modular arithmetic
 * Disadvantages: Fixed capacity, must handle full/empty with count or gap
 */
class CircularArrayQueue {
    static const int DEFAULT_CAPACITY = 8;
    int arr[DEFAULT_CAPACITY];
    int front;     // Index of the front element
    int rear;      // Index of the rear element
    int count;     // Number of elements currently in the queue
    int capacity;  // Maximum capacity

public:
    /**
     * @brief Constructs an empty circular array queue
     *
     * @param cap The capacity of the queue (default 8)
     *
     * @note front = 0, rear = -1, count = 0 indicates empty queue
     */
    CircularArrayQueue(int cap = DEFAULT_CAPACITY)
        : front(0), rear(-1), count(0), capacity(cap) {}

    /**
     * @brief Adds an element to the rear of the queue
     *
     * Algorithm:
     * 1. Check if queue is full
     * 2. Advance rear using modular arithmetic: rear = (rear + 1) % capacity
     * 3. Store value at arr[rear]
     * 4. Increment count
     *
     * The modular arithmetic makes the array circular:
     *   If rear is at the last index and we enqueue, rear wraps to 0
     *
     * Visual Example (capacity = 4):
     *   Before: front=2, rear=3, count=2
     *   Index: [_] [_] [30] [40]
     *                   ^     ^
     *                 front  rear
     *
     *   enqueue(50):
     *     rear = (3 + 1) % 4 = 0   ← WRAPS AROUND to index 0
     *     arr[0] = 50
     *     count = 3
     *
     *   After:
     *   Index: [50] [_] [30] [40]
     *           ^        ^
     *          rear    front
     *
     * @param val The value to enqueue
     *
     * @note Prints error if queue is full
     *
     * @complexity Time: O(1)
     * @complexity Space: O(1)
     */
    void enqueue(int val) {
        if (isFull()) {
            cout << "Queue is Full! Cannot enqueue " << val << endl;
            return;
        }
        // Advance rear with wrap-around using modular arithmetic
        rear = (rear + 1) % capacity;
        arr[rear] = val;
        count++;
    }

    /**
     * @brief Removes and returns the front element
     *
     * Algorithm:
     * 1. Check if queue is empty
     * 2. Save value at arr[front]
     * 3. Advance front using modular arithmetic: front = (front + 1) % capacity
     * 4. Decrement count
     * 5. Return saved value
     *
     * Visual Example (capacity = 4):
     *   Before: front=2, rear=0, count=3
     *   Index: [50] [_] [30] [40]
     *           ^        ^
     *          rear    front
     *
     *   dequeue():
     *     Save val = arr[2] = 30
     *     front = (2 + 1) % 4 = 3
     *     count = 2
     *
     *   After:
     *   Index: [50] [_] [30] [40]
     *           ^              ^
     *          rear          front
     *
     * @return The value that was at the front
     *
     * @note Prints error and returns -1 if queue is empty
     *
     * @complexity Time: O(1)
     * @complexity Space: O(1)
     */
    int dequeue() {
        if (isEmpty()) {
            cout << "Queue is Empty! Cannot dequeue" << endl;
            return -1;
        }
        // Save front value, then advance front with wrap-around
        int val = arr[front];
        front = (front + 1) % capacity;
        count--;
        return val;
    }

    /**
     * @brief Returns the front element without removing it
     *
     * @return The value at the front of the queue
     *
     * @note Prints error and returns -1 if queue is empty
     *
     * @complexity Time: O(1)
     * @complexity Space: O(1)
     */
    int peek() {
        if (isEmpty()) {
            cout << "Queue is Empty! Nothing to peek" << endl;
            return -1;
        }
        return arr[front];
    }

    /**
     * @brief Checks if the queue is empty
     * @return true if count == 0, false otherwise
     * @complexity Time: O(1)
     */
    bool isEmpty() {
        return count == 0;
    }

    /**
     * @brief Checks if the queue is full
     * @return true if count == capacity, false otherwise
     * @complexity Time: O(1)
     */
    bool isFull() {
        return count == capacity;
    }

    /**
     * @brief Returns the number of elements in the queue
     * @return Number of elements
     * @complexity Time: O(1)
     */
    int size() {
        return count;
    }

    /**
     * @brief Prints the queue showing the circular array layout
     *
     * Shows the raw array with front (F) and rear (R) markers, plus
     * the logical queue order from front to rear.
     *
     * @complexity Time: O(n)
     * @complexity Space: O(1)
     */
    void print() {
        if (isEmpty()) {
            cout << "(empty queue)" << endl;
            return;
        }

        // Print logical order: front to rear
        cout << "Front [ ";
        int idx = front;
        for (int i = 0; i < count; i++) {
            cout << arr[idx];
            if (i < count - 1) cout << ", ";
            idx = (idx + 1) % capacity;
        }
        cout << " ] Rear" << endl;
    }

    /**
     * @brief Prints the raw circular array with index markers
     *
     * Shows every slot in the array, marking front (F) and rear (R).
     * Helps visualize wrap-around behavior.
     *
     * Visual Example:
     *   Index:   0    1    2    3    4    5    6    7
     *   Array: [ 50] [  ] [  ] [ 30] [ 40] [  ] [  ] [  ]
     *            R              F
     *
     * @complexity Time: O(capacity)
     * @complexity Space: O(1)
     */
    void printRaw() {
        cout << "  Index: ";
        for (int i = 0; i < capacity; i++) {
            cout << " " << i << "   ";
        }
        cout << endl;

        cout << "  Array: ";
        for (int i = 0; i < capacity; i++) {
            // Check if this index is in the logical range of the queue
            bool occupied = false;
            int idx = front;
            for (int j = 0; j < count; j++) {
                if (idx == i) {
                    occupied = true;
                    break;
                }
                idx = (idx + 1) % capacity;
            }

            if (occupied) {
                cout << "[";
                if (arr[i] < 10) cout << " ";
                cout << arr[i] << "] ";
            } else {
                cout << "[  ] ";
            }
        }
        cout << endl;

        // Print front/rear markers
        cout << "         ";
        for (int i = 0; i < capacity; i++) {
            if (count > 0 && i == front && i == rear) {
                cout << " F=R ";
            } else if (count > 0 && i == front) {
                cout << "  F  ";
            } else if (count > 0 && i == rear) {
                cout << "  R  ";
            } else {
                cout << "     ";
            }
        }
        cout << endl;
    }
};

// ============================================================================
// LINKED-LIST QUEUE
// ============================================================================

/**
 * @brief Node for the linked-list-backed queue
 *
 * Each node stores a data value and a pointer to the next node (toward rear).
 */
struct QueueNode {
    int data;
    QueueNode* next;
};

/**
 * @brief Queue implementation using a singly linked list
 *
 * Front pointer points to the first node (for dequeue).
 * Rear pointer points to the last node (for enqueue).
 * New nodes are added at the rear; nodes are removed from the front.
 *
 * Visual Example:
 *   After enqueue(10), enqueue(20), enqueue(30):
 *
 *   front → [10] → [20] → [30] → nullptr
 *                            ^
 *                           rear
 *
 *   dequeue() removes 10:
 *   front → [20] → [30] → nullptr
 *                    ^
 *                   rear
 *
 * Advantages: No fixed capacity, grows/shrinks dynamically, no wrap-around needed
 * Disadvantages: Extra memory per node (pointer overhead), not cache-friendly
 */
class LinkedQueue {
    QueueNode* front;
    QueueNode* rear;
    int count;

public:
    /**
     * @brief Constructs an empty linked-list queue
     * @note Both front and rear are nullptr when empty
     */
    LinkedQueue() : front(nullptr), rear(nullptr), count(0) {}

    /**
     * @brief Destructor — frees all remaining nodes
     *
     * Walks from front to rear, deleting each node.
     *
     * @complexity Time: O(n)
     * @complexity Space: O(1)
     */
    ~LinkedQueue() {
        while (front != nullptr) {
            QueueNode* temp = front;
            front = front->next;
            delete temp;
        }
    }

    /**
     * @brief Adds an element to the rear of the queue
     *
     * Algorithm:
     * 1. Create a new node with the given value
     * 2. If queue is empty, set both front and rear to new node
     * 3. Otherwise, link current rear to new node, update rear
     * 4. Increment count
     *
     * Visual Example:
     *   Before: front → [10] → [20] → nullptr, rear → [20]
     *   enqueue(30):
     *     Create node [30]
     *     rear->next = [30]    (link [20] → [30])
     *     rear = [30]          (update rear pointer)
     *   After:  front → [10] → [20] → [30] → nullptr, rear → [30]
     *
     * @param val The value to enqueue
     *
     * @complexity Time: O(1)
     * @complexity Space: O(1) — one new node allocated
     */
    void enqueue(int val) {
        // Create the new node
        QueueNode* newNode = new QueueNode();
        newNode->data = val;
        newNode->next = nullptr;

        if (isEmpty()) {
            // First element: both front and rear point to it
            front = newNode;
            rear = newNode;
        } else {
            // Link current rear to new node, update rear
            rear->next = newNode;
            rear = newNode;
        }
        count++;
    }

    /**
     * @brief Removes and returns the front element
     *
     * Algorithm:
     * 1. Check if queue is empty
     * 2. Save front node's data
     * 3. Move front to the next node
     * 4. Delete the old front node
     * 5. If queue is now empty, set rear to nullptr too
     * 6. Decrement count and return saved value
     *
     * Visual Example:
     *   Before: front → [10] → [20] → [30] → nullptr, rear → [30]
     *   dequeue():
     *     Save val = 10, temp = [10]
     *     front = [10]->next = [20]
     *     delete [10]
     *   After:  front → [20] → [30] → nullptr, rear → [30]
     *   Return: 10
     *
     * @return The value that was at the front
     *
     * @note Prints error and returns -1 if queue is empty
     *
     * @complexity Time: O(1)
     * @complexity Space: O(1)
     */
    int dequeue() {
        if (isEmpty()) {
            cout << "Queue is Empty! Cannot dequeue" << endl;
            return -1;
        }
        // Save the front node and its value
        QueueNode* temp = front;
        int val = front->data;

        // Move front to the next node
        front = front->next;

        // If queue is now empty, update rear too
        if (front == nullptr) {
            rear = nullptr;
        }

        // Free the old front node
        delete temp;
        count--;

        return val;
    }

    /**
     * @brief Returns the front element without removing it
     *
     * @return The value at the front of the queue
     *
     * @note Prints error and returns -1 if queue is empty
     *
     * @complexity Time: O(1)
     * @complexity Space: O(1)
     */
    int peek() {
        if (isEmpty()) {
            cout << "Queue is Empty! Nothing to peek" << endl;
            return -1;
        }
        return front->data;
    }

    /**
     * @brief Checks if the queue is empty
     * @return true if front is nullptr (no elements), false otherwise
     * @complexity Time: O(1)
     */
    bool isEmpty() {
        return front == nullptr;
    }

    /**
     * @brief Returns the number of elements in the queue
     * @return Number of elements currently in the queue
     * @complexity Time: O(1)
     */
    int size() {
        return count;
    }

    /**
     * @brief Prints the queue from front to rear
     *
     * @complexity Time: O(n)
     * @complexity Space: O(1)
     */
    void print() {
        if (isEmpty()) {
            cout << "(empty queue)" << endl;
            return;
        }
        cout << "Front [ ";
        QueueNode* curr = front;
        while (curr != nullptr) {
            cout << curr->data;
            if (curr->next != nullptr) cout << ", ";
            curr = curr->next;
        }
        cout << " ] Rear" << endl;
    }
};

// ============================================================================
// STEP-BY-STEP ENQUEUE (Circular Array, with trace output)
// ============================================================================

/**
 * @brief Enqueues a value with detailed step-by-step output showing wrap-around
 *
 * Prints each step of the enqueue operation including the raw array layout,
 * front/rear indices, and modular arithmetic for wrap-around.
 *
 * @param queue The CircularArrayQueue to enqueue into
 * @param val The value to enqueue
 *
 * @note For teaching purposes — shows index changes and wrap-around behavior
 *
 * @complexity Time: O(capacity) for printing, O(1) for the enqueue itself
 * @complexity Space: O(1)
 */
void enqueueWithSteps(CircularArrayQueue& queue, int val) {
    cout << "  Enqueue(" << val << "):" << endl;
    cout << "    Before (size=" << queue.size() << "):" << endl;
    queue.printRaw();

    if (queue.isFull()) {
        cout << "    Queue is FULL — cannot enqueue" << endl;
        return;
    }

    queue.enqueue(val);
    cout << "    After (size=" << queue.size() << "):" << endl;
    queue.printRaw();
    cout << "    Logical order: ";
    queue.print();
}

// ============================================================================
// STEP-BY-STEP DEQUEUE (Circular Array, with trace output)
// ============================================================================

/**
 * @brief Dequeues a value with detailed step-by-step output
 *
 * Prints each step of the dequeue operation including front advancement
 * and wrap-around behavior.
 *
 * @param queue The CircularArrayQueue to dequeue from
 *
 * @note For teaching purposes — shows front pointer movement
 *
 * @complexity Time: O(capacity) for printing, O(1) for the dequeue itself
 * @complexity Space: O(1)
 */
void dequeueWithSteps(CircularArrayQueue& queue) {
    cout << "  Dequeue():" << endl;
    cout << "    Before (size=" << queue.size() << "):" << endl;
    queue.printRaw();

    if (queue.isEmpty()) {
        cout << "    Queue is EMPTY — cannot dequeue" << endl;
        return;
    }

    int val = queue.dequeue();
    cout << "    Removed: " << val << endl;
    cout << "    After (size=" << queue.size() << "):" << endl;
    queue.printRaw();
    cout << "    Logical order: ";
    queue.print();
}

// ============================================================================
// MAIN FUNCTION - Demonstration and Testing
// ============================================================================

int main() {
    cout << "========================================" << endl;
    cout << "  Queue - Teaching Example" << endl;
    cout << "========================================\n" << endl;

    // Example 1: Circular Array Queue — Enqueue and Dequeue
    cout << "Example 1: Circular Array Queue — Enqueue and Dequeue" << endl;
    cout << "-----------------------------------------------------" << endl;
    CircularArrayQueue cq;

    int values1[] = {10, 20, 30, 40, 50};
    int n1 = sizeof(values1) / sizeof(values1[0]);

    for (int i = 0; i < n1; i++) {
        cq.enqueue(values1[i]);
        cout << "After enqueue(" << values1[i] << "): ";
        cq.print();
    }
    cout << "Peek: " << cq.peek() << endl;
    cout << "Size: " << cq.size() << endl;

    cout << "\nDequeuing all elements (FIFO order):" << endl;
    while (!cq.isEmpty()) {
        cout << "  dequeue() = " << cq.dequeue() << "  →  ";
        cq.print();
    }
    cout << endl;

    // Example 2: Circular Array — Wrap-Around Demo
    cout << "Example 2: Circular Array — Wrap-Around Demo" << endl;
    cout << "---------------------------------------------" << endl;
    cout << "(Using capacity = 8 to show wrap-around behavior)" << endl;
    cout << endl;

    CircularArrayQueue wq;

    // Fill partially
    cout << "Phase 1: Enqueue 10, 20, 30, 40, 50, 60" << endl;
    int fillVals[] = {10, 20, 30, 40, 50, 60};
    for (int i = 0; i < 6; i++) {
        wq.enqueue(fillVals[i]);
    }
    cout << "Queue: ";
    wq.print();
    wq.printRaw();
    cout << endl;

    // Dequeue some from the front — front pointer advances
    cout << "Phase 2: Dequeue 4 elements (front advances to index 4)" << endl;
    for (int i = 0; i < 4; i++) {
        cout << "  dequeue() = " << wq.dequeue() << endl;
    }
    cout << "Queue: ";
    wq.print();
    wq.printRaw();
    cout << endl;

    // Now enqueue more — rear wraps around past index 7 to index 0
    cout << "Phase 3: Enqueue 70, 80, 90, 100 (rear wraps around!)" << endl;
    int wrapVals[] = {70, 80, 90, 100};
    for (int i = 0; i < 4; i++) {
        wq.enqueue(wrapVals[i]);
        cout << "  After enqueue(" << wrapVals[i] << "):" << endl;
        wq.printRaw();
    }
    cout << "Queue (logical order): ";
    wq.print();
    cout << "Size: " << wq.size() << endl;
    cout << endl;

    // Example 3: Linked Queue
    cout << "Example 3: Linked Queue" << endl;
    cout << "-----------------------" << endl;
    LinkedQueue lq;

    int values3[] = {10, 20, 30, 40, 50};
    int n3 = sizeof(values3) / sizeof(values3[0]);

    for (int i = 0; i < n3; i++) {
        lq.enqueue(values3[i]);
        cout << "After enqueue(" << values3[i] << "): ";
        lq.print();
    }
    cout << "Peek: " << lq.peek() << endl;
    cout << "Size: " << lq.size() << endl;

    cout << "\nDequeuing all elements (FIFO order):" << endl;
    while (!lq.isEmpty()) {
        cout << "  dequeue() = " << lq.dequeue() << "  →  ";
        lq.print();
    }
    cout << endl;

    // Example 4: Step-by-Step Enqueue and Dequeue
    cout << "Example 4: Step-by-Step Enqueue and Dequeue" << endl;
    cout << "--------------------------------------------" << endl;
    CircularArrayQueue sq;

    enqueueWithSteps(sq, 10);
    cout << endl;
    enqueueWithSteps(sq, 20);
    cout << endl;
    enqueueWithSteps(sq, 30);
    cout << endl;
    dequeueWithSteps(sq);
    cout << endl;
    dequeueWithSteps(sq);
    cout << endl;

    // Example 5: Full Queue
    cout << "Example 5: Full Queue" << endl;
    cout << "---------------------" << endl;
    CircularArrayQueue fq;
    cout << "Filling queue to capacity (8 elements):" << endl;
    for (int i = 1; i <= 8; i++) {
        fq.enqueue(i * 10);
    }
    cout << "Queue: ";
    fq.print();
    fq.printRaw();
    cout << "isFull(): " << (fq.isFull() ? "true" : "false") << endl;
    cout << "Attempting to enqueue(90) on full queue:" << endl;
    fq.enqueue(90);
    cout << endl;

    // Example 6: Empty Queue
    cout << "Example 6: Empty Queue" << endl;
    cout << "----------------------" << endl;
    CircularArrayQueue eq;
    cout << "isEmpty(): " << (eq.isEmpty() ? "true" : "false") << endl;
    cout << "Attempting dequeue on empty queue:" << endl;
    eq.dequeue();
    cout << "Attempting peek on empty queue:" << endl;
    eq.peek();
    cout << endl;

    // Example 7: Comparison of Both Implementations
    cout << "Example 7: Comparison of Both Implementations" << endl;
    cout << "----------------------------------------------" << endl;
    CircularArrayQueue arrQueue;
    LinkedQueue lnkQueue;

    cout << "Enqueuing 100, 200, 300 into both queues:" << endl;
    int vals[] = {100, 200, 300};
    for (int i = 0; i < 3; i++) {
        arrQueue.enqueue(vals[i]);
        lnkQueue.enqueue(vals[i]);
    }
    cout << "  Circular Array: ";
    arrQueue.print();
    cout << "  Linked Queue:   ";
    lnkQueue.print();

    cout << "\nDequeuing from both:" << endl;
    cout << "  Array dequeue:  " << arrQueue.dequeue() << endl;
    cout << "  Linked dequeue: " << lnkQueue.dequeue() << endl;
    cout << "  Circular Array: ";
    arrQueue.print();
    cout << "  Linked Queue:   ";
    lnkQueue.print();

    cout << "\nPeek at both:" << endl;
    cout << "  Array peek:  " << arrQueue.peek() << endl;
    cout << "  Linked peek: " << lnkQueue.peek() << endl;
    cout << endl;

    // Example 8: Edge Cases
    cout << "Example 8: Edge Cases" << endl;
    cout << "---------------------" << endl;

    // Single element
    cout << "Single element queue:" << endl;
    CircularArrayQueue singleQ;
    singleQ.enqueue(42);
    cout << "  peek() = " << singleQ.peek() << endl;
    cout << "  dequeue() = " << singleQ.dequeue() << endl;
    cout << "  isEmpty() = " << (singleQ.isEmpty() ? "true" : "false") << endl;
    cout << endl;

    // Enqueue-dequeue-enqueue pattern
    cout << "Enqueue-dequeue-enqueue pattern:" << endl;
    CircularArrayQueue patternQ;
    patternQ.enqueue(1);
    cout << "  enqueue(1): ";
    patternQ.print();
    cout << "  dequeue() = " << patternQ.dequeue() << ": ";
    patternQ.print();
    patternQ.enqueue(2);
    cout << "  enqueue(2): ";
    patternQ.print();
    patternQ.enqueue(3);
    cout << "  enqueue(3): ";
    patternQ.print();
    cout << "  dequeue() = " << patternQ.dequeue() << endl;
    cout << "  dequeue() = " << patternQ.dequeue() << endl;
    cout << "  isEmpty() = " << (patternQ.isEmpty() ? "true" : "false") << endl;
    cout << endl;

    // Linked queue single element — check rear reset
    cout << "Linked queue — dequeue last element (rear reset):" << endl;
    LinkedQueue singleLQ;
    singleLQ.enqueue(99);
    cout << "  peek() = " << singleLQ.peek() << endl;
    cout << "  dequeue() = " << singleLQ.dequeue() << endl;
    cout << "  isEmpty() = " << (singleLQ.isEmpty() ? "true" : "false") << endl;
    singleLQ.enqueue(100);
    cout << "  enqueue(100) after empty: ";
    singleLQ.print();
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
 * 1. FIFO PRINCIPLE:
 *    - First In, First Out — the first element enqueued is the first dequeued
 *    - Think of a line at a store: first person in line is served first
 *    - This is the fundamental difference from a stack (LIFO)
 *    - Enqueue at the rear, dequeue from the front — two different ends
 *
 * 2. CIRCULAR ARRAY TRICK (MODULAR ARITHMETIC):
 *    - The key formula: nextIndex = (currentIndex + 1) % capacity
 *    - When rear reaches index (capacity - 1), the next enqueue wraps to 0
 *    - When front reaches index (capacity - 1), the next dequeue wraps to 0
 *    - Example with capacity 4:
 *      → Index 0 → 1 → 2 → 3 → 0 → 1 → 2 → 3 → 0 → ...
 *      → (3 + 1) % 4 = 0  ← this is the wrap-around!
 *    - Without this trick, you would need to shift elements (O(n)) or waste space
 *
 * 3. FRONT AND REAR POINTERS:
 *    - front: index of the element to dequeue next
 *    - rear: index of the most recently enqueued element
 *    - Both advance forward using modular arithmetic
 *    - The count variable tracks how many elements are in the queue
 *    - Alternative to count: leave one slot empty (full when count = capacity - 1)
 *      or use a boolean flag — but count is simplest to understand
 *
 * 4. WRAP-AROUND VISUALIZATION:
 *    - The array is logically a ring, not a line:
 *      [0] [1] [2] [3] [4] [5] [6] [7]
 *       ↑                             ↑
 *       └──────────── wraps to ───────┘
 *    - Elements can span from a high index to a low index:
 *      rear=2, front=5 means data is at indices 5, 6, 7, 0, 1, 2
 *    - This is why we use count to track fullness, not (rear >= front)
 *
 * 5. CIRCULAR ARRAY VS LINKED LIST:
 *    - Circular array:
 *      → Fixed capacity, O(1) all operations
 *      → Cache-friendly (contiguous memory)
 *      → No dynamic allocation during enqueue/dequeue
 *      → Must handle wrap-around with modular arithmetic
 *    - Linked list:
 *      → Dynamic size (no overflow unless out of memory)
 *      → Each node has pointer overhead
 *      → No wrap-around logic needed
 *      → Must handle special cases (empty queue, single element)
 *
 * 6. DEQUE (DOUBLE-ENDED QUEUE):
 *    - A deque allows insertion and removal at BOTH ends
 *    - Combines stack and queue behavior
 *    - Can be used as either a stack or a queue
 *    - C++ STL: std::deque — actually the default container for std::stack
 *    - Operations: push_front, push_back, pop_front, pop_back — all O(1)
 *
 * 7. BFS CONNECTION:
 *    - Breadth-First Search uses a QUEUE to explore level by level
 *    - Enqueue the starting node, then repeatedly:
 *      → Dequeue a node, process it
 *      → Enqueue all unvisited neighbors
 *    - This ensures nodes are visited in order of distance from start
 *    - Compare: DFS uses a STACK (or recursion) — explores depth-first
 *    - Queue → BFS, Stack → DFS — this is a key distinction
 *
 * 8. PRIORITY QUEUE VS REGULAR QUEUE:
 *    - Regular queue: FIFO — first in, first out (no regard to priority)
 *    - Priority queue: elements are dequeued in order of priority
 *      → Implemented with a heap, NOT a regular queue
 *      → Insert O(log n), extract-min/max O(log n)
 *    - The name "priority queue" is misleading — it is NOT a queue
 *    - It is an abstract data type typically backed by a heap
 *    - C++ STL: std::priority_queue (max-heap by default)
 *
 * 9. APPLICATIONS:
 *    - BFS traversal (graphs and trees)
 *    - CPU scheduling: round-robin uses a circular queue
 *    - Print spooling: documents printed in order submitted
 *    - Buffering: keyboard buffer, network packet buffers
 *    - Producer-consumer pattern: producer enqueues, consumer dequeues
 *    - Simulation: customers in line, event queues
 *    - Level-order tree traversal
 *
 * 10. COMMON MISTAKES:
 *    - Forgetting modular arithmetic: rear++ without % capacity → out of bounds
 *    - Confusing full vs empty: both can have front == rear in some implementations
 *      → Solution: use a count variable to distinguish full from empty
 *    - Forgetting to update rear pointer to nullptr when linked queue becomes empty
 *      → If rear still points to freed memory, next enqueue corrupts memory
 *    - Using a linear array without wrap-around: dequeue becomes O(n) due to shifting
 *    - Confusing queue (FIFO) with stack (LIFO) — know which to use:
 *      → BFS = queue, DFS = stack
 *    - Off-by-one: rear starts at -1 (before first enqueue) in our implementation
 *    - Forgetting that the "logical order" differs from the physical array layout
 *      when wrap-around has occurred
 */
