#include <stdio.h>
#include <stdlib.h>

#define MAX_ELEMENTS 100002
#define HEAP_FULL(n) (n==MAX_ELEMENTS-1)
#define HEAP_EMPTY(n) (!n)

typedef struct {
    int key;
} node;

node heap[MAX_ELEMENTS];//인덱스와 노드 번호를 일치하도록 '0' 인덱스를 비워놓음. 노드의 개수와도 일치되는 효과가 있음
int number_of_nodes = 0;//초기 노드의 개수 0

void push(int item) {
    if(HEAP_FULL(number_of_nodes)) {
        fprintf(stderr, "The heap is full.\n");
        exit(1);//프로그램 강제 종료
    }

    int insertPos = ++number_of_nodes;//노드 개수를 미리 한 개 늘리고 그 노드의 위치를 insertPos에 저장

    while((insertPos != 1 )&&(item < heap[insertPos/2].key)) {//insertPos가 root 노드가 아니고 삽입할 아이템의 값이 insertPos의 부모 노드의 값보다 클 때
        heap[insertPos] = heap[insertPos/2];//child 노드에 parent 노드 값을 대입 (for switching. ex. '10 5' -> '10 10')
        insertPos /= 2;//insertPos가 자신의 부모 노드를 가리키도록 위치 변경
    }

    heap[insertPos].key = item;//최종적으로 연산이 끝나면 마무리로 최종 삽입 위치에 아이템 값을 삽입. (ex. '10 9 8 7 6 5' -> '10 10 9 8 7 6' -> '11 10 9 8 7 6')
}

int delete() {
    if(HEAP_EMPTY(number_of_nodes)) {
        fprintf(stderr, "The heap is empty.\n");
        exit(1);//프로그램 강제 종료
    }

    node pop_item = heap[1];//pop할 root 노드 값 저장
    node temp_item = heap[number_of_nodes--];//heap의 마지막 노드의 값을 저장하고 노드의 개수를 한 개 미리 줄인다.

    //여기서는 'parent', 'child' 변수가 pop의 insertPos 역할을 수행 (즉, 위치추적)
    int parentPos = 1;//root 노드부터 시작
    int childPos = 2;//root 노드의 child 노드부터 시작

    while(childPos <= number_of_nodes) {//child 노드가 heap 범위 내에 있을 때
        if((childPos < number_of_nodes) && (heap[childPos].key > heap[childPos + 1].key)) childPos++;//left child와 right child의 값들 중 큰 값을 택
        if((temp_item.key <= heap[childPos].key)) break;//root 노드부터 차례로 내려오다가 child 노드의 값보다 큰 순간이 오다면 parent 노드에 노드 값을 삽입하기 위해 break

        heap[parentPos] = heap[childPos];//parent 노드 값에 child 노드 값을 대입 (for switching. ex. '10 9' -> '9 9')
        parentPos = childPos;
        childPos *= 2;
    }

    heap[parentPos] = temp_item;//switching 연산이 종료되면 최종적으로 삽입할 위치에 아이템 값 대입 (ex. '10 9 8 7 6' -> '9 8 7 6 6' -> '9 8 7 6 5')

    return pop_item.key;
}

int main() {
    int N;
    scanf("%d", &N);

    int temp;
    for(int i = 0; i < N; i++) {
        scanf("%d", &temp);
        if(!temp) {
            if(!number_of_nodes) printf("0\n");
            else printf("%d\n", delete());
        }
        else push(temp);
    }

    return 0;
}
