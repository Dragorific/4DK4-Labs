
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

#include <stdio.h>

#include "trace.h"
#include "main.h"
#include "output.h"
#include "simparameters.h"
#include "call_departure.h"

/*******************************************************************************/

/*
 * Function to schedule an end of call event.
 */

long int
schedule_end_call_on_channel_event(Simulation_Run_Ptr simulation_run,
				   double event_time,
				   void * channel)
{
  Event new_event;

  new_event.description = "End of Call";
  new_event.function = end_call_on_channel_event;
  new_event.attachment = channel;

  return simulation_run_schedule_event(simulation_run, new_event, event_time);
}

long int
schedule_end_call_on_queue_event(Simulation_Run_Ptr simulation_run,
				   double event_time,
				   long int count)
{
  Event new_event;

  new_event.description = "Caller Left Queue";
  new_event.function = end_call_on_queue_event;
  new_event.attachment = count;

  return simulation_run_schedule_event(simulation_run, new_event, event_time);
}

/*******************************************************************************/

/*
 * Function which handles end of call events.
 */

void
end_call_on_channel_event(Simulation_Run_Ptr simulation_run, void * c_ptr)
{
    Call_Ptr this_call;
    Call_Ptr new_call;
    Channel_Ptr channel;
    Simulation_Run_Data_Ptr sim_data;
    double now;

    channel = (Channel_Ptr) c_ptr;

    now = simulation_run_get_time(simulation_run);
    sim_data = simulation_run_data(simulation_run);

    /* Remove the call from the channel.*/
    this_call = (Call_Ptr) server_get(channel);

    TRACE(printf("End Of Call.\n"););

    /* Collect statistics. */
    sim_data->number_of_calls_processed++;
    sim_data->accumulated_call_time += now - this_call->arrive_time;


    output_progress_msg_to_screen(simulation_run);

    /* This call is done. Free up its allocated memory.*/
    xfree((void*) this_call);

    while(fifoqueue_size(sim_data->buffer) > 0){
        new_call = (Call_Ptr) fifoqueue_get(sim_data->buffer);
        if(now-new_call->arrive_time < new_call->call_wait){
            server_put(channel, (void*) new_call);
            new_call->channel = channel;

            if((now - new_call->arrive_time) < t){
                sim_data->less_than_t++;
            }

            new_call->arrive_time = now;

            sim_data->accumulated_wait_time += now - new_call->arrive_time;
            sim_data->customers_served_queue++;

            schedule_end_call_on_channel_event(simulation_run,
                                               now + (new_call->call_duration),
                                               (void *) channel);
            break;
        } else {
            if(new_call->call_wait < t){
                sim_data->less_than_t++;
            }

            sim_data->accumulated_wait_time += new_call->call_wait;
            sim_data->customers_served_queue++;
            sim_data->blocked_call_count++;
        }
    }
}

void
end_call_on_queue_event(Simulation_Run_Ptr simulation_run, long int count)
{
    Simulation_Run_Data_Ptr sim_data;
    double now;
    now = simulation_run_get_time(simulation_run);

    sim_data = simulation_run_data(simulation_run);

    TRACE(printf("End Of Wait in Queue.\n"););

    if(count == fifoqueue_size(sim_data->buffer)){
        Call_Ptr this_call;
        this_call = (Call_Ptr) fifoqueue_get(sim_data->buffer);

        /* Collect statistics. */
        sim_data->accumulated_call_time += now - this_call->arrive_time;
        sim_data->customers_served_queue++;

        /* This call is done. Free up its allocated memory.*/
        xfree((void*) this_call);
    }
}



