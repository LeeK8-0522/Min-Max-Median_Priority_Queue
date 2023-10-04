#include <stdio.h>
#include <stdlib.h>

#define MAX_ELEMENTS 10

typedef struct {
    int key;
    int syncPos;
    struct NODE* opposite;
} NODE;

typedef struct {
    NODE nodes [MAX_ELEMENTS];
    int number_of_nodes;//저장하고 있는 노드의 개수
} HEAP;

typedef struct {
    HEAP* max_heap;//최대 힙
    HEAP* min_heap;//최소 힙
} DQ;//이중 우선순위 큐

void initialize(DQ* dq) {
    dq->max_heap = (HEAP*)malloc(sizeof(HEAP));
    dq->min_heap = (HEAP*)malloc(sizeof(HEAP));

    if(dq->max_heap == NULL || dq->min_heap == NULL) {//if memory allocation failed,
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }

    dq->max_heap->number_of_nodes = 0;
    dq->min_heap->number_of_nodes = 0;
}//우선순위 큐 자료구조 변수 초기화

void switchNode(HEAP* heap, int index1, int index2) {//해당 힙에서 index1의 노드와 index2의 노드를 switch할 때
    NODE* nodes = heap->nodes;
    NODE* opposite_nodes = nodes[index1].opposite;

    //먼저 상대편 node 정보부터 수정
    opposite_nodes[nodes[index1].syncPos].syncPos = index2;
    opposite_nodes[nodes[index2].syncPos].syncPos = index1;

    //내부 node 정보 수정
    NODE temp = nodes[index1];//깊은 복사
    nodes[index1] = nodes[index2];
    nodes[index2] = temp;
}//핵심!: 상대편 노드들의 위치는 바꾸지 X

void print_HEAP(HEAP* heap) {
    int cnt = heap->number_of_nodes;
    for(int i = 1; i <= cnt; i++) printf("%d ", heap->nodes[i].key);
    printf("\n");
}//print all nodes' elements (for debugging)

//////////
int insertNode_DQ(DQ* dq, int item) {//item을 삽입하고 삽입한 인덱스를 반환
    HEAP* max_heap = dq->max_heap;
    HEAP* min_heap = dq->min_heap;

    if(max_heap->number_of_nodes == MAX_ELEMENTS - 1 || min_heap->number_of_nodes == MAX_ELEMENTS - 1) {//if heap is full,
        fprintf(stderr, "The heap is full.\n");
        exit(1);//프로그램 강제 종료
    }

    //삽입할 노드 초기화
    int insertPos = ++max_heap->number_of_nodes;

    max_heap->nodes[insertPos].key = item;
    max_heap->nodes[insertPos].opposite = min_heap->nodes;
    max_heap->nodes[insertPos].syncPos = insertPos;

    min_heap->nodes[insertPos].key = item;
    min_heap->nodes[insertPos].opposite = max_heap->nodes;
    min_heap->nodes[insertPos].syncPos = insertPos;

    //최대 힙부터 heapify
    while((insertPos != 1 )&&(item > max_heap->nodes[insertPos/2].key)) {
        switchNode(max_heap, insertPos, insertPos/2);
        insertPos /= 2;//insertPos가 자신의 부모 노드를 가리키도록 위치 변경
    }

    //최소 힙 heapify
    insertPos = ++min_heap->number_of_nodes;//위치 초기화
    while((insertPos != 1 )&&(item < min_heap->nodes[insertPos/2].key)) {
        switchNode(min_heap, insertPos, insertPos/2);
        insertPos /= 2;//insertPos가 자신의 부모 노드를 가리키도록 위치 변경
    }

    return insertPos;
}

int delNode_MAX_HEAP(DQ* dq, int index) {//index 위치의 노드를 del 및 heapify
    HEAP* max_heap = dq->max_heap;
    int pop_item = max_heap->nodes[index].key;
    
    switchNode(max_heap, index, max_heap->number_of_nodes);//index에 있는 노드를 맨 마지막 노드와 교체
    max_heap->number_of_nodes--;//맨 마지막 노드 무시

    while(2 * index < max_heap->number_of_nodes) {
        if(2 * index + 1 < max_heap->number_of_nodes && max_heap->nodes[2 * index].key < max_heap->nodes[2 * index + 1].key && max_heap->nodes[index].key < max_heap->nodes[2 * index + 1].key) {//오른쪽 자식이 올라가는 경우
            switchNode(max_heap, index, 2 * index + 1);
            index = 2 * index + 1;
        }
        else if(max_heap->nodes[index].key < max_heap->nodes[2 * index].key) {//왼쪽 자식이 올라가는 경우
            switchNode(max_heap, index, 2 * index);
            index = 2 * index;
        }
        else break;//종료 조건 (parent key > child(s) key)
    }

    return pop_item;
}
 
int delNode_MIN_HEAP(DQ* dq, int index) {//index 위치의 노드를 del 및 heapify
    HEAP* min_heap = dq->min_heap;
    int pop_item = min_heap->nodes[index].key;

    switchNode(min_heap, index, min_heap->number_of_nodes);//index에 있는 노드를 맨 마지막 노드와 교체
    min_heap->number_of_nodes--;//맨 마지막 노드 무시

    while(2 * index < min_heap->number_of_nodes) {
        if(2 * index + 1 < min_heap->number_of_nodes && min_heap->nodes[2 * index].key > min_heap->nodes[2 * index + 1].key && min_heap->nodes[index].key > min_heap->nodes[2 * index + 1].key) {//오른쪽 자식이 올라가는 경우
            switchNode(min_heap, index, 2 * index + 1);
            index = 2 * index + 1;
        }
        else if(min_heap->nodes[index].key > min_heap->nodes[2 * index].key) {//왼쪽 자식이 올라가는 경우
            switchNode(min_heap, index, 2 * index);
            index = 2 * index;
        }
        else break;//종료 조건 (parent key < child(s) key)
    }

    return pop_item;
}
//////////


int main() {
    DQ* dq = (DQ*)malloc(sizeof(DQ));
    initialize(dq);

    insertNode_DQ(dq, 10);
    insertNode_DQ(dq, 9);
    insertNode_DQ(dq, 11);
    insertNode_DQ(dq, 5);
    insertNode_DQ(dq, 8);
    insertNode_DQ(dq, 12);

    printf("%d\n", delNode_MAX_HEAP(dq, 1));
    printf("%d\n", delNode_MIN_HEAP(dq, 1));
    printf("%d\n", delNode_MIN_HEAP(dq, 1));
    printf("%d\n", delNode_MAX_HEAP(dq, 1));
    printf("%d\n", delNode_MAX_HEAP(dq, 1));

    return 0;
}