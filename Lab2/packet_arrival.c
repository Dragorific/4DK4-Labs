
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

#include <math.h>
#include <stdio.h>
#include "main.h"
#include "packet_transmission.h"
#include "packet_arrival.h"

/******************************************************************************/

/*
 * This function will schedule a packet arrival at a time given by
 * event_time. At that time the function "packet_arrival" (located in
 * packet_arrival.c) is executed. An object can be attached to the event and
 * can be recovered in packet_arrival.c.
 */

long int
schedule_packet_arrival_event(Simulation_Run_Ptr simulation_run,
			      double event_time)
{
  Event event;

  event.description = "Packet Arrival on Switch 1";
  event.function = packet_arrival_event;
  event.attachment = (void *) NULL;

  return simulation_run_schedule_event(simulation_run, event, event_time);
}

long int
schedule_packet_arrival_event_2(Simulation_Run_Ptr simulation_run,
			      double event_time)
{
  Event event;

  event.description = "Packet Arrival on Switch 2";
  event.function = packet_arrival_event_2;
  event.attachment = (void *) NULL;

  return simulation_run_schedule_event(simulation_run, event, event_time);
}

long int
schedule_packet_arrival_event_3(Simulation_Run_Ptr simulation_run,
			      double event_time)
{
  Event event;

  event.description = "Packet Arrival on Switch 3";
  event.function = packet_arrival_event_3;
  event.attachment = (void *) NULL;

  return simulation_run_schedule_event(simulation_run, event, event_time);
}

long int
schedule_packet_arrival_event_wireless(Simulation_Run_Ptr simulation_run,
			      double event_time,
			      Packet_Ptr packet)
{
  Event event;

  event.description = "Packet Arrival from Switch 1 to new Switch";
  event.function = packet_arrival_event_wireless;
  event.attachment = (void *) packet;

  return simulation_run_schedule_event(simulation_run, event, event_time);
}

/******************************************************************************/

/*
 * This is the event function which is executed when a packet arrival event
 * occurs. It creates a new packet object and places it in either the fifo
 * queue if the server is busy. Otherwise it starts the transmission of the
 * packet. It then schedules the next packet arrival event.
 */

// This arrival event will handle Switch 1
void
packet_arrival_event(Simulation_Run_Ptr simulation_run, void * ptr)
{
    Simulation_Run_Data_Ptr data;
    Packet_Ptr new_packet;

    data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);
    if(data->number_of_packets_processed <= RUNLENGTH){
        data->arrival_count++;

        new_packet = (Packet_Ptr) xmalloc(sizeof(Packet));
        new_packet->arrive_time = simulation_run_get_time(simulation_run);
        new_packet->service_time = get_packet_transmission_time();
        new_packet->status = WAITING;

        /*
        * Start transmission if the data link is free. Otherwise put the packet into
        * the buffer.
        */

        if(server_state(data->link) == BUSY) {
            fifoqueue_put(data->buffer, (void*) new_packet);
        } else {
            start_transmission_on_link(simulation_run, new_packet, data->link);
        }

        /*
        * Schedule the next packet arrival. Independent, exponentially distributed
        * interarrival times gives us Poisson process arrivals.
        */

        schedule_packet_arrival_event(simulation_run,
                simulation_run_get_time(simulation_run) +
                exponential_generator((double) 1/data->arrival_rate));
    }
}

// This arrival event will handle Switch 2
void
packet_arrival_event_2(Simulation_Run_Ptr simulation_run, void * ptr)
{
    Simulation_Run_Data_Ptr data;
    Packet_Ptr new_packet2;

    data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);
    if(data->number_of_packets_processed2 <= RUNLENGTH){
        data->arrival_count2++;

        new_packet2 = (Packet_Ptr) xmalloc(sizeof(Packet));
        new_packet2->arrive_time = simulation_run_get_time(simulation_run);
        new_packet2->service_time = get_packet_transmission_time_23();
        new_packet2->status = WAITING;

        /*
        * Start transmission if the data link is free. Otherwise put the packet into
        * the buffer. This will run for both links since they have the same properties
        */

        if(server_state(data->link2) == BUSY) {
            //printf("Packet being buffered into Buffer 2\n");
            fifoqueue_put(data->buffer2, (void*) new_packet2);
        } else {
            printf("Packet being sent to Link 2\n");
            start_transmission_on_link2(simulation_run, new_packet2, data->link2);
        }

        /*
        * Schedule the next packet arrival. Independent, exponentially distributed
        * interarrival times gives us Poisson process arrivals.
        */

        schedule_packet_arrival_event_2(simulation_run,
                simulation_run_get_time(simulation_run) +
                exponential_generator((double) 1/data->arrival_rate_23));
    }
}

// This arrival event will handle Switch 3
void
packet_arrival_event_3(Simulation_Run_Ptr simulation_run, void * ptr)
{
    Simulation_Run_Data_Ptr data;
    Packet_Ptr new_packet3;

    data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);
    if(data->number_of_packets_processed3 <= RUNLENGTH){
        data->arrival_count3++;

        new_packet3 = (Packet_Ptr) xmalloc(sizeof(Packet));
        new_packet3->arrive_time = simulation_run_get_time(simulation_run);
        new_packet3->service_time = get_packet_transmission_time_23();
        new_packet3->status = WAITING;

        /*
        * Start transmission if the data link is free. Otherwise put the packet into
        * the buffer. This will run for both links since they have the same properties
        */

        if (server_state(data->link3) == BUSY) {
            fifoqueue_put(data->buffer3, (void*) new_packet3);
            //printf("Packet being buffered into Buffer 3\n");
        } else {
            printf("Packet being sent to Link 3\n");
            start_transmission_on_link3(simulation_run, new_packet3, data->link3);
        }

        /*
        * Schedule the next packet arrival. Independent, exponentially distributed
        * interarrival times gives us Poisson process arrivals.
        */

        schedule_packet_arrival_event_3(simulation_run,
                simulation_run_get_time(simulation_run) +
                exponential_generator((double) 1/data->arrival_rate_23));
    }
}

// This arrival event will handle the transfer from switch 1 to switches 2 or 3
void
packet_arrival_event_wireless(Simulation_Run_Ptr simulation_run, Packet_Ptr packet, void * ptr)
{
    Simulation_Run_Data_Ptr data;

    data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);

    packet->service_time = get_packet_transmission_time_23();
    packet->status = WAITING;

    double p13 = 0.3;

    //time_t t;
    //srand((unsigned) time(&t));
    double random = (double)rand()/(double)RAND_MAX;

    // Determine using probability if the packet is to be sent to Switch 2 or 3
    if(random <= p13){
        data->arrival_count3++;
        if (server_state(data->link3) == BUSY) {
            fifoqueue_put(data->buffer3, (void*) packet);
        } else {
            start_transmission_on_link13(simulation_run, packet, data->link3);
        }
    } else {
        data->arrival_count2++;
        if (server_state(data->link2) == BUSY) {
            fifoqueue_put(data->buffer2, (void*) packet);
        } else {
            start_transmission_on_link12(simulation_run, packet, data->link2);
        }
    }

}



