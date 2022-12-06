
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

#include <math.h>
#include <stdio.h>
#include "main.h"
#include "call_duration.h"
#include "simparameters.h"
#include "call_departure.h"
#include "call_arrival.h"

/*******************************************************************************/

/*
 * Function to schedule a call arrival event.
 */

long int
schedule_call_arrival_event(Simulation_Run_Ptr simulation_run,
			    double event_time)
{
  Event new_event;

  new_event.description = "Call Arrival";
  new_event.function = call_arrival_event;
  new_event.attachment = NULL;

  return simulation_run_schedule_event(simulation_run, new_event, event_time);
}

/*******************************************************************************/

/*
 * Call arrival event function.
 */

void
call_arrival_event(Simulation_Run_Ptr simulation_run, void * ptr)
{
    Call_Ptr new_call;
    Channel_Ptr free_channel;
    Simulation_Run_Data_Ptr sim_data;
    double now;

    now = simulation_run_get_time(simulation_run);

    sim_data = simulation_run_data(simulation_run);
    sim_data->call_arrival_count++;

    new_call = (Call_Ptr) xmalloc(sizeof(Call));
    new_call->arrive_time = now;
    new_call->call_duration = get_call_duration(simulation_run);
    new_call->call_wait = get_wait_duration(simulation_run);

    /* See if there is a free channel.*/
    if((free_channel = get_free_channel(simulation_run, sim_data->number_channels)) != NULL) {
        /* Yes, we found one.*/
        /* Place the call in the free channel and schedule its
           departure. */
        server_put(free_channel, (void*) new_call);
        new_call->channel = free_channel;
        sim_data->less_than_t++;

        schedule_end_call_on_channel_event(simulation_run,
                                           now + (new_call->call_duration),
                                           (void *) free_channel);
    } else {
        /* No free channel was found. The call is queue'd in and the customer departure is scheduled. */
        fifoqueue_put(sim_data->buffer, (void*) new_call);

        /*schedule_end_call_on_queue_event(simulation_run,
                                         now + (new_call->call_wait),
                                         fifoqueue_size(sim_data->buffer));*/

    }

    /* Schedule the next call arrival. */
    schedule_call_arrival_event(simulation_run,
          now + exponential_generator((double) 1/sim_data->arrival_rate));
}

/*******************************************************************************/

/*
 * Scan through the channels and return a free one, if possible. Otherwise
 * return NULL.
 */

Channel_Ptr get_free_channel(Simulation_Run_Ptr simulation_run, int NUMBER_OF_CHANNELS)
{
  Channel_Ptr *channels;
  int i;
  Simulation_Run_Data_Ptr sim_data;

  sim_data = simulation_run_data(simulation_run);
  channels = sim_data->channels;

  for (i=0; i<NUMBER_OF_CHANNELS; i++) {
    if (server_state(*(channels+i)) == FREE)
      return *(channels+i);
  }
  return (Channel_Ptr) NULL;
}


