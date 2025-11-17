#include "btree.h"
#include <cassert>

using namespace std;

const int DEFAULT_KEY = 0;

/*
NOTE: Please follow logic from CLRSv4 directly. Additionally, in cases 3a and 3b
please check for an immediate right sibling first.
*/

using namespace std;

// delete the key k from the btree
void BTree::remove(int k) { remove(root, k, true); }

// delete the key k from the btree rooted at x
void BTree::remove(Node *x, int k, bool x_root) {
  if (x == nullptr) {
    return;
  }

  int succeeds_k = find_k(x, k);

  bool k_in_x = succeeds_k != x->n && x->keys[succeeds_k] == k;
  if (k_in_x &&
      x->leaf) { // Case 1: the search arrives at a leaf node x that contains k
    assert(x->n >= t || x_root);
    const int k_index = succeeds_k;
    remove_leaf_key(x, k_index);
  } else if (k_in_x && (!x->leaf)) { // Case 2: The search arrives at an
                                     // internal node x that contains k
    const int k_index = succeeds_k;

    Node *left = x->c[k_index];
    Node *right = x->c[k_index + 1];

    bool left_has_atleast_t_keys = left->n >= t;
    bool left_has_t_minus_one_keys = left->n == t - 1;

    bool right_has_atleast_t_keys = right->n >= t;
    bool right_has_t_minus_one_keys = right->n == t - 1;

    if (left_has_atleast_t_keys) { // Case 2a, x.c_i has at least t keys
      int predecessor = max_key(left);

      x->keys[k_index] = predecessor;
      remove(left, predecessor);
    } else if (left_has_t_minus_one_keys && right_has_atleast_t_keys) { // Case 2b
      int successor = min_key(right);

      x->keys[k_index] = successor;
      remove(right, successor);
    } else if (left_has_t_minus_one_keys &&
               right_has_t_minus_one_keys) { // Case 2c
      assert(right->leaf);

      merge_left(left, right, k);

      // Remove k from x after merge
      remove_internal_key(x, k_index, k_index + 1);

      if (x_root &&
          x->n == 0) { // Still case 2c where x is the root but becomes empty.
                       // See page 516, paragraph below case 3b.
        Node *old_root = root;
        root = x->c[0];
        remove(root, k);

        delete old_root;
      } else {

        // Can finally recursively delete k from left
        remove(left, k);
      }
    }
  } else if (!k_in_x && !x->leaf) { // Case 3
    // The subtree containing k if k is in the tree
    Node *subtree_containing_k = x->c[succeeds_k];
    if (subtree_containing_k->n == t - 1) { // Case 3a or 3b
      Node *left_sibling = nullptr;
      Node *right_sibling = nullptr;

      if ((succeeds_k + 1) < (x->n + 1)) {
        right_sibling = x->c[succeeds_k + 1];
      }

      if (succeeds_k > 0) {
        left_sibling = x->c[succeeds_k - 1];
      }

      Node *sibling_with_t_keys = nullptr;
      if (right_sibling != nullptr && right_sibling->n == t) {
        sibling_with_t_keys = right_sibling;
      } else if (left_sibling != nullptr && left_sibling->n == t) {
        sibling_with_t_keys = left_sibling;
      }

      if (subtree_containing_k->n == t - 1 &&
          sibling_with_t_keys != nullptr) { // Case 3a
        if (sibling_with_t_keys ==
            right_sibling) { // Case 3a right sibling has t keys
          swap_right(x, subtree_containing_k, right_sibling, succeeds_k);
        } else { // Case 3a left sibling has t keys
          swap_left(x, subtree_containing_k, left_sibling, succeeds_k - 1);
        }
      } else { // Case 3b, Both siblings (or just one) must have t-1 keys
        assert(left_sibling != nullptr || right_sibling != nullptr);

        // Whether the sibling we're merging is going to be the right or left.
        // We check for right first per the pdf.
        bool sibling_is_right = right_sibling != nullptr;

        size_t dividing_key = sibling_is_right ? succeeds_k : succeeds_k - 1;
        if (sibling_is_right) {
          merge_left(subtree_containing_k, right_sibling,
                     x->keys[dividing_key]);
        } else {
          merge_right(subtree_containing_k, left_sibling,
                      x->keys[dividing_key]);
        }

        x->n--;

        size_t sibling_child_indice_to_remove =
            sibling_is_right ? succeeds_k + 1 : succeeds_k - 1;
        for (int i = sibling_child_indice_to_remove; i < x->n + 1; i++) {
          x->c[i] = x->c[i + 1];
        }

        if (x_root &&
            x->n == 0) { // Still case 3b where x is the root but becomes empty.
                         // See page 516, paragraph below case 3b.
          Node *old_root = root;
          root = x->c[0];

          subtree_containing_k = root;

          delete old_root;
        }
      }
    }

    remove(subtree_containing_k, k);
  }
}

// return the index i of the first key in the btree node x where k <= x.keys[i]
// if i = x.n then no such key exists
int BTree::find_k(Node *x, int k) {
  int i = 0;
  for (; i < x->n; i++) {
    if (k <= x->keys[i]) {
      break;
    }
  }

  return i;
}

// remove the key at index i from a btree leaf node x
// What to do if the indice is invalid? Currently just asserting
void BTree::remove_leaf_key(Node *x, int i) {
  assert(x != nullptr);
  assert(x->leaf == true);

  bool i_is_valid = i >= 0 && i < x->n;
  assert(i_is_valid);

  for (int j = i; j < (x->n - 1); j++) {
    x->keys[j] = x->keys[j + 1];
  }

  x->n--;

  if (x->n == 0 && x == root) {
    delete root;
    root = nullptr;
  }
}

// remove the key at index i and child at index j from a btree internal node x
void BTree::remove_internal_key(Node *x, int i, int j) {

  for (int m = i; m < x->n - 1; m++) {
    x->keys[m] = x->keys[m + 1];
  }
  x->n--;

  for (int k = j; k < x->n + 1; k++) {
    x->c[k] = x->c[j + 1];
  }
}

// return the max key in the btree rooted at node x
int BTree::max_key(Node *x) {
  if (x == nullptr) {
    return DEFAULT_KEY;
  }

  if (x->leaf) {
    return x->keys[x->n - 1];
  } else {
    int rightmost_child = t * 2 - 1;
    while (x->c[rightmost_child] == nullptr) {
      rightmost_child--;
    }

    return max_key(x->c[rightmost_child]);
  }
}

// return the min key in the btree rooted at node x
int BTree::min_key(Node *x) {
  if (x == nullptr) {
    return DEFAULT_KEY;
  }

  if (x->c[0] == nullptr) {
    return x->keys[0];
  } else {
    return min_key(x->c[0]);
  }
}

// merge key k and all keys and children from y into y's LEFT sibling x
void BTree::merge_left(Node *x, Node *y, int k) {
  assert(x != nullptr);
  assert(y != nullptr);
  assert(x->n == t - 1);
  assert(y->n == t - 1);

  x->keys[t - 1] = k;

  x->n = 2 * t - 1;
  for (int m = t; m < 2 * t - 1; m++) {
    x->keys[m] = y->keys[m - t];
  }

  for (int m = t; m < 2 * t; m++) {
    x->c[m] = y->c[m - t];
  }

  delete y;
}

// merge key k and all keys and children from y into y's RIGHT sibling x
void BTree::merge_right(Node *x, Node *y, int k) {
  assert(x != nullptr);
  assert(y != nullptr);
  assert(x->n == t - 1);
  assert(y->n == t - 1);

  // Shift all keys currently in x to the right side
  for (int i = 0; i < t - 1; i++) {
    x->keys[i + t] = x->keys[i];
  }

  // Fill in the empty left side with y now
  for (int i = 0; i < t - 1; i++) {
    x->keys[i] = y->keys[i];
  }

  // The key between the left side values and right side values will be k
  x->keys[t - 1] = k;

  // Update the number of keys in x
  x->n = 2 * t - 1;

  // Time to adjust children,

  // Move all the children in x to the right
  for (int i = 0; i < t; i++) {
    x->c[i + t] = x->c[i];
  }

  // Put y's children into x
  for (int i = 0; i < t; i++) {
    x->c[i] = y->c[i];
  }

  delete y;
}

// Give y an extra key by moving a key from its parent x down into y
// Move a key from y's LEFT sibling z up into x
// Move appropriate child pointer from z into y
// Let i be the index of the key dividing y and z in x
void BTree::swap_left(Node *x, Node *y, Node *z, int i) {
  assert(x != nullptr);
  assert(y != nullptr);
  assert(z != nullptr);

  bool y_is_not_full = y->n <= (2 * t - 1);
  assert(y_is_not_full);

  bool z_is_left_sibling_of_y = x->c[i] == z && x->c[i + 1] == y;
  assert(z_is_left_sibling_of_y);

  // We want to give `y` a key from it's parent `x`. Let's make some room by
  // pushing all keys of `y` up 1. Giving a free spot at the front.
  y->n++;
  for (int i = y->n - 1; i > 0; i--) {
    y->keys[i] = y->keys[i - 1];
  }

  // Let's not forget to push `y`'s children up too, giving a free spot to put
  // in `z`'s old rightmost child after `z` loses a key.
  for (int i = y->n; i > 0; i--) {
    y->c[i] = y->c[i - 1];
  }

  // Let's add a key to `y` by taking the dividing key and putting it in the
  // newly freed up first spot. The dividing key will be replaced by a key in
  // `z`.
  y->keys[0] = x->keys[i];

  // Replace the dividing key with the last key of the left sibling.
  x->keys[i] = z->keys[z->n - 1];

  // Let's move a child from `z` to `y` as `z` lost a child when losing a key
  // and `y` has an extra child spot from gaining a key
  y->c[0] = z->c[z->n];
  z->n--;
}

// Give y an extra key by moving a key from its parent x down into y
// Move a key from y's RIGHT sibling z up into x
// Move appropriate child pointer from z into y
// Let i be the index of the key dividing y and z in x
void BTree::swap_right(Node *x, Node *y, Node *z, int i) {
  assert(x != nullptr);
  assert(y != nullptr);
  assert(z != nullptr);

  bool y_is_not_full = y->n <= (2 * t - 1);
  assert(y_is_not_full);

  bool z_is_right_sibling_of_y = x->c[i] == y && x->c[i + 1] == z;
  assert(z_is_right_sibling_of_y);

  // Add a new key to `y`, taking the key at the dividing index.
  y->n++;
  y->keys[y->n - 1] = x->keys[i];

  // The key at the dividing index is replaced by the first key in the right
  // sibling. At this point, The original key `x->keys[i]` has been fully moved
  // from x to y.
  x->keys[i] = z->keys[0];

  // Since the first key of the right sibling (`z`) is being used to replace the
  // key at the dividing index, we need to move its first child over to y.
  // Remember, y has an empty child spot as y gained a key and z has one less
  // child as z is losing a key.
  y->c[y->n] = z->c[0];

  // Move all the keys and children in the right sibling (`z`) one spot from the
  // beginning because `z` is losing its first key.
  z->n--;
  for (int i = 0; i < z->n; i++) {
    z->keys[i] = z->keys[i + 1];
  }

  for (int i = 0; i < z->n + 1; i++) {
    z->c[i] = z->c[i + 1];
  }
}
