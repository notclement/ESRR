/************************************************************/
/*           Efficient Process Scheduling Algorithm         */
/*                using RR and SRTF (ESRR)                  */
/*                                                          */
/*          Written by: Clement (github: @notclement)       */
/************************************************************/

// Benefits:
// 1. Reduce total wait time and turnaround time compared to RR
// 2. Reduce waiting time of shorter processes and provides longer process to execute faster than SRTF
// In theory, it should be better than RR and SRTF for longer and shorter processes respectively

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main() {
  int i, j, n, RR_mode = 0, tmp, process_id, max_input = 5001, prev_burst = 0, timer_sjf_ended_at = 0, just_RR = 0;
  int process_arrival_order[max_input], curr_queue[max_input], curr_burst_left[max_input], min_time_range, max_time_range, one_left = 0;
  int process_burst[max_input], arrival_time[max_input], last_arrive = 0, tmp_proc, tmp_burst, queue_ptr = 0;
  int wt_for_each_process[max_input], turnaround_time[max_input], response_time[max_input], original_burst[max_input];
  int timeslice, max_time_needed = 0, timer, context_switches = 0, previously_ran = 0;
  float avg_wait_time = 0.0, avg_turnaround_time = 0.0, avg_response_time = 0.0;

  system("clear");

  printf("Enter the no of processes: ");
  scanf("%d", &n);

  printf("\n");

  printf("== Arrival Times ==\n");
  for (i = 0; i < n; i++) {
    printf("Arrival Time [P%d]: ", i + 1); // 0 0 2 5 7
    scanf("%d", &arrival_time[i]);
  }
  for (i = 0; i < n; i++) {
    if (last_arrive < arrival_time[i]) {
      last_arrive = arrival_time[i];
    }
    process_id = i + 1;
    process_arrival_order[i] = process_id;
    response_time[i] = -1;
  }

  printf("\n== Burst Times ==\n");
  for (i = 0; i < n; i++) {
    printf("Burst Times [%d]: ", i + 1); // 8 10 5 7 3
    scanf("%d", &process_burst[i]);
    original_burst[i] = process_burst[i];
  }
  for (i = 0; i < n; i++) {
    max_time_needed = max_time_needed + process_burst[i];
  }

  printf("\nEnter the size of time slice: ");
  scanf("%d", &timeslice);

  printf("\n");

  // run it as long as within the max amount of time needed
  for (timer = 0; timer < max_time_needed; timer++) {

    // at the start confirm is RR mode
    if (RR_mode == 0) {

      // at time 0, check who arrives
      // loop through arrival time to find arrival time 0
      if (timer == 0) {
        for (j = 0; j < n; j++) {

          // if a new process just arrives at the current time
          if (arrival_time[j] == 0) {

            // add the process id to curr_queue and burst to curr_burst_left
            curr_queue[queue_ptr] = process_arrival_order[j];
            curr_burst_left[queue_ptr] = process_burst[j];

            printf("[+] P%d arrived at T%d and placed in position %d.\n", curr_queue[queue_ptr], arrival_time[j], queue_ptr);

            queue_ptr = queue_ptr + 1;
          }

        }

      } // end of the arrival for processes at T0

      // here, we will add all the processes that arrive between the current timer - timeslice
      if (timer > 0) {

        min_time_range = timer - prev_burst + 1;
        max_time_range = timer;

        for (j = 0; j < n; j++) {

          if (arrival_time[j] >= min_time_range && arrival_time[j] <= max_time_range && timer != 0) {

            // add the process id to curr_queue and burst to curr_burst_left
            curr_queue[queue_ptr] = process_arrival_order[j];
            curr_burst_left[queue_ptr] = process_burst[j];

            printf("[+] P%d arrived at T%d and placed in position %d.\n", curr_queue[queue_ptr], arrival_time[j], queue_ptr);

            queue_ptr = queue_ptr + 1;

          }
        }

      }

      // sort by lowest burst time first
      for (int a = 0; a < queue_ptr; a++) {
        // Place currently selected element array[i]
        // to its correct place.
        for (j = a + 1; j < queue_ptr; j++) {

          // Swap if currently selected array element
          // is not at its correct position.
          if (curr_burst_left[a] > curr_burst_left[j]) {

            // swap the burst array
            tmp = curr_burst_left[a];
            curr_burst_left[a] = curr_burst_left[j];
            curr_burst_left[j] = tmp;

            // swap the processid array
            tmp = curr_queue[a];
            curr_queue[a] = curr_queue[j];
            curr_queue[j] = tmp;
          }
        }
      } // end sort

      // print the curr_queue array only when its multiple of timeslice
      printf("Time %d - ", timer);
      for (int loop = 0; loop < queue_ptr; loop++) {

        printf("P%d:%d ", curr_queue[loop], curr_burst_left[loop]);

      }
      printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

      context_switches = context_switches + 1;

      // if the timer is after the last_arrive time, means its time to shift to RR
      if (timer > last_arrive) {
        timer_sjf_ended_at = timer;
        // printf("\nRR mode activated\n\n");
        RR_mode = 1;
      }

      // do normal stuff in SJF mode
      else {

        // check if the burst for first element is enough to last the whole timeslice or not
        // if less than timeslice
        if (curr_burst_left[0] <= timeslice) {

          // check if this is the first time that it is ran
          // if so, add the timer to response_time[] in the correct order
          // use process_arrival_order to get the pid-1 in order to get the pos
          for (i = 0; i < n; i++) {
            if (curr_queue[0] == process_arrival_order[i]) {
              // only put into response_time if not done before
              if (response_time[i] == -1) {
                response_time[i] = timer;
              }
            }
          }

          // store the prev_burst state for adding new arrivals later
          prev_burst = curr_burst_left[0];

          // add the curr_burst_left to the timer, it will be less than timeslice
          timer = timer + curr_burst_left[0] - 1;

          printf("[-] P%d has left the queue (burst finished).\n", curr_queue[0]);

          // add the completed time to the turnaround_time in the correct process
          for (i = 0; i < n; i++) {
            // ensure that its the correct process_id
            if (curr_queue[0] == process_arrival_order[i]) {
              turnaround_time[i] = timer + 1;
              // printf("> adding %d to tat timer for P%d\n", timer + 1, curr_queue[0]);
            }
          }

          // shift everything 1 index forward to remove the element 0
          for (int move_ctr = 0; move_ctr < queue_ptr; move_ctr++) {
            curr_queue[move_ctr] = curr_queue[move_ctr + 1];
            curr_burst_left[move_ctr] = curr_burst_left[move_ctr + 1];
          }
          queue_ptr = queue_ptr - 1;

        }
        // if more than the timeslice
        else {

          // check if this is the first time that it is ran
          // if so, add the timer to response_time[] in the correct order
          // use process_arrival_order to get the pid-1 in order to get the pos
          for (i = 0; i < n; i++) {
            if (curr_queue[0] == process_arrival_order[i]) {
              // only put into response_time if not done before
              if (response_time[i] == -1) {
                response_time[i] = timer;
              }
            }
          }

          // store the prev_burst state for adding new arrivals later
          prev_burst = timeslice;

          // add full timeslice to the timer
          timer = timer + timeslice - 1;

          // reduce curr_burst_left by the timeslice used (which is the full timeslice)
          curr_burst_left[0] = curr_burst_left[0] - timeslice;

        }

      }

    }

    // RR mode
    if (RR_mode) {

      // printf("WE IN RR MODE\n\n");
      just_RR = 1;

      for (timer = timer_sjf_ended_at; timer < max_time_needed; timer++) {

        // if the burst left can be completed within this timeslice and when just_RR
        if (curr_burst_left[0] <= timeslice && just_RR == 1) {

          // context_switches
          if (previously_ran != curr_queue[0]) {
            context_switches = context_switches + 1;
            previously_ran = curr_queue[0];
          } else {
            previously_ran = curr_queue[0];
          }

          // check if this is the first time that it is ran
          // if so, add the timer to response_time[] in the correct order
          // use process_arrival_order to get the pid-1 in order to get the pos
          for (i = 0; i < n; i++) {
            if (curr_queue[0] == process_arrival_order[i]) {
              // only put into response_time if not done before
              if (response_time[i] == -1) {
                response_time[i] = timer;
              }
            }
          }

          printf("[-] P%d has left the queue (burst finished).\n", curr_queue[0]);

          timer = timer + curr_burst_left[0];

          // add the completed time to the turnaround_time in the correct process
          for (i = 0; i < n; i++) {
            // ensure that its the correct process_id
            if (curr_queue[0] == process_arrival_order[i]) {
              turnaround_time[i] = timer;
              // printf("> adding %d to tat timer for P%d\n", timer + 1, curr_queue[0]);
            }
          }

          // shift everything 1 index forward to remove the element 0
          for (int move_ctr = 0; move_ctr < queue_ptr; move_ctr++) {
            curr_queue[move_ctr] = curr_queue[move_ctr + 1];
            curr_burst_left[move_ctr] = curr_burst_left[move_ctr + 1];
          }

          queue_ptr = queue_ptr - 1;

          just_RR = 0;

        }

        else if (curr_burst_left[0] <= timeslice && just_RR == 0) {

          // context_switches
          if (previously_ran != curr_queue[0]) {
            context_switches = context_switches + 1;
            previously_ran = curr_queue[0];
          } else {
            previously_ran = curr_queue[0];
          }

          // check if this is the first time that it is ran
          // if so, add the timer to response_time[] in the correct order
          // use process_arrival_order to get the pid-1 in order to get the pos
          for (i = 0; i < n; i++) {
            if (curr_queue[0] == process_arrival_order[i]) {
              // only put into response_time if not done before
              if (response_time[i] == -1) {
                response_time[i] = timer;
              }
            }
          }

          printf("[-] P%d has left the queue (burst finished).\n", curr_queue[0]);

          timer = timer + curr_burst_left[0] - 1;

          // add the completed time to the turnaround_time in the correct process
          for (i = 0; i < n; i++) {
            // ensure that its the correct process_id
            if (curr_queue[0] == process_arrival_order[i]) {
              turnaround_time[i] = timer;
              // printf("> adding %d to tat timer for P%d\n", timer + 1, curr_queue[0]);
            }
          }

          // shift everything 1 index forward to remove the element 0
          for (int move_ctr = 0; move_ctr < queue_ptr; move_ctr++) {
            curr_queue[move_ctr] = curr_queue[move_ctr + 1];
            curr_burst_left[move_ctr] = curr_burst_left[move_ctr + 1];
          }

          queue_ptr = queue_ptr - 1;

        }

        // burst left is more than the timeslice, so this will not the be last time it runs
        else {

          // context_switches
          if (previously_ran != curr_queue[0]) {
            context_switches = context_switches + 1;
            previously_ran = curr_queue[0];
          } else {
            previously_ran = curr_queue[0];
          }

          // check if this is the first time that it is ran
          // if so, add the timer to response_time[] in the correct order
          // use process_arrival_order to get the pid-1 in order to get the pos
          for (i = 0; i < n; i++) {
            if (curr_queue[0] == process_arrival_order[i]) {
              // only put into response_time if not done before
              if (response_time[i] == -1) {
                response_time[i] = timer - 1;
              }
            }
          }

          curr_burst_left[0] = curr_burst_left[0] - timeslice;

          timer = timer + timeslice - 1;

          tmp_proc = curr_queue[0];
          tmp_burst = curr_burst_left[0];
          // shift everything 1 index forward to remove the element 0
          for (int move_ctr = 0; move_ctr < queue_ptr; move_ctr++) {
            curr_queue[move_ctr] = curr_queue[move_ctr + 1];
            curr_burst_left[move_ctr] = curr_burst_left[move_ctr + 1];
          }
          curr_queue[queue_ptr - 1] = tmp_proc;
          curr_burst_left[queue_ptr - 1] = tmp_burst;

        }

        // print the curr_queue array only when its multiple of timeslice
        printf("Time %d - ", timer);
        for (int loop = 0; loop < queue_ptr; loop++) {

          printf("P%d:%d ", curr_queue[loop], curr_burst_left[loop]);

        }
        printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

      }

    }

  } // end of the full time loop

  printf("\n");

  // meaning something still inside the queue
  if (queue_ptr > 0) {
    turnaround_time[curr_queue[0] - 1] = max_time_needed;
  }

  // print the total time used
  printf("Total Time Used: %d ms\n", max_time_needed);

  printf("\n");

  // get turnaround time for each process
  printf("Turnaround time:\n");
  for (i = 0 ; i < n; i++) {
    printf("P%d: %d ms\n", process_arrival_order[i], turnaround_time[i] - arrival_time[i]);
    avg_turnaround_time = avg_turnaround_time + turnaround_time[i] - arrival_time[i];
  }
  printf("\n");

  // get waiting time for each process
  printf("Waiting time:\n");
  for (i = 0 ; i < n; i++) {
    printf("P%d: %d ms\n", process_arrival_order[i], (turnaround_time[i] - arrival_time[i]) - process_burst[i]);
    avg_wait_time = avg_wait_time + (turnaround_time[i] - arrival_time[i]) - process_burst[i];
  }
  printf("\n");

  // get waiting time for each process
  printf("Response time:\n");
  for (i = 0 ; i < n; i++) {
    printf("P%d: %d ms\n", process_arrival_order[i], response_time[i]);
    avg_response_time = avg_response_time + response_time[i];
  }
  printf("\n");

  // calculate average waiting time
  printf("Average Waiting Time: %0.2f ms\n", avg_wait_time / n);

  // calculate average turnaround time
  printf("Average Turnaround Time: %0.2f ms\n", avg_turnaround_time / n);

  // calculate average response time
  printf("Average Response Time: %0.2f ms\n", avg_response_time / n);

  // print context switches
  context_switches = context_switches - 1;  // minus 1 because at time 0, we dont care about that context switch
  printf("Context switches: %d\n", context_switches);

}
