/**
 * @file two_three_tree.cpp
 * @brief Implementation of a 2-3 Tree with teaching comments
 * @author CS 302 - Data Structures
 * @date 2026
 *
 * 2-3 Tree: A self-balancing search tree in which every internal node is
 * either a 2-NODE (one key, two children) or a 3-NODE (two keys, three
 * children), and ALL leaves are at the same depth.
 *
 *   2-node:        [ K ]               Keys in left  < K
 *                  /    \              Keys in right > K
 *                 L      R
 *
 *   3-node:        [ K1 | K2 ]         Keys in left   < K1
 *                  /    |    \         K1 < keys in middle < K2
 *                 L     M     R        Keys in right  > K2
 *
 * Because every leaf is always at the same level, the tree is PERFECTLY
 * height-balanced — no "mostly balanced" fudge the way AVL and red-black
 * are. That perfect balance is achieved not by rotations but by NODE SPLITS
 * that propagate from the insertion point back up to the root.
 *
 * Insertion (bottom-up with splits):
 *   1. Descend to the correct leaf.
 *   2. Add the new key to that leaf.
 *   3. If the leaf now holds 3 keys, SPLIT it: the middle key is promoted
 *      to the parent and the two remaining keys become separate 2-nodes.
 *   4. A split may overflow the parent, triggering another split — and so
 *      on up to the root. If the root itself splits, a new root is created
 *      and the tree grows UPWARD by one level.
 *
 * Growing at the ROOT (not at the leaves!) is exactly why all leaves stay
 * at the same depth. This is the single cleverest idea in the whole design.
 *
 * Time Complexity: Search O(log n), Insert O(log n)
 * Space Complexity: O(n)
 *
 * Best for: teaching balanced trees — the split rule is more intuitive than
 * rotations, and it's the direct conceptual ancestor of B-trees (which are
 * just 2-3 trees generalized to higher fan-out for disk storage).
 */

#include <iostream>
#include <vector>

using namespace std;

// ============================================================================
// NODE DEFINITION
// ============================================================================

/**
 * @brief A node in a 2-3 tree — holds 1 or 2 keys and 2 or 3 children
 *
 * We size the arrays slightly larger than the maximum (3 keys, 4 children)
 * so the insertion routine can temporarily overflow a node before splitting
 * it. In the final, stable tree numKeys is always 1 or 2.
 */
struct Node23 {
    int   keys[3];        // room for a transient 3-key overflow state
    Node23* children[4];  // corresponding transient overflow for children
    int   numKeys;        // 1 or 2 in a stable tree; briefly 3 mid-insert
    bool  leaf;

    Node23(int key) : numKeys(1), leaf(true) {
        keys[0] = key;
        for (int i = 0; i < 4; i++) children[i] = nullptr;
    }
    Node23() : numKeys(0), leaf(false) {
        for (int i = 0; i < 4; i++) children[i] = nullptr;
    }
};

// ============================================================================
// INSERTION RESULT (for communicating splits up the recursion stack)
// ============================================================================

/**
 * @brief Return value of the recursive insert helper
 *
 * When a recursive insert splits the child, it has to hand TWO things back
 * to the parent: the promoted middle key, and the new right-hand sibling.
 * We pack them into this struct. If `split` is false the insert completed
 * without propagating, and only `left` (= unchanged subtree root) matters.
 */
struct InsertResult {
    bool    split;      // did this insert cause a split?
    Node23* left;       // left node after split (or just the updated node)
    int     promoted;   // middle key that was promoted to the parent
    Node23* right;      // new right sibling born from the split
};

// ============================================================================
// INSERTION
// ============================================================================

/**
 * @brief Splits a node that momentarily holds 3 keys into two 2-nodes
 *
 * Given a transiently overflowing node with keys [k0, k1, k2] and (for
 * internal nodes) 4 children [c0, c1, c2, c3], produce:
 *
 *       left 2-node         PROMOTED         right 2-node
 *          [k0]    ───── k1 floats up ─────     [k2]
 *          /  \                                 /  \
 *         c0   c1                              c2   c3
 *
 * k1 is the middle key and it does NOT stay in either child — it's returned
 * in InsertResult::promoted so the parent can absorb it.
 *
 * Whoever calls split() is responsible for handing `promoted` off to the
 * parent, either by absorbing it into a 2-node (making it a 3-node) or by
 * triggering another split.
 */
InsertResult split(Node23* node) {
    Node23* left  = new Node23();
    Node23* right = new Node23();

    left->leaf  = node->leaf;
    right->leaf = node->leaf;

    left->numKeys  = 1;
    left->keys[0]  = node->keys[0];
    right->numKeys = 1;
    right->keys[0] = node->keys[2];

    if (!node->leaf) {
        left->children[0]  = node->children[0];
        left->children[1]  = node->children[1];
        right->children[0] = node->children[2];
        right->children[1] = node->children[3];
    }

    int promoted = node->keys[1];
    delete node;

    return {true, left, promoted, right};
}

/**
 * @brief Recursively inserts a key into the subtree rooted at `node`
 *
 * Returns an InsertResult describing what happened:
 *   - split = false → the subtree absorbed the key without restructuring;
 *                    `left` is the (possibly same) subtree root.
 *   - split = true  → the subtree root SPLIT; the caller must insert
 *                    `promoted` into itself and wire up `left`/`right`.
 *
 * The caller (another recursive frame or the top-level insert) then decides
 * how to react to a returning split.
 */
InsertResult insert23(Node23* node, int key) {
    // ----- BASE CASE: leaf -----
    if (node->leaf) {
        // Duplicate → no change.
        for (int i = 0; i < node->numKeys; i++) {
            if (node->keys[i] == key) return {false, node, 0, nullptr};
        }

        // Insert into the leaf's sorted key list (capacity is 3).
        int i = node->numKeys - 1;
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->numKeys++;

        // If the leaf now holds 3 keys, split it; otherwise we're done.
        if (node->numKeys == 3) return split(node);
        return {false, node, 0, nullptr};
    }

    // ----- RECURSIVE CASE: internal node -----
    // Pick which child to recurse into based on standard 2-3 ordering.
    int ci = 0;
    while (ci < node->numKeys && key > node->keys[ci]) {
        if (key == node->keys[ci]) return {false, node, 0, nullptr}; // duplicate
        ci++;
    }
    if (ci < node->numKeys && key == node->keys[ci]) {
        return {false, node, 0, nullptr};
    }

    InsertResult r = insert23(node->children[ci], key);

    if (!r.split) {
        node->children[ci] = r.left;
        return {false, node, 0, nullptr};
    }

    // ----- A child split: absorb the promoted key into this node -----
    // Shift keys/children right to make room at position `ci`.
    for (int i = node->numKeys; i > ci; i--) {
        node->keys[i] = node->keys[i - 1];
    }
    for (int i = node->numKeys + 1; i > ci + 1; i--) {
        node->children[i] = node->children[i - 1];
    }
    node->keys[ci]          = r.promoted;
    node->children[ci]      = r.left;
    node->children[ci + 1]  = r.right;
    node->numKeys++;

    // If this node now holds 3 keys, split it and propagate.
    if (node->numKeys == 3) return split(node);
    return {false, node, 0, nullptr};
}

/**
 * @brief Public insert — handles the special case of the ROOT splitting
 *
 * When a split propagates all the way up, the root itself has to split.
 * The caller (this function) catches that by noticing split=true at the
 * top and creating a BRAND NEW root holding the promoted key, with the
 * two split halves as its children. This is where the tree gains a level.
 *
 * It's also the ONLY place a 2-3 tree grows taller — splitting never adds
 * depth below, because a split replaces one node with two siblings at the
 * SAME depth. That's the invariant that keeps every leaf at the same level.
 */
Node23* insert(Node23* root, int key) {
    if (root == nullptr) return new Node23(key);

    InsertResult r = insert23(root, key);
    if (!r.split) return r.left;

    // Grow upward: build a new root from the promoted key.
    Node23* newRoot = new Node23();
    newRoot->leaf        = false;
    newRoot->numKeys     = 1;
    newRoot->keys[0]     = r.promoted;
    newRoot->children[0] = r.left;
    newRoot->children[1] = r.right;
    return newRoot;
}

// ============================================================================
// SEARCH
// ============================================================================

/**
 * @brief Search — at each node check every key, then descend into the
 *        correct child. Since nodes hold at most 2 keys this is still O(1)
 *        work per level, and the total work is O(log n).
 */
bool contains(Node23* node, int key) {
    while (node != nullptr) {
        for (int i = 0; i < node->numKeys; i++) {
            if (key == node->keys[i]) return true;
        }
        if (node->leaf) return false;

        // Pick the child to descend into.
        if (key < node->keys[0]) {
            node = node->children[0];
        } else if (node->numKeys == 1 || key < node->keys[1]) {
            node = node->children[1];
        } else {
            node = node->children[2];
        }
    }
    return false;
}

// ============================================================================
// PRINTING
// ============================================================================

/**
 * @brief Pretty-print: right subtree on top, left subtree on bottom
 *
 * For a 3-node we draw the right child, then print the node with both
 * keys, then the middle child, then the left child. For 2-nodes we print
 * as usual. Indentation corresponds to depth.
 */
void prettyPrint(Node23* node, int depth = 0) {
    if (node == nullptr) return;

    auto indent = [&]() { for (int i = 0; i < depth; i++) cout << "      "; };

    if (node->numKeys == 1) {
        prettyPrint(node->children[1], depth + 1);
        indent();
        cout << "[" << node->keys[0] << "]" << endl;
        prettyPrint(node->children[0], depth + 1);
    } else {
        // 3-node: print right child, then the node line with both keys, then
        // middle child (indented one extra to mark it as the middle slot),
        // then left child. When tilted 90° left the middle child sits
        // visually between the two keys of the parent.
        prettyPrint(node->children[2], depth + 1);
        indent();
        cout << "[" << node->keys[0] << " | " << node->keys[1] << "]" << endl;
        prettyPrint(node->children[1], depth + 1);
        prettyPrint(node->children[0], depth + 1);
    }
}

/**
 * @brief In-order traversal — visits keys in sorted order
 */
void inorderPrint(Node23* node) {
    if (node == nullptr) return;
    if (node->numKeys == 1) {
        inorderPrint(node->children[0]);
        cout << node->keys[0] << " ";
        inorderPrint(node->children[1]);
    } else {
        inorderPrint(node->children[0]);
        cout << node->keys[0] << " ";
        inorderPrint(node->children[1]);
        cout << node->keys[1] << " ";
        inorderPrint(node->children[2]);
    }
}

void destroy(Node23* node) {
    if (node == nullptr) return;
    if (!node->leaf) {
        for (int i = 0; i <= node->numKeys; i++) destroy(node->children[i]);
    }
    delete node;
}

// ============================================================================
// DEMONSTRATIONS
// ============================================================================

Node23* insertSequenceWithSteps(const vector<int>& keys) {
    Node23* root = nullptr;
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
    cout << "  2-3 Tree - Teaching Example" << endl;
    cout << "========================================" << endl;

    // ------------------------------------------------------------------
    // Example 1: Ascending insert 10..70
    //   Watch 3-nodes form, then split, growing the tree upward.
    // ------------------------------------------------------------------
    cout << "\nExample 1: Ascending sequence 10..70" << endl;
    cout << "-------------------------------------" << endl;
    {
        Node23* root = insertSequenceWithSteps({10, 20, 30, 40, 50, 60, 70});
        destroy(root);
    }

    // ------------------------------------------------------------------
    // Example 2: Mixed sequence — exercises splits at various depths.
    // ------------------------------------------------------------------
    cout << "\nExample 2: Mixed sequence 50,30,70,20,40,60,80,10,25,35" << endl;
    cout << "--------------------------------------------------------" << endl;
    {
        Node23* root = insertSequenceWithSteps({50, 30, 70, 20, 40, 60, 80, 10, 25, 35});
        destroy(root);
    }

    // ------------------------------------------------------------------
    // Example 3: Search a built tree.
    // ------------------------------------------------------------------
    cout << "\nExample 3: Search" << endl;
    cout << "------------------" << endl;
    {
        Node23* root = nullptr;
        for (int k : {15, 6, 18, 3, 7, 17, 20, 2, 4, 13, 9}) root = insert(root, k);
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
    cout << "  All 2-3 tree examples completed!" << endl;
    cout << "========================================" << endl;
    return 0;
}

// ============================================================================
// KEY TEACHING POINTS
// ============================================================================
/*
 * 1. NODE TYPES
 *    Every non-leaf node is either:
 *      - 2-NODE: 1 key, 2 children (like a normal BST node)
 *      - 3-NODE: 2 keys, 3 children (keys partition into 3 ranges)
 *    No other shape is allowed in a stable tree. During insertion a node
 *    MOMENTARILY becomes a "4-node" (3 keys, 4 children); the split rule
 *    immediately fixes this.
 *
 * 2. PERFECT BALANCE
 *    Because the tree grows at the ROOT (never at the leaves), every leaf
 *    is always at the same depth. There's no "approximate" balance — the
 *    tree is always perfectly height-balanced, unlike AVL (±1) or red-black
 *    (≤ 2×). Maximum height is log₂(n+1).
 *
 * 3. THE SPLIT RULE
 *    Whenever a node would hold 3 keys, split it:
 *      - k1 stays as a left 2-node
 *      - k3 stays as a right 2-node
 *      - k2 is PROMOTED to the parent
 *    The parent absorbs the promoted key. If that makes the parent overflow,
 *    the parent splits in turn. Splits cascade upward; if the ROOT splits,
 *    a new root is created with one key and two children, making the tree
 *    one level taller.
 *
 * 4. WHY INSERTION GROWS THE TREE FROM THE ROOT
 *    Every split replaces one node with two siblings at the SAME depth, so
 *    splits never add depth below. The ONLY way the tree gets taller is a
 *    root split, which adds exactly one level above. This is the secret
 *    behind the perfect-balance invariant.
 *
 * 5. RELATIONSHIP TO RED-BLACK TREES
 *    A red-black tree IS a 2-3-4 tree encoded in binary form: each black
 *    node plus its red children corresponds to a 2-, 3-, or 4-node. Every
 *    rotation/recoloring case is just the binary reflection of a 2-3-4
 *    split. (The closer cousin is the 2-3-4 tree, in the next file.)
 *
 * 6. RELATIONSHIP TO B-TREES
 *    A B-tree of order m is a 2-3 tree generalized to allow up to m-1 keys
 *    per node. B-trees (order 100+) are the data structure behind every
 *    modern database and filesystem index, because wider nodes mean fewer
 *    disk I/Os per search.
 *
 * 7. COMMON MISTAKES
 *    - Forgetting that a 3-node has THREE children, not two.
 *    - Splitting the wrong key — it's always the MIDDLE key that promotes.
 *    - Growing the tree at the leaves ("new leaf below existing leaf") —
 *      this is the wrong mental model and it breaks the balance invariant.
 *    - Failing to propagate a split upward when the parent also overflows.
 */
