#include<stdio.h>
#include"oslabs.h"

// Priority-based Preemptive Scheduler Function. (Arrival)
struct PCB handle_process_arrival_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp){
    //Process to execute next and returns the PCB.
    if (current_process.process_id == 0
        && current_process.arrival_timestamp == 0
        && current_process.total_bursttime == 0
        && current_process.execution_starttime == 0
        && current_process.execution_endtime == 0
        && current_process.remaining_bursttime == 0
        && current_process.process_priority == 0){
            new_process.execution_starttime = timestamp;
            new_process.execution_endtime = timestamp + new_process.total_bursttime;
            new_process.remaining_bursttime = new_process.total_bursttime;

            return new_process;
    }
    //currently running process is higher prio than new process
    if(new_process.process_priority >= current_process.process_priority){
        new_process.execution_starttime = 0;
        new_process.execution_endtime = 0;
        new_process.remaining_bursttime = new_process.total_bursttime;
        ready_queue[*queue_cnt] = new_process;
        *queue_cnt  += 1;

        return current_process;
    }
    //new process has higher prio than currently running process
    else{
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;
        //adjust remaining_bursttime.
        current_process.remaining_bursttime = current_process.execution_endtime - timestamp;
        current_process.execution_endtime = 0;
        //add process to queue.
        ready_queue[*queue_cnt] = current_process;
        *queue_cnt += 1;

        return new_process;
    }
};
// Priority-based Preemptive Scheduler Function. (Completion)
struct PCB handle_process_completion_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp){
    //PCB is empty
    struct PCB null_PCB = {0, 0, 0, 0, 0, 0};
    //queue is empty - nothing to process
    if(*queue_cnt == 0){
        return null_PCB;
    }
    //find the highest priority in PCB
    int highest_priority = 0;
    int smallest_priority = ready_queue[0].process_priority;
    for(int i = 1; i < *queue_cnt; i++){
        if(ready_queue[i].process_priority < smallest_priority){
            smallest_priority = ready_queue[i].process_priority;
            highest_priority = i;
        }
    }
    null_PCB = ready_queue[highest_priority];
    for(int i = highest_priority; i < *queue_cnt - 1; i++){
        ready_queue[i] = ready_queue[i+1];
    }
    //start and end exeuction time modified as current timestamp and remaining burst time
    *queue_cnt -= 1;
    null_PCB.execution_starttime = timestamp;
    null_PCB.execution_endtime = timestamp + null_PCB.remaining_bursttime;
    return null_PCB;
};
//Shortest-Remaining-Time-Next Preemptive Scheduler (Arrival)
struct PCB handle_process_arrival_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int time_stamp){
    if (current_process.process_id == 0
        && current_process.arrival_timestamp == 0
        && current_process.total_bursttime == 0
        && current_process.execution_starttime == 0
        && current_process.execution_endtime == 0
        && current_process.remaining_bursttime == 0
        && current_process.process_priority == 0){
            new_process.execution_starttime = time_stamp;
            new_process.execution_endtime = time_stamp + new_process.total_bursttime;
            new_process.remaining_bursttime = new_process.total_bursttime;

            return new_process;
    }
    //currently running process is higher prio than new process
    if(new_process.total_bursttime >= current_process.remaining_bursttime){
        new_process.execution_starttime = 0;
        new_process.execution_endtime = 0;
        new_process.remaining_bursttime = new_process.total_bursttime;
        ready_queue[*queue_cnt] = new_process;
        *queue_cnt  += 1;

        return current_process;
    }
    //new process has higher prio than currently running process
    else{
        new_process.execution_starttime = time_stamp;
        new_process.execution_endtime = time_stamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;
        //adjust remaining_bursttime.
        current_process.remaining_bursttime = current_process.execution_endtime - time_stamp;
        current_process.execution_endtime = 0;
        current_process.execution_starttime = 0;
        //add process to queue.
        ready_queue[*queue_cnt] = current_process;
        *queue_cnt += 1;

        return new_process;
    }
};
//Shortest-Remaining-Time Preemptive Scheduler (Completion)
struct PCB handle_process_completion_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp){
    //PCB is empty
    struct PCB null_PCB = {0, 0, 0, 0, 0, 0};
    //queue is empty - nothing to process
    if(*queue_cnt == 0){
        return null_PCB;
    }
    //find the highest priority in PCB
    int smallest_bursttime_intial = 0;
    int smallest_bursttime = ready_queue[0].remaining_bursttime;
    for(int i = 1; i < *queue_cnt; i++){
        if(ready_queue[i].remaining_bursttime < smallest_bursttime){
            smallest_bursttime = ready_queue[i].remaining_bursttime;
            smallest_bursttime_intial = i;
        }
    }
    null_PCB = ready_queue[smallest_bursttime_intial];
    for(int i = smallest_bursttime_intial; i < *queue_cnt - 1; i++){
        ready_queue[i] = ready_queue[i+1];
    }
    //start and end exeuction time modified as current timestamp and remaining burst time
    *queue_cnt -= 1;
    null_PCB.execution_starttime = timestamp;
    null_PCB.execution_endtime = timestamp + null_PCB.remaining_bursttime;
    return null_PCB;
};
//Round-Robin Scheduler (Arrival)
struct PCB handle_process_arrival_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp, int time_quantum){
    if (current_process.process_id == 0
        && current_process.arrival_timestamp == 0
        && current_process.total_bursttime == 0
        && current_process.execution_starttime == 0
        && current_process.execution_endtime == 0
        && current_process.remaining_bursttime == 0
        && current_process.process_priority == 0){
            new_process.execution_starttime = timestamp;
            if (time_quantum < new_process.total_bursttime){
                new_process.execution_endtime = timestamp + time_quantum;
            }
            else{
                new_process.execution_endtime = timestamp + new_process.total_bursttime;
            }
            new_process.remaining_bursttime = new_process.total_bursttime;

            return new_process;
        }
    new_process.execution_starttime = 0;
    new_process.execution_endtime = 0;
    new_process.remaining_bursttime = new_process.total_bursttime;
    ready_queue[*queue_cnt] = new_process;
    *queue_cnt  += 1;

    return current_process;
};
//Round-Robin Scheduler (Completion)
struct PCB handle_process_completion_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int time_stamp, int time_quantum){
    struct PCB null_pcb = {0, 0, 0, 0, 0, 0, 0} ;
    if(*queue_cnt == 0) {
        return null_pcb;
    }
    int earliest_time_initial = 0;
    int earliest_time = ready_queue[0].arrival_timestamp;
    for (int i = 1; i < *queue_cnt; i++){
        if(ready_queue[i].remaining_bursttime < earliest_time){
            earliest_time = ready_queue[i].remaining_bursttime;
            earliest_time_initial = i;
        }
    }
    null_pcb = ready_queue[earliest_time_initial];
    for(int i = earliest_time_initial; i < *queue_cnt - 1; i++){
        ready_queue[i] = ready_queue[i+1];
    }
    *queue_cnt = *queue_cnt - 1;
    null_pcb.execution_starttime = time_stamp;
    if (time_quantum < null_pcb.remaining_bursttime) {
        null_pcb.execution_endtime = time_stamp + time_quantum;
    }
    else{
        null_pcb.execution_endtime = time_stamp + null_pcb.remaining_bursttime;
    }
 return null_pcb;
};

/*
//testing
int main(){
   struct PCB new_process;
   struct PCB current_process;
   struct PCB ready_queue[QUEUEMAX];
   //input queue_cnt to 0
   int queue_cnt=0;

   current_process.process_id=1;
   current_process.arrival_timestamp=1;
   current_process.total_bursttime=4;
   current_process.execution_starttime=1;
   current_process.execution_endtime=5;
   current_process.remaining_bursttime=4;
   current_process.process_priority=8;

   new_process.process_id=2;
   new_process.arrival_timestamp=2;
   new_process.total_bursttime=3;
   new_process.execution_starttime=0;
   new_process.execution_endtime=0;
   new_process.remaining_bursttime=3;
   new_process.process_priority=6;

   //input timestamp as '2'
   struct PCB proc=handle_process_arrival_pp(ready_queue,&queue_cnt,current_process,new_process,2);
   for(int i=0;i<queue_cnt;i++){
       printf("ready_queue: ");
       printf("[PID:%d, AT:%d, TBT:%d, EST:%d, EET:%d, RBT:%d, Priority:%d]\n",ready_queue[i].process_id,ready_queue[i].arrival_timestamp,ready_queue[i].total_bursttime,ready_queue[i].execution_starttime,ready_queue[i].execution_endtime,ready_queue[i].remaining_bursttime,ready_queue[i].process_priority);

       printf("queue_cnt: %d\n",queue_cnt);

       printf("PCB: ");
       printf("[PID:%d, AT:%d, TBT:%d, EST:%d, EET:%d, RBT:%d, Priority:%d]\n",proc.process_id,proc.arrival_timestamp,proc.total_bursttime,proc.execution_starttime,proc.execution_endtime,proc.remaining_bursttime,proc.process_priority);
    }
    return 0;
}
*/





