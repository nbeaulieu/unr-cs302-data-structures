/**
 * @file red_black_tree.cpp
 * @brief Implementation of a Red-Black Tree (self-balancing BST) with teaching comments
 * @author CS 302 - Data Structures
 * @date 2026
 *
 * Red-Black Tree: A Binary Search Tree (BST) where every node carries one
 * extra bit of information — its COLOR, either RED or BLACK. By enforcing a
 * small set of color rules, the tree stays approximately balanced and all
 * operations remain O(log n) in the worst case.
 *
 * The five red-black properties (CLRS):
 *   1. Every node is either RED or BLACK.
 *   2. The root is BLACK.
 *   3. Every leaf (the conceptual NIL sentinel) is BLACK.
 *   4. If a node is RED, both its children are BLACK. (No two reds in a row.)
 *   5. For every node, every path from that node down to a NIL leaf contains
 *      the same number of BLACK nodes (this is the "black-height").
 *
 * Why this matters: properties 4 and 5 together force the longest root-to-leaf
 * path to be at most TWICE the length of the shortest, so height ≤ 2·log₂(n+1).
 * That bound is what gives us O(log n) operations.
 *
 * Implementation note: this file uses a single shared SENTINEL node (NIL) in
 * place of nullptr for all leaves and the root's parent. The sentinel is
 * always BLACK and never has its key or children inspected. This trick
 * (straight from CLRS chapter 13) eliminates a flood of "if (x == nullptr)"
 * checks from the fix-up code.
 *
 * Time Complexity: Search O(log n), Insert O(log n), Delete O(log n)
 * Space Complexity: O(n)
 *
 * Best for: write-heavy ordered maps. std::map, std::set, Java's TreeMap, and
 * the Linux kernel's CFS scheduler are all backed by red-black trees.
 */

#include <iostream>
#include <vector>
#include <string>

using namespace std;

// ============================================================================
// NODE DEFINITION
// ============================================================================

enum Color { RED, BLACK };

/**
 * @brief A single node in the red-black tree
 *
 * Unlike the AVL example, every node also stores a PARENT pointer. The
 * fix-up procedure walks UPWARD from the newly inserted node and needs to
 * inspect grandparents and uncles, which is much easier with parent pointers
 * than with a recursive call stack.
 */
struct RBNode {
    int key;
    Color color;
    RBNode* left;
    RBNode* right;
    RBNode* parent;

    RBNode(int k, Color c) : key(k), color(c),
                              left(nullptr), right(nullptr), parent(nullptr) {}
};

// ============================================================================
// THE TREE WRAPPER
// ============================================================================

/**
 * @brief Wraps the root pointer and the shared NIL sentinel
 *
 * Using a class here lets us hide the sentinel from the public interface and
 * makes the rotation/fix-up code less error-prone (no static globals).
 */
class RedBlackTree {
public:
    RBNode* root;
    RBNode* NIL;     // shared sentinel — always BLACK, never inspected

    RedBlackTree() {
        NIL = new RBNode(0, BLACK);
        NIL->left = NIL->right = NIL->parent = NIL;
        root = NIL;
    }

    ~RedBlackTree() {
        destroy(root);
        delete NIL;
    }

    void insert(int key);
    bool contains(int key) const;
    void prettyPrint() const { prettyPrintHelper(root, 0, "root"); }
    void inorderPrint() const { inorderHelper(root); cout << endl; }

private:
    void leftRotate(RBNode* x);
    void rightRotate(RBNode* x);
    void insertFixup(RBNode* z);
    void destroy(RBNode* node);
    void prettyPrintHelper(RBNode* node, int depth, const string& label) const;
    void inorderHelper(RBNode* node) const;
};

// ============================================================================
// ROTATIONS — same shape as AVL, but they also fix PARENT pointers
// ============================================================================

/**
 * @brief Left rotation
 *
 *       x                              y
 *      / \         leftRotate(x)      / \
 *     A   y       ──────────────▶    x   C
 *        / \                        / \
 *       B   C                      A   B
 *
 * The mechanics are identical to the AVL version, but we also have to:
 *   - update B's parent (now x, was y)
 *   - update y's parent (now x's old parent, was x)
 *   - update the GRANDPARENT's child pointer to point to y instead of x
 *   - update root if x was the root
 *
 * No height field to maintain — color rules replace heights as the balance
 * mechanism, and they're updated by insertFixup, not by the rotations.
 */
void RedBlackTree::leftRotate(RBNode* x) {
    RBNode* y = x->right;

    // 1. Move B (y's left subtree) to be x's new right child.
    x->right = y->left;
    if (y->left != NIL) y->left->parent = x;

    // 2. Splice y into x's old position by fixing the grandparent's pointer.
    y->parent = x->parent;
    if (x->parent == NIL) {
        root = y;                       // x was the root
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }

    // 3. Make x become y's left child.
    y->left = x;
    x->parent = y;
}

/**
 * @brief Right rotation — mirror image of leftRotate
 */
void RedBlackTree::rightRotate(RBNode* x) {
    RBNode* y = x->left;

    x->left = y->right;
    if (y->right != NIL) y->right->parent = x;

    y->parent = x->parent;
    if (x->parent == NIL) {
        root = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }

    y->right = x;
    x->parent = y;
}

// ============================================================================
// INSERTION
// ============================================================================

/**
 * @brief Inserts a key the same way a plain BST would, then calls insertFixup
 *
 * The new node is always colored RED. Why? Because adding a red leaf can NEVER
 * violate property 5 (every path has the same number of black nodes) — it
 * adds zero black nodes to every path through it. The only thing it might
 * violate is property 4 (no two reds in a row), and that's exactly what
 * insertFixup is designed to repair.
 *
 * Duplicates are ignored.
 */
void RedBlackTree::insert(int key) {
    // --- Standard BST descent, tracking the parent we will attach to ---
    RBNode* y = NIL;
    RBNode* x = root;
    while (x != NIL) {
        y = x;
        if (key == x->key) return;          // ignore duplicates
        x = (key < x->key) ? x->left : x->right;
    }

    // --- Create the new node and link it in ---
    RBNode* z = new RBNode(key, RED);
    z->parent = y;
    z->left = z->right = NIL;
    if (y == NIL) {
        root = z;                            // tree was empty
    } else if (key < y->key) {
        y->left = z;
    } else {
        y->right = z;
    }

    // --- Restore red-black properties ---
    insertFixup(z);
}

/**
 * @brief Repairs property 4 (no red parent of a red node) after an insert
 *
 * The newly inserted node z is RED. The only property that can be violated
 * is property 4, and only at z and its parent. We walk UP the tree, fixing
 * the violation at each level, until either:
 *   - z's parent becomes black (no more violation), or
 *   - z reaches the root (we'll color the root black at the end).
 *
 * At each step the structure of the violation depends on whether z's UNCLE
 * (parent's sibling) is RED or BLACK. There are SIX cases — three when the
 * parent is a left child, mirrored three when the parent is a right child.
 *
 *   Case 1: Uncle is RED
 *     ── Recolor: parent BLACK, uncle BLACK, grandparent RED.
 *        Move z up to grandparent and continue. (No rotation.)
 *
 *   Case 2: Uncle is BLACK and z is the "inner" grandchild (zig-zag)
 *     ── Rotate at the parent to convert into Case 3.
 *
 *   Case 3: Uncle is BLACK and z is the "outer" grandchild (straight line)
 *     ── Recolor parent BLACK, grandparent RED, then rotate at grandparent.
 *        Loop terminates.
 *
 * The pattern is much like AVL's LL/LR/RR/RL split, with case 1 being the
 * extra "just recolor" shortcut that red-black trees gain by trading strict
 * height balance for color balance.
 */
void RedBlackTree::insertFixup(RBNode* z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            // ----- Parent is a LEFT child of grandparent -----
            RBNode* uncle = z->parent->parent->right;

            if (uncle->color == RED) {
                // Case 1: just recolor and move up.
                z->parent->color       = BLACK;
                uncle->color           = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    // Case 2: zig-zag → rotate left at parent to straighten.
                    z = z->parent;
                    leftRotate(z);
                }
                // Case 3: straight line → recolor and rotate at grandparent.
                z->parent->color       = BLACK;
                z->parent->parent->color = RED;
                rightRotate(z->parent->parent);
            }
        } else {
            // ----- Parent is a RIGHT child of grandparent (mirror) -----
            RBNode* uncle = z->parent->parent->left;

            if (uncle->color == RED) {
                z->parent->color       = BLACK;
                uncle->color           = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(z);
                }
                z->parent->color       = BLACK;
                z->parent->parent->color = RED;
                leftRotate(z->parent->parent);
            }
        }
    }
    // Property 2: the root is always BLACK. If we propagated red all the way
    // up via case 1, this final assignment is what restores property 2.
    root->color = BLACK;
}

// ============================================================================
// SEARCH
// ============================================================================

/**
 * @brief Standard BST search — colors are irrelevant for lookup
 */
bool RedBlackTree::contains(int key) const {
    RBNode* x = root;
    while (x != NIL) {
        if (key == x->key) return true;
        x = (key < x->key) ? x->left : x->right;
    }
    return false;
}

// ============================================================================
// PRINTING / VISUALIZATION
// ============================================================================

void RedBlackTree::inorderHelper(RBNode* node) const {
    if (node == NIL) return;
    inorderHelper(node->left);
    cout << node->key << " ";
    inorderHelper(node->right);
}

/**
 * @brief Pretty-prints the tree sideways with each node's color
 *
 * Tilt your head 90° to the left to see the root at the top. Each node is
 * shown as either [B]value or [R]value, so students can verify the five
 * red-black properties at a glance.
 */
void RedBlackTree::prettyPrintHelper(RBNode* node, int depth, const string& label) const {
    if (node == NIL) return;
    prettyPrintHelper(node->right, depth + 1, "R");
    for (int i = 0; i < depth; i++) cout << "      ";
    cout << label << ":" << (node->color == RED ? "[R]" : "[B]") << node->key << endl;
    prettyPrintHelper(node->left, depth + 1, "L");
}

void RedBlackTree::destroy(RBNode* node) {
    if (node == NIL) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}

// ============================================================================
// DEMONSTRATIONS
// ============================================================================

void insertSequenceWithSteps(RedBlackTree& tree, const vector<int>& keys) {
    for (size_t i = 0; i < keys.size(); i++) {
        cout << "\n>>> Insert " << keys[i] << endl;
        tree.insert(keys[i]);
        tree.prettyPrint();
        cout << "    in-order: ";
        tree.inorderPrint();
    }
}

int main() {
    cout << "========================================" << endl;
    cout << "  Red-Black Tree - Teaching Example" << endl;
    cout << "========================================" << endl;

    // ------------------------------------------------------------------
    // Example 1: Ascending insert
    //   Watch how recoloring (case 1) and rotations (cases 2/3) interact
    //   to keep the tree shallow as keys arrive in sorted order.
    // ------------------------------------------------------------------
    cout << "\nExample 1: Ascending sequence 10..70" << endl;
    cout << "-------------------------------------" << endl;
    {
        RedBlackTree tree;
        insertSequenceWithSteps(tree, {10, 20, 30, 40, 50, 60, 70});
    }

    // ------------------------------------------------------------------
    // Example 2: Mixed sequence — exercises both case 1 and case 2/3.
    // ------------------------------------------------------------------
    cout << "\nExample 2: Mixed sequence 50,30,70,20,40,60,80,10" << endl;
    cout << "--------------------------------------------------" << endl;
    {
        RedBlackTree tree;
        insertSequenceWithSteps(tree, {50, 30, 70, 20, 40, 60, 80, 10});
    }

    // ------------------------------------------------------------------
    // Example 3: Search
    // ------------------------------------------------------------------
    cout << "\nExample 3: Search after building a small tree" << endl;
    cout << "-----------------------------------------------" << endl;
    {
        RedBlackTree tree;
        for (int k : {15, 6, 18, 3, 7, 17, 20, 2, 4, 13, 9}) tree.insert(k);
        tree.prettyPrint();
        cout << "in-order: ";
        tree.inorderPrint();
        for (int k : {7, 13, 99, 2}) {
            cout << "contains(" << k << ") = "
                 << (tree.contains(k) ? "yes" : "no") << endl;
        }
    }

    cout << "\n========================================" << endl;
    cout << "  All red-black examples completed!" << endl;
    cout << "========================================" << endl;
    return 0;
}

// ============================================================================
// KEY TEACHING POINTS
// ============================================================================
/*
 * 1. THE FIVE PROPERTIES (memorize these — every fix-up case enforces them)
 *    1) Every node is RED or BLACK.
 *    2) The root is BLACK.
 *    3) Every NIL leaf is BLACK.
 *    4) A RED node has only BLACK children. (No two reds in a row.)
 *    5) Every root-to-NIL path has the same number of BLACK nodes.
 *
 * 2. WHY HEIGHT IS O(log n)
 *    Properties 4 and 5 imply that the longest path is at most twice the
 *    shortest. Combined with property 5 (constant black-height) you get
 *    height ≤ 2·log₂(n+1), so all operations are O(log n).
 *
 * 3. WHY NEW NODES ARE INSERTED RED
 *    A red leaf adds 0 to the black-height of every path, so property 5 is
 *    automatically preserved. The only property that might break is 4, and
 *    insertFixup is built specifically to fix that.
 *
 * 4. THE THREE CASES (parent is a LEFT child; mirror three for right child)
 *    Case 1 — UNCLE IS RED
 *      Recolor parent and uncle BLACK, grandparent RED, jump up to grandparent.
 *      Cheap: no rotations. The "violation" moves up two levels.
 *    Case 2 — UNCLE IS BLACK and z is an INNER child (zig-zag)
 *      Rotate at the parent to straighten the zig-zag, then fall through.
 *    Case 3 — UNCLE IS BLACK and z is an OUTER child (straight line)
 *      Recolor parent BLACK, grandparent RED, rotate at grandparent. DONE.
 *
 * 5. THE SENTINEL TRICK (NIL)
 *    Using a single shared BLACK sentinel for all leaves means:
 *      - We never have to check for nullptr in fix-up code.
 *      - Property 3 is satisfied "for free" (NIL is permanently BLACK).
 *      - The root's parent points to NIL, simplifying root detection.
 *    Pay attention to never modifying NIL's key or color.
 *
 * 6. RED-BLACK vs AVL
 *    Red-black trees are LESS strictly balanced than AVL trees:
 *      - Red-black: height ≤ 2·log₂(n+1) (looser)
 *      - AVL:       height ≤ 1.44·log₂(n+2)  (tighter)
 *    Trade-off:
 *      - AVL does more rotations on insert/delete → faster lookups, slower writes.
 *      - Red-black does fewer rotations → slightly slower lookups, faster writes.
 *    Real-world ordered maps almost always pick red-black for the write speed:
 *    std::map, std::set, java.util.TreeMap, Linux's CFS scheduler.
 *
 * 7. RED-BLACK vs 2-3-4 TREES
 *    Every red-black tree corresponds to a 2-3-4 tree where each black node
 *    plus its red children form a single 2-3-4 node. The fix-up cases above
 *    are exactly the 2-3-4 split operations re-expressed in binary form.
 *    This is the cleanest mental model for "why red-black trees work."
 *
 * 8. COMMON MISTAKES
 *    - Forgetting to color the root BLACK at the end of insertFixup.
 *    - Forgetting to update parent pointers during a rotation.
 *    - Confusing case 2 (zig-zag) with case 3 (straight) and rotating wrong.
 *    - Mixing up the LEFT-child and RIGHT-child mirror cases.
 *    - Treating NIL as a normal node — never inspect its key.
 */
