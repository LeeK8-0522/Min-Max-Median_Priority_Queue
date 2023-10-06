#include <stdio.h>
#include <stdlib.h>

#define MAX_NODES 10000

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

int log2_func(int num) {//log function (시간 복잡도: O(lg num) -> 이 코드에서는 주로 O(lg(lgn)))
    int cnt = 0;
    while(num/2 > 0) {
        cnt++;
        num /= 2;
    }

    return cnt;
}

void heapifty_down_MAX_HEAP(HEAP* heap, int index) {
    int child_index = index * 2;
    while(child_index <= heap->size) {//child 노드가 heap 범위 내에 있을 때
        if((child_index < heap->size) && (heap->nodes[child_index].key < heap->nodes[child_index + 1].key)) child_index++;//left child와 right child의 값들 중 큰 값을 택
        if((heap->nodes[index].key >= heap->nodes[child_index].key)) break;//root 노드부터 차례로 내려오다가 child 노드의 값보다 이상인 순간이 오다면 break

        swap_node(heap, index, child_index);//자식 노드와 swap
        index = child_index;
        child_index *= 2;
    }
}

void heapifty_down_MIN_HEAP(HEAP* heap, int index) {
    int child_index = index * 2;
    while(child_index <= heap->size) {//child 노드가 heap 범위 내에 있을 때
        if((child_index < heap->size) && (heap->nodes[child_index].key > heap->nodes[child_index + 1].key)) child_index++;//left child와 right child의 값들 중 작은 값을 택
        if((heap->nodes[index].key <= heap->nodes[child_index].key)) break;//root 노드부터 차례로 내려오다가 child 노드의 값보다 이하인 순간이 오다면 break

        swap_node(heap, index, child_index);//자식 노드와 swap
        index = child_index;
        child_index *= 2;
    }
}

void heapifty_up_MAX_HEAP(HEAP* heap, int index) {
    while((index != 1 )&&(heap->nodes[index].key > heap->nodes[index/2].key)) {//index가 root 노드가 아니고 삽입할 아이템의 값이 index의 부모 노드의 값보다 클 때
        swap_node(heap, index, index/2);//부모 노드와 swap
        index /= 2;//index가 자신의 부모 노드를 가리키도록 위치 변경
    }
}

void heapifty_up_MIN_HEAP(HEAP* heap, int index) {
    while((index != 1 )&&(heap->nodes[index].key < heap->nodes[index/2].key)) {//index가 root 노드가 아니고 삽입할 아이템의 값이 index의 부모 노드의 값보다 작을 때
        swap_node(heap, index, index/2);//부모 노드와 swap
        index /= 2;//index가 자신의 부모 노드를 가리키도록 위치 변경
    }
}

void insert_MAX_HEAP(HEAP* heap, int item) {
    if(heap->size == MAX_NODES - 1) {//if heap is full,
        fprintf(stderr, "The heap is full.\n");
        exit(1);//프로그램 강제 종료
    }

    int insertPos = ++heap->size;//노드 개수를 미리 한 개 늘리고 그 노드의 위치를 insertPos에 저장
    heap->nodes[insertPos].key = item;

    heapifty_up_MAX_HEAP(heap, insertPos);
}

int delete_MAX_HEAP(HEAP* heap, int index) {
    if(heap->size < index) return -1;//if there are not enough nodes, return -1

    int pop_item = heap->nodes[index].key;//pop할 root 노드 값 저장
    
    swap_node(heap, index, heap->size--);
    
    heapifty_down_MAX_HEAP(heap, 1);

    return pop_item;
}

void insert_MIN_HEAP(HEAP* heap, int item) {
    if(heap->size == MAX_NODES - 1) {//if heap is full,
        fprintf(stderr, "The heap is full.\n");
        exit(1);//프로그램 강제 종료
    }

    int insertPos = ++heap->size;//노드 개수를 미리 한 개 늘리고 그 노드의 위치를 insertPos에 저장
    heap->nodes[insertPos].key = item;

    heapifty_up_MIN_HEAP(heap, insertPos);
}

int delete_MIN_HEAP(HEAP* heap, int index) {
    if(heap->size < index) return -1;//if there are not enough nodes, return -1

    int pop_item = heap->nodes[index].key;//pop할 root 노드 값 저장
    
    swap_node(heap, 1, heap->size--);
    
    heapifty_down_MIN_HEAP(heap, 1);

    return pop_item;
}

void insert_MEDIAN_HEAP(MEDIAN_PQ* median_pq, int item) {//중요!: 최대 힙의 노드 개수가 최소 힙의 노드 개수와 같거나 1개 더 많도록 유지해야 한다.
    HEAP* max_heap = median_pq->max_heap;
    HEAP* min_heap = median_pq->min_heap;
    if(!max_heap->size&&!min_heap->size) {//if pq is empty,
        insert_MAX_HEAP(max_heap, item);
    }
    else if(max_heap->size == min_heap->size) {//최대 힙의 노드 개수와 최소 힙의 노드 개수가 같을 때
        float median = (max_heap->nodes[1].key + min_heap->nodes[1].key)/2;//이전 중앙값 (총 노드의 개수가 짝수이므로 평균값이 중앙값)
        if(item >  median) {
            insert_MIN_HEAP(min_heap, item);//최소 힙에 삽입
            int temp = delete_MIN_HEAP(min_heap, 1);//최소 힙에서 루트 노드 pop
            insert_MAX_HEAP(max_heap, temp);//pop한 노드를 최대 힙에 삽입
        }
        else {
            insert_MAX_HEAP(max_heap, item);//최대 힙에 삽입
        }
    }
    else {
        float median = max_heap->nodes[1].key;//이전 중앙값 (총 노드의 개수가 홀수이므로 최대 힙의 루트 노드가 중앙값)
        if(item > median) {
            insert_MIN_HEAP(min_heap, item);//최소 힙에 삽입
        }
        else {
            insert_MAX_HEAP(max_heap, item);//최대 힙에 삽입
            int temp = delete_MAX_HEAP(max_heap, 1);//최대 힙의 루트 노드를 pop
            insert_MIN_HEAP(min_heap, temp);//pop한 루트 노드를 최소 힙에 삽입
        }
    }
}//insert node with item 

int seek_median(MEDIAN_PQ* median_pq) {
    int total_size = median_pq->max_heap->size + median_pq->min_heap->size;//총 노드의 개수
    if(!total_size) return 0;//노드의 개수가 0개일 때
    else if(total_size % 2 == 0) return (median_pq->max_heap->nodes[1].key + median_pq->min_heap->nodes[1].key)/2;//총 노드의 개수가 짝수일 때 
    else return median_pq->max_heap->nodes[1].key;//총 노드의 개수가 홀수일 때
}//return median value but don't remove it 


int main() {
    int N, n;
    int temp;
    scanf("%d", &N);
    for(int i = 0; i < N; i++) {
        scanf("%d", &n);
        MEDIAN_PQ* median_pq = (MEDIAN_PQ*)malloc(sizeof(MEDIAN_PQ));
        if(median_pq==NULL) {//if memory allocation failed exit program
            fprintf(stderr, "Memory allocation failed.\n");
            exit(1);
        }
        initialize(median_pq);
        //우선순위 큐 초기화
        printf("%d\n", (int)(n+1)/2);//중앙값 개수 출력
        int cnt = 0;//출력한 횟수
        for(int j = 1; j <= n; j++) {
            scanf("%d", &temp);
            insert_MEDIAN_HEAP(median_pq, temp);
            if(j%2==1) {
                printf("%d ", seek_median(median_pq));
                cnt++;

                if(cnt % 10 == 0) printf("\n");//<-이 부분 주의!
            }
        }

        if(cnt % 10 != 0) printf("\n");

        free(median_pq->max_heap);
        free(median_pq->min_heap);
        free(median_pq);
    }  

    return 0;
}
