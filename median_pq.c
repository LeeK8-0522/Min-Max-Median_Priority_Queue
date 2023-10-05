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

void insertNode_MAX_HEAP(HEAP* heap, int item) {
    if(heap->size == MAX_NODES - 1) {//if heap is full,
        fprintf(stderr, "The heap is full.\n");
        exit(1);//프로그램 강제 종료
    }

    int insertPos = ++heap->size;//노드 개수를 미리 한 개 늘리고 그 노드의 위치를 insertPos에 저장
    heap->nodes[insertPos].key = item;

    while((insertPos != 1 )&&(item > heap->nodes[insertPos/2].key)) {//insertPos가 root 노드가 아니고 삽입할 아이템의 값이 insertPos의 부모 노드의 값보다 클 때
        swap_node(heap, insertPos, insertPos/2);
        insertPos /= 2;//insertPos가 자신의 부모 노드를 가리키도록 위치 변경
    }
}

int popNode_MAX_HEAP(HEAP* heap) {
    if(!heap->size) {//if heap is empty,
        return -1;
    }

    int pop_item = heap->nodes[1].key;//pop할 root 노드 값 저장
    
    swap_node(heap, 1, heap->size--);
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

void insertNode_MIN_HEAP(HEAP* heap, int item) {
    if(heap->size == MAX_NODES - 1) {//if heap is full,
        fprintf(stderr, "The heap is full.\n");
        exit(1);//프로그램 강제 종료
    }

    int insertPos = ++heap->size;//노드 개수를 미리 한 개 늘리고 그 노드의 위치를 insertPos에 저장
    heap->nodes[insertPos].key = item;

    while((insertPos != 1 )&&(item < heap->nodes[insertPos/2].key)) {//insertPos가 root 노드가 아니고 삽입할 아이템의 값이 insertPos의 부모 노드의 값보다 클 때
        swap_node(heap, insertPos, insertPos/2);
        insertPos /= 2;//insertPos가 자신의 부모 노드를 가리키도록 위치 변경
    }
}

int popNode_MIN_HEAP(HEAP* heap) {
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

        swap_node(heap, parentPos, childPos);
        parentPos = childPos;
        childPos *= 2;
    }

    return pop_item;
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
            insertNode_PQ(median_pq, temp);
            if(j%2==1) {
                printf("%d ", findMedian_PQ(median_pq));
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
