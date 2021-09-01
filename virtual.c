#include <stdio.h>
#include "oslabs.h"
#include <stdbool.h>

//Process Page - First-In-First-Out - FIFO.
//Check if page being transferred is already in memory.
int process_page_access_fifo(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp);
int process_page_access_fifo(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp){
    //If page in memory then modify last_access_timestamp of the page-table entry and retun frame number.
    if(page_table[page_number].is_valid == true){
        page_table[page_number].reference_count += 1;
        page_table[page_number].last_access_timestamp = current_timestamp;
        return page_table[page_number].frame_number;
    }
    //If page being referenced is not in memory then check if there are any free frames.
    else if((page_table[page_number].is_valid == false) && (*frame_cnt > 0)){
        //If yes, modify last_access_timestamp and return.
        page_table[page_number].frame_number = frame_pool[*frame_cnt-1];
        *frame_cnt -= 1;
        //Page-Table entry are set appropriately.
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count = 1;
        page_table[page_number].is_valid = true;
        //Return frame number associated with page-table entry.
        return page_table[page_number].frame_number;
    }
    //Page must be replaced.
    else{
        bool first_flag = false;
        int intial_frame = 0;
        int intial_index = 0;
        int arrival_stamp = 0;
        for(int p = 0; p < *table_cnt ; p++){
            if((page_table[p].is_valid == true) && (first_flag== false)){
                intial_index = p;
                intial_frame = page_table[p].frame_number;
                arrival_stamp = page_table[p].arrival_timestamp;
                first_flag = true;
            }
            else if((page_table[p].is_valid == true) && (arrival_stamp > page_table[p].arrival_timestamp) && (first_flag== true)){
                arrival_stamp = page_table[p].arrival_timestamp;
                intial_index = p;
                intial_frame = page_table[p].frame_number;
            }
        }
        if(first_flag == true){
            page_table[intial_index].arrival_timestamp = -1;
            page_table[intial_index].reference_count = -1;
            page_table[intial_index].last_access_timestamp = -1;
            page_table[intial_index].frame_number = -1;
            page_table[intial_index].is_valid = false;
            page_table[page_number].frame_number = intial_frame;
            page_table[page_number].arrival_timestamp = current_timestamp;
            page_table[page_number].is_valid = true;
            page_table[page_number].last_access_timestamp = current_timestamp;
            page_table[page_number].reference_count = 1;
        }
        return intial_frame;
    }
};

//Count page faults - First-In-First-Out - FIFO.
int count_page_faults_fifo(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt);
//The function returns the estimated number of page faults for the reference string, with respect to the pool of frames allocated to the process.
int count_page_faults_fifo(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt){
    //The function first checks if the page being referenced is already in memory.
    int timestamp = 1;
    int page_fault = 0;
    for(int i = 0; i < reference_cnt; i++){
        //If so, it modifies the last_access_timestamp and the reference_count fields of the page-table entry.
        if(page_table[refrence_string[i]].is_valid == true){
            page_table[refrence_string[i]].last_access_timestamp = timestamp;
            page_table[refrence_string[i]].reference_count += 1;
        }
        //f the page being referenced is not in memory, the function checks if there are any free frames.
        else if((page_table[refrence_string[i]].is_valid == false) && (frame_cnt > 0)){
            page_table[refrence_string[i]].frame_number = frame_pool[frame_cnt-1];
            page_table[refrence_string[i]].last_access_timestamp = timestamp;
            page_table[refrence_string[i]].reference_count += 1;
            page_table[refrence_string[i]].is_valid = true;
            frame_cnt -= 1;
            page_fault += 1;
        }
        //If the page being referenced is not in memory and there are no free frames for the process, a page needs to be replaced.
        else if((page_table[refrence_string[i]].is_valid == false) && (frame_cnt == 0)){
            page_fault += 1;
            bool first_flag = false;
            int intial_index = 0;
            int intial_frame = 0;
            int arrival_stamp = 0;
            for(int x = 0; x < table_cnt ; x++){
                if((page_table[x].is_valid == true) && (first_flag==false)){
                    intial_index = x;
                    intial_frame = page_table[x].frame_number;
                    arrival_stamp = page_table[x].arrival_timestamp;
                    first_flag = true;
                }
                else if((page_table[x].is_valid == true) && (arrival_stamp > page_table[i].arrival_timestamp) && (first_flag == true)){
                    arrival_stamp = page_table[x].arrival_timestamp;
                    intial_index = x;
                    intial_frame = page_table[x].frame_number;
                }
            }
            if(first_flag == true){
                page_table[intial_index].arrival_timestamp = 0;
                page_table[intial_index].reference_count = 0;
                page_table[intial_index].last_access_timestamp = 0;
                page_table[intial_index].frame_number = 0;
                page_table[intial_index].is_valid = false;
                page_table[refrence_string[i]].frame_number = intial_frame;
                page_table[refrence_string[i]].arrival_timestamp = arrival_stamp;
                page_table[refrence_string[i]].is_valid = true;
                page_table[refrence_string[i]].last_access_timestamp = timestamp;
                page_table[refrence_string[i]].reference_count = 0;
            }
        }
        timestamp += 1;
    }
    //The function returns the total number of page faults encountered in the simulation.
    return page_fault;
};

//Process a page access in a system - Least-Recently-Used - LRU.
int process_page_access_lru(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp);
//This function implements the logic to process a page access in a system that uses the Least-Recently-Used (LRU) policy for page replacement.
//The function determines the memory frame number for the logical page and returns this number.
int process_page_access_lru(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp){
    //Checking if page being referenced is in memory.
    if(page_table[page_number].is_valid == true){
        page_table[page_number].reference_count += 1;
        page_table[page_number].last_access_timestamp = current_timestamp;
        return page_table[page_number].frame_number;
    }
    //Checking if page being referenced is NOT in memory.
    else if((page_table[page_number].is_valid == false) && (*frame_cnt > 0)){
        page_table[page_number].frame_number = frame_pool[*frame_cnt-1];
        *frame_cnt -= 1;
        //Fields of page-table are set.
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count = 1;
        page_table[page_number].is_valid = true;
        return page_table[page_number].frame_number;
    }
    //If page being referenced is not in meomry and there are no free frames within the process...
    else{
        bool first_flag = false;
        int intial_index = 0;
        int intial_frame = 0;
        int last_timestamp = 0;
        for(int i = 0; i < *table_cnt ; i++){
            if((page_table[i].is_valid == true) && (first_flag== false)){
                intial_index = i;
                intial_frame = page_table[i].frame_number;
                last_timestamp = page_table[i].last_access_timestamp;
                first_flag = true;
            }
            else if((page_table[i].is_valid == true) && (last_timestamp > page_table[i].last_access_timestamp) && (first_flag== true)){
                last_timestamp = page_table[i].last_access_timestamp;
                intial_index = i;
                intial_frame = page_table[i].frame_number;
            }
        }
        //Mark table entry as invalid
        if(first_flag == true){
            page_table[intial_index].arrival_timestamp = -1;
            page_table[intial_index].reference_count = -1;
            page_table[intial_index].last_access_timestamp = -1;
            page_table[intial_index].frame_number = -1;
            page_table[intial_index].is_valid = false;
            page_table[page_number].frame_number = intial_frame;
            page_table[page_number].arrival_timestamp = current_timestamp;
            page_table[page_number].is_valid = true;
            page_table[page_number].last_access_timestamp = current_timestamp;
            page_table[page_number].reference_count = 1;
        }
    return intial_frame;
    }
};

//This function simulates the processing of a sequence of page references in a system that uses the Least-Recently-Used (LRU).
int count_page_faults_lru(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt);
int count_page_faults_lru(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt){
    //Timestamp starts with 1 in order to simulate timestamps.
    int timestamp = 1;
    int estimated_page_fault = 0;
    //Increment whenever the process of a new page access has begun.
    for(int i = 0; i < reference_cnt; i++){
        //If the page being referenced is not in the memory, then check if there are any free frames.
        if(page_table[refrence_string[i]].is_valid == true){
            page_table[refrence_string[i]].last_access_timestamp = timestamp;
            //Remove a frame from the process pool and inserted into the page table entry.
            page_table[refrence_string[i]].reference_count += 1;
        }
        else if((page_table[refrence_string[i]].is_valid == false) && (frame_cnt > 0)){
            page_table[refrence_string[i]].frame_number = frame_pool[frame_cnt-1];
            page_table[refrence_string[i]].last_access_timestamp = timestamp;
            page_table[refrence_string[i]].reference_count += 1;
            page_table[refrence_string[i]].is_valid = true;
            frame_cnt -= 1;
            estimated_page_fault += 1;
        }
        //Fields of page-table entry are set appropriately.
        //If the page being referenced is not in memory and there are no free frames for the process, a page needs replaced.
        //Find pages that are still in memory and set values.
        else if((page_table[refrence_string[i]].is_valid == false) && (frame_cnt == 0)){
            bool first_flag = false;
            int intial_index = 0;
            int initial_frame = 0;
            int last_arrival_timestamp = 0;
            estimated_page_fault += 1;
            for(int p = 0; p < table_cnt ; p++){
                if((page_table[p].is_valid == true) && (first_flag == false)){
                    intial_index = p;
                    initial_frame = page_table[p].frame_number;
                    last_arrival_timestamp = page_table[p].arrival_timestamp;
                    first_flag = true;
                }
                else if((page_table[p].is_valid == true) && (last_arrival_timestamp > page_table[p].last_access_timestamp) && (first_flag== true)){
                    last_arrival_timestamp = page_table[p].last_access_timestamp;
                    intial_index = p;
                    initial_frame = page_table[p].frame_number;
                }
            }
            if(first_flag == true){
                page_table[intial_index].arrival_timestamp = 0;
                page_table[intial_index].reference_count = 0;
                page_table[intial_index].last_access_timestamp = 0;
                page_table[intial_index].frame_number = 0;
                page_table[intial_index].is_valid = false;
                page_table[refrence_string[i]].frame_number = initial_frame;
                page_table[refrence_string[i]].arrival_timestamp = timestamp;
                page_table[refrence_string[i]].is_valid = true;
                page_table[refrence_string[i]].last_access_timestamp = timestamp;
                page_table[refrence_string[i]].reference_count = 0;
            }
        }
        timestamp += 1;
    }
    return estimated_page_fault;
};

//This function implements the logic to process a page access in a system that uses the Least-Frequently-Used.
//The function determines the memory frame number for the logical page and returns this number.
int process_page_access_lfu(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp);
int process_page_access_lfu(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp){
    //If the page bring referenced is not in memory, the function checks if there are any free frames.
    if((page_table[page_number].is_valid == false) && (*frame_cnt > 0)){
        //Remove frame from the process frame pool.
        page_table[page_number].frame_number = frame_pool[*frame_cnt-1];
        *frame_cnt -= 1;
        //Page-Table Entry are set appropriately.
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].arrival_timestamp = current_timestamp;
        page_table[page_number].reference_count = 1;
        page_table[page_number].is_valid = true;
        //Return the frame number associated with page-table entry.
        return page_table[page_number].frame_number;
    }
    //Check if the page being referenced is already in the memory.
    else if(page_table[page_number].is_valid == true){
        page_table[page_number].reference_count += 1;
        page_table[page_number].last_access_timestamp = current_timestamp;
        //Return the frame number.
        return page_table[page_number].frame_number;
    }
    else{
        int intial_index = 0;
        int intial_frame = 0;
        int arrival_time = 0;
        int frames_used = 0;
        bool first_flag = false;
        for(int i = 0; i < *table_cnt ; i++){
            //Selects all pages of the processes that are currently in memory and picks the page that has the smallest reference_cnt.
            if((page_table[i].is_valid == true) && (frames_used >= page_table[i].reference_count) && (first_flag == true)){
                if(((frames_used == page_table[i].reference_count) && (arrival_time > page_table[i].arrival_timestamp)) || (frames_used > page_table[i].reference_count)){
                    frames_used = page_table[i].reference_count;
                    intial_index = i;
                    intial_frame = page_table[i].frame_number;
                    arrival_time = page_table[i].arrival_timestamp;
                }
            }
            else if((page_table[i].is_valid == true) && (first_flag == false)){
                intial_index = i;
                intial_frame = page_table[i].frame_number;
                frames_used = page_table[i].reference_count;
                arrival_time = page_table[i].arrival_timestamp;
                first_flag = true;
            }
        }
        if(first_flag == true){
            page_table[intial_index].arrival_timestamp = -1;
            page_table[intial_index].reference_count = -1;
            page_table[intial_index].last_access_timestamp = -1;
            page_table[intial_index].frame_number = -1;
            page_table[intial_index].is_valid = false;
            page_table[page_number].frame_number = intial_frame;
            page_table[page_number].arrival_timestamp = current_timestamp;
            page_table[page_number].is_valid = true;
            page_table[page_number].last_access_timestamp = current_timestamp;
            page_table[page_number].reference_count = 1;
        }
        return intial_frame;
    }
};
//This function simulates the processing of a sequence of page references in a system that uses the Least-Frequently-Used.
//The function returns the estimated number of page faults for the reference string, with respect to the pool of frames allocated to the process.
int count_page_faults_lfu(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt);
int count_page_faults_lfu(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt){
    //Function starts with a timestamp of 1 and increments it whenever a new page has begun.
    int timestamp = 1;
    int page_fault = 0;
    for(int i = 0; i < reference_cnt; i++){
        //If page being referenced is not in memory, check if there are any free frames.
        if ((page_table[refrence_string[i]].is_valid == false) && (frame_cnt > 0)){
            page_table[refrence_string[i]].frame_number = frame_pool[frame_cnt-1];
            page_table[refrence_string[i]].last_access_timestamp = timestamp;
            page_table[refrence_string[i]].arrival_timestamp = timestamp;
            page_table[refrence_string[i]].reference_count += 1;
            page_table[refrence_string[i]].is_valid = true;
            //Remove frame.
            frame_cnt -= 1;
            page_fault += 1;
        }
        else if (page_table[refrence_string[i]].is_valid == true){
            page_table[refrence_string[i]].last_access_timestamp = timestamp;
            page_table[refrence_string[i]].reference_count += 1;
        }
        //If page being referenced is not in memory and there are no free frames for the process, a page needs to be replaced.
        else if((page_table[refrence_string[i]].is_valid == false) && (frame_cnt == 0)){
            page_fault += 1;
            bool first_flag = false;
            bool flag = false;
            int intial_index = 0;
            int frame = 0;
            int used_frame = 0;
            int arrival_time = 0;
            for (int p = 0; p < table_cnt ; p++){
                if((page_table[p].is_valid == true) && (first_flag == false)){
                    intial_index = p;
                    frame = page_table[p].frame_number;
                    used_frame = page_table[p].reference_count;
                    arrival_time = page_table[p].arrival_timestamp;
                    first_flag = true;
                }
                else if ((page_table[p].is_valid == true) && (used_frame >= page_table[p].reference_count) && (first_flag== true)){
                    if(((used_frame == page_table[p].reference_count) && (arrival_time > page_table[p].arrival_timestamp))){
                        used_frame = page_table[p].reference_count;
                        intial_index = p;
                        frame = page_table[p].frame_number;
                        arrival_time = page_table[p].arrival_timestamp;
                        flag = true;
                    }
                    else if((used_frame > page_table[p].reference_count) && (flag == false)){
                        used_frame = page_table[p].reference_count;
                        intial_index = p;
                        frame = page_table[p].frame_number;
                        arrival_time =page_table[p].arrival_timestamp;
                    }
                }
            }
            if (first_flag == true){
                //Smallest reference_count along with setting values to 0.
                page_table[intial_index].arrival_timestamp = 0;
                page_table[intial_index].reference_count = 0;
                page_table[intial_index].last_access_timestamp = 0;
                page_table[intial_index].frame_number = 0;
                page_table[intial_index].is_valid = false;
                page_table[refrence_string[i]].frame_number = frame;
                page_table[refrence_string[i]].arrival_timestamp = timestamp;
                page_table[refrence_string[i]].is_valid = true;
                page_table[refrence_string[i]].last_access_timestamp = timestamp;
                page_table[refrence_string[i]].reference_count = 0;
            }
        }
        timestamp += 1;
    }
    return page_fault;
};
