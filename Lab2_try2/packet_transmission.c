
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

#include <stdio.h>
#include "trace.h"
#include "main.h"
#include "output.h"
#include "packet_transmission.h"

/******************************************************************************/

/*
 * This function will schedule the end of a packet transmission at a time given
 * by event_time. At that time the function "end_packet_transmission" (defined
 * in packet_transmissionl.c) is executed. A packet object is attached to the
 * event and is recovered in end_packet_transmission.c.
 */

long
schedule_end_packet_transmission_event(Simulation_Run_Ptr simulation_run,
				       double event_time,
				       Server_Ptr link)
{
  Event event;

  event.description = "Packet Xmt End";
  event.function = end_packet_transmission_event;
  event.attachment = (void *) link;

  return simulation_run_schedule_event(simulation_run, event, event_time);
}

/******************************************************************************/

/*
 * This is the event function which is executed when the end of a packet
 * transmission event occurs. It updates its collected data then checks to see
 * if there are other packets waiting in the fifo queue. If that is the case it
 * starts the transmission of the next packet.
 */

void
end_packet_transmission_event(Simulation_Run_Ptr simulation_run, void * link)
{
    Simulation_Run_Data_Ptr data;
    Packet_Ptr this_packet, next_packet;

    TRACE(printf("End Of Packet.\n"););

    data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);

    /*
    * Packet transmission is finished. Take the packet off the data link.
    */

    this_packet = (Packet_Ptr) server_get(link);

    /* Collect statistics. */
    if(this_packet->sourceSwitch == 1 && this_packet->transfer == 0){
        this_packet->service_time = get_packet_transmission_time_23();
        this_packet->transfer = 1;

        data->number_of_packets_processed++;

        double p13 = 0.5;
        double random = (double)rand()/(double)RAND_MAX;


        // Determine using probability if the packet is to be sent to Switch 2 or 3
        if(random <= p13){
            data->arrival_count3++;
            if (server_state(data->link3) == BUSY) {
                fifoqueue_put(data->buffer3, (void*) this_packet);
            } else {
                data->number_of_packets_processed3++;
                start_transmission_on_link(simulation_run, this_packet, data->link3);
            }
        } else {
            data->arrival_count2++;
            if (server_state(data->link2) == BUSY) {
                fifoqueue_put(data->buffer2, (void*) this_packet);
            } else {
                data->number_of_packets_processed2++;
                start_transmission_on_link(simulation_run, this_packet, data->link2);
            }
        }
    } else if (this_packet->sourceSwitch == 1 && this_packet->transfer == 1){
        data->accumulated_delay += simulation_run_get_time(simulation_run) - this_packet->arrive_time;
        output_progress_msg_to_screen(simulation_run);
        xfree((void *) this_packet);
        /*if(fifoqueue_size(data->buffer) > 0) {
            next_packet = (Packet_Ptr) fifoqueue_get(data->buffer);
            start_transmission_on_link(simulation_run, next_packet, link);
        }*/
    } else if (this_packet->sourceSwitch == 2){
        data->accumulated_delay2 += simulation_run_get_time(simulation_run) - this_packet->arrive_time;
        data->number_of_packets_processed2++;
        output_progress_msg_to_screen(simulation_run);
        xfree((void *) this_packet);
        if(fifoqueue_size(data->buffer2) > 0) {
            next_packet = (Packet_Ptr) fifoqueue_get(data->buffer2);
            start_transmission_on_link(simulation_run, next_packet, link);
        }
    } else {
        data->accumulated_delay3 += simulation_run_get_time(simulation_run) - this_packet->arrive_time;
        data->number_of_packets_processed3++;
        output_progress_msg_to_screen(simulation_run);
        xfree((void *) this_packet);
        if(fifoqueue_size(data->buffer3) > 0) {
            next_packet = (Packet_Ptr) fifoqueue_get(data->buffer3);
            start_transmission_on_link(simulation_run, next_packet, link);
        }
    }
}

/*
 * This function ititiates the transmission of the packet passed to the
 * function. This is done by placing the packet in the server. The packet
 * transmission end event for this packet is then scheduled.
 */

void
start_transmission_on_link(Simulation_Run_Ptr simulation_run,
			   Packet_Ptr this_packet,
			   Server_Ptr link)
{
  TRACE(printf("Start Of Packet.\n");)

  server_put(link, (void*) this_packet);
  this_packet->status = XMTTING;

  /* Schedule the end of packet transmission event. */
  schedule_end_packet_transmission_event(simulation_run,
	 simulation_run_get_time(simulation_run) + this_packet->service_time,
	 (void *) link);
}

/*
 * Get a packet transmission time. For now it is a fixed value defined in
 * simparameters.h
 */

double
get_packet_transmission_time(void)
{
    return ((double) PACKET_XMT_TIME);
}

double
get_packet_transmission_time_23(void)
{
    return ((double) PACKET_XMT_TIME_23);
}

