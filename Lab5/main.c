
/*
 * Simulation_Run of the ALOHA Protocol
 * 
 * Copyright (C) 2014 Terence D. Todd Hamilton, Ontario, CANADA
 * todd@mcmaster.ca
 * 
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option)
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/*******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "output.h"
#include "trace.h"
#include "simparameters.h"
#include "cleanup.h"
#include "packet_arrival.h"
#include "packet_transmission.h"
#include "main.h"

/*******************************************************************************/

int
main(void)
{
  /* Get the list of random number generator seeds defined in simparameters.h */
  unsigned random_seed;
  unsigned RANDOM_SEEDS[] = {RANDOM_SEED_LIST, 0};

  Simulation_Run_Ptr simulation_run;
  Simulation_Run_Data data;
  int i, j=0;

  /* Do a new simulation_run for each random number generator seed. */
  while ((random_seed = RANDOM_SEEDS[j++]) != 0) {

    /* Set the random generator seed. */
    random_generator_initialize(random_seed);

    /* Create a new simulation_run. This gives a clock and
       eventlist. Clock time is set to zero. */
    simulation_run = (Simulation_Run_Ptr) simulation_run_new();

    /* Add our data definitions to the simulation_run. */
    simulation_run_set_data(simulation_run, (void *) & data);

    /* Create and initalize the stations. */
    data.stations = (Station_Ptr) xcalloc((unsigned int) NUMBER_OF_STATIONS,
					  sizeof(Station));

    /* Initialize various simulation_run variables. */
    data.blip_counter = 0;
    data.arrival_count = 0;
    data.number_of_packets_processed = 0;
    data.number_of_collisions = 0;
    data.accumulated_delay = 0.0;
    data.random_seed = random_seed;
    
    /* Initialize the stations. */
    for(i=0; i<NUMBER_OF_STATIONS; i++) {
      (data.stations+i)->id = i;
      (data.stations+i)->buffer = fifoqueue_new();
      (data.stations+i)->packet_count = 0;
      (data.stations+i)->accumulated_delay = 0.0;
      (data.stations+i)->mean_delay = 0;
    }

    /* Create and initialize the channel. */
    data.channel = channel_new();

    /* Schedule initial packet arrival. */
    schedule_packet_arrival_event(simulation_run, 
		    simulation_run_get_time(simulation_run) +
		    exponential_generator((double) 1/PACKET_ARRIVAL_RATE));

    /* Execute events until we are finished. */
    while(data.number_of_packets_processed < RUNLENGTH) {
      simulation_run_execute_event(simulation_run);
    }

    /* Print out some results. */
    output_results(simulation_run);

    /* Clean up memory. */
    cleanup(simulation_run);
  }

  /* Pause before finishing. */
  getchar();

  return 0;
}












