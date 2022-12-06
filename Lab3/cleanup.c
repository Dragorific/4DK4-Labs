
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

#include "simlib.h"
#include "main.h"
#include "simparameters.h"
#include "cleanup.h"

/*******************************************************************************/

void cleanup (Simulation_Run_Ptr this_simulation_run)
{
    int i;
    Simulation_Run_Data_Ptr sim_data;

    sim_data = (Simulation_Run_Data_Ptr) simulation_run_data(this_simulation_run);

    /* Clean out the channels. */
    for (i=0; i<sim_data->number_channels; i++) {
        if( (*(sim_data->channels+i))->state == BUSY)
            xfree(server_get(*(sim_data->channels+i)));
    }
    xfree(sim_data->channels);

    while (fifoqueue_size(sim_data->buffer) > 0){ /* Clean out the queue. */
        xfree(fifoqueue_get(sim_data->buffer));
    }
    xfree(sim_data->buffer);

    /* Clean up the simulation_run. */
    simulation_run_free_memory(this_simulation_run);
}



