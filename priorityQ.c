#include <stdio.h>
#include <stdlib.h>

#define MAX_NODES 10000

void heapify_down_MAX(int index);
void heapify_down_MIN(int index);
int findMedian_PQ();

typedef struct {
    int key;
    int syncPos;//상대방 인덱스 위치 추적
    NODE* opposite;//상대방 NODE 배열 주소 (min-max heap -> median_heap->max_heap(or min_heap)->nodes, median_heap->min_max_heap->nodes)
} NODE; 

typedef struct {
    NODE nodes [MAX_NODES];
    int size;
} HEAP;

typedef struct {
    HEAP* max_heap;//최대 힙
    HEAP* min_heap;//최소 힙
} MEDIAN_PQ;//중앙값을 출력하는 우선순위 자료구조

typedef struct {
    MEDIAN_PQ* median_pq;//for 중앙값 관리
    HEAP* min_max_pq;//for 최댓값, 최솟값 관리
} MIN_MAX_MEDIAN_PQ;//중앙값, 최댓값, 최솟값을 모두 관리하는 우선순위 큐

MIN_MAX_MEDIAN_PQ* MMMQ;//전역 변수로 설정




////////////////////Belows are functions definitions




void swap_node(int index1, int index2, int flag) {//0: median_heap -> min_max_heap, 1: min_max_heap -> median_heap
    NODE* nodes = (flag) ? MMMQ->min_max_pq : MMMQ->median_pq;
    NODE* opposite_nodes = (flag) ? MMMQ->median_pq : MMMQ->min_max_pq;

    //먼저, 상대편 heap의 node 수정
    if(flag) {//if opposite node is in median_heap,
        int median = findMedian_PQ(MMMQ->median_pq);
        if(nodes[index1].key <= median && nodes[index2].key <= median) {//둘 다 max heap
            
        }
        else if(nodes[index1].key <= median && nodes[index2].key > median) {//max heap, min heap

        }
        else if(nodes[index1].key > median && nodes[index2].key <= median) {//min heap, max heap

        }
        else {//둘 다 min heap

        }
    }   
    else {//if opposite node is in min_max_heap,
        opposite_nodes[nodes[index1].syncPos].syncPos = index2;
        opposite_nodes[nodes[index2].syncPos].syncPos = index1;
    }

    //내부 heap의 node 수정
    NODE temp = nodes[index1];
    nodes[index1] = nodes[index2];
    nodes[index2] = temp;
}

void initialize() {
    MMMQ = (MIN_MAX_MEDIAN_PQ*)malloc(sizeof(MIN_MAX_MEDIAN_PQ));

    MEDIAN_PQ* median_pq = MMMQ->median_pq;
    median_pq = (MEDIAN_PQ*)malloc(sizeof(median_pq));
    median_pq->max_heap = (HEAP*)malloc(sizeof(HEAP));
    median_pq->min_heap = (HEAP*)malloc(sizeof(HEAP));

    MMMQ->min_max_pq = (HEAP*)malloc(sizeof(HEAP));//init min_max heap

    if(median_pq->max_heap == NULL || median_pq->min_heap == NULL || MMMQ->min_max_pq == NULL) {//if memory allocation failed,
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }

    median_pq->max_heap->size = 0;
    median_pq->min_heap->size = 0;
}//allocate memory and init min-max-median heap

int log2_func(int num) {//log function (시간 복잡도: O(lg num) -> 이 코드에서는 주로 O(lg(lgn)))
    int cnt = 0;
    while(num/2 > 0) {
        cnt++;
        num /= 2;
    }

    return cnt;
}

//max-level끼리만 비교 (단, index를 제외한 나머지 전체 노드들은 서로서로 heap 성질 만족 -> 사용이 의미 O)
void heapify_up_MAX(int index) {
    HEAP* heap = MMMQ->min_max_pq;
    int grand_index = index/4;
    
    while(grand_index > 0) {//while having a grand parent
        if(heap->nodes[index].key > heap->nodes[grand_index].key) {//swap child node with grand parent node until condition breaks
            swap_node(heap, index, grand_index);
            index = grand_index;
            grand_index = index/4;
        }
        else break;
    }
}   

//min-level끼리만 비교 (단, index를 제외한 나머지 전체 노드들은 서로서로 heap 성질 만족 -> 사용이 의미 O)
void heapify_up_MIN(int index) {
    HEAP* heap = MMMQ->min_max_pq;
    int grand_index = index/4;
    
    while(grand_index > 0) {//while having a grand parent
        if(heap->nodes[index].key < heap->nodes[grand_index].key) {//swap child node with grand parent node until condition breaks
            swap_node(heap, index, grand_index);
            index = grand_index;
            grand_index = index/4;
        }
        else break;
    }
} 

void insert_MIN_MAX(int item) {//insert item key
    HEAP* heap = MMMQ->min_max_pq;
    int index = ++heap->size;//우선 맨 마직막 위치에 노드를 저장
    int parent_index = index / 2;
    heap->nodes[index].key = item;

    int level = log2_func(index);

    if(index != 1) {//index가 root를 가리키지 않을 때까지
        if(level%2==0) {//if inserted position is in min level,
            if(heap->nodes[index].key > heap->nodes[parent_index].key) {//해당 node가 max-level에 있어야 하는지, min-level에 있어야 하는지 판단 (동시에 index를 제외한 나머지 노드들은 서로서로 heap 성질 만족 O)
                swap_node(heap, index, parent_index);
                heapify_up_MAX(parent_index);//heapify upper heaps
            }
            else {
                heapify_up_MIN(index);//heapify upper heaps
            }
        }
        else {//if inserted position is in max level,
            if(heap->nodes[index].key < heap->nodes[parent_index].key) {//해당 node가 max-level에 있어야 하는지, min-level에 있어야 하는지 판단(동시에 index를 제외한 나머지 노드들은 서로서로 heap 성질 만족 O)
                swap_node(heap, index, parent_index);
                heapify_up_MIN(parent_index);//heapify upper heaps
            }
            else {
                heapify_up_MAX(index);//heapify upper heaps
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

void heapify_down(int index) {//heapify down for min-level && max-level
    int level = log2_func(index);

    if(level % 2 == 0) heapify_down_MIN(index);
    else heapify_down_MAX(index);
}

//(단, index 노드의 두 child node를 root 노드로 하는 두 heap은 모두 이미 heapify 완료 O) 
void heapify_down_MAX(int index) {
    HEAP* heap = MMMQ->min_max_pq;
    int child_index = index * 2;

    if(child_index <= heap->size) {
        int max_index = find_max(heap, index);

        if(max_index >= 4 * index && max_index <= 4 * index + 3) {//if max_index is in grand child 
            if(heap->nodes[max_index].key > heap->nodes[index].key) {
                swap_node(heap, index, max_index);//swap with max value node
                if(heap->nodes[max_index].key < heap->nodes[max_index/2].key) swap_node(heap, max_index, max_index/2);//if condition statified, swap with its parent node

                heapify_down(heap);
            }
        }   
        else if(heap->nodes[max_index].key > heap->nodes[index].key) {//if max_index is in child and smaller than its parent key
            swap_node(heap, index, max_index);//swap with max value node
        }
    }
}  

//(단, index 노드의 두 child node를 root 노드로 하는 두 heap은 모두 이미 heapify 완료 O) 
void heapify_down_MIN(int index) {
    HEAP* heap = MMMQ->min_max_pq;

    int child_index = index * 2;

    if(child_index <= heap->size) {
        int min_index = find_min(heap, index);

        if(min_index >= 4 * index && min_index <= 4 * index + 3) {//if min_index is in grand child 
            if(heap->nodes[min_index].key < heap->nodes[index].key) {
                swap_node(heap, index, min_index);//swap with max value node
                if(heap->nodes[min_index].key > heap->nodes[min_index/2].key) swap_node(heap, min_index, min_index/2);//if condition satified, swap with its parent node

                heapify_down(min_index);
            }
        }   
        else if(heap->nodes[min_index].key < heap->nodes[index].key) {//if max_index is in child and smaller than its parent key
            swap_node(heap, index, min_index);//swap with max value node
        }
    }
}

int delete_MIN_MAX(int index) {//index 노드 삭제 연산
    HEAP* heap = MMMQ->min_max_pq;

    if(heap->size == 0) return -1;

    int level = log2_func(index);
    int deleted_key = heap->nodes[index].key;
    swap_node(heap, index, heap->size--);//index 노드와 마지막 노드와 swap

    heapify_down(index);//index 노드를 root 노드로 하는 sub heap을 heapify
    
    //index 노드 upper level에 대한 heapify
    if(level%2) {//for min_level
        while(index/4 > 0) {//while having grand parent
            if(heap->nodes[index].key < heap->nodes[index/4].key) {
                swap_node(heap, index, index/4);//swap with grand parent node
                index /= 4;
            }
            else break;
        } 
    }
    else {//for max_level,
        while(index/4 > 0) {//while having grand parent
            if(heap->nodes[index].key > heap->nodes[index/4].key) {
                swap_node(heap, index, index/4);//swap with grand parent node
                index /= 4;
            }
            else break;
        } 
    }

    return deleted_key;
}

int foo() {
    HEAP* heap = MMMQ->min_max_pq;

    if(heap->size == 1) return 1;
    else if(heap->size == 2) return 2;
    else return (heap->nodes[2].key > heap->nodes[3].key) ? 2 : 3;

    return 0;
}//return node index having max key value

int seekMIN() {
    HEAP* heap = MMMQ->min_max_pq;

    return heap->nodes[1].key;
}//seek min key value

int seekMAX() {
    HEAP* heap = MMMQ->min_max_pq;

    if(heap->size == 1) return heap->nodes[1].key;
    else if(heap->size == 2) return heap->nodes[2].key;
    else return (heap->nodes[2].key > heap->nodes[3].key) ? heap->nodes[2].key : heap->nodes[3].key;
}//seek max key value

////////////////////finish implementing min-max priority queue.






void insertNode_MAX_HEAP(int item) {
    HEAP* heap = MMMQ->median_pq->max_heap;

    if(heap->size == MAX_NODES - 1) {//if heap is full,
        fprintf(stderr, "The heap is full.\n");
        exit(1);//프로그램 강제 종료
    }

    int insertPos = ++heap->size;//노드 개수를 미리 한 개 늘리고 그 노드의 위치를 insertPos에 저장
    heap->nodes[insertPos].key = item;

    while((insertPos != 1 )&&(item > heap->nodes[insertPos/2].key)) {//insertPos가 root 노드가 아니고 삽입할 아이템의 값이 insertPos의 부모 노드의 값보다 클 때
        swap_node(heap, insertPos, insertPos/2);//부모 노드와 swap
        insertPos /= 2;//insertPos가 자신의 부모 노드를 가리키도록 위치 변경
    }
}

int popNode_MAX_HEAP() {//upper level -> lower level
    HEAP* heap = MMMQ->median_pq->max_heap;

    if(!heap->size) {//if heap is empty,
        return -1;//아무런 연산을 하지 않고 return
    }

    int pop_item = heap->nodes[1].key;//pop할 root 노드 값 저장
    
    swap_node(heap, 1, heap->size--);//root 노드와 마지막 노드를 swap
    //여기서는 'parentPos', 'childPos' 변수가 pop의 insertPos 역할을 수행 (즉, 위치추적)
    int parentPos = 1;//root 노드부터 시작
    int childPos = 2;//root 노드의 child 노드부터 시작

    while(childPos <= heap->size) {//child 노드가 heap 범위 내에 있을 때
        if((childPos < heap->size) && (heap->nodes[childPos].key < heap->nodes[childPos + 1].key)) childPos++;//left child와 right child의 값들 중 큰 값을 택
        if((heap->nodes[parentPos].key >= heap->nodes[childPos].key)) break;//root 노드부터 차례로 내려오다가 child 노드의 값보다 이상인 순간이 오다면 break

        swap_node(heap, parentPos, childPos);
        parentPos = childPos;
        childPos *= 2;
    }

    return pop_item;
}

void insertNode_MIN_HEAP(int item) {//lower level -> upper level
    HEAP* heap = MMMQ->median_pq->min_heap;

    if(heap->size == MAX_NODES - 1) {//if heap is full,
        fprintf(stderr, "The heap is full.\n");
        exit(1);//프로그램 강제 종료
    }

    int insertPos = ++heap->size;//노드 개수를 미리 한 개 늘리고 그 노드의 위치를 insertPos에 저장
    heap->nodes[insertPos].key = item;//마지막 노드 key에 item값 저장

    while((insertPos != 1 )&&(item < heap->nodes[insertPos/2].key)) {//insertPos가 root 노드가 아니고 삽입할 아이템의 값이 insertPos의 부모 노드의 값보다 클 때
        swap_node(heap, insertPos, insertPos/2);//부모 노드와 swap
        insertPos /= 2;//insertPos가 자신의 부모 노드를 가리키도록 위치 변경
    }
}

int popNode_MIN_HEAP() {
    HEAP* heap = MMMQ->median_pq->min_heap;

    if(!heap->size) {//if heap is empty,
        return -1;
    }

    int pop_item = heap->nodes[1].key;//pop할 root 노드 값 저장
    
    swap_node(heap, 1, heap->size--);
    //여기서는 'parentPos', 'childPos' 변수가 pop의 insertPos 역할을 수행 (즉, 위치추적)
    int parentPos = 1;//root 노드부터 시작
    int childPos = 2;//root 노드의 child 노드부터 시작

    while(childPos <= heap->size) {//child 노드가 heap 범위 내에 있을 때
        if((childPos < heap->size) && (heap->nodes[childPos].key > heap->nodes[childPos + 1].key)) childPos++;//left child와 right child의 값들 중 작은 값을 택
        if((heap->nodes[parentPos].key <= heap->nodes[childPos].key)) break;//root 노드부터 차례로 내려오다가 child 노드의 값보다 이하인 순간이 오다면 break

        swap_node(heap, parentPos, childPos);//자식 노드와 swap
        parentPos = childPos;
        childPos *= 2;
    }

    return pop_item;
}


void insertNode_PQ(int item) {//중요!: 최대 힙의 노드 개수가 최소 힙의 노드 개수와 같거나 1개 더 많도록 유지해야 한다.
    HEAP* max_heap = MMMQ->median_pq->max_heap;
    HEAP* min_heap = MMMQ->median_pq->min_heap;

    if(!max_heap->size&&!min_heap->size) {//if pq is empty,
        insertNode_MAX_HEAP(item);
    }
    else if(max_heap->size == min_heap->size) {//최대 힙의 노드 개수와 최소 힙의 노드 개수가 같을 때
        float median = (max_heap->nodes[1].key + min_heap->nodes[1].key)/2;//이전 중앙값 (총 노드의 개수가 짝수이므로 평균값이 중앙값)
        if(item >  median) {
            insertNode_MIN_HEAP(item);//최소 힙에 삽입
            int temp = popNode_MIN_HEAP(min_heap);//최소 힙에서 루트 노드 pop
            insertNode_MAX_HEAP(temp);//pop한 노드를 최대 힙에 삽입
        }
        else {
            insertNode_MAX_HEAP(item);//최대 힙에 삽입
        }
    }
    else {
        float median = max_heap->nodes[1].key;//이전 중앙값 (총 노드의 개수가 홀수이므로 최대 힙의 루트 노드가 중앙값)
        if(item > median) {
            insertNode_MIN_HEAP(item);//최소 힙에 삽입
        }
        else {
            insertNode_MAX_HEAP(item);//최대 힙에 삽입
            int temp = popNode_MAX_HEAP(max_heap);//최대 힙의 루트 노드를 pop
            insertNode_MIN_HEAP(temp);//pop한 루트 노드를 최소 힙에 삽입
        }
    }
}//insert node with item 

int findMedian_PQ() {
    MEDIAN_PQ* median_pq = MMMQ->median_pq;

    int total_size = median_pq->max_heap->size + median_pq->min_heap->size;//총 노드의 개수
    if(!total_size) return 0;//노드의 개수가 0개일 때
    else if(total_size % 2 == 0) return (median_pq->max_heap->nodes[1].key + median_pq->min_heap->nodes[1].key)/2;//총 노드의 개수가 짝수일 때 
    else return median_pq->max_heap->nodes[1].key;//총 노드의 개수가 홀수일 때
}//return median value but don't remove it 

////////////////////finish implementing median heap 





void insert(int element) {

}


int delete_min() {

}


int delete_max() {

}


int delete_median() {

}


int find_min() {

}


int find_max() {

}


int find_median() {

}

////////////////////finidh implementing min-max-median heap functions

int main() {
    initialize(MMMQ);//allocate memory and init MMMQ



    return 0;
}
