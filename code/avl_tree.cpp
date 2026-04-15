/**
 * @file avl_tree.cpp
 * @brief Implementation of an AVL (self-balancing BST) with teaching comments
 * @author CS 302 - Data Structures
 * @date 2026
 *
 * AVL Tree: A Binary Search Tree (BST) that stays BALANCED after every insert
 * and delete. For every node the heights of its two subtrees differ by at most
 * one. That invariant guarantees the tree height stays O(log n), so search,
 * insert, and delete are all O(log n) in the worst case.
 *
 * Named after its inventors: Adelson-Velsky and Landis (1962) — the first
 * self-balancing BST ever described.
 *
 * Key idea:
 *   1. Do a normal BST insert.
 *   2. Walk back up to the root. At each ancestor, update the height and
 *      compute its balance factor = height(left) - height(right).
 *   3. If |balance factor| > 1 the node is unbalanced — apply one of four
 *      rotation cases (LL, RR, LR, RL) to restore balance.
 *
 * Time Complexity: Search O(log n), Insert O(log n), Delete O(log n)
 * Space Complexity: O(n) — one node per key, plus a height field per node
 *
 * Best for: dictionaries that need worst-case O(log n) lookups and where
 * reads dominate writes (AVL trees are MORE strictly balanced than red-black
 * trees, giving faster lookups but slightly more rotations on writes).
 */

#include <iostream>
#include <vector>
#include <algorithm>    // for std::max
#include <string>

using namespace std;

// ============================================================================
// NODE DEFINITION
// ============================================================================

/**
 * @brief A single node in the AVL tree
 *
 * Each node stores a key, pointers to left and right children, and the
 * height of the subtree rooted at this node. Storing the height avoids
 * recomputing it on every rebalance.
 */
struct AVLNode {
    int key;          // the value stored in this node
    AVLNode* left;    // pointer to left subtree (keys < this->key)
    AVLNode* right;   // pointer to right subtree (keys > this->key)
    int height;       // height of the subtree rooted here (leaf = 1)

    AVLNode(int k) : key(k), left(nullptr), right(nullptr), height(1) {}
};

// ============================================================================
// HEIGHT AND BALANCE HELPERS
// ============================================================================

/**
 * @brief Returns the height of a node, treating nullptr as height 0
 *
 * Defining the height of an empty subtree as 0 means a leaf has height 1,
 * and the formula height(n) = 1 + max(height(left), height(right)) works
 * uniformly without special-casing null children.
 */
int height(AVLNode* node) {
    return node == nullptr ? 0 : node->height;
}

/**
 * @brief Balance factor = height(left subtree) - height(right subtree)
 *
 * Interpretation:
 *   +2 or more → left-heavy, too tall on the left     (rotate right)
 *   +1, 0, -1  → balanced, AVL invariant holds
 *   -2 or less → right-heavy, too tall on the right   (rotate left)
 */
int balanceFactor(AVLNode* node) {
    return node == nullptr ? 0 : height(node->left) - height(node->right);
}

/**
 * @brief Recomputes a node's height from its children
 *
 * Call this after any structural change (rotation, child replacement).
 * A node's height is 1 plus the taller of its two children.
 */
void updateHeight(AVLNode* node) {
    node->height = 1 + max(height(node->left), height(node->right));
}

// ============================================================================
// ROTATIONS — the mechanical heart of an AVL tree
// ============================================================================

/**
 * @brief Right rotation — used to fix a LEFT-heavy subtree
 *
 *         y                           x
 *        / \      rotate right       / \
 *       x   C    ───────────────▶   A   y
 *      / \                             / \
 *     A   B                           B   C
 *
 * The old left child (x) becomes the new root of this subtree.
 * The old root (y) becomes x's right child.
 * x's old right child (B) is "handed off" to y as its new left child.
 *
 * BST ordering is preserved: A < x < B < y < C both before and after.
 *
 * @param y  the unbalanced (left-heavy) node we're rotating around
 * @return   the new root of this subtree (x)
 */
AVLNode* rotateRight(AVLNode* y) {
    AVLNode* x = y->left;       // x will be the new subtree root
    AVLNode* B = x->right;      // B is the "orphaned" middle subtree

    // Perform the rotation: x moves up, y moves down-right.
    x->right = y;
    y->left  = B;

    // Heights must be recomputed bottom-up: y is now lower, so update y first.
    updateHeight(y);
    updateHeight(x);

    return x;
}

/**
 * @brief Left rotation — used to fix a RIGHT-heavy subtree
 *
 *       x                             y
 *      / \         rotate left       / \
 *     A   y      ───────────────▶   x   C
 *        / \                       / \
 *       B   C                     A   B
 *
 * Mirror image of rotateRight. A < x < B < y < C is preserved.
 *
 * @param x  the unbalanced (right-heavy) node we're rotating around
 * @return   the new root of this subtree (y)
 */
AVLNode* rotateLeft(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* B = y->left;

    y->left  = x;
    x->right = B;

    updateHeight(x);
    updateHeight(y);

    return y;
}

// ============================================================================
// INSERTION
// ============================================================================

/**
 * @brief Recursively inserts a key and rebalances on the way back up
 *
 * Strategy:
 *   1. Do a plain BST insertion (recurse left or right).
 *   2. After the recursive call returns, update the current node's height.
 *   3. Compute its balance factor. If it's outside [-1, +1], the node is
 *      unbalanced and we apply ONE of FOUR rotation cases:
 *
 *   LEFT-LEFT  (LL):  balance > 1 AND key inserted into x's LEFT subtree
 *                     → single rotateRight(y)
 *
 *   RIGHT-RIGHT (RR): balance < -1 AND key inserted into x's RIGHT subtree
 *                     → single rotateLeft(y)
 *
 *   LEFT-RIGHT (LR):  balance > 1 AND key inserted into left child's RIGHT
 *                     → rotateLeft(left child), then rotateRight(y)
 *
 *   RIGHT-LEFT (RL):  balance < -1 AND key inserted into right child's LEFT
 *                     → rotateRight(right child), then rotateLeft(y)
 *
 * Key detail: we detect the case by comparing the new key to the unbalanced
 * node's CHILD, not by looking at the grandchild pointer. This works because
 * the new key is what caused the imbalance in the first place.
 *
 * Duplicate keys are ignored (AVL trees traditionally store a set of keys).
 *
 * @complexity Time O(log n) — one root-to-leaf path, constant work per level.
 * @complexity Space O(log n) — recursion stack depth is the tree height.
 */
AVLNode* avlInsert(AVLNode* node, int key) {
    // --- Step 1: standard BST insertion ---
    if (node == nullptr) {
        return new AVLNode(key);
    }
    if (key < node->key) {
        node->left = avlInsert(node->left, key);
    } else if (key > node->key) {
        node->right = avlInsert(node->right, key);
    } else {
        return node;   // duplicate: do nothing
    }

    // --- Step 2: update the height of this ancestor ---
    updateHeight(node);

    // --- Step 3: check balance factor and rotate if needed ---
    int bf = balanceFactor(node);

    // Left-Left: left-heavy AND new key went into the LEFT subtree's left side
    if (bf > 1 && key < node->left->key) {
        return rotateRight(node);
    }

    // Right-Right: right-heavy AND new key went into the RIGHT subtree's right side
    if (bf < -1 && key > node->right->key) {
        return rotateLeft(node);
    }

    // Left-Right: left-heavy AND new key went into the LEFT subtree's right side
    if (bf > 1 && key > node->left->key) {
        node->left = rotateLeft(node->left);   // first straighten the zig-zag
        return rotateRight(node);              // then single rotate right
    }

    // Right-Left: right-heavy AND new key went into the RIGHT subtree's left side
    if (bf < -1 && key < node->right->key) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    // Already balanced — return unchanged.
    return node;
}

// ============================================================================
// SEARCH
// ============================================================================

/**
 * @brief Standard BST search — the AVL invariant does not change the algorithm
 *
 * Because the tree is still a BST, we search by comparing: go left if smaller,
 * right if larger, stop if equal. Balance just means this loop is guaranteed
 * to run in O(log n) steps rather than (potentially) O(n).
 */
bool avlContains(AVLNode* node, int key) {
    while (node != nullptr) {
        if (key == node->key) return true;
        node = (key < node->key) ? node->left : node->right;
    }
    return false;
}

// ============================================================================
// PRINTING / VISUALIZATION
// ============================================================================

/**
 * @brief In-order traversal — prints keys in sorted order
 *
 * Because a BST's in-order traversal is sorted, this is a nice sanity check:
 * if the output isn't sorted, something is wrong with the tree.
 */
void inorderPrint(AVLNode* node) {
    if (node == nullptr) return;
    inorderPrint(node->left);
    cout << node->key << " ";
    inorderPrint(node->right);
}

/**
 * @brief Pretty-prints the tree sideways (right subtree on top, left on bottom)
 *
 * Tilt your head 90° to the left and the root is at the top. Each node is
 * shown with its height (h) and balance factor (bf) so students can verify
 * the AVL invariant visually.
 */
void prettyPrint(AVLNode* node, int depth = 0, const string& label = "root") {
    if (node == nullptr) return;
    prettyPrint(node->right, depth + 1, "R");
    for (int i = 0; i < depth; i++) cout << "      ";
    cout << label << ":" << node->key
         << " (h=" << node->height
         << ", bf=" << balanceFactor(node) << ")" << endl;
    prettyPrint(node->left, depth + 1, "L");
}

// ============================================================================
// CLEANUP
// ============================================================================

/**
 * @brief Post-order deletion — frees children before freeing the parent
 */
void destroyTree(AVLNode* node) {
    if (node == nullptr) return;
    destroyTree(node->left);
    destroyTree(node->right);
    delete node;
}

// ============================================================================
// DEMONSTRATIONS
// ============================================================================

/**
 * @brief Inserts a sequence of keys, printing the tree after each insert
 *
 * This is the most useful visualization for learning AVL trees: you get to
 * see exactly when each rotation fires and how it restores balance.
 */
AVLNode* insertSequenceWithSteps(const vector<int>& keys) {
    AVLNode* root = nullptr;
    for (size_t i = 0; i < keys.size(); i++) {
        cout << "\n>>> Insert " << keys[i] << endl;
        root = avlInsert(root, keys[i]);
        prettyPrint(root);
        cout << "    in-order: ";
        inorderPrint(root);
        cout << endl;
    }
    return root;
}

int main() {
    cout << "========================================" << endl;
    cout << "  AVL Tree - Teaching Example" << endl;
    cout << "========================================" << endl;

    // ------------------------------------------------------------------
    // Example 1: Left-Left case
    //   Insert 30, 20, 10 — a strictly descending sequence forces an
    //   LL imbalance at the root (30), fixed by a single right rotation.
    // ------------------------------------------------------------------
    cout << "\nExample 1: LEFT-LEFT case (single right rotation)" << endl;
    cout << "---------------------------------------------------" << endl;
    {
        AVLNode* root = insertSequenceWithSteps({30, 20, 10});
        destroyTree(root);
    }

    // ------------------------------------------------------------------
    // Example 2: Right-Right case
    //   Insert 10, 20, 30 — ascending forces an RR imbalance, fixed
    //   by a single left rotation.
    // ------------------------------------------------------------------
    cout << "\nExample 2: RIGHT-RIGHT case (single left rotation)" << endl;
    cout << "---------------------------------------------------" << endl;
    {
        AVLNode* root = insertSequenceWithSteps({10, 20, 30});
        destroyTree(root);
    }

    // ------------------------------------------------------------------
    // Example 3: Left-Right case
    //   Insert 30, 10, 20 — the "zig-zag" from 30 through 10 to 20
    //   requires a rotateLeft on the left child, then rotateRight on 30.
    // ------------------------------------------------------------------
    cout << "\nExample 3: LEFT-RIGHT case (rotate left then right)" << endl;
    cout << "----------------------------------------------------" << endl;
    {
        AVLNode* root = insertSequenceWithSteps({30, 10, 20});
        destroyTree(root);
    }

    // ------------------------------------------------------------------
    // Example 4: Right-Left case
    //   Mirror of example 3.
    // ------------------------------------------------------------------
    cout << "\nExample 4: RIGHT-LEFT case (rotate right then left)" << endl;
    cout << "----------------------------------------------------" << endl;
    {
        AVLNode* root = insertSequenceWithSteps({10, 30, 20});
        destroyTree(root);
    }

    // ------------------------------------------------------------------
    // Example 5: Larger sequence — watch rotations fire as the tree grows.
    //   Without AVL rebalancing, ascending {10,20,30,...,80} would degrade
    //   into a linked list of height 8. AVL keeps it at height 4.
    // ------------------------------------------------------------------
    cout << "\nExample 5: Larger ascending sequence (many rotations)" << endl;
    cout << "------------------------------------------------------" << endl;
    {
        AVLNode* root = insertSequenceWithSteps({10, 20, 30, 40, 50, 60, 70, 80});
        cout << "\nFinal tree height: " << height(root)
             << " (a plain BST would have height 8)" << endl;
        cout << "Contains 50? " << (avlContains(root, 50) ? "yes" : "no") << endl;
        cout << "Contains 99? " << (avlContains(root, 99) ? "yes" : "no") << endl;
        destroyTree(root);
    }

    cout << "\n========================================" << endl;
    cout << "  All AVL examples completed!" << endl;
    cout << "========================================" << endl;
    return 0;
}

// ============================================================================
// KEY TEACHING POINTS
// ============================================================================
/*
 * 1. WHY BALANCE?
 *    A plain BST has worst-case O(n) operations: insert 1,2,3,...,n in order
 *    and you get a linked list. AVL enforces height = O(log n) so search,
 *    insert, and delete are always O(log n).
 *
 * 2. THE INVARIANT
 *    For EVERY node: |height(left) - height(right)| <= 1. If this is ever
 *    violated (balance factor becomes +2 or -2), one rotation (or a double
 *    rotation) restores it.
 *
 * 3. FOUR ROTATION CASES
 *    - LL → single right rotation
 *    - RR → single left rotation
 *    - LR → left rotation on the child, then right rotation on the node
 *    - RL → right rotation on the child, then left rotation on the node
 *    LR and RL exist because a zig-zag shape can't be fixed by one rotation.
 *
 * 4. ROTATIONS ARE O(1) AND PRESERVE BST ORDER
 *    A rotation only reassigns a handful of pointers and updates two heights.
 *    In-order traversal of a rotated subtree is unchanged.
 *
 * 5. WHY WE RECURSE UP
 *    An insertion can only unbalance nodes on the path from the new leaf up
 *    to the root. Walking back up the recursion rebalances each such node
 *    at most once per insert.
 *
 * 6. AVL vs RED-BLACK
 *    AVL is MORE strictly balanced (≤ 1.44 log₂ n) than red-black (≤ 2 log₂ n).
 *    → AVL: faster lookups, more rotations on writes. Good for read-heavy use.
 *    → Red-black: fewer rotations, slightly taller. Good for write-heavy use
 *      (used by std::map, Linux's CFS scheduler, and Java's TreeMap).
 *
 * 7. COMMON MISTAKES
 *    - Forgetting to update heights after a rotation (heights go STALE).
 *    - Updating the parent's height BEFORE the rotated children's heights.
 *    - Using the balance factor of the child instead of comparing the
 *      inserted key when picking LL vs LR (either works but be consistent).
 *    - Not handling duplicate keys — decide up front whether you allow them.
 */
