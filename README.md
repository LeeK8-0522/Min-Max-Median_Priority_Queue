README:
<min-max heap ... (자료형: HEAP*, 이름: min_max_pq)>:
삽입: void insert_MEDIAN_HEAP(int item)
삭제: int delete_MAX_HEAP(int index) , int delete_MIN_HEAP(int index)
탐색: int seek_median()
heapify: 
-> down: void heapify_down_MIN_MAX_HEAP(int index) 
-> up: void heapify_up_MIN_MAX_HEAP(int index)

<median heap ... (자료형: MEDIAN_PQ*, 이름: median_pq)>:
삽입: void insert_MIN_MAX_HEAP(int item)
삭제: int delete_MIN_MAX_HEAP(int index)
탐색: int seek_max(), int seek_min()
heapify: 
-> down: void heapify_down_MIN_HEAP(int index), void heapify_down_MAX_HEAP(int index)
-> up: void heapify_up_MIN_HEAP(int index), void heapify_up_MAX_HEAP(int index)

<min-max-medain heap ... (자료형: MIN_MAX_MEDIAN_PQ*, 이름: MMMQ)>:
삽입:
삭제:
탐색:

