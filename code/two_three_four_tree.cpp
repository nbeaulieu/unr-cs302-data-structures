/**
 * @file two_three_four_tree.cpp
 * @brief Implementation of a 2-3-4 Tree (a.k.a. 2-4 Tree) with teaching comments
 * @author CS 302 - Data Structures
 * @date 2026
 *
 * 2-3-4 Tree: A self-balancing search tree in which every internal node is a
 * 2-NODE, 3-NODE, or 4-NODE, and ALL leaves are at the same depth.
 *
 *   2-node:  [ K ]               3-node:  [ K1 | K2 ]        4-node:  [ K1 | K2 | K3 ]
 *            /   \                       /    |    \                 /    |    |    \
 *           L     R                     L     M     R               L    M1   M2     R
 *
 * Inserting uses TOP-DOWN PREEMPTIVE SPLITTING: as we descend the tree
 * looking for the right leaf, whenever we encounter a 4-node we SPLIT it
 * immediately — BEFORE we even know if we'll need the room. The split's
 * middle key is pushed up into the parent; because we pre-split the parent
 * too on the way down, we can always guarantee the parent has room.
 *
 * Why preemptive? Because the root-split case can never cascade beyond the
 * parent above us: that parent was visited one step ago and we already
 * split it if needed. This makes insertion a SINGLE DOWNWARD PASS — no
 * returning up the recursion to propagate splits — and it's the direct
 * ancestor of the red-black insert algorithm.
 *
 * Contrast with the 2-3 tree in the neighboring file:
 *   - 2-3 tree:   bottom-up splitting. Splits propagate back up on return.
 *   - 2-3-4 tree: top-down preemptive splitting. One pass, no propagation.
 *
 * Time Complexity: Search O(log n), Insert O(log n)
 * Space Complexity: O(n)
 *
 * A 2-3-4 tree is EXACTLY isomorphic to a red-black tree: every black node
 * plus its red children corresponds to one 2-, 3-, or 4-node. Understanding
 * 2-3-4 trees is the clearest path to understanding red-black trees, which
 * are the production form of the same data structure.
 */

#include <iostream>
#include <vector>

using namespace std;

// ============================================================================
// NODE DEFINITION
// ============================================================================

/**
 * @brief A node in a 2-3-4 tree — holds 1, 2, or 3 keys and the matching
 *        number of children (2, 3, or 4 respectively)
 *
 * We never allow a node to hold 4 keys at rest. The split-on-the-way-down
 * invariant guarantees the caller has already made room in the parent, so
 * the only transient overflow happens inside splitChild() for a single
 * instant before it divides the 4-node into two 2-nodes.
 */
struct Node234 {
    int      keys[3];        // up to 3 keys in a stable node
    Node234* children[4];    // up to 4 children
    int      numKeys;        // 1, 2, or 3
    bool     leaf;

    Node234() : numKeys(0), leaf(true) {
        for (int i = 0; i < 4; i++) children[i] = nullptr;
    }
};

// ============================================================================
// SPLIT (the only rebalancing primitive — called top-down, never bottom-up)
// ============================================================================

/**
 * @brief Splits the 4-node `parent->children[i]` into two 2-nodes and
 *        promotes its middle key up into `parent`
 *
 * Precondition: parent->children[i] is a 4-node AND parent is NOT full
 * (so there is room to absorb one more key). The top-down insert enforces
 * both of these: we pre-split on the way down, so any parent we reach is
 * guaranteed to have room for one more key.
 *
 *        parent: [ ... ? ... ]                 parent: [ ... K2 ? ... ]
 *                     |                                    /      \
 *         [K1 | K2 | K3]                ───▶         [K1]          [K3]
 *         /  |    |   \                              /  \          /  \
 *        A   B    C    D                            A    B        C    D
 *
 * K2 is promoted; K1 becomes a 2-node left child; K3 becomes a 2-node
 * right child. Children of the original 4-node are redistributed A,B to
 * the left and C,D to the right.
 *
 * @param parent  the parent to insert the promoted middle key into
 * @param i       the child index of the 4-node to split
 */
void splitChild(Node234* parent, int i) {
    Node234* full  = parent->children[i];
    Node234* right = new Node234();
    right->leaf    = full->leaf;
    right->numKeys = 1;
    right->keys[0] = full->keys[2];
    if (!full->leaf) {
        right->children[0] = full->children[2];
        right->children[1] = full->children[3];
    }
    int promoted = full->keys[1];

    // Shrink the original 4-node down to a 2-node holding just the left key.
    full->numKeys = 1;

    // Make room in the parent: shift keys and child pointers right.
    for (int k = parent->numKeys; k > i; k--) {
        parent->keys[k] = parent->keys[k - 1];
    }
    for (int k = parent->numKeys + 1; k > i + 1; k--) {
        parent->children[k] = parent->children[k - 1];
    }
    parent->keys[i]          = promoted;
    parent->children[i + 1]  = right;
    parent->numKeys++;
}

// ============================================================================
// INSERTION — single top-down pass, splitting 4-nodes preemptively
// ============================================================================

/**
 * @brief Inserts a key into a node that is GUARANTEED not to be full
 *
 * Because the caller guarantees `node` has fewer than 3 keys, we never
 * overflow it by adding one more key. When recursing into a child we
 * first check whether the child is a 4-node; if so we split it BEFORE
 * descending — that's the preemptive rule in action.
 */
void insertNonFull(Node234* node, int key) {
    int i = node->numKeys - 1;

    if (node->leaf) {
        // Duplicate check.
        for (int k = 0; k < node->numKeys; k++) {
            if (node->keys[k] == key) return;
        }
        // Shift larger keys right to open a slot, then drop `key` into place.
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->numKeys++;
        return;
    }

    // Internal node: find the child `key` belongs in.
    while (i >= 0 && key < node->keys[i]) i--;
    i++;

    // Duplicate with this node's keys? Just return.
    if (i > 0 && node->keys[i - 1] == key) return;

    // Preemptive split: if the chosen child is a 4-node, split it now.
    if (node->children[i]->numKeys == 3) {
        splitChild(node, i);
        // The split may have promoted a key equal to `key`, or changed which
        // side of the split we should descend into.
        if (key == node->keys[i]) return;      // duplicate
        if (key > node->keys[i]) i++;
    }
    insertNonFull(node->children[i], key);
}

/**
 * @brief Top-level insert — handles the ONE situation in which the tree
 *        grows taller: when the root itself is a 4-node at insert time
 *
 * We build a brand-new empty root above the old root and split the old
 * root as its only child. That's the ONLY place the tree gains height.
 * Every other split happens between an existing parent and an existing
 * child, at the same depth as before.
 */
Node234* insert(Node234* root, int key) {
    if (root == nullptr) {
        root = new Node234();
        root->leaf        = true;
        root->numKeys     = 1;
        root->keys[0]     = key;
        return root;
    }
    if (root->numKeys == 3) {
        Node234* newRoot = new Node234();
        newRoot->leaf        = false;
        newRoot->numKeys     = 0;
        newRoot->children[0] = root;
        splitChild(newRoot, 0);
        insertNonFull(newRoot, key);
        return newRoot;
    }
    insertNonFull(root, key);
    return root;
}

// ============================================================================
// SEARCH
// ============================================================================

/**
 * @brief Standard multi-way search
 *
 * At each node we scan its (up to 3) keys in order and either find the
 * target, or descend into the child between the two keys the target falls
 * between. Because each level is O(1) work and the tree height is O(log n),
 * total search time is O(log n).
 */
bool contains(Node234* node, int key) {
    while (node != nullptr) {
        int i = 0;
        while (i < node->numKeys && key > node->keys[i]) i++;
        if (i < node->numKeys && key == node->keys[i]) return true;
        if (node->leaf) return false;
        node = node->children[i];
    }
    return false;
}

// ============================================================================
// PRINTING
// ============================================================================

/**
 * @brief Sideways pretty-print with rightmost child on top
 *
 * Tilt your head 90° left to read the tree upright. Each node shows its
 * keys separated by `|`, so a 2-node prints as [K], a 3-node as [K1|K2],
 * and a 4-node as [K1|K2|K3]. Tip: during an insert trace no node should
 * ever appear as a 4-node at rest — the print happens only BETWEEN inserts,
 * after the preemptive split logic has restored the invariant.
 */
void prettyPrint(Node234* node, int depth = 0) {
    if (node == nullptr) return;
    auto indent = [&]() { for (int i = 0; i < depth; i++) cout << "      "; };

    // Print children right-to-left so the rightmost sits at the top visually.
    for (int c = node->numKeys; c >= 0; c--) {
        if (!node->leaf) prettyPrint(node->children[c], depth + 1);
        if (c == node->numKeys) {
            indent();
            cout << "[";
            for (int k = 0; k < node->numKeys; k++) {
                cout << node->keys[k];
                if (k + 1 < node->numKeys) cout << " | ";
            }
            cout << "]" << endl;
        }
    }
}

/**
 * @brief In-order traversal — prints keys in sorted order
 */
void inorderPrint(Node234* node) {
    if (node == nullptr) return;
    for (int i = 0; i < node->numKeys; i++) {
        if (!node->leaf) inorderPrint(node->children[i]);
        cout << node->keys[i] << " ";
    }
    if (!node->leaf) inorderPrint(node->children[node->numKeys]);
}

void destroy(Node234* node) {
    if (node == nullptr) return;
    if (!node->leaf) {
        for (int i = 0; i <= node->numKeys; i++) destroy(node->children[i]);
    }
    delete node;
}

// ============================================================================
// DEMONSTRATIONS
// ============================================================================

Node234* insertSequenceWithSteps(const vector<int>& keys) {
    Node234* root = nullptr;
    for (size_t i = 0; i < keys.size(); i++) {
        cout << "\n>>> Insert " << keys[i] << endl;
        root = insert(root, keys[i]);
        prettyPrint(root);
        cout << "    in-order: ";
        inorderPrint(root);
        cout << endl;
    }
    return root;
}

int main() {
    cout << "========================================" << endl;
    cout << "  2-3-4 Tree - Teaching Example" << endl;
    cout << "========================================" << endl;

    // ------------------------------------------------------------------
    // Example 1: Ascending insert 10..80
    //   Watch how nodes fill up into 4-nodes, then get pre-split by the
    //   NEXT insert before descending. The tree grows in height exactly
    //   when the ROOT itself is a 4-node at insert time.
    // ------------------------------------------------------------------
    cout << "\nExample 1: Ascending sequence 10..80" << endl;
    cout << "-------------------------------------" << endl;
    {
        Node234* root = insertSequenceWithSteps({10, 20, 30, 40, 50, 60, 70, 80});
        destroy(root);
    }

    // ------------------------------------------------------------------
    // Example 2: Mixed sequence
    // ------------------------------------------------------------------
    cout << "\nExample 2: Mixed sequence" << endl;
    cout << "--------------------------" << endl;
    {
        Node234* root = insertSequenceWithSteps({50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 45, 55});
        destroy(root);
    }

    // ------------------------------------------------------------------
    // Example 3: Search
    // ------------------------------------------------------------------
    cout << "\nExample 3: Search" << endl;
    cout << "------------------" << endl;
    {
        Node234* root = nullptr;
        for (int k : {15, 6, 18, 3, 7, 17, 20, 2, 4, 13, 9}) root = insert(root, k);
        prettyPrint(root);
        cout << "in-order: ";
        inorderPrint(root);
        cout << endl;
        for (int k : {7, 13, 99, 2}) {
            cout << "contains(" << k << ") = "
                 << (contains(root, k) ? "yes" : "no") << endl;
        }
        destroy(root);
    }

    cout << "\n========================================" << endl;
    cout << "  All 2-3-4 tree examples completed!" << endl;
    cout << "========================================" << endl;
    return 0;
}

// ============================================================================
// KEY TEACHING POINTS
// ============================================================================
/*
 * 1. NODE TYPES
 *    - 2-node: 1 key,  2 children
 *    - 3-node: 2 keys, 3 children
 *    - 4-node: 3 keys, 4 children
 *    A node at rest is ALWAYS one of these three shapes. During insert a
 *    4-node is split into two 2-nodes + a promoted middle key before it
 *    ever causes trouble.
 *
 * 2. PREEMPTIVE SPLITTING (top-down)
 *    When descending the tree during insert, if we're about to step into a
 *    4-node we SPLIT IT FIRST. The middle key rises into the parent above
 *    (which we just came from, and we had already pre-split IT if full). So
 *    the parent is guaranteed to have room, and the split never cascades.
 *    Result: insert is a SINGLE DOWNWARD PASS.
 *
 * 3. CONTRAST WITH 2-3 TREE
 *    A 2-3 tree uses BOTTOM-UP splitting: find the leaf, split if needed,
 *    propagate splits on the return path. A 2-3-4 tree uses TOP-DOWN
 *    preemptive splitting. Both are correct; the 2-3-4 approach is cleaner
 *    for concurrent implementations and maps directly onto red-black code.
 *
 * 4. HEIGHT AND BALANCE
 *    Like the 2-3 tree, all leaves stay at the same depth because splits
 *    never add depth below — a split replaces one node with two siblings
 *    at the SAME level. The only height-increasing operation is a ROOT
 *    split, which adds exactly one level above. Height is O(log n).
 *
 * 5. ISOMORPHISM WITH RED-BLACK TREES
 *    This is the single most important takeaway. A red-black tree is just
 *    a 2-3-4 tree written in binary:
 *      - a 2-node          → one black node
 *      - a 3-node          → one black node with ONE red child
 *      - a 4-node          → one black node with TWO red children
 *    Splitting a 4-node corresponds exactly to the "uncle is red → recolor"
 *    case in red-black insertFixup. The 4-to-2 split pushing a key up is
 *    EXACTLY a recolor that pushes red up one level.
 *    If you can picture 2-3-4 splits, you understand red-black trees.
 *
 * 6. RELATIONSHIP TO B-TREES
 *    A 2-3-4 tree is the smallest B-tree: it's a B-tree of order 4. Real
 *    database B-trees have thousands of keys per node so disk I/O per
 *    lookup stays near 1 or 2, but the insert algorithm is the same idea,
 *    just with the split threshold raised.
 *
 * 7. COMMON MISTAKES
 *    - Forgetting to pre-split the ROOT when it's a 4-node (requires a
 *      special case that grows a new parent above).
 *    - After splitting a child on the way down, forgetting to RECOMPARE
 *      the search key against the promoted middle and possibly descend
 *      into the NEW right half instead.
 *    - Splitting by promoting the WRONG key (always the middle).
 *    - Trying to split bottom-up like a 2-3 tree — that works too, but
 *      defeats the whole point of 2-3-4's single-pass design.
 */
