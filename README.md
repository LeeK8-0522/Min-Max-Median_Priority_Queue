README:

//function declaration
int log2_func(int num);
void swap_node(HEAP* heap, int index1, int index2);
void initialize();
void destroy();
void heapify_up_MIN_MAX_HEAP(int index);
void insert_MIN_MAX_HEAP(int item);
int find_min_child(int index);
int find_max_child(int index);
void heapify_down_MIN_MAX_HEAP(int index);
NODE delete_MIN_MAX_HEAP(int index);
int foo();
int find_min();
int find_max();
void heapifty_down_MAX_HEAP(int index);
void heapifty_down_MIN_HEAP(int index);
void heapifty_up_MAX_HEAP(int index);
void heapifty_up_MIN_HEAP(int index);
void insert_MAX_HEAP(int item);
NODE delete_MAX_HEAP(int index);
void insert_MIN_HEAP(int item);
NODE delete_MIN_HEAP(int index);
void balance_MEDIAN_HEAP();
void insert_MEDIAN_HEAP(int item);
int find_median();
//finish declaring functions

<min-max heap ... (자료형: HEAP*, 이름: min_max_pq)>:
삽입: void insert_MIN_MAX_HEAP(int item)
삭제: NODE delete_MIN_MAX_HEAP(int index)
탐색: int find_median()
heapify: 
-> down: void heapify_down_MIN_MAX_HEAP(int index) 
-> up: void heapify_up_MIN_MAX_HEAP(int index)

<median heap ... (자료형: MEDIAN_PQ*, 이름: median_pq)>:
삽입: void insert_MEDIAN_HEAP(int item);
삭제: NODE delete_MIN_HEAP(int index), NODE delete_MAX_HEAP(int index);
탐색: int find_max(), int find_min()
heapify: 
-> down: void heapify_down_MIN_HEAP(int index), void heapify_down_MAX_HEAP(int index)
-> up: void heapify_up_MIN_HEAP(int index), void heapify_up_MAX_HEAP(int index)
balance:
-> void balance_MEDIAN_HEAP()

<min-max-medain heap ... (자료형: MIN_MAX_MEDIAN_PQ*, 이름: MMMQ)>:
삽입: void insert(int element)
삭제(최대, 최소, 중앙): int delete_max(), int delete_min(), int delete_median()
탐색: int find_max(), int find_min(), int find_median()

