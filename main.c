#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#define HEAP_CAPACITY 640000
#define CHUNK_LIST_CAP 1024

#define UNIMPLEMEMTED \
    do{ \
        fprintf(stderr, "%s:%d TODO:%s is not implemented\n",\
        __FILE__,__LINE__,__func__); \
        abort(); \
    }while(0) \

typedef struct{
    char *start;
    size_t size;
}Chunk;  

typedef struct{
    Chunk chunks[CHUNK_LIST_CAP];
    size_t count;
}chunk_list;

char heap[HEAP_CAPACITY] = {0};
size_t heap_size = 0;

chunk_list alcd_chunks = {0};
chunk_list freed_chunks = {
    .count =1,
    .chunks = {
        [0] = {.start = heap, .size = CHUNK_LIST_CAP}  //initially, it's one big empty chunk, one single chunk
    }

};

// we made separate data structureds, because we wanted to define functions and operations on these

//comparison function
int chunk_start_compar(const void *a,const void *b){

    const Chunk *ac = a;
    const Chunk *bc = b;

    return ac->start - bc->start;

}

int chunk_list_find(const chunk_list *list, void *ptr){

    Chunk key ={
        .start = ptr
    };
    Chunk *result = bsearch(&key, list->chunks,
              list->count, sizeof(Chunk),
              chunk_start_compar);

    if(result != NULL){
        assert(list->chunks <= result);
        return (int)(result - list->chunks);

    }
    else{
        return -1;
    }
}

void chunk_list_remove(chunk_list *list, size_t index){
    
    assert(index < list->count);

    for(size_t i = index; i < list->count -1; i++){

        list->chunks[i] = list->chunks[i+1];

    }

    list->count--;

}

void chunk_list_insert(chunk_list *list, void *ptr, size_t size){
    
    assert(list->count <= CHUNK_LIST_CAP);
    
    list->chunks[list->count].start=ptr;
    list->chunks[list->count].size=size;

    for(size_t i=list->count; i>0 && list->chunks[i].start<list->chunks[i-1].start; --i){  //sorting by pointers
        const Chunk t= list->chunks[i-1];
        list->chunks[i-1] = list->chunks[i];
        list->chunks[i] = t;

    }

    list->count++;

}

void chunk_list_merge(chunk_list *list){

    size_t i=0;

    while(i+1 < list->count){

        Chunk *a = &list->chunks[i];

        const Chunk *b = &list->chunks[i+1];

        if(a->start + a->size == b->start){
            a->size += b->size;
            chunk_list_remove(list, i+1);
        }
        else{
            i++;
        }


    }
}


void *heap_alloc(size_t size){  // void * is a generic pointer type that can point to any object type
    

    chunk_list_merge(&freed_chunks);
    
    if(size > 0){
        for(size_t i=0; i<freed_chunks.count; i++){
        if(freed_chunks.chunks[i].size >= size){
            Chunk chunk = freed_chunks.chunks[i];
            chunk_list_remove(&freed_chunks, i);  // removing that "free" chunk which can accomodate. After removal, we need to "insert" our rquired chunk, plus, the empty "tail" chunk, if any available.

            size_t tailSize = chunk.size - size;

            chunk_list_insert(&alcd_chunks, chunk.start, size);

            if(tailSize > 0){
                chunk_list_insert(&freed_chunks, chunk.start + size, tailSize);

            }

            return chunk.start;
        }
    }

    }
    

    return NULL;



    // if(size >0){
    //     assert(heap_size + size <= HEAP_CAPACITY);
    // void *ptr = heap + heap_size;

    // chunk_list_insert(&alcd_chunks, ptr, size);

    // const Chunk chunk = {
    //     .start = ptr,
    //     .size = size
    // };

    // assert(hp_alcd_size <= hp_alcd_cap);   // code which was improved upon
    // hp_alcd[hp_alcd_size++] = chunk;

    // heap_size += size;
    // return ptr;

    // }
    // else{
    //     return NULL;  
    // }
    

}

void heap_free(void *ptr){

    if(ptr != NULL){

        int index = chunk_list_find(&alcd_chunks, ptr);
    assert(index >= 0);

    chunk_list_insert(&freed_chunks, alcd_chunks.chunks[index].start, alcd_chunks.chunks[index].size);
    chunk_list_remove(&alcd_chunks, index);

    }


}
 
void heap_collect(){
    UNIMPLEMEMTED;
}

void chunk_list_dump(const chunk_list *list){

    printf("Allocated chunks (%zu):\n", list->count);  // list -> count, is just shorthand for (*list).count.
    for(size_t i=0; i< list->count; ++i){
        printf("start: %p, size: %zu\n",
        list->chunks[i].start,
        list->chunks[i].size);
    }
}

int main(){
                                //root[i] = i + 'A'; // here, root[i] is same as *(root + i). 
    for(int i = 0; i<10; ++i){
        void *p = heap_alloc(i);
        if(i%2 == 0){
            heap_free(p);
        }
    }

    // heap_alloc(800);

    chunk_list_dump(&alcd_chunks);
    chunk_list_dump(&freed_chunks);

    return 0;
}



