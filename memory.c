#include <stdio.h>
#include "oslabs.h"
#include <stdbool.h>

//This method allocates memory according to the Best Fit scheme.
struct MEMORY_BLOCK best_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id);
struct MEMORY_BLOCK best_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id){
    struct MEMORY_BLOCK allocated_memory, memory;
        allocated_memory.start_address = 0;
        allocated_memory.end_address = 0;
        allocated_memory.segment_size = 0;
        allocated_memory.process_id = 0;
        bool match_found = false;
        int memory_map_intial = 0;
        int best_fit_segment = 0;

    //Look through candidate memory_blocks and find size closest to request_size.
    for(int i = 0; i <= *map_cnt - 1; i++){
        //If block found size has same size as the request_size; return pointer to the memory block.
        if(request_size == memory_map[i].segment_size){
            match_found = true;
            memory_map_intial = i;
            best_fit_segment = request_size;
            break;
        }
        //If free block found is larger than the request_size, block splits into two pieces.
        else if((memory_map[i].segment_size > request_size) && (memory_map[i].process_id == 0)){
            if (match_found == false){
                match_found = true;
                memory_map_intial = i;
                best_fit_segment = memory_map[i].segment_size;
            }
        }
        else{
            if(best_fit_segment > memory_map[i].segment_size){
                memory_map_intial = i;
                best_fit_segment = memory_map[i].segment_size;
            }
        }
    }
    //If block finds proper allocated space in the memory.
    if(match_found == true){
        memory = memory_map[memory_map_intial];
        allocated_memory.start_address = memory.start_address;
        allocated_memory.end_address = allocated_memory.start_address + request_size - 1;
        allocated_memory.process_id = process_id;
        allocated_memory.segment_size = request_size;
        *map_cnt = *map_cnt + 1;

        for(int i = memory_map_intial; i <= *map_cnt; i++){
            memory_map[memory_map_intial+1] = memory_map[i];
        }

        memory_map[memory_map_intial+1].start_address = allocated_memory.end_address + 1;
        memory_map[memory_map_intial+1].end_address = memory_map[memory_map_intial].end_address;
        memory_map[memory_map_intial+1].process_id =memory_map[memory_map_intial].process_id;
        memory_map[memory_map_intial+1].segment_size =memory_map[memory_map_intial].segment_size - request_size;
        memory_map[memory_map_intial] = allocated_memory;
    }
    return allocated_memory;
};

//This method allocates memory according to the First Fit scheme
struct MEMORY_BLOCK first_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id);
struct MEMORY_BLOCK first_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id){
    struct MEMORY_BLOCK memory, allocated_memory;
    allocated_memory.end_address = 0;
    allocated_memory.start_address = 0;
    allocated_memory.process_id = 0;
    allocated_memory.segment_size = 0;
    bool match_found = false;
    int memory_map_initialize = 0;
    int best_fit_segment = 0;

    //look for first-fit version and test for multiple memories to see the best/first one.
    for(int i = 0; i <= *map_cnt - 1; i++){
        if (request_size <= memory_map[i].segment_size){
            match_found = true;
            memory_map_initialize = i;
            best_fit_segment = request_size;
            break;
        }
    }
        // If match is found find the new space and switch with that space instead.
        if (match_found == true){
            memory = memory_map[memory_map_initialize];
            allocated_memory.start_address = memory.start_address;
            allocated_memory.end_address = allocated_memory.start_address + request_size - 1;
            allocated_memory.process_id = process_id;
            allocated_memory.segment_size = request_size;
            *map_cnt = *map_cnt + 1;

            for(int i = memory_map_initialize; i <= *map_cnt; i++){
                memory_map[memory_map_initialize + 1] = memory_map[i];
            }
            memory_map[memory_map_initialize+1].start_address = allocated_memory.end_address + 1;
            memory_map[memory_map_initialize+1].end_address = memory_map[memory_map_initialize].end_address;
            memory_map[memory_map_initialize+1].segment_size = memory_map[memory_map_initialize].segment_size - request_size;
            memory_map[memory_map_initialize+1].process_id = memory_map[memory_map_initialize].process_id;
            memory_map[memory_map_initialize] = allocated_memory;
        }
    return allocated_memory;
};

//This method allocates memory according to the Worst Fit scheme
struct MEMORY_BLOCK worst_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id);
struct MEMORY_BLOCK worst_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id) {
    struct MEMORY_BLOCK memory, allocated_memory;
    allocated_memory.end_address = 0;
    allocated_memory.start_address = 0;
    allocated_memory.process_id = 0;
    allocated_memory.segment_size = 0;
    bool match_found = false;
    int memory_map_initial = 0 , worst_fit_segmant = 0;
    //Finding worst match/version and replacing it.
    for (int i = 0; i <= *map_cnt - 1; i++) {
        if ((memory_map[i].segment_size >= request_size) && (memory_map[i].process_id == 0)) {
            if (match_found == false){
                match_found = true;
                memory_map_initial = i;
                worst_fit_segmant = memory_map[i].segment_size;
            }
            else{
                if (worst_fit_segmant < memory_map[i].segment_size){
                    memory_map_initial = i;
                    worst_fit_segmant = memory_map[i].segment_size;
                }
            }
        }
    }
    //Switching the load after identifying the worst match.
    if (match_found == true){
        memory = memory_map[memory_map_initial];
        allocated_memory.start_address = memory.start_address;
        allocated_memory.end_address = allocated_memory.start_address + request_size - 1;
        allocated_memory.process_id = process_id;
        allocated_memory.segment_size = request_size;
        *map_cnt = *map_cnt + 1;
            for (int i = memory_map_initial; i <= *map_cnt; i++){
            memory_map[memory_map_initial+1] = memory_map[i];
            }
        memory_map[memory_map_initial+1].start_address = allocated_memory.end_address + 1;
        memory_map[memory_map_initial+1].end_address = memory_map[memory_map_initial].end_address;
        memory_map[memory_map_initial+1].process_id = memory_map[memory_map_initial].process_id;
        memory_map[memory_map_initial+1].segment_size = memory_map[memory_map_initial].segment_size - request_size;
        memory_map[memory_map_initial] = allocated_memory;
    }
return allocated_memory;
};
//This method allocates memory according to the Next Fit scheme
struct MEMORY_BLOCK next_fit_allocate(int request_size, struct MEMORY_BLOCK   memory_map[MAPMAX],int *map_cnt, int process_id, int last_address);
struct MEMORY_BLOCK next_fit_allocate(int request_size, struct MEMORY_BLOCK   memory_map[MAPMAX],int *map_cnt, int process_id, int last_address){
    struct MEMORY_BLOCK memory, allocated_memory;
    allocated_memory.end_address = 0;
    allocated_memory.start_address = 0;
    allocated_memory.process_id = 0;
    allocated_memory.segment_size = 0;
    bool match_found = false;
    int memory_map_initial = 0 , next_fit_segmant = 0;
    int initial = 0;

    //Looping and finding the next best fit and seeing what is next.
    for(int i = last_address; i <= *map_cnt + last_address - 1; i++){
        if(i >= *map_cnt){
            initial = i - *map_cnt;
        }
        else{
            initial = i;
            if((request_size <= memory_map[initial].segment_size) && (memory_map[i].process_id == 0)){
                match_found = true;
                memory_map_initial = initial;
                next_fit_segmant = request_size;
            }
        }
    }
    if(match_found == true){
        memory = memory_map[memory_map_initial];
        allocated_memory.start_address = memory.start_address;
        allocated_memory.end_address = allocated_memory.start_address + request_size - 1;
        allocated_memory.process_id = process_id;
        allocated_memory.segment_size = request_size;
        *map_cnt = *map_cnt + 1;
    for(int i = memory_map_initial; i <= *map_cnt; i++){
        memory_map[memory_map_initial + 1] = memory_map[i];
    }
    memory_map[memory_map_initial + 1].start_address = allocated_memory.end_address + 1;
    memory_map[memory_map_initial + 1].end_address = memory_map[memory_map_initial].end_address;
    memory_map[memory_map_initial + 1].process_id = memory_map[memory_map_initial].process_id;
    memory_map[memory_map_initial + 1].segment_size = memory_map[memory_map_initial].segment_size - request_size;
    memory_map[memory_map_initial] = allocated_memory;
    }
    return allocated_memory;
};
//This method releases a memory block. Accordingly, it modifies the memory map passed in.
void release_memory(struct MEMORY_BLOCK freed_block, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt);
void release_memory(struct MEMORY_BLOCK freed_block, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt) {
    bool flag = false;
    //If the memory block is adjacent to the release block then its free...
    if ((*map_cnt == 1) && (memory_map[0].end_address == 0) && (memory_map[0].start_address == 0) && (memory_map[0].process_id == 0) && (memory_map[0].segment_size == 0)){
        return;
    }
    else{
        for (int i = 0; i < *map_cnt; i++){
            if((freed_block.start_address == memory_map[i].start_address) && (freed_block.end_address == memory_map[i].end_address) && (freed_block.process_id == memory_map[i].process_id)) {
                memory_map[i].process_id = 0;
                if (i > 0){
                    if (memory_map[i-1].process_id == 0){
                        memory_map[i-1].end_address = freed_block.end_address;
                        memory_map[i-1].segment_size = memory_map[i-1].segment_size + freed_block.segment_size;
                        for (int index = i; index <= *map_cnt; index++){
                            memory_map[index] = memory_map[index + 1];
                        }
                        *map_cnt -= 1;
                        flag = true;
                    }
                }
                if (i < *map_cnt-1){
                    if (flag == false){
                        i = i+1;
                    }
                    if (memory_map[i].process_id == 0){
                        memory_map[i].start_address = memory_map[i-1].start_address;
                        memory_map[i].segment_size = memory_map[i].end_address -memory_map[i].start_address+1;
                        for (int index = i; index <= *map_cnt; index++){
                            memory_map[index-1] = memory_map[index];
                        }
                    *map_cnt -= 1;
                    }
                }
            }
        }
    }
};
