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

typedef struct {
    HEAP* max_heap;//최대 힙
    HEAP* min_heap;//최소 힙
} MEDIAN_PQ;//중앙값을 출력하는 우선순위 자료구조

typedef struct {
    MEDIAN_PQ* median_pq;//for 중앙값 관리
    HEAP* min_max_pq;//for 최댓값, 최솟값 관리
} MIN_MAX_MEDIAN_PQ;

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

void initialize(MEDIAN_PQ* median_pq) {
    median_pq->max_heap = (HEAP*)malloc(sizeof(HEAP));
    median_pq->min_heap = (HEAP*)malloc(sizeof(HEAP));

    if(median_pq->max_heap == NULL || median_pq->min_heap == NULL) {//if memory allocation failed,
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }

    median_pq->max_heap->size = 0;
    median_pq->min_heap->size = 0;
}//우선순위 큐 자료구조 변수 초기화

////////////////////finish implementing min-max priority queue.




void insertNode_MAX_HEAP(HEAP* heap, int item) {
    int index = ++heap->size;//노드 개수를 미리 한 개 늘리고 그 노드의 위치를 index에 저장
    heap->nodes[index].key = item;

    while((index != 1 )&&(item > heap->nodes[index/2].key)) {//index가 root 노드가 아니고 삽입할 아이템의 값이 index의 부모 노드의 값보다 클 때
        swap_node(heap, index, index/2);//index 노드와 부모 노드를 swap
        index /= 2;//index가 자신의 부모 노드를 가리키도록 위치 변경
    }
}

int popNode_MAX_HEAP(HEAP* heap) {
    if(!heap->size) {//if heap is empty,
        return -1;
    }

    NODE pop_item = heap->nodes[1];//pop할 root 노드 값 저장
    NODE temp_item = heap->nodes[heap->size--];//heap의 마지막 노드의 값을 저장하고 노드의 개수를 한 개 미리 줄인다.

    //여기서는 'parentPos', 'childPos' 변수가 pop의 insertPos 역할을 수행 (즉, 위치추적)
    int parentPos = 1;//root 노드부터 시작
    int childPos = 2;//root 노드의 child 노드부터 시작

    while(childPos <= heap->size) {//child 노드가 heap 범위 내에 있을 때
        if((childPos < heap->size) && (heap->nodes[childPos].key < heap->nodes[childPos + 1].key)) childPos++;//left child와 right child의 값들 중 큰 값을 택
        if((temp_item.key >= heap->nodes[childPos].key)) break;//root 노드부터 차례로 내려오다가 child 노드의 값보다 이상인 순간이 오다면 parent 노드에 노드 값을 삽입하기 위해 break

        heap->nodes[parentPos] = heap->nodes[childPos];//parent 노드 값에 child 노드 값을 대입 (for switching. ex. '10 9' -> '9 9')
        parentPos = childPos;
        childPos *= 2;
    }

    heap->nodes[parentPos] = temp_item;//switching 연산이 종료되면 최종적으로 삽입할 위치에 아이템 값 대입 (ex. '10 9 8 7 6' -> '9 8 7 6 6' -> '9 8 7 6 5')

    return pop_item.key;
}

void insertNode_MIN_HEAP(HEAP* heap, int item) {
    if(heap->size == MAX_NODES - 1) {//if heap is full,
        fprintf(stderr, "The heap is full.\n");
        exit(1);//프로그램 강제 종료
    }

    int insertPos = ++heap->size;//노드 개수를 미리 한 개 늘리고 그 노드의 위치를 insertPos에 저장

    while((insertPos != 1 )&&(item < heap->nodes[insertPos/2].key)) {//insertPos가 root 노드가 아니고 삽입할 아이템의 값이 insertPos의 부모 노드의 값보다 작을 때
        heap->nodes[insertPos] = heap->nodes[insertPos/2];//child 노드에 parent 노드 값을 대입 (for switching. ex. '10 5' -> '10 10')
        insertPos /= 2;//insertPos가 자신의 부모 노드를 가리키도록 위치 변경
    }

    heap->nodes[insertPos].key = item;//최종적으로 연산이 끝나면 마무리로 최종 삽입 위치에 아이템 값을 삽입. (ex. '10 9 8 7 6 5' -> '10 10 9 8 7 6' -> '11 10 9 8 7 6')
}

int popNode_MIN_HEAP(HEAP* heap) {
    if(!heap->size) {//if heap is empty,
        fprintf(stderr, "The heap is empty.\n");
        exit(1);//프로그램 강제 종료
    }

    NODE pop_item = heap->nodes[1];//pop할 root 노드 값 저장
    NODE temp_item = heap->nodes[heap->size--];//heap의 마지막 노드의 값을 저장하고 노드의 개수를 한 개 미리 줄인다.

    //여기서는 'parentPos', 'childPos' 변수가 pop의 insertPos 역할을 수행 (즉, 위치추적)
    int parentPos = 1;//root 노드부터 시작
    int childPos = 2;//root 노드의 child 노드부터 시작

    while(childPos <= heap->size) {//child 노드가 heap 범위 내에 있을 때
        if((childPos < heap->size) && (heap->nodes[childPos].key > heap->nodes[childPos + 1].key)) childPos++;//left child와 right child의 값들 중 작은 값을 택
        if((temp_item.key <= heap->nodes[childPos].key)) break;//root 노드부터 차례로 내려오다가 child 노드의 값보다 이하인 순간이 오다면 parent 노드에 노드 값을 삽입하기 위해 break

        heap->nodes[parentPos] = heap->nodes[childPos];//parent 노드 값에 child 노드 값을 대입 (for switching. ex. '10 9' -> '9 9')
        parentPos = childPos;
        childPos *= 2;
    }

    heap->nodes[parentPos] = temp_item;//switching 연산이 종료되면 최종적으로 삽입할 위치에 아이템 값 대입 (ex. '10 9 8 7 6' -> '9 8 7 6 6' -> '9 8 7 6 5')

    return pop_item.key;
}

void insertNode_PQ(MEDIAN_PQ* median_pq, int item) {//중요!: 최대 힙의 노드 개수가 최소 힙의 노드 개수와 같거나 1개 더 많도록 유지해야 한다.
    HEAP* max_heap = median_pq->max_heap;
    HEAP* min_heap = median_pq->min_heap;
    if(!max_heap->size&&!min_heap->size) {//if pq is empty,
        insertNode_MAX_HEAP(max_heap, item);
    }
    else if(max_heap->size == min_heap->size) {//최대 힙의 노드 개수와 최소 힙의 노드 개수가 같을 때
        float median = (max_heap->nodes[1].key + min_heap->nodes[1].key)/2;//이전 중앙값 (총 노드의 개수가 짝수이므로 평균값이 중앙값)
        if(item >  median) {
            insertNode_MIN_HEAP(min_heap, item);//최소 힙에 삽입
            int temp = popNode_MIN_HEAP(min_heap);//최소 힙에서 루트 노드 pop
            insertNode_MAX_HEAP(max_heap, temp);//pop한 노드를 최대 힙에 삽입
        }
        else {
            insertNode_MAX_HEAP(max_heap, item);//최대 힙에 삽입
        }
    }
    else {
        float median = max_heap->nodes[1].key;//이전 중앙값 (총 노드의 개수가 홀수이므로 최대 힙의 루트 노드가 중앙값)
        if(item > median) {
            insertNode_MIN_HEAP(min_heap, item);//최소 힙에 삽입
        }
        else {
            insertNode_MAX_HEAP(max_heap, item);//최대 힙에 삽입
            int temp = popNode_MAX_HEAP(max_heap);//최대 힙의 루트 노드를 pop
            insertNode_MIN_HEAP(min_heap, temp);//pop한 루트 노드를 최소 힙에 삽입
        }
    }
}//insert node with item 

int findMedian_PQ(MEDIAN_PQ* median_pq) {
    int total_size = median_pq->max_heap->size + median_pq->min_heap->size;//총 노드의 개수
    if(!total_size) return 0;//노드의 개수가 0개일 때
    else if(total_size % 2 == 0) return (median_pq->max_heap->nodes[1].key + median_pq->min_heap->nodes[1].key)/2;//총 노드의 개수가 짝수일 때 
    else return median_pq->max_heap->nodes[1].key;//총 노드의 개수가 홀수일 때
}//return median value but don't remove it 

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

