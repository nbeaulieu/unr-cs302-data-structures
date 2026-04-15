/**
 * @file dictionary.cpp
 * @brief Implementation of a Hash Table (Dictionary/Map) with four collision resolution strategies
 * @author CS 302 - Data Structures
 * @date 2026
 *
 * Hash Table (Dictionary): A data structure that maps keys to values using a
 * hash function to compute an index into an array of buckets (slots).
 *
 * Key Feature: Average-case O(1) insert, search, and delete via hashing.
 * A hash function converts a key into an array index: index = hash(key) % capacity.
 *
 * Collision Resolution Strategies (all four demonstrated):
 *
 *   1. SEPARATE CHAINING
 *      Each bucket holds a linked list. Colliding entries are appended to the chain.
 *      + Simple, never "fills up," handles high load factors
 *      - Extra pointer overhead per entry
 *
 *   2. LINEAR PROBING (Open Addressing)
 *      On collision, check the next slot, then the next, etc. (index + 1, + 2, + 3...)
 *      + Cache-friendly (sequential memory access), no extra pointers
 *      - Suffers from "primary clustering" — long runs of filled slots
 *
 *   3. QUADRATIC PROBING (Open Addressing)
 *      On collision, check index + 1², + 2², + 3², etc.
 *      + Reduces primary clustering compared to linear probing
 *      - Can fail to find empty slots if load factor > 0.5 (unless capacity is prime)
 *
 *   4. DOUBLE HASHING (Open Addressing)
 *      On collision, use a second hash function for the step size: index + i * h2(key)
 *      + Each key gets its own probe sequence — no clustering at all
 *      - Requires a good second hash function; slightly more computation per probe
 *
 * Time Complexity: Insert O(1) avg, Search O(1) avg, Delete O(1) avg
 *                  Worst case O(n) if all keys collide
 * Space Complexity: O(n + capacity)
 *
 * Best for: Fast key-value lookup, frequency counting, caching, symbol tables
 */

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <cassert>

using namespace std;

// ════════════════════════════════════════════════════════════════════════════
// HASH FUNCTIONS
// ════════════════════════════════════════════════════════════════════════════
//
// Two common hashing approaches:
//
//   1. MODULAR HASHING (for integer keys)
//      index = key % capacity
//      Simple and fast. Works directly on numeric keys.
//
//   2. HORNER'S RULE (for string keys)
//      Treats the string as a polynomial and evaluates it efficiently.
//      hash = ((...((c₀ · R + c₁) · R + c₂) · R + c₃)...) % capacity
//      where R is a prime base (we use 31) and cᵢ are character codes.
//
// Both produce an index in [0, capacity - 1]. Collisions happen when two
// different keys produce the same index — this is inevitable and is why
// we need collision resolution strategies.
// ════════════════════════════════════════════════════════════════════════════

/**
 * @brief MODULAR HASHING — for integer keys
 *
 * The simplest hash function: index = key % capacity.
 *
 * Visual Example (capacity = 7):
 *   key = 15 → 15 % 7 = 1
 *   key = 22 → 22 % 7 = 1   ← COLLISION with 15!
 *   key = 29 → 29 % 7 = 1   ← COLLISION again! (all multiples of 7 apart)
 *   key = 10 → 10 % 7 = 3   (no collision)
 *
 * Why capacity should be PRIME:
 *   If capacity = 10 and keys are multiples of 5 (5, 10, 15, 20, ...):
 *     5 % 10 = 5,  10 % 10 = 0,  15 % 10 = 5,  20 % 10 = 0
 *   Only slots 0 and 5 are used — terrible distribution!
 *   With a prime like 7:
 *     5 % 7 = 5,  10 % 7 = 3,  15 % 7 = 1,  20 % 7 = 6
 *   Much better spread.
 *
 * @param key The integer key
 * @param capacity The number of buckets (should be prime for best distribution)
 * @return An index in [0, capacity - 1]
 *
 * @complexity Time: O(1)
 * @complexity Space: O(1)
 */
int modularHash(int key, int capacity) {
    return ((key % capacity) + capacity) % capacity;  // handles negative keys
}

/**
 * @brief HORNER'S RULE — for string keys
 *
 * Treats the string as a polynomial in base R (we use R = 31):
 *   For string s = c₀c₁c₂...cₖ:
 *   hash(s) = c₀·R^k + c₁·R^(k-1) + ... + cₖ₋₁·R + cₖ
 *
 * Horner's rule evaluates this WITHOUT computing large powers:
 *   hash = 0
 *   hash = hash · R + c₀        = c₀
 *   hash = hash · R + c₁        = c₀·R + c₁
 *   hash = hash · R + c₂        = c₀·R² + c₁·R + c₂
 *   ...and so on
 *
 * Each step: multiply accumulated value by R, add next character.
 *
 * Visual Example — "cat" with R = 31, capacity = 7:
 *
 *   Characters: 'c' = 99,  'a' = 97,  't' = 116
 *
 *   Step 0: hash = 0
 *   Step 1: hash = 0  · 31 + 99  = 99           (just 'c')
 *   Step 2: hash = 99 · 31 + 97  = 3069 + 97    = 3166    ('c'·31 + 'a')
 *   Step 3: hash = 3166 · 31 + 116 = 98146 + 116 = 98262  ('c'·31² + 'a'·31 + 't')
 *
 *   index = 98262 % 7 = 4   → "cat" goes into slot 4
 *
 * Why Horner's rule?
 *   1. Avoids computing R^k directly (which overflows for long strings)
 *   2. Each step is just multiply + add — O(k) total, no power function
 *   3. Uses all characters, so "cat" ≠ "act" ≠ "tac" (order matters)
 *
 * Why R = 31 (a prime)?
 *   - Odd prime avoids patterns with even-valued character codes
 *   - Small enough to avoid overflow quickly, large enough for spread
 *   - Widely used in practice (Java's String.hashCode() uses 31)
 *
 * Collision example — "cat" vs "act":
 *   "cat": ((99 · 31 + 97) · 31 + 116) % 7 = 98262 % 7 = 4
 *   "act": ((97 · 31 + 99) · 31 + 116) % 7 = 96324 % 7 = 2
 *   Different! The ordering of characters matters because of the powers of R.
 *   (Without the polynomial structure, anagrams would always collide.)
 *
 * @param key The string key
 * @return The raw unsigned hash value (caller applies % capacity)
 *
 * @complexity Time: O(k) where k = key length
 * @complexity Space: O(1)
 */
unsigned long hornersHash(const string& key) {
    unsigned long hash = 0;
    for (char c : key) {
        hash = hash * 31 + c;  // Horner's rule: multiply by R, add next char
    }
    return hash;
}

// Alias used by the hash table classes (applies Horner's rule for strings)
unsigned long hashString(const string& key) {
    return hornersHash(key);
}


// ════════════════════════════════════════════════════════════════════════════
// STRATEGY 1: SEPARATE CHAINING
// ════════════════════════════════════════════════════════════════════════════

/**
 * @brief Hash table using separate chaining for collision resolution
 *
 * Each bucket is a linked list (chain). When two keys hash to the same index,
 * both entries are stored in that bucket's chain.
 *
 * Visual Example (capacity = 7, 5 entries, one collision at bucket 3):
 *
 *   Bucket 0: []
 *   Bucket 1: [("apple", 5)]
 *   Bucket 2: []
 *   Bucket 3: [("banana", 3) → ("cherry", 8)]   ← collision: two entries share bucket 3
 *   Bucket 4: [("date", 12)]
 *   Bucket 5: []
 *   Bucket 6: [("fig", 7)]
 *
 * To find "cherry": hash → bucket 3, walk chain: skip "banana", match "cherry" → 8
 */

struct ChainEntry {
    string key;
    int value;
};

class ChainingHashTable {
private:
    vector<list<ChainEntry>> buckets;
    int numEntries;
    int capacity;

    int getIndex(const string& key) const {
        return hashString(key) % capacity;
    }

public:
    ChainingHashTable(int cap = 7) : buckets(cap), numEntries(0), capacity(cap) {}

    /**
     * @brief Inserts or updates a key-value pair using chaining
     *
     * Visual Example — Collision:
     *   add("cherry", 8) when bucket 3 already has ("banana", 3):
     *
     *   Before: Bucket 3: [("banana", 3)]
     *   hash("cherry") % 7 = 3   ← same bucket!
     *   Walk chain: "banana" ≠ "cherry" → end of chain
     *   Append new entry
     *   After:  Bucket 3: [("banana", 3) → ("cherry", 8)]
     *
     * @complexity Time: O(1) average, O(n) worst case (long chain)
     * @complexity Space: O(1)
     */
    void add(const string& key, int value) {
        int index = getIndex(key);

        // Walk the chain to check for existing key
        for (ChainEntry& entry : buckets[index]) {
            if (entry.key == key) {
                cout << "    [Chaining] add(\"" << key << "\", " << value
                     << "): Updated in bucket " << index << " (was " << entry.value << ")" << endl;
                entry.value = value;
                return;
            }
        }

        // Not found — append to chain
        buckets[index].push_back({key, value});
        numEntries++;
        cout << "    [Chaining] add(\"" << key << "\", " << value
             << "): Inserted into bucket " << index
             << " (chain length: " << buckets[index].size() << ")" << endl;
    }

    /**
     * @brief Searches for a key by walking the chain at its bucket
     *
     * Visual Example:
     *   get("cherry") → hash % 7 = 3
     *   Bucket 3: [("banana", 3) → ("cherry", 8)]
     *   Check "banana" — no match
     *   Check "cherry" — MATCH → return 8
     *
     * @complexity Time: O(1) average, O(n) worst case
     */
    int get(const string& key, bool& found) const {
        int index = getIndex(key);
        int comparisons = 0;

        for (const ChainEntry& entry : buckets[index]) {
            comparisons++;
            if (entry.key == key) {
                found = true;
                cout << "    [Chaining] get(\"" << key << "\"): Found in bucket " << index
                     << " after " << comparisons << " comparison(s) → " << entry.value << endl;
                return entry.value;
            }
        }

        found = false;
        cout << "    [Chaining] get(\"" << key << "\"): Not found (bucket " << index
             << ", " << comparisons << " comparison(s))" << endl;
        return -1;
    }

    /**
     * @brief Removes a key by unlinking it from its chain
     *
     * Visual Example:
     *   remove("banana") → hash % 7 = 3
     *   Before: Bucket 3: [("banana", 3) → ("cherry", 8)]
     *   Found "banana" → unlink from list
     *   After:  Bucket 3: [("cherry", 8)]
     *
     * @complexity Time: O(1) average, O(n) worst case
     */
    bool remove(const string& key) {
        int index = getIndex(key);

        for (auto it = buckets[index].begin(); it != buckets[index].end(); ++it) {
            if (it->key == key) {
                cout << "    [Chaining] remove(\"" << key << "\"): Removed from bucket " << index
                     << " (value was " << it->value << ")" << endl;
                buckets[index].erase(it);
                numEntries--;
                return true;
            }
        }

        cout << "    [Chaining] remove(\"" << key << "\"): Not found" << endl;
        return false;
    }

    void display() const {
        cout << "    Chaining Hash Table (" << numEntries << " entries, "
             << capacity << " buckets, load factor: "
             << (double)numEntries / capacity << ")" << endl;
        for (int i = 0; i < capacity; i++) {
            cout << "      Bucket " << i << ": ";
            if (buckets[i].empty()) {
                cout << "[]" << endl;
            } else {
                bool first = true;
                cout << "[";
                for (const ChainEntry& e : buckets[i]) {
                    if (!first) cout << " → ";
                    cout << "(\"" << e.key << "\", " << e.value << ")";
                    first = false;
                }
                cout << "]" << endl;
            }
        }
    }
};


// ════════════════════════════════════════════════════════════════════════════
// STRATEGY 2: LINEAR PROBING (Open Addressing)
// ════════════════════════════════════════════════════════════════════════════

/**
 * @brief Hash table using linear probing for collision resolution
 *
 * All entries are stored directly in the bucket array (no linked lists).
 * On collision, probe sequentially: try index, index+1, index+2, ...
 * wrapping around to 0 when reaching the end.
 *
 * Visual Example (capacity = 7):
 *
 *   add("apple", 5)  → hash = 1 → slot 1 is empty → place at 1
 *   add("banana", 3) → hash = 3 → slot 3 is empty → place at 3
 *   add("cherry", 8) → hash = 3 → slot 3 occupied → try 4 → empty → place at 4
 *   add("date", 12)  → hash = 4 → slot 4 occupied → try 5 → empty → place at 5
 *
 *   Index: [  0  ][  1  ][  2  ][  3  ][  4  ][  5  ][  6  ]
 *          [     ][ apl ][     ][ ban ][ chr ][ dat ][     ]
 *                                 ↑      ↑      ↑
 *                              home    probed  probed
 *                             for ban  from 3  from 4
 *
 *   Notice: "cherry" and "date" are displaced from their home bucket.
 *   This creates a CLUSTER of consecutive filled slots (3, 4, 5).
 *   Clusters grow and merge — this is "primary clustering."
 */

enum SlotState { EMPTY, OCCUPIED, DELETED };

struct ProbingEntry {
    string key;
    int value;
    SlotState state;
};

class LinearProbingHashTable {
private:
    vector<ProbingEntry> table;
    int numEntries;
    int capacity;

    int getIndex(const string& key) const {
        return hashString(key) % capacity;
    }

public:
    LinearProbingHashTable(int cap = 7) : table(cap, {"", 0, EMPTY}), numEntries(0), capacity(cap) {}

    /**
     * @brief Inserts or updates using linear probing: index + 0, + 1, + 2, ...
     *
     * Probe sequence: try index, index+1, index+2, ... (mod capacity)
     *
     * Visual Example — Collision with linear probe:
     *   add("cherry", 8): hash = 3
     *
     *   Probe 0: slot 3 → OCCUPIED ("banana") → not our key → probe next
     *   Probe 1: slot 4 → EMPTY → insert here!
     *
     *   [  0  ][  1  ][  2  ][  3  ][  4  ][  5  ][  6  ]
     *   [     ][ apl ][     ][ ban ][ chr ][     ][     ]
     *                                 ↑ home  ↑ landed
     *
     * @note On DELETED slots: remember it as a candidate for insertion, but keep
     *       probing to check if the key already exists further along the chain.
     *       Only use the saved DELETED slot if we reach EMPTY without finding the key.
     *       This prevents inserting a duplicate when a DELETED slot sits before
     *       the existing entry in the probe sequence.
     *
     * @complexity Time: O(1) average, O(n) worst case
     * @complexity Space: O(1)
     */
    void add(const string& key, int value) {
        int index = getIndex(key);
        int home = index;
        int probes = 0;
        int firstDeleted = -1;  // Remember first DELETED slot for reuse

        while (probes < capacity) {
            if (table[index].state == EMPTY) {
                // Empty slot — key is definitely not in the table
                // Insert at first DELETED slot if we passed one, otherwise here
                int insertAt = (firstDeleted != -1) ? firstDeleted : index;
                table[insertAt] = {key, value, OCCUPIED};
                numEntries++;
                cout << "    [Linear] add(\"" << key << "\", " << value
                     << "): Inserted at slot " << insertAt
                     << " (home: " << home << ", " << probes << " probe(s))" << endl;
                return;
            }
            if (table[index].state == DELETED) {
                // Remember this slot but keep probing — key may exist beyond it
                if (firstDeleted == -1) firstDeleted = index;
            } else if (table[index].key == key) {
                // Key already exists — update in place
                cout << "    [Linear] add(\"" << key << "\", " << value
                     << "): Updated at slot " << index << " (was " << table[index].value << ")" << endl;
                table[index].value = value;
                return;
            }
            // Collision — linear probe: move to next slot
            index = (index + 1) % capacity;
            probes++;
        }

        // Checked all slots without finding EMPTY — table is full of OCCUPIED/DELETED
        if (firstDeleted != -1) {
            table[firstDeleted] = {key, value, OCCUPIED};
            numEntries++;
            cout << "    [Linear] add(\"" << key << "\", " << value
                 << "): Inserted at DELETED slot " << firstDeleted << endl;
            return;
        }

        cout << "    [Linear] add(\"" << key << "\"): Table is full!" << endl;
    }

    /**
     * @brief Searches using linear probing
     *
     * Probe through consecutive slots until we find the key, an EMPTY slot
     * (key not present), or we've checked all slots.
     *
     * Visual Example:
     *   get("cherry"): hash = 3
     *   Probe 0: slot 3 → "banana" ≠ "cherry" → probe next
     *   Probe 1: slot 4 → "cherry" = "cherry" → FOUND! Return 8
     *
     * @note We do NOT stop at DELETED slots — the key may be beyond them.
     *       We only stop at EMPTY (never been used) — the key can't be past that.
     *
     * @complexity Time: O(1) average, O(n) worst case
     */
    int get(const string& key, bool& found) const {
        int index = getIndex(key);
        int probes = 0;

        while (probes < capacity) {
            if (table[index].state == EMPTY) {
                // Empty slot — key is not in the table
                found = false;
                cout << "    [Linear] get(\"" << key << "\"): Not found after "
                     << probes + 1 << " probe(s)" << endl;
                return -1;
            }
            if (table[index].state == OCCUPIED && table[index].key == key) {
                found = true;
                cout << "    [Linear] get(\"" << key << "\"): Found at slot " << index
                     << " after " << probes + 1 << " probe(s) → " << table[index].value << endl;
                return table[index].value;
            }
            // OCCUPIED (wrong key) or DELETED — keep probing
            index = (index + 1) % capacity;
            probes++;
        }

        found = false;
        cout << "    [Linear] get(\"" << key << "\"): Not found (table full, checked all slots)" << endl;
        return -1;
    }

    /**
     * @brief Removes using "lazy deletion" — marks the slot as DELETED
     *
     * We cannot simply set the slot to EMPTY because that would break the
     * probe chain for keys that were inserted past this slot.
     *
     * Visual Example:
     *   remove("banana") at slot 3:
     *
     *   Before: [     ][ apl ][     ][ ban ][ chr ][     ][     ]
     *   After:  [     ][ apl ][     ][ DEL ][ chr ][     ][     ]
     *                                  ↑ marked DELETED, not EMPTY
     *
     *   Why? If we set slot 3 to EMPTY, then get("cherry") would:
     *     hash = 3, slot 3 is EMPTY → conclude "cherry" not found!
     *   But "cherry" IS in the table at slot 4! The DELETED marker
     *   tells search to keep probing past this slot.
     *
     * @complexity Time: O(1) average, O(n) worst case
     */
    bool remove(const string& key) {
        int index = getIndex(key);
        int probes = 0;

        while (probes < capacity) {
            if (table[index].state == EMPTY) {
                cout << "    [Linear] remove(\"" << key << "\"): Not found" << endl;
                return false;
            }
            if (table[index].state == OCCUPIED && table[index].key == key) {
                cout << "    [Linear] remove(\"" << key << "\"): Marked DELETED at slot " << index
                     << " (value was " << table[index].value << ")" << endl;
                table[index].state = DELETED;  // Lazy deletion
                numEntries--;
                return true;
            }
            index = (index + 1) % capacity;
            probes++;
        }

        cout << "    [Linear] remove(\"" << key << "\"): Not found" << endl;
        return false;
    }

    void display() const {
        cout << "    Linear Probing Hash Table (" << numEntries << " entries, "
             << capacity << " slots)" << endl;
        for (int i = 0; i < capacity; i++) {
            cout << "      Slot " << i << ": ";
            if (table[i].state == EMPTY)
                cout << "[EMPTY]" << endl;
            else if (table[i].state == DELETED)
                cout << "[DELETED]" << endl;
            else
                cout << "(\"" << table[i].key << "\", " << table[i].value << ")" << endl;
        }
    }
};


// ════════════════════════════════════════════════════════════════════════════
// STRATEGY 3: QUADRATIC PROBING (Open Addressing)
// ════════════════════════════════════════════════════════════════════════════

/**
 * @brief Hash table using quadratic probing for collision resolution
 *
 * Instead of checking consecutive slots (linear), quadratic probing checks:
 *   index + 1², index + 2², index + 3², ...
 *
 * This "jumps" over clusters, reducing the primary clustering problem.
 *
 * Visual Example (capacity = 7):
 *
 *   add("apple", 5)  → hash = 1 → slot 1 empty → place at 1
 *   add("banana", 3) → hash = 3 → slot 3 empty → place at 3
 *   add("cherry", 8) → hash = 3 → collision!
 *       Probe 1: (3 + 1²) % 7 = 4 → empty → place at 4
 *   add("date", 12)  → hash = 4 → collision!
 *       Probe 1: (4 + 1²) % 7 = 5 → empty → place at 5
 *   add("elder", 1)  → hash = 3 → collision!
 *       Probe 1: (3 + 1²) % 7 = 4 → occupied → continue
 *       Probe 2: (3 + 2²) % 7 = 0 → empty → place at 0
 *
 *   Index: [  0  ][  1  ][  2  ][  3  ][  4  ][  5  ][  6  ]
 *          [ eld ][ apl ][     ][ ban ][ chr ][ dat ][     ]
 *            ↑                    ↑      ↑      ↑
 *          jumped                home  +1²    +1²
 *          to +2²               for    from   from
 *          from 3              ban/chr   3      4
 *
 *   Compare with linear probing: "elder" would go to slot 6 (the next
 *   consecutive slot after the cluster). With quadratic probing, it jumps
 *   to slot 0, breaking the cluster apart.
 */

class QuadraticProbingHashTable {
private:
    vector<ProbingEntry> table;
    int numEntries;
    int capacity;

    int getIndex(const string& key) const {
        return hashString(key) % capacity;
    }

public:
    QuadraticProbingHashTable(int cap = 7) : table(cap, {"", 0, EMPTY}), numEntries(0), capacity(cap) {}

    /**
     * @brief Inserts or updates using quadratic probing: index + 1², + 2², + 3², ...
     *
     * Probe sequence: try index + 0, index + 1, index + 4, index + 9, ... (mod capacity)
     *
     * Visual Example — Multiple collisions:
     *   add("elder", 1): hash = 3
     *
     *   Probe 0: slot (3 + 0) % 7 = 3 → OCCUPIED ("banana") → probe next
     *   Probe 1: slot (3 + 1) % 7 = 4 → OCCUPIED ("cherry") → probe next
     *   Probe 2: slot (3 + 4) % 7 = 0 → EMPTY → insert here!
     *
     * @note Quadratic probing may fail to find an empty slot even when one exists
     *       if the load factor > 0.5 and the capacity is not prime. Keep load factor
     *       below 0.5 for guaranteed insertion with a prime-sized table.
     *
     * @note On DELETED slots: remember it as a candidate, but keep probing to
     *       check if the key exists further along. Prevents duplicate entries.
     *
     * @complexity Time: O(1) average, O(n) worst case
     * @complexity Space: O(1)
     */
    void add(const string& key, int value) {
        int home = getIndex(key);
        int probes = 0;
        int firstDeleted = -1;

        while (probes < capacity) {
            int index = (home + probes * probes) % capacity;

            if (table[index].state == EMPTY) {
                int insertAt = (firstDeleted != -1) ? firstDeleted : index;
                table[insertAt] = {key, value, OCCUPIED};
                numEntries++;
                cout << "    [Quadratic] add(\"" << key << "\", " << value
                     << "): Inserted at slot " << insertAt
                     << " (home: " << home << ", " << probes << " probe(s))" << endl;
                return;
            }
            if (table[index].state == DELETED) {
                if (firstDeleted == -1) firstDeleted = index;
            } else if (table[index].key == key) {
                cout << "    [Quadratic] add(\"" << key << "\", " << value
                     << "): Updated at slot " << index << " (was " << table[index].value << ")" << endl;
                table[index].value = value;
                return;
            }
            probes++;
        }

        if (firstDeleted != -1) {
            table[firstDeleted] = {key, value, OCCUPIED};
            numEntries++;
            cout << "    [Quadratic] add(\"" << key << "\", " << value
                 << "): Inserted at DELETED slot " << firstDeleted << endl;
            return;
        }

        cout << "    [Quadratic] add(\"" << key << "\"): Could not insert (table full or cycle)" << endl;
    }

    /**
     * @brief Searches using quadratic probing
     *
     * Visual Example:
     *   get("elder"): hash = 3
     *   Probe 0: slot (3 + 0) = 3 → "banana" ≠ "elder"
     *   Probe 1: slot (3 + 1) = 4 → "cherry" ≠ "elder"
     *   Probe 2: slot (3 + 4) = 0 → "elder" = "elder" → FOUND!
     *
     * @complexity Time: O(1) average, O(n) worst case
     */
    int get(const string& key, bool& found) const {
        int home = getIndex(key);
        int probes = 0;

        while (probes < capacity) {
            int index = (home + probes * probes) % capacity;

            if (table[index].state == EMPTY) {
                found = false;
                cout << "    [Quadratic] get(\"" << key << "\"): Not found after "
                     << probes + 1 << " probe(s)" << endl;
                return -1;
            }
            if (table[index].state == OCCUPIED && table[index].key == key) {
                found = true;
                cout << "    [Quadratic] get(\"" << key << "\"): Found at slot " << index
                     << " after " << probes + 1 << " probe(s) → " << table[index].value << endl;
                return table[index].value;
            }
            probes++;
        }

        found = false;
        cout << "    [Quadratic] get(\"" << key << "\"): Not found (checked " << probes << " slots)" << endl;
        return -1;
    }

    /**
     * @brief Removes using lazy deletion (same rationale as linear probing)
     *
     * @complexity Time: O(1) average, O(n) worst case
     */
    bool remove(const string& key) {
        int home = getIndex(key);
        int probes = 0;

        while (probes < capacity) {
            int index = (home + probes * probes) % capacity;

            if (table[index].state == EMPTY) {
                cout << "    [Quadratic] remove(\"" << key << "\"): Not found" << endl;
                return false;
            }
            if (table[index].state == OCCUPIED && table[index].key == key) {
                cout << "    [Quadratic] remove(\"" << key << "\"): Marked DELETED at slot " << index
                     << " (value was " << table[index].value << ")" << endl;
                table[index].state = DELETED;
                numEntries--;
                return true;
            }
            probes++;
        }

        cout << "    [Quadratic] remove(\"" << key << "\"): Not found" << endl;
        return false;
    }

    void display() const {
        cout << "    Quadratic Probing Hash Table (" << numEntries << " entries, "
             << capacity << " slots)" << endl;
        for (int i = 0; i < capacity; i++) {
            cout << "      Slot " << i << ": ";
            if (table[i].state == EMPTY)
                cout << "[EMPTY]" << endl;
            else if (table[i].state == DELETED)
                cout << "[DELETED]" << endl;
            else
                cout << "(\"" << table[i].key << "\", " << table[i].value << ")" << endl;
        }
    }
};


// ════════════════════════════════════════════════════════════════════════════
// STRATEGY 4: DOUBLE HASHING (Open Addressing)
// ════════════════════════════════════════════════════════════════════════════

/**
 * @brief Hash table using double hashing for collision resolution
 *
 * Uses a SECOND hash function to compute the probe step size.
 * Each key gets its own unique step, so keys that collide at the same
 * home slot follow DIFFERENT probe sequences — eliminating both
 * primary clustering (linear) and secondary clustering (quadratic).
 *
 * Probe sequence:
 *   slot = (h1(key) + i * h2(key)) % capacity,  for i = 0, 1, 2, ...
 *
 * Second hash function:
 *   h2(key) = PRIME - (hash(key) % PRIME)
 *   where PRIME < capacity (we use 5 for capacity = 7)
 *   This guarantees h2 is always in [1, PRIME] — never zero!
 *
 * Visual Example (capacity = 7, PRIME2 = 5):
 *
 *   add("banana", 3)  → h1 = 3, step = 5-(hash%5) = ?  → slot 3 empty → place at 3
 *   add("cherry", 8)  → h1 = 3, collision!
 *       h2("cherry") = 5 - (hash("cherry") % 5) = step
 *       Probe 1: (3 + 1*step) % 7 → lands somewhere different from linear!
 *   add("elder", 1)   → h1 = 3, collision!
 *       h2("elder") = different step than "cherry"
 *       → follows a DIFFERENT probe path, avoiding clustering
 *
 *   Key insight: "cherry" and "elder" both hash to slot 3, but their
 *   SECOND hash gives them different step sizes, so they probe different
 *   sequences. Linear and quadratic probing give ALL keys at slot 3
 *   the same probe sequence.
 */

class DoubleHashingHashTable {
private:
    vector<ProbingEntry> table;
    int numEntries;
    int capacity;
    int prime2;  // Smaller prime for h2

    int h1(const string& key) const {
        return hashString(key) % capacity;
    }

    /**
     * @brief Second hash function: determines the probe step size
     *
     * h2(key) = prime2 - (hash(key) % prime2)
     *
     * This guarantees:
     *   - h2 is never 0 (range is [1, prime2])
     *   - Different keys get different step sizes
     *   - If prime2 and capacity are both prime, every slot is visited
     *
     * @return Step size in [1, prime2]
     */
    int h2(const string& key) const {
        return prime2 - (int)(hashString(key) % prime2);
    }

public:
    /**
     * @param cap Table capacity (should be prime)
     * @param p2 Smaller prime for h2 (must be < capacity, e.g., 5 for capacity 7)
     */
    DoubleHashingHashTable(int cap = 7, int p2 = 5)
        : table(cap, {"", 0, EMPTY}), numEntries(0), capacity(cap), prime2(p2) {}

    /**
     * @brief Inserts or updates using double hashing: index + i * h2(key)
     *
     * Visual Example:
     *   add("cherry", 8): h1 = 3, h2 = step
     *
     *   Probe 0: slot 3              → OCCUPIED → probe next
     *   Probe 1: slot (3 + step) % 7 → depends on step → insert if empty
     *
     *   Different keys get different steps, so "cherry" and "elder" follow
     *   entirely different probe sequences even though both start at slot 3.
     *
     * @complexity Time: O(1) average, O(n) worst case
     * @complexity Space: O(1)
     */
    void add(const string& key, int value) {
        int home = h1(key);
        int step = h2(key);
        int probes = 0;
        int firstDeleted = -1;

        while (probes < capacity) {
            int index = (home + probes * step) % capacity;

            if (table[index].state == EMPTY) {
                int insertAt = (firstDeleted != -1) ? firstDeleted : index;
                table[insertAt] = {key, value, OCCUPIED};
                numEntries++;
                cout << "    [Double] add(\"" << key << "\", " << value
                     << "): Inserted at slot " << insertAt
                     << " (h1: " << home << ", h2: " << step
                     << ", " << probes << " probe(s))" << endl;
                return;
            }
            if (table[index].state == DELETED) {
                if (firstDeleted == -1) firstDeleted = index;
            } else if (table[index].key == key) {
                cout << "    [Double] add(\"" << key << "\", " << value
                     << "): Updated at slot " << index << " (was " << table[index].value << ")" << endl;
                table[index].value = value;
                return;
            }
            probes++;
        }

        if (firstDeleted != -1) {
            table[firstDeleted] = {key, value, OCCUPIED};
            numEntries++;
            cout << "    [Double] add(\"" << key << "\", " << value
                 << "): Inserted at DELETED slot " << firstDeleted << endl;
            return;
        }

        cout << "    [Double] add(\"" << key << "\"): Could not insert (table full)" << endl;
    }

    /**
     * @brief Searches using double hashing
     *
     * @complexity Time: O(1) average, O(n) worst case
     */
    int get(const string& key, bool& found) const {
        int home = h1(key);
        int step = h2(key);
        int probes = 0;

        while (probes < capacity) {
            int index = (home + probes * step) % capacity;

            if (table[index].state == EMPTY) {
                found = false;
                cout << "    [Double] get(\"" << key << "\"): Not found after "
                     << probes + 1 << " probe(s)" << endl;
                return -1;
            }
            if (table[index].state == OCCUPIED && table[index].key == key) {
                found = true;
                cout << "    [Double] get(\"" << key << "\"): Found at slot " << index
                     << " after " << probes + 1 << " probe(s) → " << table[index].value << endl;
                return table[index].value;
            }
            probes++;
        }

        found = false;
        cout << "    [Double] get(\"" << key << "\"): Not found (checked all slots)" << endl;
        return -1;
    }

    /**
     * @brief Removes using lazy deletion
     *
     * @complexity Time: O(1) average, O(n) worst case
     */
    bool remove(const string& key) {
        int home = h1(key);
        int step = h2(key);
        int probes = 0;

        while (probes < capacity) {
            int index = (home + probes * step) % capacity;

            if (table[index].state == EMPTY) {
                cout << "    [Double] remove(\"" << key << "\"): Not found" << endl;
                return false;
            }
            if (table[index].state == OCCUPIED && table[index].key == key) {
                cout << "    [Double] remove(\"" << key << "\"): Marked DELETED at slot " << index
                     << " (value was " << table[index].value << ")" << endl;
                table[index].state = DELETED;
                numEntries--;
                return true;
            }
            probes++;
        }

        cout << "    [Double] remove(\"" << key << "\"): Not found" << endl;
        return false;
    }

    void display() const {
        cout << "    Double Hashing Hash Table (" << numEntries << " entries, "
             << capacity << " slots, prime2 = " << prime2 << ")" << endl;
        for (int i = 0; i < capacity; i++) {
            cout << "      Slot " << i << ": ";
            if (table[i].state == EMPTY)
                cout << "[EMPTY]" << endl;
            else if (table[i].state == DELETED)
                cout << "[DELETED]" << endl;
            else
                cout << "(\"" << table[i].key << "\", " << table[i].value << ")" << endl;
        }
    }
};


// ════════════════════════════════════════════════════════════════════════════
// DEMONSTRATIONS
// ════════════════════════════════════════════════════════════════════════════

/**
 * @brief Demonstrates both hash functions and shows how collisions arise
 */
void demoHashFunctions() {
    cout << "=== Demo 1: Hash Functions — Modular Hashing & Horner's Rule ===" << endl;
    cout << endl;

    // ── Modular Hashing (integer keys) ──
    cout << "  ┌─── Modular Hashing: index = key % capacity ───┐" << endl;
    cout << endl;

    int capacity = 7;
    int intKeys[] = {10, 15, 22, 29, 36, 43, 3};
    cout << "    capacity = " << capacity << " (prime — better distribution)" << endl;
    cout << endl;
    for (int k : intKeys) {
        int index = modularHash(k, capacity);
        cout << "    " << k << " % " << capacity << " = " << index << endl;
    }
    cout << endl;
    cout << "    Notice: 15, 22, 29 all map to index 1 (they differ by 7 = capacity)." << endl;
    cout << "    Keys that are multiples of capacity apart ALWAYS collide!" << endl;
    cout << endl;

    cout << "    Non-prime capacity makes it worse:" << endl;
    int badCap = 10;
    int evenKeys[] = {10, 20, 30, 40, 50, 15, 25};
    cout << "    capacity = " << badCap << " (non-prime)" << endl;
    for (int k : evenKeys) {
        cout << "    " << k << " % " << badCap << " = " << modularHash(k, badCap) << endl;
    }
    cout << "    → Multiples of 10 all collide at 0. Multiples of 5 only use 0 and 5." << endl;
    cout << "    → A prime capacity avoids these patterns." << endl;
    cout << endl;

    // ── Horner's Rule (string keys) ──
    cout << "  ┌─── Horner's Rule: hash = (...((0 · 31 + c₀) · 31 + c₁) · 31 + c₂...) ───┐" << endl;
    cout << endl;

    cout << "    Step-by-step for \"cat\" (R = 31):" << endl;
    string word = "cat";
    unsigned long h = 0;
    for (int i = 0; i < (int)word.size(); i++) {
        unsigned long prev = h;
        h = h * 31 + word[i];
        cout << "      Step " << i + 1 << ": hash = " << prev << " × 31 + "
             << (int)word[i] << " ('" << word[i] << "') = " << h << endl;
    }
    cout << "      index = " << h << " % 7 = " << h % 7 << endl;
    cout << endl;

    cout << "    Step-by-step for \"act\" (R = 31):" << endl;
    word = "act";
    h = 0;
    for (int i = 0; i < (int)word.size(); i++) {
        unsigned long prev = h;
        h = h * 31 + word[i];
        cout << "      Step " << i + 1 << ": hash = " << prev << " × 31 + "
             << (int)word[i] << " ('" << word[i] << "') = " << h << endl;
    }
    cout << "      index = " << h << " % 7 = " << h % 7 << endl;
    cout << endl;

    cout << "    \"cat\" → slot " << hornersHash("cat") % 7
         << ",  \"act\" → slot " << hornersHash("act") % 7
         << "  (anagrams hash DIFFERENTLY — order matters!)" << endl;
    cout << endl;

    // Show several strings and their indices
    cout << "    Distribution of fruit names (capacity = 7):" << endl;
    string fruits[] = {"apple", "banana", "cherry", "date", "elderberry", "fig", "grape"};
    int bucketCount[7] = {};
    for (const string& f : fruits) {
        int idx = hornersHash(f) % 7;
        bucketCount[idx]++;
        cout << "      \"" << f << "\" → " << hornersHash(f) << " % 7 = " << idx << endl;
    }
    cout << "    Bucket counts: ";
    for (int i = 0; i < 7; i++) cout << "[" << i << "]:" << bucketCount[i] << "  ";
    cout << endl;
    cout << "    Any bucket with count > 1 has collisions that need resolution." << endl;
    cout << endl;
}

/**
 * @brief Inserts the same keys into all three tables for direct comparison
 *
 * This demonstrates how each strategy handles the same collisions differently.
 */
void demoSideBySideInsert() {
    cout << "=== Demo 2: Side-by-Side Insert — Same Keys, Four Strategies ===" << endl;
    cout << endl;

    ChainingHashTable         chaining(7);
    LinearProbingHashTable    linear(7);
    QuadraticProbingHashTable quadratic(7);
    DoubleHashingHashTable    doubleH(7, 5);

    string keys[] = {"apple", "banana", "cherry", "date", "elderberry"};
    int vals[]    = {5, 3, 8, 12, 1};

    for (int i = 0; i < 5; i++) {
        cout << "  --- Inserting (\"" << keys[i] << "\", " << vals[i] << ") ---" << endl;
        chaining.add(keys[i], vals[i]);
        linear.add(keys[i], vals[i]);
        quadratic.add(keys[i], vals[i]);
        doubleH.add(keys[i], vals[i]);
        cout << endl;
    }

    cout << "  ┌─── Final Table States ───┐" << endl << endl;

    chaining.display();
    cout << endl;
    linear.display();
    cout << endl;
    quadratic.display();
    cout << endl;
    doubleH.display();
    cout << endl;
}

/**
 * @brief Demonstrates how each strategy handles searching after collisions
 */
void demoSideBySideSearch() {
    cout << "=== Demo 3: Side-by-Side Search ===" << endl;
    cout << endl;

    ChainingHashTable         chaining(7);
    LinearProbingHashTable    linear(7);
    QuadraticProbingHashTable quadratic(7);
    DoubleHashingHashTable    doubleH(7, 5);

    string keys[] = {"apple", "banana", "cherry", "date", "elderberry"};
    int vals[]    = {5, 3, 8, 12, 1};
    for (int i = 0; i < 5; i++) {
        chaining.add(keys[i], vals[i]);
        linear.add(keys[i], vals[i]);
        quadratic.add(keys[i], vals[i]);
        doubleH.add(keys[i], vals[i]);
    }

    cout << "  --- Searching for keys (compare probe counts) ---" << endl;
    bool found;

    cout << endl << "  Search for \"cherry\" (collides with others):" << endl;
    chaining.get("cherry", found);
    linear.get("cherry", found);
    quadratic.get("cherry", found);
    doubleH.get("cherry", found);

    cout << endl << "  Search for \"apple\" (no collision):" << endl;
    chaining.get("apple", found);
    linear.get("apple", found);
    quadratic.get("apple", found);
    doubleH.get("apple", found);

    cout << endl << "  Search for \"fig\" (not in table):" << endl;
    chaining.get("fig", found);
    linear.get("fig", found);
    quadratic.get("fig", found);
    doubleH.get("fig", found);
    cout << endl;
}

/**
 * @brief Demonstrates deletion and the DELETED marker in open addressing
 *
 * Key insight: In open addressing, we cannot simply set a slot to EMPTY
 * because that would break the probe chain for keys inserted past it.
 * Instead, we mark it DELETED — search probes past it, insert reuses it.
 */
void demoDeletion() {
    cout << "=== Demo 4: Deletion — Lazy Delete in Open Addressing ===" << endl;
    cout << endl;

    LinearProbingHashTable linear(7);

    cout << "  Insert three keys that collide at the same home bucket:" << endl;
    linear.add("banana", 3);
    linear.add("cherry", 8);   // will probe from banana's slot
    linear.add("elderberry", 1);
    cout << endl;
    linear.display();
    cout << endl;

    cout << "  Remove \"banana\" (the first in the cluster):" << endl;
    linear.remove("banana");
    cout << endl;
    linear.display();
    cout << endl;

    cout << "  Search for \"elderberry\" (must probe past DELETED slot):" << endl;
    bool found;
    linear.get("elderberry", found);
    cout << "    → " << (found ? "Still found!" : "LOST!") << " (DELETED marker keeps probe chain intact)" << endl;
    cout << endl;

    cout << "  If we had used EMPTY instead of DELETED, the search would stop" << endl;
    cout << "  at the former banana slot and incorrectly report \"elderberry\" not found." << endl;
    cout << endl;
}

/**
 * @brief Demonstrates primary clustering with linear probing vs quadratic
 *
 * Visual Comparison:
 *
 *   Linear probing — primary clustering:
 *     All collisions pile into a single consecutive run.
 *     [     ][ XXX ][ XXX ][ XXX ][ XXX ][ XXX ][     ]
 *              ←── one big cluster ──→
 *     New keys hashing ANYWHERE in the cluster must probe to the end.
 *
 *   Quadratic probing — breaks clusters:
 *     Collisions spread out with increasing jumps (1, 4, 9, ...).
 *     [ XXX ][     ][ XXX ][     ][ XXX ][     ][ XXX ]
 *     Keys are scattered, avoiding long consecutive runs.
 */
void demoClustering() {
    cout << "=== Demo 5: Clustering — Linear vs Quadratic vs Double ===" << endl;
    cout << endl;

    // Use a table where many keys hash to the same slot
    cout << "  Inserting 5 keys into a table of size 11:" << endl;
    cout << "  (Comparing how each strategy distributes colliding keys)" << endl;
    cout << endl;

    LinearProbingHashTable    linear(11);
    QuadraticProbingHashTable quadratic(11);
    DoubleHashingHashTable    doubleH(11, 7);

    // These keys are chosen to demonstrate clustering behavior
    string keys[] = {"a", "b", "c", "d", "e"};
    int vals[]    = {1, 2, 3, 4, 5};

    for (int i = 0; i < 5; i++) {
        cout << "  --- Insert (\"" << keys[i] << "\", " << vals[i] << ") ---" << endl;
        linear.add(keys[i], vals[i]);
        quadratic.add(keys[i], vals[i]);
        doubleH.add(keys[i], vals[i]);
        cout << endl;
    }

    cout << "  ┌─── Compare Final Layouts ───┐" << endl << endl;
    linear.display();
    cout << endl;
    quadratic.display();
    cout << endl;
    doubleH.display();
    cout << endl;

    cout << "  Linear: consecutive clusters." << endl;
    cout << "  Quadratic: fixed jumps (+1, +4, +9) — same path for all keys at same home." << endl;
    cout << "  Double: each key gets its OWN step from h2 — best distribution." << endl;
    cout << endl;
}

/**
 * @brief Practical use case: word frequency counting with chaining
 */
void demoWordFrequency() {
    cout << "=== Demo 6: Practical Use — Word Frequency Counter ===" << endl;
    cout << endl;

    string words[] = {"the", "cat", "sat", "on", "the", "mat", "the", "cat", "on", "the"};
    int numWords = 10;

    ChainingHashTable freq(7);

    cout << "  Counting words: ";
    for (int i = 0; i < numWords; i++) cout << "\"" << words[i] << "\" ";
    cout << endl << endl;

    for (int i = 0; i < numWords; i++) {
        bool found;
        int count = freq.get(words[i], found);
        if (found) {
            freq.add(words[i], count + 1);
        } else {
            freq.add(words[i], 1);
        }
    }
    cout << endl;

    cout << "  Final word frequencies:" << endl;
    freq.display();
    cout << endl;
}


// ═══ Main ══════════════════════════════════════════════════════════════════

int main() {
    cout << "╔══════════════════════════════════════════════════════════════╗" << endl;
    cout << "║   HASH TABLE (DICTIONARY)                                  ║" << endl;
    cout << "║   Hash Functions: Modular Hashing & Horner's Rule          ║" << endl;
    cout << "║   Collision: Chaining, Linear, Quadratic, Double Hashing   ║" << endl;
    cout << "║   CS 302 — Data Structures                                 ║" << endl;
    cout << "╚══════════════════════════════════════════════════════════════╝" << endl;
    cout << endl;

    demoHashFunctions();
    demoSideBySideInsert();
    demoSideBySideSearch();
    demoDeletion();
    demoClustering();
    demoWordFrequency();

    cout << "═══ All demonstrations complete. ═══" << endl;
    return 0;
}
