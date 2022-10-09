
/*
 *
 * Simulation_Run of A Single Server Queueing System
 *
 * Copyright (C) 2014 Terence D. Todd Hamilton, Ontario, CANADA,
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

/******************************************************************************/

#include "simlib.h"
#include "main.h"
#include "cleanup_memory.h"

/******************************************************************************/

/*
 * When a simulation_run run is finished, this function cleans up the memory
 * that has been allocated.
 */

void
cleanup_memory (Simulation_Run_Ptr simulation_run)
{
  Simulation_Run_Data_Ptr data;
  Fifoqueue_Ptr buffer;
  Fifoqueue_Ptr buffer2;
  Fifoqueue_Ptr buffer3;
  Server_Ptr link1;
  Server_Ptr link2;
  Server_Ptr link3;

  data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);

  buffer = data->buffer;
  buffer2 = data->buffer2;
  buffer3 = data->buffer2;

  link1 = data->link;
  link2 = data->link2;
  link2 = data->link3;

  if(link1->state == BUSY) /* Clean out the first server. */
    xfree(server_get(link1));
  xfree(link1);

  if(link2->state == BUSY) /* Clean out the second server. */
    xfree(server_get(link2));
  xfree(link2);

  if(link3->state == BUSY) /* Clean out the third server. */
    xfree(server_get(link3));
  xfree(link3);

  while (fifoqueue_size(buffer) > 0) /* Clean out the sw1 queue. */
    xfree(fifoqueue_get(buffer));
  xfree(buffer);

  while (fifoqueue_size(buffer2) > 0) /* Clean out the sw2 queue. */
    xfree(fifoqueue_get(buffer2));
  xfree(buffer2);

  while (fifoqueue_size(buffer3) > 0) /* Clean out the sw3 queue. */
    xfree(fifoqueue_get(buffer3));
  xfree(buffer3);

  simulation_run_free_memory(simulation_run); /* Clean up the simulation_run. */
}



