/**
 * @file linked_list.cpp
 * @brief Implementation of Singly and Doubly Linked Lists with comprehensive documentation
 * @author CS 302 - Data Structures
 * @date 2026
 *
 * Linked List: A dynamic data structure where each element (node) contains data
 * and a pointer to the next node. Nodes are scattered in memory and connected
 * through pointers, allowing O(1) insertion/deletion at known positions.
 *
 * Singly Linked List: Each node points to the next node only (one direction).
 * Doubly Linked List: Each node points to both the next and previous nodes.
 *
 * Key Feature: Dynamic size with O(1) insertion/deletion at head.
 * No shifting needed — just update pointers.
 *
 * Time Complexity: Access O(n), Search O(n), Insert at head O(1), Delete at head O(1)
 * Space Complexity: O(n) for n nodes, each node stores data + pointer(s)
 *
 * Best for: Frequent insertion/deletion, unknown size, implementing stacks/queues
 */

#include <iostream>
#include <cassert>

using namespace std;

// ============================================================================
// NODE STRUCTURES
// ============================================================================

/**
 * @brief Node for a singly linked list
 *
 * Each node contains an integer value and a pointer to the next node.
 * The last node in the list has next = nullptr (null pointer).
 *
 * Memory layout: [data | next] → [data | next] → [data | nullptr]
 */
struct Node {
    int data;
    Node* next;
    Node(int d) : data(d), next(nullptr) {}
};

/**
 * @brief Node for a doubly linked list
 *
 * Each node contains an integer value and pointers to both the previous
 * and next nodes. This allows traversal in both directions.
 *
 * Memory layout: nullptr ←[prev | data | next]⇄[prev | data | next]→ nullptr
 */
struct DNode {
    int data;
    DNode* prev;
    DNode* next;
    DNode(int d) : data(d), prev(nullptr), next(nullptr) {}
};

// ============================================================================
// SINGLY LINKED LIST OPERATIONS
// ============================================================================

/**
 * @brief Inserts a new node at the head (front) of the singly linked list
 *
 * Creates a new node and makes it the new head by pointing its next to
 * the current head. This is O(1) because no traversal is needed.
 *
 * Visual Example:
 *   Before: head → [20|→] → [30|∅]
 *   Insert 10 at head:
 *     Step 1: Create new node [10|→]
 *     Step 2: Point new node's next to current head (20)
 *     Step 3: Update head to point to new node
 *   After:  head → [10|→] → [20|→] → [30|∅]
 *
 * @param head Pointer reference to the head of the list (updated to new node)
 * @param value The value to insert
 *
 * @note head is passed by reference — it is updated to point to the new node
 * @note Works correctly on an empty list (head = nullptr)
 *
 * @complexity Time: O(1) — no traversal needed
 * @complexity Space: O(1) — allocates one new node
 */
void insertAtHead(Node*& head, int value) {
    // Create a new node with the given value
    Node* newNode = new Node(value);

    // Point new node's next to the current head
    newNode->next = head;

    // Update head to point to the new node
    head = newNode;
}

/**
 * @brief Inserts a new node at the tail (end) of the singly linked list
 *
 * Must traverse the entire list to find the last node, then append.
 * This is O(n) because we need to walk to the end.
 *
 * Visual Example:
 *   Before: head → [10|→] → [20|→] → [30|∅]
 *   Insert 40 at tail:
 *     Step 1: Create new node [40|∅]
 *     Step 2: Traverse to last node (30)
 *     Step 3: Point last node's next to new node
 *   After:  head → [10|→] → [20|→] → [30|→] → [40|∅]
 *
 * @param head Pointer reference to the head of the list
 * @param value The value to insert
 *
 * @note If the list is empty, the new node becomes the head
 * @note Maintaining a tail pointer would make this O(1)
 *
 * @complexity Time: O(n) — must traverse to the end
 * @complexity Space: O(1) — allocates one new node
 */
void insertAtTail(Node*& head, int value) {
    // Create a new node
    Node* newNode = new Node(value);

    // Special case: empty list
    if (head == nullptr) {
        head = newNode;
        return;
    }

    // Traverse to the last node
    Node* current = head;
    while (current->next != nullptr) {
        current = current->next;
    }

    // Append the new node
    current->next = newNode;
}

/**
 * @brief Inserts a new node at the given position (0-based) in the singly linked list
 *
 * Position 0 means insert at head. Position n means insert at tail.
 * For any other position, we traverse to the node just before the target
 * position and splice the new node into the chain.
 *
 * Visual Example:
 *   Insert 25 at position 2 in: head → [10|→] → [20|→] → [30|→] → [40|∅]
 *
 *   Step 1: Traverse to node at position 1 (value 20) — the node BEFORE target
 *   Step 2: Create new node [25|→]
 *   Step 3: Point new node's next to position 2 node (30)
 *   Step 4: Point position 1 node's next to new node
 *
 *   Before: [10|→] → [20|→] → [30|→] → [40|∅]
 *   After:  [10|→] → [20|→] → [25|→] → [30|→] → [40|∅]
 *
 * @param head Pointer reference to the head of the list
 * @param pos The 0-based position at which to insert
 * @param value The value to insert
 *
 * @note Position 0 inserts at head, position n inserts at tail
 * @note Invalid positions print an error message
 *
 * @complexity Time: O(n) — must traverse to the target position
 * @complexity Space: O(1) — allocates one new node
 */
void insertAtPosition(Node*& head, int pos, int value) {
    // Position 0 is the same as insert at head
    if (pos == 0) {
        insertAtHead(head, value);
        return;
    }

    // Traverse to the node just BEFORE the target position
    Node* current = head;
    for (int i = 0; i < pos - 1; i++) {
        if (current == nullptr) {
            cout << "    Error: Position " << pos << " is out of bounds" << endl;
            return;
        }
        current = current->next;
    }

    if (current == nullptr) {
        cout << "    Error: Position " << pos << " is out of bounds" << endl;
        return;
    }

    // Create new node and splice it in
    Node* newNode = new Node(value);
    newNode->next = current->next;
    current->next = newNode;
}

/**
 * @brief Deletes the head (first) node of the singly linked list
 *
 * Removes the first node and updates head to point to the second node.
 * The memory of the removed node is freed.
 *
 * Visual Example:
 *   Before: head → [10|→] → [20|→] → [30|∅]
 *   Delete head:
 *     Step 1: Save pointer to head node (10)
 *     Step 2: Update head to head->next (20)
 *     Step 3: Delete the saved node (free memory)
 *   After:  head → [20|→] → [30|∅]
 *
 * @param head Pointer reference to the head of the list
 *
 * @note Must free the memory of the removed node to avoid memory leaks
 * @note If the list has one node, head becomes nullptr (empty list)
 *
 * @complexity Time: O(1) — no traversal needed
 * @complexity Space: O(1) — frees one node
 */
void deleteAtHead(Node*& head) {
    if (head == nullptr) {
        cout << "    Error: List is empty, nothing to delete" << endl;
        return;
    }

    // Save the node to delete
    Node* temp = head;

    // Move head to the next node
    head = head->next;

    // Free the memory
    delete temp;
}

/**
 * @brief Deletes the tail (last) node of the singly linked list
 *
 * Must traverse to the second-to-last node, then remove the last.
 * This is O(n) because singly linked lists cannot go backwards.
 *
 * Visual Example:
 *   Before: head → [10|→] → [20|→] → [30|∅]
 *   Delete tail:
 *     Step 1: Traverse to second-to-last node (20)
 *     Step 2: Save pointer to last node (30)
 *     Step 3: Set second-to-last node's next to nullptr
 *     Step 4: Delete the saved node (free memory)
 *   After:  head → [10|→] → [20|∅]
 *
 * @param head Pointer reference to the head of the list
 *
 * @note With a doubly linked list and tail pointer, this would be O(1)
 * @note Single-element list: head becomes nullptr
 *
 * @complexity Time: O(n) — must traverse to the end
 * @complexity Space: O(1) — frees one node
 */
void deleteAtTail(Node*& head) {
    if (head == nullptr) {
        cout << "    Error: List is empty, nothing to delete" << endl;
        return;
    }

    // Special case: only one node
    if (head->next == nullptr) {
        delete head;
        head = nullptr;
        return;
    }

    // Traverse to the second-to-last node
    Node* current = head;
    while (current->next->next != nullptr) {
        current = current->next;
    }

    // Delete the last node
    delete current->next;
    current->next = nullptr;
}

/**
 * @brief Deletes the node at the given position (0-based) in the singly linked list
 *
 * Position 0 deletes the head. For any other position, traverse to the node
 * just before the target and unlink the target node.
 *
 * Visual Example:
 *   Delete at position 1 from: head → [10|→] → [20|→] → [30|∅]
 *
 *   Step 1: Traverse to node at position 0 (value 10) — the node BEFORE target
 *   Step 2: Save pointer to target node (20)
 *   Step 3: Point previous node's next to target's next (30)
 *   Step 4: Delete the target node (free memory)
 *
 *   Before: [10|→] → [20|→] → [30|∅]
 *   After:  [10|→] → [30|∅]
 *
 * @param head Pointer reference to the head of the list
 * @param pos The 0-based position of the node to delete
 *
 * @note Must free memory of the removed node
 * @note Invalid positions print an error message
 *
 * @complexity Time: O(n) — must traverse to the target position
 * @complexity Space: O(1) — frees one node
 */
void deleteAtPosition(Node*& head, int pos) {
    if (head == nullptr) {
        cout << "    Error: List is empty, nothing to delete" << endl;
        return;
    }

    // Position 0 is the same as delete at head
    if (pos == 0) {
        deleteAtHead(head);
        return;
    }

    // Traverse to the node BEFORE the target position
    Node* current = head;
    for (int i = 0; i < pos - 1; i++) {
        if (current->next == nullptr) {
            cout << "    Error: Position " << pos << " is out of bounds" << endl;
            return;
        }
        current = current->next;
    }

    // Check if target node exists
    if (current->next == nullptr) {
        cout << "    Error: Position " << pos << " is out of bounds" << endl;
        return;
    }

    // Unlink and delete the target node
    Node* temp = current->next;
    current->next = temp->next;
    delete temp;
}

/**
 * @brief Searches for a value in the singly linked list
 *
 * Traverses the list from head to tail, checking each node's data.
 * Returns the 0-based position of the first occurrence, or -1 if not found.
 *
 * Visual Example:
 *   Search for 30 in: head → [10|→] → [20|→] → [30|→] → [40|∅]
 *
 *   Check position 0: data = 10 ≠ 30 → move to next
 *   Check position 1: data = 20 ≠ 30 → move to next
 *   Check position 2: data = 30 = 30 → FOUND at position 2!
 *
 * @param head Pointer to the head of the list
 * @param value The value to search for
 * @return The 0-based position of the first occurrence, or -1 if not found
 *
 * @note Returns the FIRST occurrence — there may be duplicates
 * @note Cannot use binary search on linked lists (no random access)
 *
 * @complexity Time: O(n) — may need to traverse the entire list
 * @complexity Space: O(1) — only uses a traversal pointer
 */
int search(Node* head, int value) {
    Node* current = head;
    int position = 0;

    while (current != nullptr) {
        if (current->data == value) {
            return position;
        }
        current = current->next;
        position++;
    }

    return -1;  // Not found
}

/**
 * @brief Reverses the singly linked list in-place
 *
 * Uses three pointers (prev, current, next) to reverse the direction of
 * all pointers. After reversal, the old tail becomes the new head.
 *
 * Visual Example:
 *   Before: head → [10|→] → [20|→] → [30|∅]
 *
 *   Step 1: prev=∅, curr=10, next=20
 *           10→∅    (reverse 10's pointer)
 *   Step 2: prev=10, curr=20, next=30
 *           10←20   (reverse 20's pointer)
 *   Step 3: prev=20, curr=30, next=∅
 *           20←30   (reverse 30's pointer)
 *   Step 4: curr=∅, set head=prev(30)
 *
 *   After:  head → [30|→] → [20|→] → [10|∅]
 *
 * @param head Pointer reference to the head of the list (updated to new head)
 *
 * @note Modifies the list in-place — no new nodes are created
 * @note The three-pointer technique is a classic interview question
 *
 * @complexity Time: O(n) — visits each node exactly once
 * @complexity Space: O(1) — only uses three pointer variables
 */
void reverseList(Node*& head) {
    Node* prev = nullptr;
    Node* current = head;
    Node* next = nullptr;

    while (current != nullptr) {
        // Save the next node before we change current's pointer
        next = current->next;

        // Reverse the pointer: point current back to previous
        current->next = prev;

        // Move prev and current one step forward
        prev = current;
        current = next;
    }

    // prev is now the new head (old tail)
    head = prev;
}

/**
 * @brief Prints the singly linked list
 *
 * @param head Pointer to the head of the list
 *
 * @note Displays in format: [10|→]—→[20|→]—→[30|∅]
 */
void printList(Node* head) {
    if (head == nullptr) {
        cout << "(empty list)" << endl;
        return;
    }

    Node* current = head;
    while (current != nullptr) {
        if (current->next != nullptr) {
            cout << "[" << current->data << "|→]—→";
        } else {
            cout << "[" << current->data << "|∅]";
        }
        current = current->next;
    }
    cout << endl;
}

// ============================================================================
// SINGLY LINKED LIST — STEP-BY-STEP VARIANTS
// ============================================================================

/**
 * @brief Inserts at head with step-by-step output
 *
 * @param head Pointer reference to the head of the list
 * @param value The value to insert
 *
 * @note Shows each pointer update during insertion
 *
 * @complexity Time: O(1) plus O(n) for printing
 * @complexity Space: O(1)
 */
void insertAtHeadWithSteps(Node*& head, int value) {
    cout << "  Inserting " << value << " at head..." << endl;
    cout << "    Before: ";
    printList(head);

    Node* newNode = new Node(value);
    cout << "    Created node [" << value << "|∅]" << endl;

    newNode->next = head;
    if (head != nullptr) {
        cout << "    Set new node's next → " << head->data << endl;
    } else {
        cout << "    Set new node's next → nullptr (list was empty)" << endl;
    }

    head = newNode;
    cout << "    Updated head → " << value << endl;
    cout << "    After:  ";
    printList(head);
}

/**
 * @brief Inserts at tail with step-by-step output
 *
 * @param head Pointer reference to the head of the list
 * @param value The value to insert
 *
 * @note Shows the traversal to the end and pointer update
 *
 * @complexity Time: O(n) plus O(n) for printing
 * @complexity Space: O(1)
 */
void insertAtTailWithSteps(Node*& head, int value) {
    cout << "  Inserting " << value << " at tail..." << endl;
    cout << "    Before: ";
    printList(head);

    Node* newNode = new Node(value);
    cout << "    Created node [" << value << "|∅]" << endl;

    if (head == nullptr) {
        head = newNode;
        cout << "    List was empty — new node becomes head" << endl;
    } else {
        Node* current = head;
        int steps = 0;
        while (current->next != nullptr) {
            cout << "    Traverse: at node " << current->data << " → move to next" << endl;
            current = current->next;
            steps++;
        }
        cout << "    Traverse: at node " << current->data << " → this is the tail" << endl;
        current->next = newNode;
        cout << "    Set " << current->data << "'s next → " << value << "  (" << steps + 1 << " node(s) traversed)" << endl;
    }

    cout << "    After:  ";
    printList(head);
}

/**
 * @brief Deletes at head with step-by-step output
 *
 * @param head Pointer reference to the head of the list
 *
 * @note Shows pointer update and memory deallocation
 *
 * @complexity Time: O(1) plus O(n) for printing
 * @complexity Space: O(1)
 */
void deleteAtHeadWithSteps(Node*& head) {
    cout << "  Deleting head..." << endl;
    cout << "    Before: ";
    printList(head);

    if (head == nullptr) {
        cout << "    Error: List is empty, nothing to delete" << endl;
        return;
    }

    int removedValue = head->data;
    Node* temp = head;
    head = head->next;
    delete temp;

    cout << "    Removed node with value " << removedValue << endl;
    if (head != nullptr) {
        cout << "    Updated head → " << head->data << endl;
    } else {
        cout << "    Updated head → nullptr (list is now empty)" << endl;
    }
    cout << "    After:  ";
    printList(head);
}

/**
 * @brief Deletes at tail with step-by-step output
 *
 * @param head Pointer reference to the head of the list
 *
 * @note Shows the traversal to find the second-to-last node
 *
 * @complexity Time: O(n) plus O(n) for printing
 * @complexity Space: O(1)
 */
void deleteAtTailWithSteps(Node*& head) {
    cout << "  Deleting tail..." << endl;
    cout << "    Before: ";
    printList(head);

    if (head == nullptr) {
        cout << "    Error: List is empty, nothing to delete" << endl;
        return;
    }

    if (head->next == nullptr) {
        int removedValue = head->data;
        delete head;
        head = nullptr;
        cout << "    Removed node with value " << removedValue << " (was the only node)" << endl;
        cout << "    After:  ";
        printList(head);
        return;
    }

    Node* current = head;
    while (current->next->next != nullptr) {
        cout << "    Traverse: at node " << current->data << " → move to next" << endl;
        current = current->next;
    }

    int removedValue = current->next->data;
    cout << "    Traverse: at node " << current->data << " → next is the tail (" << removedValue << ")" << endl;
    delete current->next;
    current->next = nullptr;

    cout << "    Removed node with value " << removedValue << endl;
    cout << "    Set " << current->data << "'s next → nullptr (new tail)" << endl;
    cout << "    After:  ";
    printList(head);
}

/**
 * @brief Searches with step-by-step output
 *
 * @param head Pointer to the head of the list
 * @param value The value to search for
 *
 * @note Shows each comparison during traversal
 *
 * @complexity Time: O(n) plus O(n) for printing
 * @complexity Space: O(1)
 */
void searchWithSteps(Node* head, int value) {
    cout << "  Searching for " << value << "..." << endl;

    Node* current = head;
    int position = 0;
    int comparisons = 0;

    while (current != nullptr) {
        comparisons++;
        if (current->data == value) {
            cout << "    Position " << position << ": data = " << current->data
                 << " = " << value << " → FOUND!" << endl;
            cout << "    Result: Found at position " << position << " after "
                 << comparisons << " comparison(s)" << endl;
            return;
        } else {
            cout << "    Position " << position << ": data = " << current->data
                 << " ≠ " << value << " → move to next" << endl;
        }
        current = current->next;
        position++;
    }

    cout << "    Result: Not found after " << comparisons << " comparison(s)" << endl;
}

/**
 * @brief Reverses the list with step-by-step output
 *
 * @param head Pointer reference to the head of the list
 *
 * @note Shows each pointer reversal during the three-pointer technique
 *
 * @complexity Time: O(n) plus O(n) for printing
 * @complexity Space: O(1)
 */
void reverseListWithSteps(Node*& head) {
    cout << "  Reversing list..." << endl;
    cout << "    Before: ";
    printList(head);

    Node* prev = nullptr;
    Node* current = head;
    Node* next = nullptr;
    int step = 1;

    while (current != nullptr) {
        next = current->next;

        cout << "    Step " << step << ": curr=" << current->data
             << ", set " << current->data << "'s next → ";
        if (prev != nullptr) {
            cout << prev->data;
        } else {
            cout << "nullptr";
        }
        cout << " (was → ";
        if (next != nullptr) {
            cout << next->data;
        } else {
            cout << "nullptr";
        }
        cout << ")" << endl;

        current->next = prev;
        prev = current;
        current = next;
        step++;
    }

    head = prev;
    cout << "    Updated head → " << head->data << endl;
    cout << "    After:  ";
    printList(head);
}

// ============================================================================
// DOUBLY LINKED LIST OPERATIONS
// ============================================================================

/**
 * @brief Inserts a new node at the head of the doubly linked list
 *
 * Visual Example:
 *   Before: head → [∅|20|→] ⇄ [←|30|∅] ← tail
 *   Insert 10 at head:
 *     Step 1: Create [∅|10|∅]
 *     Step 2: Set 10's next → 20
 *     Step 3: Set 20's prev → 10
 *     Step 4: Update head → 10
 *   After:  head → [∅|10|→] ⇄ [←|20|→] ⇄ [←|30|∅] ← tail
 *
 * @param head Pointer reference to the head of the list
 * @param tail Pointer reference to the tail of the list
 * @param value The value to insert
 *
 * @note Updates both head and potentially tail (if list was empty)
 *
 * @complexity Time: O(1) — no traversal needed
 * @complexity Space: O(1) — allocates one new node
 */
void dInsertAtHead(DNode*& head, DNode*& tail, int value) {
    DNode* newNode = new DNode(value);

    if (head == nullptr) {
        // Empty list: new node is both head and tail
        head = newNode;
        tail = newNode;
    } else {
        // Link new node to current head
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }
}

/**
 * @brief Inserts a new node at the tail of the doubly linked list
 *
 * With a tail pointer, we can insert at the end in O(1) — no traversal needed.
 * This is a key advantage of maintaining a tail pointer.
 *
 * Visual Example:
 *   Before: head → [∅|10|→] ⇄ [←|20|∅] ← tail
 *   Insert 30 at tail:
 *     Step 1: Create [∅|30|∅]
 *     Step 2: Set 30's prev → 20
 *     Step 3: Set 20's next → 30
 *     Step 4: Update tail → 30
 *   After:  head → [∅|10|→] ⇄ [←|20|→] ⇄ [←|30|∅] ← tail
 *
 * @param head Pointer reference to the head of the list
 * @param tail Pointer reference to the tail of the list
 * @param value The value to insert
 *
 * @note O(1) because we have a tail pointer — no traversal needed
 * @note Compare with singly linked list where tail insert is O(n)
 *
 * @complexity Time: O(1) — direct access via tail pointer
 * @complexity Space: O(1) — allocates one new node
 */
void dInsertAtTail(DNode*& head, DNode*& tail, int value) {
    DNode* newNode = new DNode(value);

    if (tail == nullptr) {
        // Empty list: new node is both head and tail
        head = newNode;
        tail = newNode;
    } else {
        // Link new node to current tail
        newNode->prev = tail;
        tail->next = newNode;
        tail = newNode;
    }
}

/**
 * @brief Deletes the head node of the doubly linked list
 *
 * Visual Example:
 *   Before: head → [∅|10|→] ⇄ [←|20|→] ⇄ [←|30|∅] ← tail
 *   Delete head:
 *     Step 1: Save pointer to head (10)
 *     Step 2: Update head → 20
 *     Step 3: Set 20's prev → nullptr
 *     Step 4: Delete old head (free memory)
 *   After:  head → [∅|20|→] ⇄ [←|30|∅] ← tail
 *
 * @param head Pointer reference to the head of the list
 * @param tail Pointer reference to the tail of the list
 *
 * @note Updates tail to nullptr if the list becomes empty
 *
 * @complexity Time: O(1) — no traversal needed
 * @complexity Space: O(1) — frees one node
 */
void dDeleteAtHead(DNode*& head, DNode*& tail) {
    if (head == nullptr) {
        cout << "    Error: List is empty, nothing to delete" << endl;
        return;
    }

    DNode* temp = head;

    if (head == tail) {
        // Only one node
        head = nullptr;
        tail = nullptr;
    } else {
        head = head->next;
        head->prev = nullptr;
    }

    delete temp;
}

/**
 * @brief Deletes the tail node of the doubly linked list
 *
 * With a doubly linked list and tail pointer, deleting the last node is O(1).
 * We can go directly to the tail's previous node — no traversal needed.
 * This is a key advantage over singly linked lists where tail deletion is O(n).
 *
 * Visual Example:
 *   Before: head → [∅|10|→] ⇄ [←|20|→] ⇄ [←|30|∅] ← tail
 *   Delete tail:
 *     Step 1: Save pointer to tail (30)
 *     Step 2: Update tail → 20
 *     Step 3: Set 20's next → nullptr
 *     Step 4: Delete old tail (free memory)
 *   After:  head → [∅|10|→] ⇄ [←|20|∅] ← tail
 *
 * @param head Pointer reference to the head of the list
 * @param tail Pointer reference to the tail of the list
 *
 * @note O(1) — compare with singly linked list where this is O(n)
 * @note Updates head to nullptr if the list becomes empty
 *
 * @complexity Time: O(1) — direct access via tail and prev pointers
 * @complexity Space: O(1) — frees one node
 */
void dDeleteAtTail(DNode*& head, DNode*& tail) {
    if (tail == nullptr) {
        cout << "    Error: List is empty, nothing to delete" << endl;
        return;
    }

    DNode* temp = tail;

    if (head == tail) {
        // Only one node
        head = nullptr;
        tail = nullptr;
    } else {
        tail = tail->prev;
        tail->next = nullptr;
    }

    delete temp;
}

/**
 * @brief Prints the doubly linked list from head to tail
 *
 * @param head Pointer to the head of the list
 *
 * @note Displays in format: [∅|10|→]⇄[←|20|→]⇄[←|30|∅]
 */
void dPrintList(DNode* head) {
    if (head == nullptr) {
        cout << "(empty list)" << endl;
        return;
    }

    DNode* current = head;
    while (current != nullptr) {
        if (current->prev == nullptr && current->next == nullptr) {
            cout << "[∅|" << current->data << "|∅]";
        } else if (current->prev == nullptr) {
            cout << "[∅|" << current->data << "|→]⇄";
        } else if (current->next == nullptr) {
            cout << "[←|" << current->data << "|∅]";
        } else {
            cout << "[←|" << current->data << "|→]⇄";
        }
        current = current->next;
    }
    cout << endl;
}

/**
 * @brief Prints the doubly linked list from tail to head (reverse)
 *
 * Demonstrates bidirectional traversal — a key advantage of doubly linked lists.
 *
 * @param tail Pointer to the tail of the list
 *
 * @note This operation is impossible with singly linked lists without reversing first
 *
 * @complexity Time: O(n) — visits each node once
 * @complexity Space: O(1)
 */
void dPrintReverse(DNode* tail) {
    if (tail == nullptr) {
        cout << "(empty list)" << endl;
        return;
    }

    cout << "Reverse: ";
    DNode* current = tail;
    while (current != nullptr) {
        cout << current->data;
        if (current->prev != nullptr) {
            cout << " → ";
        }
        current = current->prev;
    }
    cout << endl;
}

// ============================================================================
// MAIN FUNCTION - Demonstration and Testing
// ============================================================================

int main() {
    cout << "========================================" << endl;
    cout << "  Linked Lists - Teaching Example" << endl;
    cout << "========================================\n" << endl;

    // Example 1: Singly Linked List — Insert Operations
    cout << "Example 1: Singly Linked List — Insert Operations" << endl;
    cout << "--------------------------------------------------" << endl;
    Node* list1 = nullptr;

    insertAtHeadWithSteps(list1, 30);
    cout << endl;
    insertAtHeadWithSteps(list1, 20);
    cout << endl;
    insertAtHeadWithSteps(list1, 10);
    cout << endl;
    insertAtTailWithSteps(list1, 40);
    cout << endl;

    cout << "Insert 25 at position 2:" << endl;
    insertAtPosition(list1, 2, 25);
    cout << "  List: ";
    printList(list1);
    cout << endl;

    // Example 2: Singly Linked List — Delete Operations
    cout << "Example 2: Singly Linked List — Delete Operations" << endl;
    cout << "--------------------------------------------------" << endl;
    Node* list2 = nullptr;
    insertAtTail(list2, 10);
    insertAtTail(list2, 20);
    insertAtTail(list2, 30);
    insertAtTail(list2, 40);
    insertAtTail(list2, 50);
    cout << "Starting list: ";
    printList(list2);

    deleteAtHeadWithSteps(list2);
    cout << endl;
    deleteAtTailWithSteps(list2);
    cout << endl;

    cout << "Delete at position 1:" << endl;
    cout << "  Before: ";
    printList(list2);
    deleteAtPosition(list2, 1);
    cout << "  After:  ";
    printList(list2);
    cout << endl;

    // Example 3: Singly Linked List — Search
    cout << "Example 3: Singly Linked List — Search" << endl;
    cout << "---------------------------------------" << endl;
    Node* list3 = nullptr;
    insertAtTail(list3, 10);
    insertAtTail(list3, 20);
    insertAtTail(list3, 30);
    insertAtTail(list3, 40);
    insertAtTail(list3, 50);
    cout << "List: ";
    printList(list3);

    searchWithSteps(list3, 30);
    cout << endl;
    searchWithSteps(list3, 99);
    cout << endl;

    // Example 4: Singly Linked List — Reverse
    cout << "Example 4: Singly Linked List — Reverse" << endl;
    cout << "----------------------------------------" << endl;
    Node* list4 = nullptr;
    insertAtTail(list4, 10);
    insertAtTail(list4, 20);
    insertAtTail(list4, 30);
    insertAtTail(list4, 40);

    reverseListWithSteps(list4);
    cout << endl;

    // Example 5: Doubly Linked List — Insert Operations
    cout << "Example 5: Doubly Linked List — Insert Operations" << endl;
    cout << "--------------------------------------------------" << endl;
    DNode* dHead = nullptr;
    DNode* dTail = nullptr;

    cout << "Insert 20 at head:" << endl;
    dInsertAtHead(dHead, dTail, 20);
    cout << "  Forward:  ";
    dPrintList(dHead);

    cout << "Insert 10 at head:" << endl;
    dInsertAtHead(dHead, dTail, 10);
    cout << "  Forward:  ";
    dPrintList(dHead);

    cout << "Insert 30 at tail:" << endl;
    dInsertAtTail(dHead, dTail, 30);
    cout << "  Forward:  ";
    dPrintList(dHead);

    cout << "Insert 40 at tail:" << endl;
    dInsertAtTail(dHead, dTail, 40);
    cout << "  Forward:  ";
    dPrintList(dHead);
    cout << "  ";
    dPrintReverse(dTail);
    cout << endl;

    // Example 6: Doubly Linked List — Delete Operations
    cout << "Example 6: Doubly Linked List — Delete Operations" << endl;
    cout << "--------------------------------------------------" << endl;
    cout << "Starting list: ";
    dPrintList(dHead);

    cout << "Delete head:" << endl;
    dDeleteAtHead(dHead, dTail);
    cout << "  Forward:  ";
    dPrintList(dHead);

    cout << "Delete tail:" << endl;
    dDeleteAtTail(dHead, dTail);
    cout << "  Forward:  ";
    dPrintList(dHead);
    cout << "  ";
    dPrintReverse(dTail);
    cout << endl;

    // Example 7: Edge Cases — Single Node and Empty List
    cout << "Example 7: Edge Cases — Single Node and Empty List" << endl;
    cout << "---------------------------------------------------" << endl;

    // Single node singly linked list
    cout << "Singly linked list with one node:" << endl;
    Node* single = nullptr;
    insertAtHead(single, 42);
    cout << "  List: ";
    printList(single);
    searchWithSteps(single, 42);
    cout << endl;
    reverseListWithSteps(single);
    cout << endl;
    deleteAtHeadWithSteps(single);
    cout << endl;

    // Empty list operations
    cout << "Operations on empty singly linked list:" << endl;
    Node* empty = nullptr;
    cout << "  List: ";
    printList(empty);
    cout << "  Search for 10: position = " << search(empty, 10) << endl;
    deleteAtHead(empty);
    cout << endl;

    // Single node doubly linked list
    cout << "Doubly linked list with one node:" << endl;
    DNode* dSingle = nullptr;
    DNode* dSTail = nullptr;
    dInsertAtHead(dSingle, dSTail, 42);
    cout << "  Forward: ";
    dPrintList(dSingle);
    cout << "  ";
    dPrintReverse(dSTail);
    dDeleteAtHead(dSingle, dSTail);
    cout << "  After delete: ";
    dPrintList(dSingle);
    cout << endl;

    // Example 8: Duplicates and Bidirectional Traversal
    cout << "Example 8: Duplicates and Bidirectional Traversal" << endl;
    cout << "--------------------------------------------------" << endl;

    // Singly linked list with duplicates
    Node* dups = nullptr;
    insertAtTail(dups, 10);
    insertAtTail(dups, 20);
    insertAtTail(dups, 10);
    insertAtTail(dups, 30);
    insertAtTail(dups, 10);
    cout << "List with duplicates: ";
    printList(dups);
    cout << "Search for 10 (finds first occurrence):" << endl;
    searchWithSteps(dups, 10);
    cout << endl;

    // Doubly linked list bidirectional traversal
    DNode* biDir = nullptr;
    DNode* biTail = nullptr;
    dInsertAtTail(biDir, biTail, 10);
    dInsertAtTail(biDir, biTail, 20);
    dInsertAtTail(biDir, biTail, 30);
    dInsertAtTail(biDir, biTail, 40);
    dInsertAtTail(biDir, biTail, 50);
    cout << "Doubly linked list — bidirectional traversal:" << endl;
    cout << "  Forward:  ";
    dPrintList(biDir);
    cout << "  ";
    dPrintReverse(biTail);
    cout << endl;

    // Clean up remaining allocated memory
    while (list1) deleteAtHead(list1);
    while (list2) deleteAtHead(list2);
    while (list3) deleteAtHead(list3);
    while (list4) deleteAtHead(list4);
    while (dups) deleteAtHead(dups);
    while (dHead) dDeleteAtHead(dHead, dTail);
    while (biDir) dDeleteAtHead(biDir, biTail);

    cout << "========================================" << endl;
    cout << "  All examples completed successfully!" << endl;
    cout << "========================================" << endl;

    return 0;
}

// ============================================================================
// KEY TEACHING POINTS
// ============================================================================
/*
 * 1. DYNAMIC MEMORY ALLOCATION:
 *    - Each node is individually allocated on the heap with `new`
 *    - Nodes can be scattered anywhere in memory — not contiguous
 *    - The list grows and shrinks dynamically — no fixed capacity
 *    - Every `new` must have a corresponding `delete` to avoid memory leaks
 *    - Contrast with arrays: arrays allocate a contiguous block up front
 *
 * 2. POINTER MANIPULATION:
 *    - Linked lists are built entirely on pointers connecting nodes
 *    - Insert: create node, update next/prev pointers to splice it in
 *    - Delete: update neighboring pointers to bypass the node, then free it
 *    - The order of pointer updates matters — changing too early can lose nodes
 *    - Always save a pointer to a node before unlinking it (or you lose access)
 *    - Drawing diagrams is essential for getting pointer updates right
 *
 * 3. O(1) HEAD INSERTION:
 *    - Inserting at the head requires no traversal — just pointer updates
 *    - Create new node, point it to current head, update head pointer
 *    - This makes linked lists ideal for stack implementations (push/pop at head)
 *    - Contrast with arrays: inserting at index 0 requires shifting ALL elements O(n)
 *    - Tail insertion is O(1) only if you maintain a tail pointer
 *
 * 4. O(n) ACCESS (NO RANDOM ACCESS):
 *    - To access the kth element, you must traverse k nodes from the head
 *    - There is no way to "jump" to an index — you must follow pointers one by one
 *    - arr[k] is O(1) for arrays, but the linked list equivalent is O(k)
 *    - This makes linked lists unsuitable for algorithms that need random access
 *    - Binary search does NOT work efficiently on linked lists
 *
 * 5. NO SHIFTING NEEDED:
 *    - Insert/delete at any known position requires only pointer updates — O(1)
 *    - No elements need to be moved — contrast with arrays where shifting is O(n)
 *    - The catch: FINDING the position is O(n) (you must traverse to it)
 *    - So total insert at arbitrary position = O(n) traverse + O(1) pointer update
 *    - But if you already have a pointer to the position, insertion is truly O(1)
 *
 * 6. MEMORY OVERHEAD (POINTERS):
 *    - Each singly linked list node stores: data + 1 pointer (next)
 *    - Each doubly linked list node stores: data + 2 pointers (prev, next)
 *    - For small data types (int = 4 bytes), a pointer (8 bytes on 64-bit) is significant overhead
 *    - An array of ints: 4 bytes per element, no overhead
 *    - A linked list of ints: 4 + 8 = 12 bytes per node (singly) or 4 + 16 = 20 bytes (doubly)
 *    - This overhead matters for large collections of small elements
 *
 * 7. SINGLY VS DOUBLY LINKED LIST TRADEOFFS:
 *    - Singly linked: less memory per node, simpler code, forward traversal only
 *    - Doubly linked: more memory per node, bidirectional traversal, O(1) tail delete
 *    - Singly linked: deleting a known node requires the PREVIOUS node (O(n) to find)
 *    - Doubly linked: deleting a known node is O(1) — prev pointer gives direct access
 *    - Singly linked: reversing requires a full traversal
 *    - Doubly linked: reverse traversal is trivial using prev pointers
 *    - Use singly when memory is tight and you only traverse forward
 *    - Use doubly when you need bidirectional traversal or O(1) tail operations
 *
 * 8. MEMORY LEAKS:
 *    - Every `new` must be balanced by a `delete` — otherwise memory is leaked
 *    - When removing a node, ALWAYS save a pointer to it before unlinking
 *    - Losing the only pointer to a node = permanent memory leak
 *    - When destroying a list, delete ALL nodes (not just the head pointer)
 *    - Common mistake: setting head = nullptr without deleting the nodes
 *    - In C++, consider using smart pointers (unique_ptr) to automate cleanup
 *    - Valgrind can detect memory leaks in C/C++ programs
 *
 * 9. SENTINEL NODES:
 *    - A sentinel (dummy) node is a special node at the head and/or tail
 *    - It simplifies code by eliminating special cases for empty lists
 *    - Without sentinel: every operation must check if head/tail is nullptr
 *    - With sentinel: the list always has at least one node, simplifying logic
 *    - The sentinel's data is unused — it serves only as a placeholder
 *    - Trade-off: slightly more memory for simpler, less error-prone code
 *    - Used internally by many standard library implementations
 *
 * 10. COMPARISON WITH ARRAYS:
 *    - Access: Array O(1) vs Linked List O(n) — arrays win for random access
 *    - Insert at head: Array O(n) vs Linked List O(1) — linked lists win
 *    - Insert at tail: Array O(1) amortized vs Linked List O(1) with tail pointer
 *    - Insert at middle: Array O(n) shift vs Linked List O(n) traverse + O(1) splice
 *    - Delete: Array O(n) shift vs Linked List O(1) at known node
 *    - Search: Both O(n) for unsorted, but arrays support binary search if sorted
 *    - Memory: Arrays are contiguous (cache-friendly), linked lists are scattered
 *    - Size: Arrays have fixed capacity (or amortized resize), linked lists grow freely
 *    - Choose based on your dominant operation pattern
 */
