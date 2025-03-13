# Standard COIL/HOIL Usage Examples

This document provides examples of standard COIL/HOIL usage patterns for typical application development scenarios. These examples focus on common programming tasks that might be encountered when using COIL/HOIL as an intermediate representation.

## 1. Data Structure Operations

This example shows how to define and manipulate common data structures in HOIL.

### Linked List Implementation

```
MODULE "linked_list";

// Node type definition
TYPE list_node<T> {
    data: T,
    next: ptr<list_node<T>>
}

// List type definition
TYPE linked_list<T> {
    head: ptr<list_node<T>>,
    tail: ptr<list_node<T>>,
    length: i32
}

// Initialize a new linked list
FUNCTION list_init<T>() -> linked_list<T> {
    ENTRY:
        // Create empty list
        list = UNDEF linked_list<T>;
        
        // Initialize fields
        list.head = NULL;
        list.tail = NULL;
        list.length = 0;
        
        RET list;
}

// Create a new node
FUNCTION create_node<T>(data: T) -> ptr<list_node<T>> {
    ENTRY:
        // Allocate memory for node
        node_size = SIZEOF(list_node<T>);
        node_ptr = CALL malloc(node_size);
        
        // Check allocation success
        is_null = CMP_EQ node_ptr, NULL;
        BR is_null, ALLOC_FAIL, INIT_NODE;
        
    ALLOC_FAIL:
        RET NULL;
        
    INIT_NODE:
        // Cast to node pointer
        node = CONVERT node_ptr, ptr<list_node<T>>;
        
        // Initialize node
        data_ptr = ADD node, OFFSETOF(list_node<T>, data);
        STORE [data_ptr], data;
        
        next_ptr = ADD node, OFFSETOF(list_node<T>, next);
        STORE [next_ptr], NULL;
        
        RET node;
}

// Append element to the list
FUNCTION list_append<T>(list: ptr<linked_list<T>>, data: T) -> i32 {
    ENTRY:
        // Create new node
        node = CALL create_node<T>(data);
        
        // Check node creation success
        is_null = CMP_EQ node, NULL;
        BR is_null, APPEND_FAIL, APPEND_NODE;
        
    APPEND_FAIL:
        result = LOAD_I32 0;  // Failure
        RET result;
        
    APPEND_NODE:
        // Get list fields
        head_ptr = ADD list, OFFSETOF(linked_list<T>, head);
        tail_ptr = ADD list, OFFSETOF(linked_list<T>, tail);
        length_ptr = ADD list, OFFSETOF(linked_list<T>, length);
        
        head = LOAD [head_ptr];
        tail = LOAD [tail_ptr];
        length = LOAD [length_ptr];
        
        // Check if list is empty
        is_empty = CMP_EQ head, NULL;
        BR is_empty, APPEND_TO_EMPTY, APPEND_TO_TAIL;
        
    APPEND_TO_EMPTY:
        // Set head and tail to new node
        STORE [head_ptr], node;
        STORE [tail_ptr], node;
        BR ALWAYS, UPDATE_LENGTH;
        
    APPEND_TO_TAIL:
        // Set current tail's next to new node
        tail_next_ptr = ADD tail, OFFSETOF(list_node<T>, next);
        STORE [tail_next_ptr], node;
        
        // Update tail to new node
        STORE [tail_ptr], node;
        
    UPDATE_LENGTH:
        // Increment length
        new_length = ADD length, 1;
        STORE [length_ptr], new_length;
        
        result = LOAD_I32 1;  // Success
        RET result;
}

// Get element at specific index
FUNCTION list_get<T>(list: ptr<linked_list<T>>, index: i32) -> T {
    ENTRY:
        // Get list fields
        head_ptr = ADD list, OFFSETOF(linked_list<T>, head);
        length_ptr = ADD list, OFFSETOF(linked_list<T>, length);
        
        head = LOAD [head_ptr];
        length = LOAD [length_ptr];
        
        // Check bounds
        is_negative = CMP_LT index, 0;
        is_too_large = CMP_GE index, length;
        out_of_bounds = OR is_negative, is_too_large;
        
        BR out_of_bounds, GET_FAIL, GET_ELEMENT;
        
    GET_FAIL:
        // Return default value for type T
        default_val = UNDEF T;
        RET default_val;
        
    GET_ELEMENT:
        // Traverse to the node at index
        current = head;
        i = LOAD_I32 0;
        BR ALWAYS, TRAVERSE_CHECK;
        
    TRAVERSE_CHECK:
        is_at_index = CMP_EQ i, index;
        BR is_at_index, RETRIEVE_DATA, TRAVERSE_NEXT;
        
    TRAVERSE_NEXT:
        next_ptr = ADD current, OFFSETOF(list_node<T>, next);
        current = LOAD [next_ptr];
        
        i = ADD i, 1;
        BR ALWAYS, TRAVERSE_CHECK;
        
    RETRIEVE_DATA:
        // Get data from node
        data_ptr = ADD current, OFFSETOF(list_node<T>, data);
        data = LOAD [data_ptr];
        
        RET data;
}

// Delete a linked list and free all nodes
FUNCTION list_destroy<T>(list: ptr<linked_list<T>>) -> void {
    ENTRY:
        // Get head
        head_ptr = ADD list, OFFSETOF(linked_list<T>, head);
        head = LOAD [head_ptr];
        
        // Check if empty
        is_empty = CMP_EQ head, NULL;
        BR is_empty, CLEAR_LIST, FREE_NODES;
        
    FREE_NODES:
        current = head;
        BR ALWAYS, FREE_CHECK;
        
    FREE_CHECK:
        is_null = CMP_EQ current, NULL;
        BR is_null, CLEAR_LIST, FREE_NODE;
        
    FREE_NODE:
        // Save next pointer
        next_ptr = ADD current, OFFSETOF(list_node<T>, next);
        next = LOAD [next_ptr];
        
        // Free current node
        CALL free(current);
        
        // Move to next
        current = next;
        BR ALWAYS, FREE_CHECK;
        
    CLEAR_LIST:
        // Reset list fields
        head_ptr = ADD list, OFFSETOF(linked_list<T>, head);
        tail_ptr = ADD list, OFFSETOF(linked_list<T>, tail);
        length_ptr = ADD list, OFFSETOF(linked_list<T>, length);
        
        STORE [head_ptr], NULL;
        STORE [tail_ptr], NULL;
        STORE [length_ptr], 0;
        
        RET;
}
```

### Binary Tree Implementation

```
MODULE "binary_tree";

// Tree node definition
TYPE tree_node<T> {
    data: T,
    left: ptr<tree_node<T>>,
    right: ptr<tree_node<T>>
}

// Create a new tree node
FUNCTION create_tree_node<T>(data: T) -> ptr<tree_node<T>> {
    ENTRY:
        // Allocate memory for node
        node_size = SIZEOF(tree_node<T>);
        node_ptr = CALL malloc(node_size);
        
        // Check allocation success
        is_null = CMP_EQ node_ptr, NULL;
        BR is_null, ALLOC_FAIL, INIT_NODE;
        
    ALLOC_FAIL:
        RET NULL;
        
    INIT_NODE:
        // Cast to node pointer
        node = CONVERT node_ptr, ptr<tree_node<T>>;
        
        // Initialize node
        data_ptr = ADD node, OFFSETOF(tree_node<T>, data);
        STORE [data_ptr], data;
        
        left_ptr = ADD node, OFFSETOF(tree_node<T>, left);
        STORE [left_ptr], NULL;
        
        right_ptr = ADD node, OFFSETOF(tree_node<T>, right);
        STORE [right_ptr], NULL;
        
        RET node;
}

// Insert value into BST
FUNCTION bst_insert<T>(root: ptr<ptr<tree_node<T>>>, data: T) -> i32 {
    ENTRY:
        // Get the root node pointer
        root_node = LOAD [root];
        
        // Check if tree is empty
        is_null = CMP_EQ root_node, NULL;
        BR is_null, INSERT_AT_ROOT, INSERT_RECURSIVE;
        
    INSERT_AT_ROOT:
        // Create new root node
        new_node = CALL create_tree_node<T>(data);
        
        // Check creation success
        created = CMP_NE new_node, NULL;
        BR created, UPDATE_ROOT, INSERT_FAIL;
        
    UPDATE_ROOT:
        STORE [root], new_node;
        result = LOAD_I32 1;  // Success
        RET result;
        
    INSERT_RECURSIVE:
        // Call recursive insert helper
        result = CALL bst_insert_helper<T>(root_node, data);
        RET result;
        
    INSERT_FAIL:
        result = LOAD_I32 0;  // Failure
        RET result;
}

// Helper function for BST insertion
FUNCTION bst_insert_helper<T>(node: ptr<tree_node<T>>, data: T) -> i32 {
    ENTRY:
        // Get current node data
        data_ptr = ADD node, OFFSETOF(tree_node<T>, data);
        node_data = LOAD [data_ptr];
        
        // Compare data with node data (assumes T has comparison operators)
        is_less = CMP_LT data, node_data;
        BR is_less, INSERT_LEFT, CHECK_RIGHT;
        
    INSERT_LEFT:
        // Get left child
        left_ptr = ADD node, OFFSETOF(tree_node<T>, left);
        left = LOAD [left_ptr];
        
        // Check if left is null
        is_null = CMP_EQ left, NULL;
        BR is_null, CREATE_LEFT, RECURSE_LEFT;
        
    CREATE_LEFT:
        // Create new node for left
        new_node = CALL create_tree_node<T>(data);
        
        // Check creation success
        created = CMP_NE new_node, NULL;
        BR created, UPDATE_LEFT, INSERT_FAIL;
        
    UPDATE_LEFT:
        STORE [left_ptr], new_node;
        result = LOAD_I32 1;  // Success
        RET result;
        
    RECURSE_LEFT:
        // Recursively insert in left subtree
        result = CALL bst_insert_helper<T>(left, data);
        RET result;
        
    CHECK_RIGHT:
        // Check if data equals node data
        is_equal = CMP_EQ data, node_data;
        BR is_equal, INSERT_DUPLICATE, INSERT_RIGHT;
        
    INSERT_DUPLICATE:
        // In this implementation, duplicates are not allowed
        result = LOAD_I32 0;  // Failure
        RET result;
        
    INSERT_RIGHT:
        // Get right child
        right_ptr = ADD node, OFFSETOF(tree_node<T>, right);
        right = LOAD [right_ptr];
        
        // Check if right is null
        is_null = CMP_EQ right, NULL;
        BR is_null, CREATE_RIGHT, RECURSE_RIGHT;
        
    CREATE_RIGHT:
        // Create new node for right
        new_node = CALL create_tree_node<T>(data);
        
        // Check creation success
        created = CMP_NE new_node, NULL;
        BR created, UPDATE_RIGHT, INSERT_FAIL;
        
    UPDATE_RIGHT:
        STORE [right_ptr], new_node;
        result = LOAD_I32 1;  // Success
        RET result;
        
    RECURSE_RIGHT:
        // Recursively insert in right subtree
        result = CALL bst_insert_helper<T>(right, data);
        RET result;
        
    INSERT_FAIL:
        result = LOAD_I32 0;  // Failure
        RET result;
}

// Search for a value in the BST
FUNCTION bst_search<T>(root: ptr<tree_node<T>>, data: T) -> ptr<tree_node<T>> {
    ENTRY:
        // Check if tree is empty
        is_null = CMP_EQ root, NULL;
        BR is_null, SEARCH_FAIL, CHECK_NODE;
        
    SEARCH_FAIL:
        RET NULL;
        
    CHECK_NODE:
        // Get node data
        data_ptr = ADD root, OFFSETOF(tree_node<T>, data);
        node_data = LOAD [data_ptr];
        
        // Compare data with node data
        is_equal = CMP_EQ data, node_data;
        BR is_equal, FOUND, CONTINUE_SEARCH;
        
    FOUND:
        RET root;
        
    CONTINUE_SEARCH:
        // Determine which subtree to search
        is_less = CMP_LT data, node_data;
        BR is_less, SEARCH_LEFT, SEARCH_RIGHT;
        
    SEARCH_LEFT:
        // Get left child
        left_ptr = ADD root, OFFSETOF(tree_node<T>, left);
        left = LOAD [left_ptr];
        
        // Recursively search left subtree
        result = CALL bst_search<T>(left, data);
        RET result;
        
    SEARCH_RIGHT:
        // Get right child
        right_ptr = ADD root, OFFSETOF(tree_node<T>, right);
        right = LOAD [right_ptr];
        
        // Recursively search right subtree
        result = CALL bst_search<T>(right, data);
        RET result;
}

// In-order traversal (with callback function)
FUNCTION bst_inorder<T>(root: ptr<tree_node<T>>, 
                      callback: ptr<function(T) -> void>) -> void {
    ENTRY:
        // Check if tree is empty
        is_null = CMP_EQ root, NULL;
        BR is_null, TRAVERSE_DONE, TRAVERSE_TREE;
        
    TRAVERSE_TREE:
        // Get left child
        left_ptr = ADD root, OFFSETOF(tree_node<T>, left);
        left = LOAD [left_ptr];
        
        // Traverse left subtree
        CALL bst_inorder<T>(left, callback);
        
        // Process current node
        data_ptr = ADD root, OFFSETOF(tree_node<T>, data);
        data = LOAD [data_ptr];
        CALL [callback](data);
        
        // Get right child
        right_ptr = ADD root, OFFSETOF(tree_node<T>, right);
        right = LOAD [right_ptr];
        
        // Traverse right subtree
        CALL bst_inorder<T>(right, callback);
        
    TRAVERSE_DONE:
        RET;
}

// Free a binary tree
FUNCTION bst_destroy<T>(root: ptr<tree_node<T>>) -> void {
    ENTRY:
        // Check if tree is empty
        is_null = CMP_EQ root, NULL;
        BR is_null, DESTROY_DONE, DESTROY_TREE;
        
    DESTROY_TREE:
        // Get children
        left_ptr = ADD root, OFFSETOF(tree_node<T>, left);
        left = LOAD [left_ptr];
        
        right_ptr = ADD root, OFFSETOF(tree_node<T>, right);
        right = LOAD [right_ptr];
        
        // Recursively destroy subtrees
        CALL bst_destroy<T>(left);
        CALL bst_destroy<T>(right);
        
        // Free current node
        CALL free(root);
        
    DESTROY_DONE:
        RET;
}
```

## 2. Algorithm Implementations

The following examples demonstrate common algorithms implemented in HOIL.

### Sorting Algorithms

```
MODULE "sorting";

// Swap two elements in an array
FUNCTION swap(arr: ptr<i32>, i: i32, j: i32) -> void {
    ENTRY:
        // Calculate element addresses
        elem_size = SIZEOF(i32);
        
        i_offset = MUL i, elem_size;
        i_ptr = ADD arr, i_offset;
        
        j_offset = MUL j, elem_size;
        j_ptr = ADD arr, j_offset;
        
        // Load values
        i_val = LOAD [i_ptr];
        j_val = LOAD [j_ptr];
        
        // Swap values
        STORE [i_ptr], j_val;
        STORE [j_ptr], i_val;
        
        RET;
}

// Bubble Sort
FUNCTION bubble_sort(arr: ptr<i32>, n: i32) -> void {
    ENTRY:
        i = LOAD_I32 0;
        BR ALWAYS, OUTER_LOOP_CHECK;
        
    OUTER_LOOP_CHECK:
        // Check outer loop condition
        outer_cond = CMP_LT i, n;
        BR outer_cond, OUTER_LOOP_BODY, SORT_DONE;
        
    OUTER_LOOP_BODY:
        j = LOAD_I32 0;
        n_minus_i_minus_1 = SUB n, i;
        n_minus_i_minus_1 = SUB n_minus_i_minus_1, 1;
        BR ALWAYS, INNER_LOOP_CHECK;
        
    INNER_LOOP_CHECK:
        // Check inner loop condition
        inner_cond = CMP_LT j, n_minus_i_minus_1;
        BR inner_cond, INNER_LOOP_BODY, INNER_LOOP_DONE;
        
    INNER_LOOP_BODY:
        // Compare adjacent elements
        j_ptr = CALL get_element_ptr(arr, j);
        j_val = LOAD [j_ptr];
        
        j_plus_1 = ADD j, 1;
        j_plus_1_ptr = CALL get_element_ptr(arr, j_plus_1);
        j_plus_1_val = LOAD [j_plus_1_ptr];
        
        // Check if swap is needed
        need_swap = CMP_GT j_val, j_plus_1_val;
        BR need_swap, DO_SWAP, SKIP_SWAP;
        
    DO_SWAP:
        CALL swap(arr, j, j_plus_1);
        
    SKIP_SWAP:
        // Increment inner loop counter
        j = ADD j, 1;
        BR ALWAYS, INNER_LOOP_CHECK;
        
    INNER_LOOP_DONE:
        // Increment outer loop counter
        i = ADD i, 1;
        BR ALWAYS, OUTER_LOOP_CHECK;
        
    SORT_DONE:
        RET;
}

// Quick Sort (partition function)
FUNCTION partition(arr: ptr<i32>, low: i32, high: i32) -> i32 {
    ENTRY:
        // Choose pivot (rightmost element)
        pivot_ptr = CALL get_element_ptr(arr, high);
        pivot = LOAD [pivot_ptr];
        
        // Index of smaller element
        i = SUB low, 1;
        
        // Initialize loop counter
        j = low;
        BR ALWAYS, LOOP_CHECK;
        
    LOOP_CHECK:
        // Check loop condition
        loop_cond = CMP_LT j, high;
        BR loop_cond, LOOP_BODY, LOOP_DONE;
        
    LOOP_BODY:
        // Get current element
        j_ptr = CALL get_element_ptr(arr, j);
        j_val = LOAD [j_ptr];
        
        // Compare with pivot
        is_smaller = CMP_LE j_val, pivot;
        BR is_smaller, PROCESS_SMALLER, SKIP_SMALLER;
        
    PROCESS_SMALLER:
        // Increment index of smaller element
        i = ADD i, 1;
        
        // Swap elements
        CALL swap(arr, i, j);
        
    SKIP_SMALLER:
        // Increment loop counter
        j = ADD j, 1;
        BR ALWAYS, LOOP_CHECK;
        
    LOOP_DONE:
        // Place pivot in its correct position
        i_plus_1 = ADD i, 1;
        CALL swap(arr, i_plus_1, high);
        
        // Return partition index
        result = ADD i, 1;
        RET result;
}

// Quick Sort helper
FUNCTION quick_sort_helper(arr: ptr<i32>, low: i32, high: i32) -> void {
    ENTRY:
        // Check base case
        valid_range = CMP_LT low, high;
        BR valid_range, CONTINUE_SORT, SORT_DONE;
        
    CONTINUE_SORT:
        // Get partition index
        pi = CALL partition(arr, low, high);
        
        // Sort elements before and after partition
        pi_minus_1 = SUB pi, 1;
        CALL quick_sort_helper(arr, low, pi_minus_1);
        
        pi_plus_1 = ADD pi, 1;
        CALL quick_sort_helper(arr, pi_plus_1, high);
        
    SORT_DONE:
        RET;
}

// Quick Sort
FUNCTION quick_sort(arr: ptr<i32>, n: i32) -> void {
    ENTRY:
        // Check if array has at least 2 elements
        has_elements = CMP_GT n, 1;
        BR has_elements, START_SORT, SORT_DONE;
        
    START_SORT:
        low = LOAD_I32 0;
        high = SUB n, 1;
        CALL quick_sort_helper(arr, low, high);
        
    SORT_DONE:
        RET;
}

// Merge Sort (merge function)
FUNCTION merge(arr: ptr<i32>, left: i32, mid: i32, right: i32) -> void {
    ENTRY:
        // Calculate sizes of subarrays
        n1 = SUB mid, left;
        n1 = ADD n1, 1;
        
        n2 = SUB right, mid;
        
        // Create temporary arrays
        L_size = MUL n1, SIZEOF(i32);
        L = CALL malloc(L_size);
        
        R_size = MUL n2, SIZEOF(i32);
        R = CALL malloc(R_size);
        
        // Check allocation success
        L_null = CMP_EQ L, NULL;
        R_null = CMP_EQ R, NULL;
        alloc_failed = OR L_null, R_null;
        
        BR alloc_failed, CLEANUP, COPY_DATA;
        
    COPY_DATA:
        // Copy data to temporary arrays
        i = LOAD_I32 0;
        BR ALWAYS, COPY_L_CHECK;
        
    COPY_L_CHECK:
        copy_l_cond = CMP_LT i, n1;
        BR copy_l_cond, COPY_L_BODY, COPY_L_DONE;
        
    COPY_L_BODY:
        // Calculate source and destination indices
        src_idx = ADD left, i;
        src_ptr = CALL get_element_ptr(arr, src_idx);
        src_val = LOAD [src_ptr];
        
        dst_ptr = CALL get_element_ptr(L, i);
        STORE [dst_ptr], src_val;
        
        i = ADD i, 1;
        BR ALWAYS, COPY_L_CHECK;
        
    COPY_L_DONE:
        j = LOAD_I32 0;
        BR ALWAYS, COPY_R_CHECK;
        
    COPY_R_CHECK:
        copy_r_cond = CMP_LT j, n2;
        BR copy_r_cond, COPY_R_BODY, COPY_R_DONE;
        
    COPY_R_BODY:
        // Calculate source and destination indices
        src_idx = ADD mid, j;
        src_idx = ADD src_idx, 1;
        src_ptr = CALL get_element_ptr(arr, src_idx);
        src_val = LOAD [src_ptr];
        
        dst_ptr = CALL get_element_ptr(R, j);
        STORE [dst_ptr], src_val;
        
        j = ADD j, 1;
        BR ALWAYS, COPY_R_CHECK;
        
    COPY_R_DONE:
        // Merge the temporary arrays back
        i = LOAD_I32 0;
        j = LOAD_I32 0;
        k = left;
        BR ALWAYS, MERGE_CHECK;
        
    MERGE_CHECK:
        i_valid = CMP_LT i, n1;
        j_valid = CMP_LT j, n2;
        both_valid = AND i_valid, j_valid;
        
        BR both_valid, MERGE_COMPARE, CHECK_REMAINING;
        
    MERGE_COMPARE:
        // Compare elements from both arrays
        L_ptr = CALL get_element_ptr(L, i);
        L_val = LOAD [L_ptr];
        
        R_ptr = CALL get_element_ptr(R, j);
        R_val = LOAD [R_ptr];
        
        L_smaller = CMP_LE L_val, R_val;
        BR L_smaller, MERGE_FROM_L, MERGE_FROM_R;
        
    MERGE_FROM_L:
        arr_ptr = CALL get_element_ptr(arr, k);
        STORE [arr_ptr], L_val;
        
        i = ADD i, 1;
        BR ALWAYS, MERGE_NEXT;
        
    MERGE_FROM_R:
        arr_ptr = CALL get_element_ptr(arr, k);
        STORE [arr_ptr], R_val;
        
        j = ADD j, 1;
        BR ALWAYS, MERGE_NEXT;
        
    MERGE_NEXT:
        k = ADD k, 1;
        BR ALWAYS, MERGE_CHECK;
        
    CHECK_REMAINING:
        // Check if elements remain in L
        i_remaining = CMP_LT i, n1;
        BR i_remaining, COPY_REMAINING_L, CHECK_REMAINING_R;
        
    COPY_REMAINING_L:
        // Copy remaining elements from L
        L_ptr = CALL get_element_ptr(L, i);
        L_val = LOAD [L_ptr];
        
        arr_ptr = CALL get_element_ptr(arr, k);
        STORE [arr_ptr], L_val;
        
        i = ADD i, 1;
        k = ADD k, 1;
        BR ALWAYS, CHECK_REMAINING;
        
    CHECK_REMAINING_R:
        // Check if elements remain in R
        j_remaining = CMP_LT j, n2;
        BR j_remaining, COPY_REMAINING_R, CLEANUP;
        
    COPY_REMAINING_R:
        // Copy remaining elements from R
        R_ptr = CALL get_element_ptr(R, j);
        R_val = LOAD [R_ptr];
        
        arr_ptr = CALL get_element_ptr(arr, k);
        STORE [arr_ptr], R_val;
        
        j = ADD j, 1;
        k = ADD k, 1;
        BR ALWAYS, CHECK_REMAINING_R;
        
    CLEANUP:
        // Free temporary arrays
        CALL free(L);
        CALL free(R);
        
        RET;
}

// Merge Sort helper
FUNCTION merge_sort_helper(arr: ptr<i32>, left: i32, right: i32) -> void {
    ENTRY:
        // Check base case
        valid_range = CMP_LT left, right;
        BR valid_range, CONTINUE_SORT, SORT_DONE;
        
    CONTINUE_SORT:
        // Calculate middle point
        mid = ADD left, right;
        mid = DIV mid, 2;
        
        // Sort first and second halves
        CALL merge_sort_helper(arr, left, mid);
        
        mid_plus_1 = ADD mid, 1;
        CALL merge_sort_helper(arr, mid_plus_1, right);
        
        // Merge the sorted halves
        CALL merge(arr, left, mid, right);
        
    SORT_DONE:
        RET;
}

// Merge Sort
FUNCTION merge_sort(arr: ptr<i32>, n: i32) -> void {
    ENTRY:
        // Check if array has at least 2 elements
        has_elements = CMP_GT n, 1;
        BR has_elements, START_SORT, SORT_DONE;
        
    START_SORT:
        left = LOAD_I32 0;
        right = SUB n, 1;
        CALL merge_sort_helper(arr, left, right);
        
    SORT_DONE:
        RET;
}

// Utility function to get pointer to array element
FUNCTION get_element_ptr(arr: ptr<i32>, index: i32) -> ptr<i32> {
    ENTRY:
        elem_size = SIZEOF(i32);
        offset = MUL index, elem_size;
        ptr = ADD arr, offset;
        RET ptr;
}
```

### Graph Algorithms

```
MODULE "graph_algorithms";

// Graph representations

// Adjacency Matrix Graph
TYPE matrix_graph {
    vertices: i32,
    matrix: ptr<ptr<i32>>
}

// Adjacency List Node
TYPE adj_list_node {
    vertex: i32,
    weight: i32,
    next: ptr<adj_list_node>
}

// Adjacency List Graph
TYPE list_graph {
    vertices: i32,
    adj_lists: ptr<ptr<adj_list_node>>
}

// Create a new adjacency matrix graph
FUNCTION create_matrix_graph(vertices: i32) -> ptr<matrix_graph> {
    ENTRY:
        // Allocate graph structure
        graph_size = SIZEOF(matrix_graph);
        graph_ptr = CALL malloc(graph_size);
        
        // Check allocation success
        is_null = CMP_EQ graph_ptr, NULL;
        BR is_null, CREATE_FAIL, INIT_GRAPH;
        
    CREATE_FAIL:
        RET NULL;
        
    INIT_GRAPH:
        // Set number of vertices
        graph = CONVERT graph_ptr, ptr<matrix_graph>;
        vertices_ptr = ADD graph, OFFSETOF(matrix_graph, vertices);
        STORE [vertices_ptr], vertices;
        
        // Allocate matrix rows
        row_ptr_size = MUL vertices, SIZEOF(ptr<i32>);
        matrix = CALL malloc(row_ptr_size);
        
        is_matrix_null = CMP_EQ matrix, NULL;
        BR is_matrix_null, FREE_GRAPH, ALLOC_ROWS;
        
    FREE_GRAPH:
        CALL free(graph);
        RET NULL;
        
    ALLOC_ROWS:
        // Store matrix pointer
        matrix_ptr = ADD graph, OFFSETOF(matrix_graph, matrix);
        STORE [matrix_ptr], matrix;
        
        // Allocate each row
        i = LOAD_I32 0;
        BR ALWAYS, ROW_ALLOC_CHECK;
        
    ROW_ALLOC_CHECK:
        row_done = CMP_GE i, vertices;
        BR row_done, INIT_MATRIX, ALLOC_ROW;
        
    ALLOC_ROW:
        // Allocate row
        row_size = MUL vertices, SIZEOF(i32);
        row = CALL malloc(row_size);
        
        is_row_null = CMP_EQ row, NULL;
        BR is_row_null, FREE_PREV_ROWS, SET_ROW;
        
    FREE_PREV_ROWS:
        // Free previously allocated rows
        j = LOAD_I32 0;
        BR ALWAYS, FREE_ROW_CHECK;
        
    FREE_ROW_CHECK:
        free_done = CMP_GE j, i;
        BR free_done, FREE_MATRIX, FREE_ROW;
        
    FREE_ROW:
        // Get row pointer
        row_offset = MUL j, SIZEOF(ptr<i32>);
        row_ptr_ptr = ADD matrix, row_offset;
        row_ptr = LOAD [row_ptr_ptr];
        
        // Free row
        CALL free(row_ptr);
        
        j = ADD j, 1;
        BR ALWAYS, FREE_ROW_CHECK;
        
    FREE_MATRIX:
        CALL free(matrix);
        CALL free(graph);
        RET NULL;
        
    SET_ROW:
        // Set row pointer in matrix
        row_offset = MUL i, SIZEOF(ptr<i32>);
        row_ptr_ptr = ADD matrix, row_offset;
        STORE [row_ptr_ptr], row;
        
        i = ADD i, 1;
        BR ALWAYS, ROW_ALLOC_CHECK;
        
    INIT_MATRIX:
        // Initialize matrix with zeros
        i = LOAD_I32 0;
        BR ALWAYS, INIT_ROW_CHECK;
        
    INIT_ROW_CHECK:
        row_done = CMP_GE i, vertices;
        BR row_done, INIT_DONE, INIT_ROW;
        
    INIT_ROW:
        j = LOAD_I32 0;
        BR ALWAYS, INIT_CELL_CHECK;
        
    INIT_CELL_CHECK:
        cell_done = CMP_GE j, vertices;
        BR cell_done, INIT_ROW_DONE, INIT_CELL;
        
    INIT_CELL:
        // Get cell pointer
        row_offset = MUL i, SIZEOF(ptr<i32>);
        row_ptr_ptr = ADD matrix, row_offset;
        row_ptr = LOAD [row_ptr_ptr];
        
        cell_offset = MUL j, SIZEOF(i32);
        cell_ptr = ADD row_ptr, cell_offset;
        
        // Set cell to zero
        STORE [cell_ptr], 0;
        
        j = ADD j, 1;
        BR ALWAYS, INIT_CELL_CHECK;
        
    INIT_ROW_DONE:
        i = ADD i, 1;
        BR ALWAYS, INIT_ROW_CHECK;
        
    INIT_DONE:
        RET graph;
}

// Add edge to adjacency matrix graph
FUNCTION add_edge_matrix(graph: ptr<matrix_graph>, src: i32, dest: i32, weight: i32) -> void {
    ENTRY:
        // Get vertices count
        vertices_ptr = ADD graph, OFFSETOF(matrix_graph, vertices);
        vertices = LOAD [vertices_ptr];
        
        // Check bounds
        src_valid = CMP_LT src, vertices;
        dest_valid = CMP_LT dest, vertices;
        
        src_non_neg = CMP_GE src, 0;
        dest_non_neg = CMP_GE dest, 0;
        
        src_ok = AND src_valid, src_non_neg;
        dest_ok = AND dest_valid, dest_non_neg;
        
        both_ok = AND src_ok, dest_ok;
        
        BR both_ok, ADD_EDGE, DONE;
        
    ADD_EDGE:
        // Get matrix pointer
        matrix_ptr = ADD graph, OFFSETOF(matrix_graph, matrix);
        matrix = LOAD [matrix_ptr];
        
        // Get src row
        src_offset = MUL src, SIZEOF(ptr<i32>);
        src_row_ptr = ADD matrix, src_offset;
        src_row = LOAD [src_row_ptr];
        
        // Set edge weight
        dest_offset = MUL dest, SIZEOF(i32);
        edge_ptr = ADD src_row, dest_offset;
        STORE [edge_ptr], weight;
        
    DONE:
        RET;
}

// Create a new adjacency list graph
FUNCTION create_list_graph(vertices: i32) -> ptr<list_graph> {
    ENTRY:
        // Allocate graph structure
        graph_size = SIZEOF(list_graph);
        graph_ptr = CALL malloc(graph_size);
        
        // Check allocation success
        is_null = CMP_EQ graph_ptr, NULL;
        BR is_null, CREATE_FAIL, INIT_GRAPH;
        
    CREATE_FAIL:
        RET NULL;
        
    INIT_GRAPH:
        // Set number of vertices
        graph = CONVERT graph_ptr, ptr<list_graph>;
        vertices_ptr = ADD graph, OFFSETOF(list_graph, vertices);
        STORE [vertices_ptr], vertices;
        
        // Allocate adjacency lists array
        adj_lists_size = MUL vertices, SIZEOF(ptr<adj_list_node>);
        adj_lists = CALL malloc(adj_lists_size);
        
        is_lists_null = CMP_EQ adj_lists, NULL;
        BR is_lists_null, FREE_GRAPH, INIT_LISTS;
        
    FREE_GRAPH:
        CALL free(graph);
        RET NULL;
        
    INIT_LISTS:
        // Store adjacency lists pointer
        adj_lists_ptr = ADD graph, OFFSETOF(list_graph, adj_lists);
        STORE [adj_lists_ptr], adj_lists;
        
        // Initialize lists to NULL
        i = LOAD_I32 0;
        BR ALWAYS, INIT_LIST_CHECK;
        
    INIT_LIST_CHECK:
        list_done = CMP_GE i, vertices;
        BR list_done, INIT_DONE, INIT_LIST;
        
    INIT_LIST:
        // Set list head to NULL
        list_offset = MUL i, SIZEOF(ptr<adj_list_node>);
        list_ptr_ptr = ADD adj_lists, list_offset;
        STORE [list_ptr_ptr], NULL;
        
        i = ADD i, 1;
        BR ALWAYS, INIT_LIST_CHECK;
        
    INIT_DONE:
        RET graph;
}

// Create a new adjacency list node
FUNCTION create_adj_node(vertex: i32, weight: i32) -> ptr<adj_list_node> {
    ENTRY:
        // Allocate node
        node_size = SIZEOF(adj_list_node);
        node_ptr = CALL malloc(node_size);
        
        // Check allocation success
        is_null = CMP_EQ node_ptr, NULL;
        BR is_null, CREATE_FAIL, INIT_NODE;
        
    CREATE_FAIL:
        RET NULL;
        
    INIT_NODE:
        // Initialize node fields
        node = CONVERT node_ptr, ptr<adj_list_node>;
        
        vertex_ptr = ADD node, OFFSETOF(adj_list_node, vertex);
        STORE [vertex_ptr], vertex;
        
        weight_ptr = ADD node, OFFSETOF(adj_list_node, weight);
        STORE [weight_ptr], weight;
        
        next_ptr = ADD node, OFFSETOF(adj_list_node, next);
        STORE [next_ptr], NULL;
        
        RET node;
}

// Add edge to adjacency list graph
FUNCTION add_edge_list(graph: ptr<list_graph>, src: i32, dest: i32, weight: i32) -> void {
    ENTRY:
        // Get vertices count
        vertices_ptr = ADD graph, OFFSETOF(list_graph, vertices);
        vertices = LOAD [vertices_ptr];
        
        // Check bounds
        src_valid = CMP_LT src, vertices;
        dest_valid = CMP_LT dest, vertices;
        
        src_non_neg = CMP_GE src, 0;
        dest_non_neg = CMP_GE dest, 0;
        
        src_ok = AND src_valid, src_non_neg;
        dest_ok = AND dest_valid, dest_non_neg;
        
        both_ok = AND src_ok, dest_ok;
        
        BR both_ok, ADD_EDGE, DONE;
        
    ADD_EDGE:
        // Create new node
        new_node = CALL create_adj_node(dest, weight);
        is_null = CMP_EQ new_node, NULL;
        BR is_null, DONE, INSERT_NODE;
        
    INSERT_NODE:
        // Get adjacency list for source vertex
        adj_lists_ptr = ADD graph, OFFSETOF(list_graph, adj_lists);
        adj_lists = LOAD [adj_lists_ptr];
        
        src_offset = MUL src, SIZEOF(ptr<adj_list_node>);
        head_ptr = ADD adj_lists, src_offset;
        head = LOAD [head_ptr];
        
        // Insert at beginning of list
        next_ptr = ADD new_node, OFFSETOF(adj_list_node, next);
        STORE [next_ptr], head;
        STORE [head_ptr], new_node;
        
    DONE:
        RET;
}

// Breadth-First Search (using adjacency list)
FUNCTION bfs_list(graph: ptr<list_graph>, start: i32) -> void {
    ENTRY:
        // Get vertices count
        vertices_ptr = ADD graph, OFFSETOF(list_graph, vertices);
        vertices = LOAD [vertices_ptr];
        
        // Check start vertex bounds
        start_valid = CMP_LT start, vertices;
        start_non_neg = CMP_GE start, 0;
        start_ok = AND start_valid, start_non_neg;
        
        BR start_ok, INIT_BFS, DONE;
        
    INIT_BFS:
        // Create visited array
        visited_size = MUL vertices, SIZEOF(i32);
        visited = CALL malloc(visited_size);
        
        is_visited_null = CMP_EQ visited, NULL;
        BR is_visited_null, DONE, INIT_VISITED;
        
    INIT_VISITED:
        // Initialize all vertices as not visited
        i = LOAD_I32 0;
        BR ALWAYS, INIT_CHECK;
        
    INIT_CHECK:
        init_done = CMP_GE i, vertices;
        BR init_done, INIT_QUEUE, MARK_UNVISITED;
        
    MARK_UNVISITED:
        // Set visited[i] = 0
        offset = MUL i, SIZEOF(i32);
        visited_ptr = ADD visited, offset;
        STORE [visited_ptr], 0;
        
        i = ADD i, 1;
        BR ALWAYS, INIT_CHECK;
        
    INIT_QUEUE:
        // Create queue for BFS
        queue = CALL create_queue(vertices);
        is_queue_null = CMP_EQ queue, NULL;
        BR is_queue_null, FREE_VISITED, START_BFS;
        
    FREE_VISITED:
        CALL free(visited);
        RET;
        
    START_BFS:
        // Mark start vertex as visited
        start_offset = MUL start, SIZEOF(i32);
        start_visited_ptr = ADD visited, start_offset;
        STORE [start_visited_ptr], 1;
        
        // Enqueue start vertex
        CALL enqueue(queue, start);
        
        // BFS loop
        BR ALWAYS, BFS_LOOP_CHECK;
        
    BFS_LOOP_CHECK:
        // Check if queue is empty
        is_empty = CALL is_queue_empty(queue);
        BR is_empty, BFS_DONE, PROCESS_VERTEX;
        
    PROCESS_VERTEX:
        // Dequeue a vertex
        current = CALL dequeue(queue);
        
        // Process current vertex (print it)
        CALL print_vertex(current);
        
        // Get adjacency list for current vertex
        adj_lists_ptr = ADD graph, OFFSETOF(list_graph, adj_lists);
        adj_lists = LOAD [adj_lists_ptr];
        
        current_offset = MUL current, SIZEOF(ptr<adj_list_node>);
        head_ptr = ADD adj_lists, current_offset;
        adj_list = LOAD [head_ptr];
        
        BR ALWAYS, PROCESS_ADJACENT;
        
    PROCESS_ADJACENT:
        // Process all adjacent vertices
        is_null = CMP_EQ adj_list, NULL;
        BR is_null, BFS_LOOP_CHECK, CHECK_ADJACENT;
        
    CHECK_ADJACENT:
        // Get adjacent vertex
        vertex_ptr = ADD adj_list, OFFSETOF(adj_list_node, vertex);
        adj_vertex = LOAD [vertex_ptr];
        
        // Check if it's visited
        adj_offset = MUL adj_vertex, SIZEOF(i32);
        adj_visited_ptr = ADD visited, adj_offset;
        is_visited = LOAD [adj_visited_ptr];
        
        is_not_visited = CMP_EQ is_visited, 0;
        BR is_not_visited, VISIT_ADJACENT, NEXT_ADJACENT;
        
    VISIT_ADJACENT:
        // Mark as visited
        STORE [adj_visited_ptr], 1;
        
        // Enqueue vertex
        CALL enqueue(queue, adj_vertex);
        
    NEXT_ADJACENT:
        // Move to next adjacent vertex
        next_ptr = ADD adj_list, OFFSETOF(adj_list_node, next);
        adj_list = LOAD [next_ptr];
        BR ALWAYS, PROCESS_ADJACENT;
        
    BFS_DONE:
        // Clean up
        CALL free_queue(queue);
        CALL free(visited);
        
    DONE:
        RET;
}

// Depth-First Search (using adjacency list)
FUNCTION dfs_util(graph: ptr<list_graph>, vertex: i32, visited: ptr<i32>) -> void {
    ENTRY:
        // Mark current vertex as visited
        vertex_offset = MUL vertex, SIZEOF(i32);
        visited_ptr = ADD visited, vertex_offset;
        STORE [visited_ptr], 1;
        
        // Process current vertex (print it)
        CALL print_vertex(vertex);
        
        // Get adjacency list for current vertex
        adj_lists_ptr = ADD graph, OFFSETOF(list_graph, adj_lists);
        adj_lists = LOAD [adj_lists_ptr];
        
        vertex_offset = MUL vertex, SIZEOF(ptr<adj_list_node>);
        head_ptr = ADD adj_lists, vertex_offset;
        adj_list = LOAD [head_ptr];
        
        BR ALWAYS, PROCESS_ADJACENT;
        
    PROCESS_ADJACENT:
        // Process all adjacent vertices
        is_null = CMP_EQ adj_list, NULL;
        BR is_null, DFS_DONE, CHECK_ADJACENT;
        
    CHECK_ADJACENT:
        // Get adjacent vertex
        adj_vertex_ptr = ADD adj_list, OFFSETOF(adj_list_node, vertex);
        adj_vertex = LOAD [adj_vertex_ptr];
        
        // Check if it's visited
        adj_offset = MUL adj_vertex, SIZEOF(i32);
        adj_visited_ptr = ADD visited, adj_offset;
        is_visited = LOAD [adj_visited_ptr];
        
        is_not_visited = CMP_EQ is_visited, 0;
        BR is_not_visited, VISIT_ADJACENT, NEXT_ADJACENT;
        
    VISIT_ADJACENT:
        // Recursive DFS
        CALL dfs_util(graph, adj_vertex, visited);
        
    NEXT_ADJACENT:
        // Move to next adjacent vertex
        next_ptr = ADD adj_list, OFFSETOF(adj_list_node, next);
        adj_list = LOAD [next_ptr];
        BR ALWAYS, PROCESS_ADJACENT;
        
    DFS_DONE:
        RET;
}

// DFS traversal of the graph
FUNCTION dfs_list(graph: ptr<list_graph>, start: i32) -> void {
    ENTRY:
        // Get vertices count
        vertices_ptr = ADD graph, OFFSETOF(list_graph, vertices);
        vertices = LOAD [vertices_ptr];
        
        // Check start vertex bounds
        start_valid = CMP_LT start, vertices;
        start_non_neg = CMP_GE start, 0;
        start_ok = AND start_valid, start_non_neg;
        
        BR start_ok, INIT_DFS, DONE;
        
    INIT_DFS:
        // Create visited array
        visited_size = MUL vertices, SIZEOF(i32);
        visited = CALL malloc(visited_size);
        
        is_visited_null = CMP_EQ visited, NULL;
        BR is_visited_null, DONE, INIT_VISITED;
        
    INIT_VISITED:
        // Initialize all vertices as not visited
        i = LOAD_I32 0;
        BR ALWAYS, INIT_CHECK;
        
    INIT_CHECK:
        init_done = CMP_GE i, vertices;
        BR init_done, START_DFS, MARK_UNVISITED;
        
    MARK_UNVISITED:
        // Set visited[i] = 0
        offset = MUL i, SIZEOF(i32);
        visited_ptr = ADD visited, offset;
        STORE [visited_ptr], 0;
        
        i = ADD i, 1;
        BR ALWAYS, INIT_CHECK;
        
    START_DFS:
        // Start DFS from start vertex
        CALL dfs_util(graph, start, visited);
        
        // Clean up
        CALL free(visited);
        
    DONE:
        RET;
}

// Utility functions
// These would be implemented elsewhere

// Simple queue implementation for BFS
EXTERN FUNCTION create_queue(capacity: i32) -> ptr<void>;
EXTERN FUNCTION enqueue(queue: ptr<void>, item: i32) -> void;
EXTERN FUNCTION dequeue(queue: ptr<void>) -> i32;
EXTERN FUNCTION is_queue_empty(queue: ptr<void>) -> i32;
EXTERN FUNCTION free_queue(queue: ptr<void>) -> void;

// Print a vertex (for demonstration)
EXTERN FUNCTION print_vertex(vertex: i32) -> void;
```

## 3. Numeric Computations

This example demonstrates numeric algorithms implemented in HOIL.

```
MODULE "numeric";

// Mathematical constants
CONSTANT PI: f64 = 3.14159265358979323846;
CONSTANT E: f64 = 2.71828182845904523536;

// Compute factorial iteratively
FUNCTION factorial(n: i32) -> i64 {
    ENTRY:
        // Check for invalid input
        is_negative = CMP_LT n, 0;
        BR is_negative, RETURN_INVALID, CHECK_BASE_CASE;
        
    RETURN_INVALID:
        result = LOAD_I64 -1;
        RET result;
        
    CHECK_BASE_CASE:
        // Base cases: 0! = 1, 1! = 1
        is_base = CMP_LE n, 1;
        BR is_base, RETURN_ONE, COMPUTE_FACTORIAL;
        
    RETURN_ONE:
        result = LOAD_I64 1;
        RET result;
        
    COMPUTE_FACTORIAL:
        result = LOAD_I64 1;
        i = LOAD_I32 2;
        BR ALWAYS, LOOP_CHECK;
        
    LOOP_CHECK:
        loop_cond = CMP_LE i, n;
        BR loop_cond, LOOP_BODY, LOOP_DONE;
        
    LOOP_BODY:
        // Multiply result by i
        i_64 = CONVERT i, i64;
        result = MUL result, i_64;
        
        // Increment counter
        i = ADD i, 1;
        BR ALWAYS, LOOP_CHECK;
        
    LOOP_DONE:
        RET result;
}

// Compute GCD using Euclidean algorithm
FUNCTION gcd(a: i32, b: i32) -> i32 {
    ENTRY:
        // Ensure positive inputs
        a_abs = CALL abs_i32(a);
        b_abs = CALL abs_i32(b);
        
        // Check for zero inputs
        a_zero = CMP_EQ a_abs, 0;
        b_zero = CMP_EQ b_abs, 0;
        
        BR a_zero, RETURN_B, CHECK_B_ZERO;
        
    CHECK_B_ZERO:
        BR b_zero, RETURN_A, COMPUTE_GCD;
        
    RETURN_A:
        RET a_abs;
        
    RETURN_B:
        RET b_abs;
        
    COMPUTE_GCD:
        // Initialize values
        x = a_abs;
        y = b_abs;
        BR ALWAYS, GCD_LOOP;
        
    GCD_LOOP:
        // Check termination condition
        is_done = CMP_EQ y, 0;
        BR is_done, GCD_DONE, GCD_ITERATION;
        
    GCD_ITERATION:
        // Calculate remainder
        remainder = REM x, y;
        
        // Update values
        x = y;
        y = remainder;
        BR ALWAYS, GCD_LOOP;
        
    GCD_DONE:
        RET x;
}

// Compute absolute value of an integer
FUNCTION abs_i32(x: i32) -> i32 {
    ENTRY:
        is_negative = CMP_LT x, 0;
        BR is_negative, NEGATE, RETURN_SAME;
        
    NEGATE:
        result = NEG x;
        RET result;
        
    RETURN_SAME:
        RET x;
}

// Compute exponentiation by squaring
FUNCTION power(base: f64, exponent: i32) -> f64 {
    ENTRY:
        // Check special cases
        is_zero_exp = CMP_EQ exponent, 0;
        BR is_zero_exp, RETURN_ONE, CHECK_NEGATIVE;
        
    RETURN_ONE:
        result = LOAD_F64 1.0;
        RET result;
        
    CHECK_NEGATIVE:
        is_neg = CMP_LT exponent, 0;
        BR is_neg, HANDLE_NEGATIVE, COMPUTE_POWER;
        
    HANDLE_NEGATIVE:
        // For negative exponent, compute 1/base^(-exponent)
        neg_exp = NEG exponent;
        temp = CALL power(base, neg_exp);
        result = DIV 1.0, temp;
        RET result;
        
    COMPUTE_POWER:
        result = LOAD_F64 1.0;
        current_base = base;
        current_exp = exponent;
        BR ALWAYS, POWER_LOOP;
        
    POWER_LOOP:
        is_done = CMP_EQ current_exp, 0;
        BR is_done, POWER_DONE, CHECK_BIT;
        
    CHECK_BIT:
        // Check if current bit is set
        bit_set = AND current_exp, 1;
        is_set = CMP_NE bit_set, 0;
        BR is_set, MULTIPLY_RESULT, SKIP_MULTIPLY;
        
    MULTIPLY_RESULT:
        result = MUL result, current_base;
        
    SKIP_MULTIPLY:
        // Square the base
        current_base = MUL current_base, current_base;
        
        // Move to next bit
        current_exp = SHR current_exp, 1;
        BR ALWAYS, POWER_LOOP;
        
    POWER_DONE:
        RET result;
}

// Compute square root using Newton's method
FUNCTION sqrt(x: f64) -> f64 {
    ENTRY:
        // Check for negative input
        is_negative = CMP_LT x, 0.0;
        BR is_negative, RETURN_NAN, CHECK_SPECIAL;
        
    RETURN_NAN:
        // Return NaN for negative input
        result = DIV 0.0, 0.0;  // NaN
        RET result;
        
    CHECK_SPECIAL:
        // Check for 0 or 1
        is_zero = CMP_EQ x, 0.0;
        is_one = CMP_EQ x, 1.0;
        
        BR is_zero, RETURN_ZERO, CHECK_ONE;
        
    CHECK_ONE:
        BR is_one, RETURN_ONE, COMPUTE_SQRT;
        
    RETURN_ZERO:
        RET x;  // Return 0.0
        
    RETURN_ONE:
        RET x;  // Return 1.0
        
    COMPUTE_SQRT:
        // Initial guess
        guess = DIV x, 2.0;
        
        // Newton's method loop
        iterations = LOAD_I32 10;  // Fixed iterations for simplicity
        i = LOAD_I32 0;
        BR ALWAYS, NEWTON_CHECK;
        
    NEWTON_CHECK:
        newton_done = CMP_GE i, iterations;
        BR newton_done, NEWTON_DONE, NEWTON_ITERATION;
        
    NEWTON_ITERATION:
        // Newton's formula: guess = (guess + x/guess) / 2
        quot = DIV x, guess;
        sum = ADD guess, quot;
        guess = DIV sum, 2.0;
        
        i = ADD i, 1;
        BR ALWAYS, NEWTON_CHECK;
        
    NEWTON_DONE:
        RET guess;
}

// Compute sin(x) using Taylor series
FUNCTION sin(x: f64) -> f64 {
    ENTRY:
        // Normalize angle to [-PI, PI]
        two_pi = MUL PI, 2.0;
        normalized = CALL fmod(x, two_pi);
        
        is_gt_pi = CMP_GT normalized, PI;
        BR is_gt_pi, SUBTRACT_TWO_PI, CHECK_LT_NEG_PI;
        
    SUBTRACT_TWO_PI:
        normalized = SUB normalized, two_pi;
        BR ALWAYS, COMPUTE_SIN;
        
    CHECK_LT_NEG_PI:
        is_lt_neg_pi = CMP_LT normalized, NEG PI;
        BR is_lt_neg_pi, ADD_TWO_PI, COMPUTE_SIN;
        
    ADD_TWO_PI:
        normalized = ADD normalized, two_pi;
        
    COMPUTE_SIN:
        // Taylor series for sin(x)
        // sin(x) = x - x^3/3! + x^5/5! - x^7/7! + ...
        
        result = normalized;
        
        // x^3 term
        x_squared = MUL normalized, normalized;
        x_cubed = MUL x_squared, normalized;
        term = DIV x_cubed, 6.0;  // 3! = 6
        result = SUB result, term;
        
        // x^5 term
        x_fifth = MUL x_cubed, x_squared;
        term = DIV x_fifth, 120.0;  // 5! = 120
        result = ADD result, term;
        
        // x^7 term
        x_seventh = MUL x_fifth, x_squared;
        term = DIV x_seventh, 5040.0;  // 7! = 5040
        result = SUB result, term;
        
        // x^9 term
        x_ninth = MUL x_seventh, x_squared;
        term = DIV x_ninth, 362880.0;  // 9! = 362880
        result = ADD result, term;
        
        RET result;
}

// Compute cos(x) using Taylor series
FUNCTION cos(x: f64) -> f64 {
    ENTRY:
        // Normalize angle to [-PI, PI]
        two_pi = MUL PI, 2.0;
        normalized = CALL fmod(x, two_pi);
        
        is_gt_pi = CMP_GT normalized, PI;
        BR is_gt_pi, SUBTRACT_TWO_PI, CHECK_LT_NEG_PI;
        
    SUBTRACT_TWO_PI:
        normalized = SUB normalized, two_pi;
        BR ALWAYS, COMPUTE_COS;
        
    CHECK_LT_NEG_PI:
        is_lt_neg_pi = CMP_LT normalized, NEG PI;
        BR is_lt_neg_pi, ADD_TWO_PI, COMPUTE_COS;
        
    ADD_TWO_PI:
        normalized = ADD normalized, two_pi;
        
    COMPUTE_COS:
        // Taylor series for cos(x)
        // cos(x) = 1 - x^2/2! + x^4/4! - x^6/6! + ...
        
        result = LOAD_F64 1.0;
        
        // x^2 term
        x_squared = MUL normalized, normalized;
        term = DIV x_squared, 2.0;  // 2! = 2
        result = SUB result, term;
        
        // x^4 term
        x_fourth = MUL x_squared, x_squared;
        term = DIV x_fourth, 24.0;  // 4! = 24
        result = ADD result, term;
        
        // x^6 term
        x_sixth = MUL x_fourth, x_squared;
        term = DIV x_sixth, 720.0;  // 6! = 720
        result = SUB result, term;
        
        // x^8 term
        x_eighth = MUL x_sixth, x_squared;
        term = DIV x_eighth, 40320.0;  // 8! = 40320
        result = ADD result, term;
        
        RET result;
}

// Compute e^x using Taylor series
FUNCTION exp(x: f64) -> f64 {
    ENTRY:
        // Check special cases
        is_zero = CMP_EQ x, 0.0;
        BR is_zero, RETURN_ONE, COMPUTE_EXP;
        
    RETURN_ONE:
        result = LOAD_F64 1.0;
        RET result;
        
    COMPUTE_EXP:
        // Taylor series for e^x
        // e^x = 1 + x + x^2/2! + x^3/3! + ...
        
        result = LOAD_F64 1.0;
        term = LOAD_F64 1.0;
        
        // Use up to 15 terms for accuracy
        iterations = LOAD_I32 15;
        i = LOAD_I32 1;
        BR ALWAYS, EXP_CHECK;
        
    EXP_CHECK:
        exp_done = CMP_GT i, iterations;
        BR exp_done, EXP_DONE, EXP_ITERATION;
        
    EXP_ITERATION:
        // Calculate next term: term = term * x / i
        term = MUL term, x;
        i_f64 = CONVERT i, f64;
        term = DIV term, i_f64;
        
        // Add term to result
        result = ADD result, term;
        
        i = ADD i, 1;
        BR ALWAYS, EXP_CHECK;
        
    EXP_DONE:
        RET result;
}

// Compute natural logarithm using Taylor series
FUNCTION ln(x: f64) -> f64 {
    ENTRY:
        // Check for invalid input
        is_le_zero = CMP_LE x, 0.0;
        BR is_le_zero, RETURN_NAN, CHECK_SPECIAL;
        
    RETURN_NAN:
        // Return NaN for non-positive input
        result = DIV 0.0, 0.0;  // NaN
        RET result;
        
    CHECK_SPECIAL:
        // Check for x = 1
        is_one = CMP_EQ x, 1.0;
        BR is_one, RETURN_ZERO, COMPUTE_LN;
        
    RETURN_ZERO:
        result = LOAD_F64 0.0;
        RET result;
        
    COMPUTE_LN:
        // Scale x to [0.5, 1.5] for better convergence
        scaled_x = x;
        scale_factor = LOAD_F64 0.0;
        
        BR ALWAYS, SCALE_CHECK;
        
    SCALE_CHECK:
        is_too_large = CMP_GT scaled_x, 1.5;
        BR is_too_large, SCALE_DOWN, CHECK_TOO_SMALL;
        
    SCALE_DOWN:
        scaled_x = DIV scaled_x, E;
        scale_factor = ADD scale_factor, 1.0;
        BR ALWAYS, SCALE_CHECK;
        
    CHECK_TOO_SMALL:
        is_too_small = CMP_LT scaled_x, 0.5;
        BR is_too_small, SCALE_UP, TAYLOR_EXPANSION;
        
    SCALE_UP:
        scaled_x = MUL scaled_x, E;
        scale_factor = SUB scale_factor, 1.0;
        BR ALWAYS, SCALE_CHECK;
        
    TAYLOR_EXPANSION:
        // Use Taylor series for ln(1+y) where y = x-1
        y = SUB scaled_x, 1.0;
        
        result = LOAD_F64 0.0;
        current_term = y;
        sign = LOAD_F64 1.0;
        
        // Use up to 15 terms for accuracy
        iterations = LOAD_I32 15;
        i = LOAD_I32 1;
        BR ALWAYS, LN_CHECK;
        
    LN_CHECK:
        ln_done = CMP_GT i, iterations;
        BR ln_done, APPLY_SCALE, LN_ITERATION;
        
    LN_ITERATION:
        // Add term to result
        term = MUL current_term, sign;
        i_f64 = CONVERT i, f64;
        term = DIV term, i_f64;
        result = ADD result, term;
        
        // Update for next iteration
        current_term = MUL current_term, y;
        sign = NEG sign;
        
        i = ADD i, 1;
        BR ALWAYS, LN_CHECK;
        
    APPLY_SCALE:
        // Apply scaling: ln(x) = ln(scaled_x) + scale_factor
        result = ADD result, scale_factor;
        RET result;
}

// Compute modulo for floating point
FUNCTION fmod(x: f64, y: f64) -> f64 {
    ENTRY:
        // Check for division by zero
        is_zero = CMP_EQ y, 0.0;
        BR is_zero, RETURN_NAN, COMPUTE_FMOD;
        
    RETURN_NAN:
        // Return NaN for division by zero
        result = DIV 0.0, 0.0;  // NaN
        RET result;
        
    COMPUTE_FMOD:
        // Calculate integer quotient
        quotient = DIV x, y;
        int_quotient = CALL floor(quotient);
        
        // Calculate remainder
        product = MUL int_quotient, y;
        result = SUB x, product;
        
        RET result;
}

// Floor function
FUNCTION floor(x: f64) -> f64 {
    ENTRY:
        // Convert to integer (truncate) and back to float
        temp = CONVERT x, i64;
        result = CONVERT temp, f64;
        
        // Check if we need to adjust
        is_negative = CMP_LT x, 0.0;
        is_fractional = CMP_LT result, x;
        
        needs_adjust = AND is_negative, is_fractional;
        BR needs_adjust, ADJUST, RETURN_RESULT;
        
    ADJUST:
        result = SUB result, 1.0;
        
    RETURN_RESULT:
        RET result;
}
```

## 4. Conclusion

These examples demonstrate standard programming patterns in HOIL, including:

1. **Data Structure Implementation**: Linked lists, trees, and other data structures
2. **Algorithm Implementation**: Sorting, searching, and graph traversal
3. **Numeric Computation**: Mathematical functions and numerical algorithms

These patterns show how HOIL can be used for typical application development scenarios, providing a foundation for developing more complex applications. The human-readable syntax of HOIL makes it accessible for direct programming while maintaining the efficiency of the underlying COIL binary format.