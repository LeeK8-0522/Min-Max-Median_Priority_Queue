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

void swap_node(HEAP* heap, int index1, int index2) {
    NODE temp = heap->nodes[index1];
    heap->nodes[index1] = heap->nodes[index2];
    heap->nodes[index2] = temp;
}

void initialize(HEAP* heap) {
    heap->size = 0;
}

int log2_func(int num) {
    int cnt = 0;
    while(num/2 > 0) {
        cnt++;
        num /= 2;
    }

    return cnt;
}

void heapify_down_MAX(HEAP* heap, int index);
void heapify_down_MIN(HEAP* heap, int index);

//max-level끼리만 비교 (단, index를 제외한 나머지 전체 노드들은 서로서로 heap 성질 만족 -> 사용이 의미 O)
void heapify_up_MAX(HEAP* heap, int index) { 
    int grand_index = index/4;
    
    while(grand_index > 0) {//while having a grand parent
        if(heap->nodes[index].key > heap->nodes[grand_index].key) {
            swap_node(heap, index, grand_index);
            index = grand_index;
            grand_index = index/4;
        }
        else break;
    }
}   

//min-level끼리만 비교 (단, index를 제외한 나머지 전체 노드들은 서로서로 heap 성질 만족 -> 사용이 의미 O)
void heapify_up_MIN(HEAP* heap, int index) {
    int grand_index = index/4;
    
    while(grand_index > 0) {//while having a grand parent
        if(heap->nodes[index].key < heap->nodes[grand_index].key) {
            swap_node(heap, index, grand_index);
            index = grand_index;
            grand_index = index/4;
        }
        else break;
    }
} 

void insert(HEAP* heap, int item) {//insert item key
    int index = ++heap->size;//inserted position
    int parent_index = index / 2;//parent's index position
    heap->nodes[index].key = item;

    int level = log2_func(index);

    if(index != 1) {
        if(level%2==0) {//if inserted position is in min level,
            if(heap->nodes[index].key > heap->nodes[parent_index].key) {//해당 node가 max-level에 있어야 하는지, min-level에 있어야 하는지 판단 (동시에 index를 제외한 나머지 노드들은 서로서로 heap 성질 만족 O)
                swap_node(heap, index, parent_index);
                heapify_up_MAX(heap, parent_index);//heapify upper heaps
            }
            else {
                heapify_up_MIN(heap, index);//heapify upper heaps
            }
        }
        else {//if inserted position is in max level,
            if(heap->nodes[index].key < heap->nodes[parent_index].key) {//해당 node가 max-level에 있어야 하는지, min-level에 있어야 하는지 판단(동시에 index를 제외한 나머지 노드들은 서로서로 heap 성질 만족 O)
                swap_node(heap, index, parent_index);
                heapify_up_MIN(heap, parent_index);//heapify upper heaps
            }
            else {
                heapify_up_MAX(heap, index);//heapify upper heaps
            }
        }
    }
}

int find_min(HEAP* heap, int index) {//index 노드의 child, grand child 노드들 중에서 최솟인 인덱스 반환
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
}

int find_max(HEAP* heap, int index) {//index 노드의 child, grand child 노드들 중에서 최대인 인덱스 반환
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
}

void heapify_down(HEAP* heap, int index) {
    int level = log2_func(index);

    if(level % 2 == 0) heapify_down_MIN(heap, index);
    else heapify_down_MAX(heap, index);
}

//(단, index 노드의 두 child node를 root 노드로 하는 두 heap은 모두 이미 heapify 완료 O) 
void heapify_down_MAX(HEAP* heap, int index) {
    int child_index = index * 2;

    if(child_index <= heap->size) {
        int max_index = find_max(heap, index);

        if(max_index >= 4 * index && max_index <= 4 * index + 3) {//if max_index is in grand child 
            if(heap->nodes[max_index].key > heap->nodes[index].key) {
                swap_node(heap, index, max_index);
                if(heap->nodes[max_index].key < heap->nodes[max_index/2].key) swap_node(heap, max_index, max_index/2);

                heapify_down(heap, max_index);
            }
        }   
        else if(heap->nodes[max_index].key > heap->nodes[index].key) {//if max_index is in child and smaller than its parent key
            swap_node(heap, index, max_index);
        }
    }
}  

//(단, index 노드의 두 child node를 root 노드로 하는 두 heap은 모두 이미 heapify 완료 O) 
void heapify_down_MIN(HEAP* heap, int index) {
    int child_index = index * 2;

    if(child_index <= heap->size) {
        int min_index = find_min(heap, index);

        if(min_index >= 4 * index && min_index <= 4 * index + 3) {//if min_index is in grand child 
            if(heap->nodes[min_index].key < heap->nodes[index].key) {
                swap_node(heap, index, min_index);
                if(heap->nodes[min_index].key > heap->nodes[min_index/2].key) swap_node(heap, min_index, min_index/2);

                heapify_down(heap, min_index);
            }
        }   
        else if(heap->nodes[min_index].key < heap->nodes[index].key) {//if max_index is in child and smaller than its parent key
            swap_node(heap, index, min_index);
        }
    }
}

int delete(HEAP* heap, int index) {
    if(heap->size == 0) return -1;

    int level = log2_func(index);
    int deleted_key = heap->nodes[index].key;
    swap_node(heap, index, heap->size--);

    heapify_down(heap, index);
    
    if(level%2) {//for min_level
        while(index/4 > 0) {//while having grand parent
            if(heap->nodes[index].key < heap->nodes[index/4].key) {
                swap_node(heap, index, index/4);
                index /= 4;
            }
            else break;
        } 
    }
    else {//for max_level,
        while(index/4 > 0) {//while having grand parent
            if(heap->nodes[index].key > heap->nodes[index/4].key) {
                swap_node(heap, index, index/4);
                index /= 4;
            }
            else break;
        } 
    }

    return deleted_key;
}

int foo(HEAP* heap) {
    if(heap->size == 1) return 1;
    else if(heap->size == 2) return 2;
    else return (heap->nodes[2].key > heap->nodes[3].key) ? 2 : 3;

    return 0;
}

int seekMIN(HEAP* heap) {
    return heap->nodes[1].key;
}

int seekMAX(HEAP* heap) {
    if(heap->size == 1) return heap->nodes[1].key;
    else if(heap->size == 2) return heap->nodes[2].key;
    else return (heap->nodes[2].key > heap->nodes[3].key) ? heap->nodes[2].key : heap->nodes[3].key;
}

int main() {
    int T;
    char instruction;
    int k, n;

    scanf("%d", &T);
    getchar();
    for(int i = 0; i < T; i++) {
        scanf("%d", &k);
        getchar();

        HEAP* heap = (HEAP*)malloc(sizeof(HEAP));
        initialize(heap);
        for(int i = 0; i < k; i++) {
            scanf("%c %d", &instruction, &n);
            getchar();

            if(instruction == 'I') insert(heap, n);
            else {
                if(n == 1) delete(heap, foo(heap));
                else delete(heap, 1);
            }
        }

        if(heap->size == 0) printf("EMPTY\n");
        else printf("%d %d\n", seekMAX(heap), seekMIN(heap));
    }

    return 0;
}

