
/*
 *
 * Call Blocking in Circuit Switched Networks
 *
 * Copyright (C) 2014 Terence D. Todd
 * Hamilton, Ontario, CANADA
 * todd@mcmaster.ca
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 */

/*******************************************************************************/

#ifndef _MAIN_H_
#define _MAIN_H_

/*******************************************************************************/

#include "simlib.h"

/*******************************************************************************/

typedef Server Channel;
typedef Server_Ptr Channel_Ptr;

typedef enum {XMTTING, WAITING} Call_Status;

typedef struct _call_
{
  double arrive_time;
  double call_duration;
  double call_wait;
  Channel_Ptr channel;
} Call, * Call_Ptr;

typedef struct _simulation_run_data_
{
  Channel_Ptr * channels;
  Fifoqueue_Ptr buffer;
  int arrival_rate;
  int number_channels;
  double queue_duration;
  double call_duration;

  long int blip_counter;
  long int call_arrival_count;
  long int calls_processed;
  long int blocked_call_count;
  long int number_of_calls_processed;
  long int customers_served_queue;
  long int less_than_t;

  double accumulated_call_time;
  double accumulated_wait_time;
  unsigned random_seed;

} Simulation_Run_Data, * Simulation_Run_Data_Ptr;

/*******************************************************************************/

/*
 * Function prototypes
 */

extern int main(void);

/*******************************************************************************/

#endif /* main.h */






