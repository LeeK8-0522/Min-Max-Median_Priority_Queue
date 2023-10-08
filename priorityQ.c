#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 500100

typedef struct {
    int key;
    int syncPos;//같은 키를 갖는 상대편 노드의 인덱스 (for 위치 추적)
    struct HEAP* opposite_heap;//상대편 노드가 속해있는 heap의 주소
} NODE;//node structure

typedef struct {
    NODE nodes [MAX_NODES];
    int size;
} HEAP;//heap structure

typedef struct {
    HEAP* max_heap;//최대 힙
    HEAP* min_heap;//최소 힙
} MEDIAN_PQ;//median heap structure

typedef struct {
    MEDIAN_PQ* median_pq;//for 중앙값 관리
    HEAP* min_max_pq;//for 최댓값, 최솟값 관리
} MIN_MAX_MEDIAN_PQ;//min-max-median heap structure

MIN_MAX_MEDIAN_PQ* MMMQ;//전역변수로 설정

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
void insert(int element);
int delete_max();
int delete_min();
int delete_median();
//finish declaring functions



int log2_func(int num) {//log function (시간 복잡도: O(lg num) -> 이 코드에서는 주로 O(lg(lgn)))
    int cnt = 0;
    while(num/2 > 0) {
        cnt++;
        num /= 2;
    }

    return cnt;
}

void swap_node(HEAP* heap, int index1, int index2) {//0: min-max heap -> median heap, 1: median heap -> min-max heap
    HEAP* opposite_heap1 = heap->nodes[index1].opposite_heap;
    HEAP* opposite_heap2 = heap->nodes[index2].opposite_heap;
    
    //상대편 노드 정보 수정
    opposite_heap1->nodes[heap->nodes[index1].syncPos].syncPos = index2;
    opposite_heap2->nodes[heap->nodes[index2].syncPos].syncPos = index1;

    //내부 정보 수정
    NODE temp = heap->nodes[index1];
    heap->nodes[index1] = heap->nodes[index2];
    heap->nodes[index2] = temp;
}

void initialize() {
    MMMQ = (MIN_MAX_MEDIAN_PQ*)malloc(sizeof(MIN_MAX_MEDIAN_PQ));//allocate memory

    //allocate memory and init median heap
    MMMQ->median_pq = (MEDIAN_PQ*)malloc(sizeof(MEDIAN_PQ));
    MMMQ->median_pq->max_heap = (HEAP*)malloc(sizeof(HEAP));
    MMMQ->median_pq->min_heap = (HEAP*)malloc(sizeof(HEAP));
    MMMQ->median_pq->max_heap->size = 0;
    MMMQ->median_pq->min_heap->size = 0;

    //allocate memory and init min-max heap
    MMMQ->min_max_pq = (HEAP*)malloc(sizeof(HEAP));
    MMMQ->min_max_pq->size = 0;
}//init min-max heap

void destroy() {
    free(MMMQ->min_max_pq);

    free(MMMQ->median_pq->max_heap);
    free(MMMQ->median_pq->min_heap);
    free(MMMQ->median_pq);

    free(MMMQ);
}//free allocate all related to MMMQ global variable

void heapify_up_MIN_MAX_HEAP(int index) {//heapify-up for min-level && max-level
    HEAP* heap = MMMQ->min_max_pq;
    
    int level = log2_func(index);
    int grand_index = index/4;
    
    if(level % 2 == 0) {//min-level끼리만 비교 (단, index를 제외한 나머지 전체 노드들은 서로서로 heap 성질 만족 -> 사용이 의미 O)
        while(grand_index > 0) {//while having a grand parent
            if(heap->nodes[index].key < heap->nodes[grand_index].key) {//swap child node with grand parent node until condition breaks
                swap_node(heap, index, grand_index);
                index = grand_index;
                grand_index = index/4;
            }
            else break;
        }
    }
    else {//max-level끼리만 비교 (단, index를 제외한 나머지 전체 노드들은 서로서로 heap 성질 만족 -> 사용이 의미 O)
        while(grand_index > 0) {//while having a grand parent
            if(heap->nodes[index].key > heap->nodes[grand_index].key) {//swap child node with grand parent node until condition breaks
                swap_node(heap, index, grand_index);
                index = grand_index;
                grand_index = index/4;
            }
            else break;
        }
    }
}

void insert_MIN_MAX_HEAP(int item) {//insert item key
    HEAP* heap = MMMQ->min_max_pq;

    int index = ++heap->size;//우선 맨 마직막 위치에 노드를 저장
    int parent_index = index / 2;
    heap->nodes[index].key = item;

    int level = log2_func(index);

    if(index != 1) {//index가 root를 가리키지 않을 때까지
        if(level%2==0) {//if inserted position is in min level,
            if(heap->nodes[index].key > heap->nodes[parent_index].key) {//해당 node가 max-level에 있어야 하는지, min-level에 있어야 하는지 판단 (동시에 index를 제외한 나머지 노드들은 서로서로 heap 성질 만족 O)
                swap_node(heap, index, parent_index);
                heapify_up_MIN_MAX_HEAP(parent_index);//heapify upper heaps
            }
            else {
                heapify_up_MIN_MAX_HEAP(index);//heapify upper heaps
            }
        }
        else {//if inserted position is in max level,
            if(heap->nodes[index].key < heap->nodes[parent_index].key) {//해당 node가 max-level에 있어야 하는지, min-level에 있어야 하는지 판단(동시에 index를 제외한 나머지 노드들은 서로서로 heap 성질 만족 O)
                swap_node(heap, index, parent_index);
                heapify_up_MIN_MAX_HEAP(parent_index);//heapify upper heaps
            }
            else {
                heapify_up_MIN_MAX_HEAP(index);//heapify upper heaps
            }
        }
    }
}

int find_min_child(int index) {//index 노드의 child, grand child 노드들 중에서 최솟인 인덱스 반환
    HEAP* heap = MMMQ->min_max_pq;

    int min_key;
    int min_index = -1;
    int size = heap->size;
    
    if(index * 2 <= size) {
        min_key = heap->nodes[index * 2].key;
        min_index = index * 2;
    }
    
    if(index * 2 + 1 <= size) {
        if(min_key > heap->nodes[index * 2 + 1].key) {
            min_key = heap->nodes[index * 2 + 1].key;
            min_index = index * 2 + 1;
        }
    }
    
    if(index * 4 <= size) {
        if(min_key > heap->nodes[index * 4].key) {
            min_key = heap->nodes[index * 4].key;
            min_index = index * 4;
        }
    }
    
    if(index * 4 + 1 <= size) {
        if(min_key > heap->nodes[index * 4 + 1].key) {
            min_key = heap->nodes[index * 4 + 1].key;
            min_index = index * 4 + 1;
        }
    }
    
    if(index * 4 + 2 <= size) {
        if(min_key > heap->nodes[index * 4 + 2].key) {
            min_key = heap->nodes[index * 4 + 2].key;
            min_index = index * 4 + 2;
        }
    }
    
    if(index * 4 + 3 <= size) {
        if(min_key > heap->nodes[index * 4 + 3].key) {
            min_key = heap->nodes[index * 4 + 3].key;
            min_index = index * 4 + 3;
        }
    }

    return min_index;
}//메모리에의 잘못된 접근을 피하기 위해 모두 case 분류

int find_max_child(int index) {//index 노드의 child, grand child 노드들 중에서 최대인 인덱스 반환
    HEAP* heap = MMMQ->min_max_pq;
    
    int max_key;
    int max_index = -1;
    int size = heap->size;
    
    if(index * 2 <= size) {
        max_key = heap->nodes[index * 2].key;
        max_index = index * 2;
    }
    
    if(index * 2 + 1 <= size) {
        if(max_key < heap->nodes[index * 2 + 1].key) {
            max_key = heap->nodes[index * 2 + 1].key;
            max_index = index * 2 + 1;
        }
    }
    
    if(index * 4 <= size) {
        if(max_key < heap->nodes[index * 4].key) {
            max_key = heap->nodes[index * 4].key;
            max_index = index * 4;
        }
    }
    
    if(index * 4 + 1 <= size) {
        if(max_key < heap->nodes[index * 4 + 1].key) {
            max_key = heap->nodes[index * 4 + 1].key;
            max_index = index * 4 + 1;
        }
    }
    
    if(index * 4 + 2 <= size) {
        if(max_key < heap->nodes[index * 4 + 2].key) {
            max_key = heap->nodes[index * 4 + 2].key;
            max_index = index * 4 + 2;
        }
    }
    
    if(index * 4 + 3 <= size) {
        if(max_key < heap->nodes[index * 4 + 3].key) {
            max_key = heap->nodes[index * 4 + 3].key;
            max_index = index * 4 + 3;
        }
    }

    return max_index;
}//메모리에의 잘못된 접근을 피하기 위해 모두 case 분류


//(단, index 노드의 두 child node를 root 노드로 하는 두 heap은 모두 이미 heapify 완료 O) 
void heapify_down_MIN_MAX_HEAP(int index) {//heapify-down for min-level && max-level
    HEAP* heap = MMMQ->min_max_pq;

    int child_index = index * 2;
    int level = log2_func(index);

    if(child_index <= heap->size) {
        if(level % 2 == 0) {//if index node is in min level,
            int min_index = find_min_child(index);//index 노드의 child, grand child 노드들 중에서 최소인 노드 찾기

            if(min_index >= 4 * index && min_index <= 4 * index + 3) {//if min_index is in grand child 
                if(heap->nodes[min_index].key < heap->nodes[index].key) {
                    swap_node(heap, index, min_index);//swap with max value node
                    if(heap->nodes[min_index].key > heap->nodes[min_index/2].key) swap_node(heap, min_index, min_index/2);//if condition satified, swap with its parent node

                    heapify_down_MIN_MAX_HEAP(min_index);//heapify-down in min-max heap from min_index node
                }
            }   
            else if(heap->nodes[min_index].key < heap->nodes[index].key) {//if max_index is in child and smaller than its parent key
                swap_node(heap, index, min_index);//swap with max value node
            }
        }
        else {//if index node is in max level,
            int max_index = find_max_child(index);//index 노드의 child, grand child 노드들 중에서 최대인 노드 찾기

            if(max_index >= 4 * index && max_index <= 4 * index + 3) {//if max_index is in grand child 
                if(heap->nodes[max_index].key > heap->nodes[index].key) {
                    swap_node(heap, index, max_index);//swap with max value node
                    if(heap->nodes[max_index].key < heap->nodes[max_index/2].key) swap_node(heap, max_index, max_index/2);//if condition statified, swap with its parent node

                    heapify_down_MIN_MAX_HEAP(max_index);//heapify-down in min-max heap from max_index node
                }
            }   
            else if(heap->nodes[max_index].key > heap->nodes[index].key) {//if max_index is in child and smaller than its parent key
                swap_node(heap, index, max_index);//swap with max value node
            }
        }
    }
}

NODE delete_MIN_MAX_HEAP(int index) {//min-max heap에서 index 노드 삭제 연산
    HEAP* heap = MMMQ->min_max_pq;

    swap_node(heap, index, heap->size--);//index 노드와 마지막 노드와 swap

    heapify_down_MIN_MAX_HEAP(index);//index 노드를 root 노드로 하는 sub heap에 대하여 heapify
    heapify_up_MIN_MAX_HEAP(index);//index 노드의 upper level에 있는 노드들에 대하여 heapify

    return heap->nodes[heap->size + 1];//return deleted node info
}

int foo() {
    HEAP* heap = MMMQ->min_max_pq;

    if(heap->size == 1) return 1;
    else if(heap->size == 2) return 2;
    else return (heap->nodes[2].key > heap->nodes[3].key) ? 2 : 3;

    return 0;
}//return node index having max key value

int find_min() {//fetches but does not remove the minimum element
    HEAP* heap = MMMQ->min_max_pq;

    if(!heap->size) return -1;//if heap is empty,

    return heap->nodes[1].key;
}//seek min key value

int find_max() {//fetches but does not remove the maximum element
    HEAP* heap = MMMQ->min_max_pq;

    if(!heap->size) return -1;//if heap is empty,

    if(heap->size == 1) return heap->nodes[1].key;
    else if(heap->size == 2) return heap->nodes[2].key;
    else return (heap->nodes[2].key > heap->nodes[3].key) ? heap->nodes[2].key : heap->nodes[3].key;
}//seek max key value

////////////////////finish implementing min-max priority queue.





void heapifty_down_MAX_HEAP(int index) {//heapify-down in max heap from index node
    HEAP* heap = MMMQ->median_pq->max_heap;

    int child_index = index * 2;
    while(child_index <= heap->size) {//child 노드가 heap 범위 내에 있을 때
        if((child_index < heap->size) && (heap->nodes[child_index].key < heap->nodes[child_index + 1].key)) child_index++;//left child와 right child의 값들 중 큰 값을 택
        if((heap->nodes[index].key >= heap->nodes[child_index].key)) break;//root 노드부터 차례로 내려오다가 child 노드의 값보다 이상인 순간이 오다면 break

        swap_node(heap, index, child_index);//자식 노드와 swap
        index = child_index;
        child_index *= 2;
    }
}

void heapifty_down_MIN_HEAP(int index) {//heapify-down in min heap from index node
    HEAP* heap = MMMQ->median_pq->min_heap;

    int child_index = index * 2;
    while(child_index <= heap->size) {//child 노드가 heap 범위 내에 있을 때
        if((child_index < heap->size) && (heap->nodes[child_index].key > heap->nodes[child_index + 1].key)) child_index++;//left child와 right child의 값들 중 작은 값을 택
        if((heap->nodes[index].key <= heap->nodes[child_index].key)) break;//root 노드부터 차례로 내려오다가 child 노드의 값보다 이하인 순간이 오다면 break

        swap_node(heap, index, child_index);//자식 노드와 swap
        index = child_index;
        child_index *= 2;
    }
}

void heapifty_up_MAX_HEAP(int index) {//heapify-up in max heap from index node
    HEAP* heap = MMMQ->median_pq->max_heap;

    while((index != 1 )&&(heap->nodes[index].key > heap->nodes[index/2].key)) {//index가 root 노드가 아니고 삽입할 아이템의 값이 index의 부모 노드의 값보다 클 때
        swap_node(heap, index, index/2);//부모 노드와 swap
        index /= 2;//index가 자신의 부모 노드를 가리키도록 위치 변경
    }
}

void heapifty_up_MIN_HEAP(int index) {//heapify-up in min heap from index node
    HEAP* heap = MMMQ->median_pq->min_heap;

    while((index != 1 )&&(heap->nodes[index].key < heap->nodes[index/2].key)) {//index가 root 노드가 아니고 삽입할 아이템의 값이 index의 부모 노드의 값보다 작을 때
        swap_node(heap, index, index/2);//부모 노드와 swap
        index /= 2;//index가 자신의 부모 노드를 가리키도록 위치 변경
    }
}

void insert_MAX_HEAP(int item) {//median heap의 max_heap에 item을 삽입
    HEAP* heap = MMMQ->median_pq->max_heap;

    int insertPos = ++heap->size;//노드 개수를 미리 한 개 늘리고 그 노드의 위치를 insertPos에 저장
    heap->nodes[insertPos].key = item;

    //상대편 node 초기화
    MMMQ->min_max_pq->nodes[MMMQ->min_max_pq->size + 1].opposite_heap = MMMQ->median_pq->max_heap;
    MMMQ->min_max_pq->nodes[MMMQ->min_max_pq->size + 1].syncPos = insertPos;
    //자신의 node 초기화
    heap->nodes[insertPos].opposite_heap = MMMQ->min_max_pq;
    heap->nodes[insertPos].syncPos = MMMQ->min_max_pq->size + 1;

    heapifty_up_MAX_HEAP(insertPos);//heapify-up from inserted position
}

NODE delete_MAX_HEAP(int index) {//median heap의 max_heap에서 index 위치의 노드를 삭제하고 삭제한 노드의 정보를 반환
    HEAP* heap = MMMQ->median_pq->max_heap;
    
    swap_node(heap, index, heap->size--);//마지막 노드와 swap, 그리고 힙의 크기 줄이기
    
    heapifty_down_MAX_HEAP(index);//heapify-down from index node

    return heap->nodes[heap->size + 1];//return deleted node info
}

void insert_MIN_HEAP(int item) {//median heap의 min_heap에 item을 삽입
    HEAP* heap = MMMQ->median_pq->min_heap;

    int insertPos = ++heap->size;//노드 개수를 미리 한 개 늘리고 그 노드의 위치를 insertPos에 저장
    heap->nodes[insertPos].key = item;

    //상대편 node 초기화
    MMMQ->min_max_pq->nodes[MMMQ->min_max_pq->size + 1].opposite_heap = MMMQ->median_pq->min_heap;
    MMMQ->min_max_pq->nodes[MMMQ->min_max_pq->size + 1].syncPos = insertPos;
    //자신의 node 초기화
    heap->nodes[insertPos].opposite_heap = MMMQ->min_max_pq;
    heap->nodes[insertPos].syncPos = MMMQ->min_max_pq->size + 1;

    heapifty_up_MIN_HEAP(insertPos);//heapify-up from inserted position
}

NODE delete_MIN_HEAP(int index) {//median heap의 min_heap에서 index 위치의 노드를 삭제하고 삭제한 노드의 정보를 반환
    HEAP* heap = MMMQ->median_pq->min_heap;
    
    swap_node(heap, index, heap->size--);//마지막 노드와 swap, 그리고 힙의 크기 줄이기
    
    heapifty_down_MIN_HEAP(index);//heapify-down from index node

    return heap->nodes[heap->size + 1];//return deleted node info
}

void balance_MEDIAN_HEAP() {//max heap의 노드 개수와 min heap의 노드 개수사이의 균형 맞추기
    HEAP* max_heap = MMMQ->median_pq->max_heap;
    HEAP* min_heap = MMMQ->median_pq->min_heap;

    if(max_heap->size >= min_heap->size + 2) {//최대 힙의 노드 개수가 최소 힙의 노드 개수보다 2개이상 많을 때,
        NODE temp = delete_MAX_HEAP(1);//최대 힙의 루트 노드를 pop
        delete_MIN_MAX_HEAP(temp.syncPos);
        insert_MIN_HEAP(temp.key);//pop한 루트 노드를 최소 힙에 삽입
        insert_MIN_MAX_HEAP(temp.key);
    } 
    else if(max_heap->size < min_heap->size) {//최소 힙의 노드 개수가 최대 힙의 노드 개수보다 많을 때,
        NODE temp = delete_MIN_HEAP(1);//최소 힙에서 루트 노드 pop
        delete_MIN_MAX_HEAP(temp.syncPos);
        insert_MAX_HEAP(temp.key);//pop한 노드를 최대 힙에 삽입
        insert_MIN_MAX_HEAP(temp.key);
    }
}

void insert_MEDIAN_HEAP(int item) {//중요!: 최대 힙의 노드 개수가 최소 힙의 노드 개수와 같거나 1개 더 많도록 유지해야 한다.
    HEAP* max_heap = MMMQ->median_pq->max_heap;
    HEAP* min_heap = MMMQ->median_pq->min_heap;

    if(max_heap->size == 0) {//if pq is empty,
        insert_MAX_HEAP(item);//최대 힙에 삽입
        insert_MIN_MAX_HEAP(item);
    }
    else if(max_heap->size == min_heap->size) {//최대 힙의 노드 개수와 최소 힙의 노드 개수가 같을 때
        float median = max_heap->nodes[1].key;//이전 중앙값
        if(item >  median) {//item이 중앙값보다 크다면, 최소힙에 삽입
            insert_MIN_HEAP(item);//최소 힙에 삽입
            insert_MIN_MAX_HEAP(item);

            balance_MEDIAN_HEAP();//balance median heap
        }
        else {//최대 힙에 삽입해야 할 때
            insert_MAX_HEAP(item);//최대 힙에 삽입
            insert_MIN_MAX_HEAP(item);
        }
    }
    else {
        float median = max_heap->nodes[1].key;//이전 중앙값 (총 노드의 개수가 홀수이므로 최대 힙의 루트 노드가 중앙값)
        if(item > median) {//item이 중앙값보다 크다면, 최소힙에 삽입
            insert_MIN_HEAP(item);//최소 힙에 삽입
            insert_MIN_MAX_HEAP(item);
        }
        else {
            insert_MAX_HEAP(item);//최대 힙에 삽입
            insert_MIN_MAX_HEAP(item);
            
            balance_MEDIAN_HEAP();//balance median heap
        }
    }
}//insert node with item in median heap

int find_median() {//fetches but does not remove the median element
    MEDIAN_PQ* median_pq = MMMQ->median_pq;

    if(!MMMQ->min_max_pq->size) return -1;//노드의 개수가 0개일 때
    else return median_pq->max_heap->nodes[1].key;//return median key
}//return median value but don't remove it 

////////////////////finish implementing median heap

void insert(int element) {//inserts an integer element into the priority queue.
    insert_MEDIAN_HEAP(element);//insert median heap 함수가 insert min-max heap 함수의 기능을 포함
}

int delete_min() {//deletes and returns the min element
    if(!MMMQ->min_max_pq->size) return -1;//if heap is empty, do nothing
    
    HEAP* min_heap = MMMQ->median_pq->min_heap;
    HEAP* max_heap = MMMQ->median_pq->max_heap;

    NODE deleted_node = delete_MIN_MAX_HEAP(1);//delete root node in min-max heap
    if(deleted_node.key > find_median()) {//if deleted node is in min heap,
        delete_MIN_HEAP(deleted_node.syncPos);
        
        balance_MEDIAN_HEAP();//balance median heap
    }
    else {//if deleted node is in max heap,
        delete_MAX_HEAP(deleted_node.syncPos);
        
        balance_MEDIAN_HEAP();//balance median heap
    }

    return deleted_node.key;//return median key
}

int delete_max() {//deletes and returns the max element
    if(!MMMQ->min_max_pq->size) return -1;//if heap is empty, do nothing

    NODE deleted_node = delete_MIN_MAX_HEAP(foo());//delete root node in min-max heap using foo() function (foo : min-max heap에서 max 노드의 인덱스를 반환)
    if(deleted_node.key > find_median()) {//삭제할 노드의 키 값이 중앙값보다 크다면 min heap에 위치
        delete_MIN_HEAP(deleted_node.syncPos);//최소 힙에서 해당 키를 갖고 있는 노드를 삭제

        balance_MEDIAN_HEAP();//balance median heap
    }
    else {//삭제할 노드가 max heap에 있는 경우
        delete_MAX_HEAP(deleted_node.syncPos);//최대 힙에서 해당 키를 갖고 있는 노드를 삭제

        balance_MEDIAN_HEAP();//balance median heap
    }

    return deleted_node.key;//return max key
}

int delete_median() {//deletes and returns the median element.
    if(!MMMQ->min_max_pq->size) return -1;//if heap is empty, do nothing

    NODE deleted_node = delete_MAX_HEAP(1);//delete root node in max heap of median heap
    delete_MIN_MAX_HEAP(deleted_node.syncPos);//delete synchronized node in min-max heap

    balance_MEDIAN_HEAP();//balance median heap

    return deleted_node.key;//return median key
}


int main() {
    initialize();//allocate memory and init 

    int T;
    scanf("%d", &T);
    getchar();//for 버퍼링 비우기

    char str[20];
    int len;//for storing length of instruction
    char instruction;
    int n;
    for(int i = 0; i < T; i++) {
        fgets(str, sizeof(str), stdin);//get instruction from standard input
        if(str[0] == 'I') {//'I (number)'
            sscanf(str, "%c %d", &instruction, &n);//scanf 'instruction' and 'n' from str
            insert(n);
        }   
        else if(str[0] == 'D') {//delete operation
            if(str[2] == 'M') delete_min();//'D M'
            else if(str[2] == 'X') delete_max();//'D X'
            else delete_median();//'D E'
        }
        else {//fetch operation
            if(str[2] == 'M') {//'F M'
                if(!MMMQ->min_max_pq->size) printf("NULL\n");//if heap is emty, print "NULL"
                else printf("%d\n", find_min());//print min key
            }
            else if(str[2] == 'X') {//'F X'
                if(!MMMQ->min_max_pq->size) printf("NULL\n");//if heap is emty, print "NULL"
                else printf("%d\n", find_max());//print max key
            }
            else {//'F E'
                if(!MMMQ->min_max_pq->size) printf("NULL\n");//if heap is emty, print "NULL"
                else printf("%d\n", find_median());//print median key
            }
        }
    }
    
    destroy();//free memeory 

    return 0;
}
