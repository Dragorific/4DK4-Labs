
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
schedule_end_packet_transmission_event_wireless(Simulation_Run_Ptr simulation_run,
				       double event_time,
				       Server_Ptr link)
{
  Event event;

  event.description = "Packet Xmt End, transfer to Switch 2 or 3";
  event.function = end_packet_transmission_event_wireless;
  event.attachment = (void *) link;

  return simulation_run_schedule_event(simulation_run, event, event_time);
}

long
schedule_end_packet_transmission_event_wireless12(Simulation_Run_Ptr simulation_run,
				       double event_time,
				       Server_Ptr link)
{
  Event event;

  event.description = "Packet Xmt End, transfer from Switch 1 to 2";
  event.function = end_packet_transmission_event_wireless12;
  event.attachment = (void *) link;

  return simulation_run_schedule_event(simulation_run, event, event_time);
}

long
schedule_end_packet_transmission_event_wireless13(Simulation_Run_Ptr simulation_run,
				       double event_time,
				       Server_Ptr link)
{
  Event event;

  event.description = "Packet Xmt End, transfer from Switch 1 to 3";
  event.function = end_packet_transmission_event_wireless13;
  event.attachment = (void *) link;

  return simulation_run_schedule_event(simulation_run, event, event_time);
}


long
schedule_end_packet_transmission_event_2(Simulation_Run_Ptr simulation_run,
				       double event_time,
				       Server_Ptr link)
{
  Event event;

  event.description = "Packet Xmt on Link 2 End";
  event.function = end_packet_transmission_event_2;
  event.attachment = (void *) link;

  return simulation_run_schedule_event(simulation_run, event, event_time);
}

long
schedule_end_packet_transmission_event_3(Simulation_Run_Ptr simulation_run,
				       double event_time,
				       Server_Ptr link)
{
  Event event;

  event.description = "Packet Xmt on Link 3 End";
  event.function = end_packet_transmission_event_3;
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
end_packet_transmission_event_wireless(Simulation_Run_Ptr simulation_run, void * link)
{
    Simulation_Run_Data_Ptr data;
    Packet_Ptr this_packet, next_packet;

    TRACE(printf("Packet being sent to Switches 2 or 3.\n"););

    data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);

    /*
    * Packet transmission is finished on Switch 1. Take the packet off the data link and send off to Switch 2 or 3.
    */
    this_packet = (Packet_Ptr) server_get(link);
    data->number_of_packets_processed++;

    //printf("Packet being sent out of Link 1\n");
    output_progress_msg_to_screen(simulation_run);

    schedule_packet_arrival_event_wireless(simulation_run, simulation_run_get_time(simulation_run), (void*) this_packet);

    if(fifoqueue_size(data->buffer) > 0) {
        next_packet = (Packet_Ptr) fifoqueue_get(data->buffer);
        start_transmission_on_link(simulation_run, next_packet, link);
    }

}

void
end_packet_transmission_event_wireless12(Simulation_Run_Ptr simulation_run, void * link)
{
    Simulation_Run_Data_Ptr data;
    Packet_Ptr this_packet, next_packet;

    TRACE(printf("End Of Packet at Switch 2 from Switch 1.\n\n"););

    data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);

    /*
    * Packet transmission is finished on Switch 2, after arriving from Switch 1.
    */

    this_packet = (Packet_Ptr) server_get(link);

    /* Collect statistics. */
    data->number_of_packets_processed2++;
    data->accumulated_delay += simulation_run_get_time(simulation_run) -
    this_packet->arrive_time;

    printf("Packet leaving Link 2 from Link 1\n");
    /* Output activity blip every so often. */
    output_progress_msg_to_screen2(simulation_run);

    /* This packet is done ... give the memory back. */
    xfree((void *) this_packet);
}

void
end_packet_transmission_event_wireless13(Simulation_Run_Ptr simulation_run, void * link)
{
    Simulation_Run_Data_Ptr data;
    Packet_Ptr this_packet, next_packet;

    TRACE(printf("End Of Packet at Switch 3 from Switch 1.\n\n"););

    data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);

    /*
    * Packet transmission is finished on Switch 3, after arriving from Switch 1.
    */

    this_packet = (Packet_Ptr) server_get(link);

    /* Collect statistics. */
    data->number_of_packets_processed3++;
    data->accumulated_delay += simulation_run_get_time(simulation_run) - this_packet->arrive_time;

    printf("Packet leaving Link 3 from Link 1\n");

    /* Output activity blip every so often. */
    output_progress_msg_to_screen3(simulation_run);

    /* This packet is done ... give the memory back. */
    xfree((void *) this_packet);
}

void
end_packet_transmission_event_2(Simulation_Run_Ptr simulation_run, void * link)
{
    Simulation_Run_Data_Ptr data;
    Packet_Ptr this_packet, next_packet;

    TRACE(printf("End Of Packet at Switch 2.\n"););

    data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);

    /*
    * Packet transmission is finished. Take the packet off the data link.
    */

    this_packet = (Packet_Ptr) server_get(link);

    /* Collect statistics. */
    data->number_of_packets_processed2++;
    data->accumulated_delay2 += simulation_run_get_time(simulation_run) - this_packet->arrive_time;

    printf("Packet leaving Link 2 \n");
    /* Output activity blip every so often. */
    output_progress_msg_to_screen2(simulation_run);

    /* This packet is done ... give the memory back. */
    xfree((void *) this_packet);

    /*
    * See if there is are packets waiting in the buffer. If so, take the next one
    * out and transmit it immediately.
    */

    if(fifoqueue_size(data->buffer2) > 0) {
        next_packet = (Packet_Ptr) fifoqueue_get(data->buffer2);
        start_transmission_on_link2(simulation_run, next_packet, link);
    }
}

void
end_packet_transmission_event_3(Simulation_Run_Ptr simulation_run, void * link)
{
    Simulation_Run_Data_Ptr data;
    Packet_Ptr this_packet, next_packet;

    TRACE(printf("End Of Packet at Switch 3.\n"););

    data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);

    /*
    * Packet transmission is finished. Take the packet off the data link.
    */

    this_packet = (Packet_Ptr) server_get(link);

    /* Collect statistics. */
    data->number_of_packets_processed3++;
    data->accumulated_delay3 += simulation_run_get_time(simulation_run) -
    this_packet->arrive_time;

    printf("Packet leaving Link 3 \n");

    /* Output activity blip every so often. */
    output_progress_msg_to_screen3(simulation_run);

    /* This packet is done ... give the memory back. */
    xfree((void *) this_packet);

    /*
    * See if there is are packets waiting in the buffer. If so, take the next one
    * out and transmit it immediately.
    */

    if(fifoqueue_size(data->buffer3) > 0) {
        next_packet = (Packet_Ptr) fifoqueue_get(data->buffer3);
        start_transmission_on_link3(simulation_run, next_packet, link);
    }
}

/*
 * This function initiates the transmission of the packet passed to the
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
  schedule_end_packet_transmission_event_wireless(simulation_run,
	 simulation_run_get_time(simulation_run) + this_packet->service_time,
	 (void *) link);
}

void
start_transmission_on_link2(Simulation_Run_Ptr simulation_run,
			   Packet_Ptr this_packet,
			   Server_Ptr link)
{
  TRACE(printf("Start Of Packet.\n");)
  printf("Transmitting on 2.\n");

  server_put(link, (void*) this_packet);
  this_packet->status = XMTTING;

  /* Schedule the end of packet transmission event. */
  schedule_end_packet_transmission_event_2(simulation_run,
	 simulation_run_get_time(simulation_run) + this_packet->service_time,
	 (void *) link);
}

void
start_transmission_on_link3(Simulation_Run_Ptr simulation_run,
			   Packet_Ptr this_packet,
			   Server_Ptr link)
{
  TRACE(printf("Start Of Packet.\n");)
  printf("Transmitting on 3.\n");

  server_put(link, (void*) this_packet);
  this_packet->status = XMTTING;

  /* Schedule the end of packet transmission event. */
  schedule_end_packet_transmission_event_3(simulation_run,
	 simulation_run_get_time(simulation_run) + this_packet->service_time,
	 (void *) link);
}

void
start_transmission_on_link12(Simulation_Run_Ptr simulation_run,
			   Packet_Ptr this_packet,
			   Server_Ptr link)
{
  TRACE(printf("Start Of Packet.\n");)
  printf("Transmitting from 1 to 2.\n");

  server_put(link, (void*) this_packet);
  this_packet->status = XMTTING;

  /* Schedule the end of packet transmission event. */
  schedule_end_packet_transmission_event_wireless12(simulation_run,
	 simulation_run_get_time(simulation_run) + this_packet->service_time,
	 (void *) link);
}



void
start_transmission_on_link13(Simulation_Run_Ptr simulation_run,
			   Packet_Ptr this_packet,
			   Server_Ptr link)
{
  TRACE(printf("Start Of Packet.\n");)
  printf("Transmitting from 1 to 3.\n");

  server_put(link, (void*) this_packet);
  this_packet->status = XMTTING;

  /* Schedule the end of packet transmission event. */
  schedule_end_packet_transmission_event_wireless13(simulation_run,
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


