#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

//--------------------------------------------------------------------------------
std::vector<int> parseLineOfNumbers(const char* buf){
    std::vector<int> vec;
    int value = 0;
    std::istringstream iss(buf);
    while(iss >> value){
        vec.push_back(value);
    }
    return vec;
}

//--------------------------------------------------------------------------------
void print(const char* txt, std::vector<int> vec){
    printf("%s", txt);
    for(int i: vec){
        printf(" %4d", i);
    }
    printf("\n");
}

//--------------------------------------------------------------------------------
void heapifyRecursive(int arr[], int n, int index){
    int left  = 2*index+1;
    int right = 2*index+2;
    int indexOfMax = index;

    if(left<n && arr[left]>arr[indexOfMax])
        indexOfMax = left;
    if(right<n && arr[right]>arr[indexOfMax])
        indexOfMax = right;

    if(indexOfMax != index){
        int aux = arr[index];
        arr[index] = arr[indexOfMax];
        arr[indexOfMax] = aux;
        heapifyRecursive(arr, n, indexOfMax);
    }
}

//--------------------------------------------------------------------------------
void heapSortUsingHeapifyRecursive(int arr[], int n){
    int parentOfLastItem = n/2-1;
    for(int i=parentOfLastItem; i>=0; --i){
        heapifyRecursive(arr, n, i);
    }
    for(int i=n-1; i>0; --i){
        //swap 1st (which is max) with last
        int aux = arr[i];
        arr[i] = arr[0];
        arr[0] = aux;

        // call max heapify on the reduced heap
        heapifyRecursive(arr, i, 0);
    }
}

//--------------------------------------------------------------------------------
#if 0
void siftDown(int arr[], int root, int end){
    while(2*root+1 <= end){
        int leftChild = 2*root+1;
        int indexOfMax = root;
        if(arr[indexOfMax] < arr[leftChild]){//left child
            indexOfMax = leftChild;
        }
        if(leftChild+1<=end && arr[indexOfMax]<arr[leftChild+1]){//right child
            indexOfMax = leftChild+1;
        }
        if(indexOfMax == root){
            break;
        }
        int aux = arr[root];
        arr[root] = arr[indexOfMax];
        arr[indexOfMax] = aux;
        root = indexOfMax;
    }
#else
void siftDown(int arr[], int len, int index){
        int val = arr[index];
        for(int child=2*index+1; child<len; child=2*index+1){
            if(child+1<len && arr[child]<arr[child+1]){//right child exists and is bigger
                ++child;
            }
            if(val >= arr[child]){//in order
                break;
            }
            arr[index] = arr[child];
            index = child;
        }
        arr[index] = val;
#endif
}

//--------------------------------------------------------------------------------
void heapSort(int arr[], int n){
    //heapify
    int parentOfLastItem = n/2-1;
    for(int i=parentOfLastItem; i>=0; --i){
        siftDown(arr, n, i);
    }
    for(int end=n-1; end>0; --end){
        //swap 1st (which is max) with last
        int aux = arr[end];
        arr[end] = arr[0];
        arr[0] = aux;

        siftDown(arr, end, 0);
    }
}

//================================================================================
int main(){
    char buf[1024] = {};
    FILE* file = fopen("1_HeapSort.test", "rt");
    while(fgets(buf, sizeof(buf), file)){
        std::vector<int> vec = parseLineOfNumbers(buf);
        print("original: ", vec);
        heapSort(vec.data(), vec.size());
        print("sorted  : ", vec);
        printf("\n");
    }
    return 0;
}