#include <stdio.h>
#include <stdlib.h>

#define MAX_NODES 1000010

typedef struct {
    int key;
} NODE; 

typedef struct {
    NODE nodes [MAX_NODES];
    int size;
} HEAP;

HEAP* min_max_pq;//전역변수로 설정

int log2_func(int num) {//log function (시간 복잡도: O(lg num) -> 이 코드에서는 주로 O(lg(lgn)))
    int cnt = 0;
    while(num/2 > 0) {
        cnt++;
        num /= 2;
    }

    return cnt;
}

void swap_node(int index1, int index2) {
    NODE temp = min_max_pq->nodes[index1];
    min_max_pq->nodes[index1] = min_max_pq->nodes[index2];
    min_max_pq->nodes[index2] = temp;
}

void initialize() {
    min_max_pq->size = 0;
}//init min-max heap

void heapify_up_MIN_MAX_HEAP(int index) {
    HEAP* heap = min_max_pq;
    
    int level = log2_func(index);
    int grand_index = index/4;
    
    if(level % 2 == 0) {//min-level끼리만 비교 (단, index를 제외한 나머지 전체 노드들은 서로서로 heap 성질 만족 -> 사용이 의미 O)
        while(grand_index > 0) {//while having a grand parent
            if(heap->nodes[index].key < heap->nodes[grand_index].key) {//swap child node with grand parent node until condition breaks
                swap_node(index, grand_index);
                index = grand_index;
                grand_index = index/4;
            }
            else break;
        }
    }
    else {//max-level끼리만 비교 (단, index를 제외한 나머지 전체 노드들은 서로서로 heap 성질 만족 -> 사용이 의미 O)
        while(grand_index > 0) {//while having a grand parent
            if(heap->nodes[index].key > heap->nodes[grand_index].key) {//swap child node with grand parent node until condition breaks
                swap_node(index, grand_index);
                index = grand_index;
                grand_index = index/4;
            }
            else break;
        }
    }
}

void insert_MIN_MAX_HEAP(int item) {//insert item key
    HEAP* heap = min_max_pq;
    int index = ++heap->size;//우선 맨 마직막 위치에 노드를 저장
    int parent_index = index / 2;
    heap->nodes[index].key = item;

    int level = log2_func(index);

    if(index != 1) {//index가 root를 가리키지 않을 때까지
        if(level%2==0) {//if inserted position is in min level,
            if(heap->nodes[index].key > heap->nodes[parent_index].key) {//해당 node가 max-level에 있어야 하는지, min-level에 있어야 하는지 판단 (동시에 index를 제외한 나머지 노드들은 서로서로 heap 성질 만족 O)
                swap_node(index, parent_index);
                heapify_up_MIN_MAX_HEAP(parent_index);//heapify upper heaps
            }
            else {
                heapify_up_MIN_MAX_HEAP(index);//heapify upper heaps
            }
        }
        else {//if inserted position is in max level,
            if(heap->nodes[index].key < heap->nodes[parent_index].key) {//해당 node가 max-level에 있어야 하는지, min-level에 있어야 하는지 판단(동시에 index를 제외한 나머지 노드들은 서로서로 heap 성질 만족 O)
                swap_node(index, parent_index);
                heapify_up_MIN_MAX_HEAP(parent_index);//heapify upper heaps
            }
            else {
                heapify_up_MIN_MAX_HEAP(index);//heapify upper heaps
            }
        }
    }
}

int find_min_child(int index) {//index 노드의 child, grand child 노드들 중에서 최솟인 인덱스 반환
    HEAP* heap = min_max_pq;

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
    HEAP* heap = min_max_pq;
    
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
void heapify_down_MIN_MAX_HEAP(int index) {//heapify down for min-level && max-level
    HEAP* heap = min_max_pq;
    int child_index = index * 2;
    int level = log2_func(index);

    if(child_index <= heap->size) {
        if(level % 2 == 0) {
            int min_index = find_min_child(index);

            if(min_index >= 4 * index && min_index <= 4 * index + 3) {//if min_index is in grand child 
                if(heap->nodes[min_index].key < heap->nodes[index].key) {
                    swap_node(index, min_index);//swap with max value node
                    if(heap->nodes[min_index].key > heap->nodes[min_index/2].key) swap_node(min_index, min_index/2);//if condition satified, swap with its parent node

                    heapify_down_MIN_MAX_HEAP(min_index);
                }
            }   
            else if(heap->nodes[min_index].key < heap->nodes[index].key) {//if max_index is in child and smaller than its parent key
                swap_node(index, min_index);//swap with max value node
            }
        }
        else {
            int max_index = find_max_child(index);

            if(max_index >= 4 * index && max_index <= 4 * index + 3) {//if max_index is in grand child 
                if(heap->nodes[max_index].key > heap->nodes[index].key) {
                    swap_node(index, max_index);//swap with max value node
                    if(heap->nodes[max_index].key < heap->nodes[max_index/2].key) swap_node(max_index, max_index/2);//if condition statified, swap with its parent node

                    heapify_down_MIN_MAX_HEAP(max_index);
                }
            }   
            else if(heap->nodes[max_index].key > heap->nodes[index].key) {//if max_index is in child and smaller than its parent key
                swap_node(index, max_index);//swap with max value node
            }
        }
    }
}

int delete_MIN_MAX_HEAP(int index) {//index 노드 삭제 연산
    HEAP* heap = min_max_pq;

    if(heap->size == 0) return -1;

    int level = log2_func(index);
    int deleted_key = heap->nodes[index].key;
    swap_node(index, heap->size--);//index 노드와 마지막 노드와 swap

    heapify_down_MIN_MAX_HEAP(index);//index 노드를 root 노드로 하는 sub heap을 heapify
    
    //index 노드 upper level에 대한 heapify
    if(level%2==0) {//for min_level
        while(index/4 > 0) {//while having grand parent
            if(heap->nodes[index].key < heap->nodes[index/4].key) {
                swap_node(index, index/4);//swap with grand parent node
                index /= 4;
            }
            else break;
        } 
    }
    else {//for max_level,
        while(index/4 > 0) {//while having grand parent
            if(heap->nodes[index].key > heap->nodes[index/4].key) {
                swap_node(index, index/4);//swap with grand parent node
                index /= 4;
            }
            else break;
        } 
    }

    return deleted_key;
}

int foo() {
    HEAP* heap = min_max_pq;

    if(heap->size == 1) return 1;
    else if(heap->size == 2) return 2;
    else return (heap->nodes[2].key > heap->nodes[3].key) ? 2 : 3;

    return 0;
}//return node index having max key value

int seek_min() {
    HEAP* heap = min_max_pq;

    return heap->nodes[1].key;
}//seek min key value

int seek_max() {
    HEAP* heap = min_max_pq;

    if(heap->size == 1) return heap->nodes[1].key;
    else if(heap->size == 2) return heap->nodes[2].key;
    else return (heap->nodes[2].key > heap->nodes[3].key) ? heap->nodes[2].key : heap->nodes[3].key;
}//seek max key value

////////////////////finish implementing min-max priority queue.


int main() {
    int T;
    char instruction;
    int k, n;

    scanf("%d", &T);
    getchar();
    for(int i = 0; i < T; i++) {
        scanf("%d", &k);
        getchar();

        min_max_pq = (HEAP*)malloc(sizeof(HEAP));
        initialize(min_max_pq);
        for(int i = 0; i < k; i++) {
            scanf("%c %d", &instruction, &n);
            getchar();

            if(instruction == 'I') insert_MIN_MAX_HEAP(n);
            else {
                if(n == 1) delete_MIN_MAX_HEAP(foo(min_max_pq));
                else delete_MIN_MAX_HEAP(1);
            }
        }

        if(min_max_pq->size == 0) printf("EMPTY\n");
        else printf("%d %d\n", seek_max(), seek_min());

        free(min_max_pq);
    }

    return 0;
}
