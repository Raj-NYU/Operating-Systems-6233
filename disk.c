#include "oslabs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//Set all nullRCB to 0.
bool nullRCB(struct RCB requests){
    if((requests.request_id == 0) && (requests.arrival_timestamp == 0) && (requests.cylinder == 0) && (requests.address == 0) && (requests.process_id == 0)){
        return true;
    }
    else{
        return false;
    }
}

//Algorithm for First-Come-First-Served Scheduling Arrival (FCFS).
struct RCB handle_request_arrival_fcfs(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp);
struct RCB handle_request_arrival_fcfs(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp){
    //If disk is free return new arriving requests
    //Else add new request to queue_cnt and return current_request
    if(nullRCB(current_request)){
        return new_request;
    }
    else{
        if(*queue_cnt <= QUEUEMAX){
            request_queue[*queue_cnt] = new_request;
            *queue_cnt += 1;
            return current_request;
        }
        else{
            return current_request;
        }
    }
};

//Algorithm for First-Come-First-Served Scheduling Completion (FCFS).
struct RCB handle_request_completion_fcfs(struct RCB request_queue[QUEUEMAX],int *queue_cnt);
struct RCB handle_request_completion_fcfs(struct RCB request_queue[QUEUEMAX],int *queue_cnt){
    //If request_queue is empty indicating there is no request next.
    struct RCB nullRCB = {0, 0, 0, 0, 0};
    if(*queue_cnt == 0){
        return nullRCB;
    }
    //Otherwise find the earliest arrival time RCB in the request queue.
    else{
        int firstInitial = 0;
        struct RCB nextRCB;
        int arrival = request_queue[0].arrival_timestamp;
        for(int i = 1; i < *queue_cnt; i++){
            if(arrival > request_queue[i].arrival_timestamp){
                arrival = request_queue[i].arrival_timestamp;
                firstInitial= i;
            }
        }
        nextRCB = request_queue[firstInitial];
        for(int i = firstInitial; i < *queue_cnt; i++){
            request_queue[i] = request_queue[i+1];
        }
        //Remove RCB from request queue and return next RCB.
        *queue_cnt -= 1;
        return nextRCB;
    }
};

//Algorithm for Shortest-Seek-Time-First Scheduling Arrival (SSTF).
struct RCB handle_request_arrival_sstf(struct RCB request_queue[QUEUEMAX],int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp);
struct RCB handle_request_arrival_sstf(struct RCB request_queue[QUEUEMAX],int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp){
    //If the disk is free, return RCB.
    if(nullRCB(current_request)){
        return new_request;
    }
    else{
        //Otherwise return the RCB of the currently serviced request.
        if(*queue_cnt <= QUEUEMAX){
            //Add newly request to request queue.
            request_queue[*queue_cnt] = new_request;
            *queue_cnt += 1;
            return current_request;
        }
        else{
            return current_request;
        }
    }
};

//Algorithm for Shortest-Seek-Time-First Scheduling Completion (SSTF).
struct RCB handle_request_completion_sstf(struct RCB request_queue[QUEUEMAX],int *queue_cnt,int current_cylinder);
struct RCB handle_request_completion_sstf(struct RCB request_queue[QUEUEMAX],int *queue_cnt,int current_cylinder){
    //Handle Empty Queue
    struct RCB nullRCB = {0, 0, 0, 0, 0};
    if(*queue_cnt == 0){
        return nullRCB;
    }
    else{
        struct RCB nextRCB;
        int firstInitial = 0, firstCylinder = 0, firstArrival = 0;
        firstCylinder = abs(current_cylinder - request_queue[0].cylinder);
        firstArrival = request_queue[0].arrival_timestamp;
        //Looking for the smallest gap between the cylinder(s).
        for(int i = 1; i < *queue_cnt; i++){
            if(firstCylinder == abs(current_cylinder - request_queue[i].cylinder)){
                if(firstArrival > request_queue[i].arrival_timestamp){
                    firstCylinder = abs(current_cylinder - request_queue[i].cylinder);
                    firstArrival = request_queue[i].arrival_timestamp;
                    firstInitial = i;
                }
            }
            else if(firstCylinder > abs(current_cylinder - request_queue[i].cylinder)){
                firstCylinder = abs(current_cylinder - request_queue[i].cylinder);
                firstArrival = request_queue[i].arrival_timestamp;
                firstInitial = i;
            }
        }
        nextRCB = request_queue[firstInitial];
        for(int i = firstInitial; i < *queue_cnt; i++){
            request_queue[i] = request_queue[i+1];
        }
        *queue_cnt -= 1;
        return nextRCB;
    }
};

//Algorithm for LOOK Scheduling Arrival (LOOK).
struct RCB handle_request_arrival_look(struct RCB request_queue[QUEUEMAX],int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp);
struct RCB handle_request_arrival_look(struct RCB request_queue[QUEUEMAX],int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp){
    //If disk space is free return new request.
    if(nullRCB(new_request)){
        return new_request;
    }
    //Add new request to the queue and return current request.
    else{
        if(*queue_cnt <= QUEUEMAX){
            request_queue[*queue_cnt] = new_request;
            *queue_cnt += 1;
            return current_request;
        }
        else{
            return current_request;
        }
    }
};

//Algorithm for LOOK Scheduling Completion (LOOK).
struct RCB handle_request_completion_look(struct RCB request_queue[QUEUEMAX],int  *queue_cnt, int current_cylinder, int scan_direction);
struct RCB handle_request_completion_look(struct RCB request_queue[QUEUEMAX],int  *queue_cnt, int current_cylinder, int scan_direction){
    //Handle empty queue.
    struct RCB nullRCB = {0, 0, 0, 0, 0};
    if(*queue_cnt <= 0){
        return nullRCB;
    }
    //Picks next service from request queue.
    else{
        struct RCB nextRCB;
        int pointer = 0, initialCylinder = 0, intialArrival = 0;
        bool flag = false, posCylinder = false, firstCylinder = false, firstDirection = false, oppositeDirection = false;
        for (int i = 0; i < *queue_cnt; i++){
            //Pick the request with the earliest arrival time.
            if (current_cylinder == request_queue[i].cylinder){
                if (firstCylinder == false){
                    intialArrival = request_queue[i].arrival_timestamp;
                    pointer = i;
                    firstCylinder = true;
                    flag = true;
                }
                else if (intialArrival > request_queue[i].arrival_timestamp){
                    intialArrival = request_queue[i].arrival_timestamp;
                    pointer = i;
                }
            }
            //Pick the closest to the current cylinder if the scan directino is 1.
            else if ((scan_direction == 1) && (flag == false)){
                if(request_queue[i].cylinder - current_cylinder > 0){
                    //looking for larger cylinder
                    if (firstDirection == false){
                        initialCylinder = request_queue[i].cylinder - current_cylinder;
                        pointer = i;
                        posCylinder = true;
                        firstDirection = true;
                    }
                    else if (initialCylinder > request_queue[i].cylinder - current_cylinder){
                        initialCylinder = request_queue[i].cylinder - current_cylinder;
                        pointer = i;
                    }
                }
                else if ((posCylinder == false) && (current_cylinder - request_queue[i].cylinder > 0)){
                    if (oppositeDirection == false){
                        initialCylinder = abs(request_queue[i].cylinder - current_cylinder);
                        pointer = i;
                        oppositeDirection = true;
                    }
                    else if(initialCylinder > abs(request_queue[i].cylinder - current_cylinder)){
                        initialCylinder = abs(request_queue[i].cylinder - current_cylinder);
                        pointer = i;
                    }
                }
            }
            //Pick the closest to the current cylinder if scan direction is 0.
            else if ((scan_direction == 0) && (flag == false)){
                if(current_cylinder - request_queue[i].cylinder > 0){
                    //looking for larger cylinder
                    if (firstDirection == false){
                        initialCylinder = current_cylinder - request_queue[i].cylinder;
                        pointer = i;
                        posCylinder = true;
                        firstDirection = true;
                    }
                    else if (initialCylinder > current_cylinder - request_queue[i].cylinder){
                        initialCylinder = current_cylinder - request_queue[i].cylinder;
                        pointer = i;
                    }
                }
                else if ((posCylinder == false) && (current_cylinder - request_queue[i].cylinder < 0)){
                    if (oppositeDirection == false){
                        initialCylinder = abs(current_cylinder - request_queue[i].cylinder);
                        pointer = i;
                        oppositeDirection = true;
                    }
                    else if(initialCylinder > abs(request_queue[i].cylinder - current_cylinder)){
                        initialCylinder = abs(current_cylinder - request_queue[i].cylinder);
                        pointer = i;
                    }
                }
            }
        }
        nextRCB = request_queue[pointer];
        for (int i = pointer; i < *queue_cnt ; i++){
            request_queue[i] = request_queue[i+1];
        }
        //Remove RCB from queue and return the next.
        *queue_cnt -= 1;
        return nextRCB;
    }
};
