
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

#include <stdio.h>
#include <math.h>
#include "trace.h"
#include "output.h"
#include "channel.h"
#include "packet_transmission.h"

/*******************************************************************************/

long int
schedule_transmission_start_event(Simulation_Run_Ptr simulation_run,
				  Time event_time,
				  void * packet)
{
  Event event;

  event.description = "Start Of Packet";
  event.function = transmission_start_event;
  event.attachment = packet;

  return simulation_run_schedule_event(simulation_run, event, event_time);
}

/*******************************************************************************/

void
transmission_start_event(Simulation_Run_Ptr simulation_run, void * ptr)
{
    Packet_Ptr this_packet;
    Simulation_Run_Data_Ptr data;
    Time now;
    Channel_Ptr channel;
    double next_slot;

    this_packet = (Packet_Ptr) ptr;
    data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);
    channel = data->channel;
    now = simulation_run_get_time(simulation_run);

    if(get_channel_state(channel) != IDLE) {
        /* The channel is now colliding. Schedule the transmission in the next slot to simulate a packet "waiting".*/
        next_slot = channel->arrive_time + MEAN_SLOT_DURATION + 2*epsilon;
        channel->arrive_time = next_slot;
        channel->service_time = this_packet->service_time;
        this_packet->collision_count++;

        schedule_transmission_start_event(simulation_run,
                                          next_slot,
                                          (void *) this_packet);

    } else {
        /* This packet is starting to transmit. */
        increment_transmitting_stn_count(channel);
        this_packet->status = TRANSMITTING;

        /* The channel is successful, for now. */
        set_channel_state(channel, SUCCESS);
        channel->arrive_time = now;
        channel->service_time = this_packet->service_time;

        /* Schedule the end of packet transmission event. */
        schedule_transmission_end_event(simulation_run,
                                        now + this_packet->service_time,
                                        (void *) this_packet);
    }
}

/*******************************************************************************/

long int
schedule_transmission_end_event(Simulation_Run_Ptr simulation_run,
				Time event_time,
				void * packet)
{
    Event event;

    event.description = "End of Packet";
    event.function = transmission_end_event;
    event.attachment = packet;

    return simulation_run_schedule_event(simulation_run, event, event_time);
}

/*******************************************************************************/

void
transmission_end_event(Simulation_Run_Ptr simulation_run, void * packet)
{
    Packet_Ptr this_packet, next_packet;
    Buffer_Ptr buffer;
    Time now;
    Simulation_Run_Data_Ptr data;
    Channel_Ptr channel;

    data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);
    channel = data->channel;
    now = simulation_run_get_time(simulation_run);
    this_packet = (Packet_Ptr) packet;
    buffer = (data->stations+this_packet->station_id)->buffer;

    /* This station has stopped transmitting. */
    decrement_transmitting_stn_count(channel);

    /* Transmission was a success. The channel is now IDLE. */
    set_channel_state(channel, IDLE);

    TRACE(printf("Success.\n"););

    // Schedule the transmission on the data channel
    schedule_transmission_queue_event(simulation_run,
                                      now,
                                      (void*) this_packet);

    // Take out the packet from the station buffer
    fifoqueue_get(buffer);

    /* See if there is another packet at this station. If so, enable
    it for transmission. We will transmit immediately. */
    if(fifoqueue_size(buffer) > 0) {
        next_packet = fifoqueue_see_front(buffer);

        schedule_transmission_start_event(simulation_run,
                    now + epsilon,
                    (void*) next_packet);
    }
}

/*******************************************************************************/

long int
schedule_transmission_queue_event(Simulation_Run_Ptr simulation_run,
                                  Time event_time,
                                  void * packet)
{
    Event event;

    event.description = "Packet on Data Channel";
    event.function = transmission_queue_event;
    event.attachment = packet;

    return simulation_run_schedule_event(simulation_run, event, event_time);
}

/*******************************************************************************/

void
transmission_queue_event(Simulation_Run_Ptr simulation_run, void * packet)
{
    Packet_Ptr this_packet;
    Channel_Ptr channel;
    Time now;
    Simulation_Run_Data_Ptr data;

    this_packet = (Packet_Ptr) packet;
    data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);
    channel = data->data_channel;
    now = simulation_run_get_time(simulation_run);

    if(get_channel_state(channel) != IDLE) {
        /* When the channel is being used, queue the packet for FCFS.*/
        fifoqueue_put(data->buffer, (void*) this_packet);

    } else {
        /* This packet is starting to transmit. */
        increment_transmitting_stn_count(channel);
        this_packet->status = TRANSMITTING;

        /* The channel is successful, for now. */
        set_channel_state(channel, SUCCESS);

        /* Schedule the end of packet transmission event. */
        schedule_transmission_queue_end_event(simulation_run,
                                        now + this_packet->service_time,
                                        (void *) this_packet);
    }
}

/*******************************************************************************/

long int
schedule_transmission_queue_end_event(Simulation_Run_Ptr simulation_run,
                                  Time event_time,
                                  void * packet)
{
    Event event;

    event.description = "Packet on Data Channel";
    event.function = transmission_queue_end_event;
    event.attachment = packet;

    return simulation_run_schedule_event(simulation_run, event, event_time);
}

/*******************************************************************************/

void
transmission_queue_end_event(Simulation_Run_Ptr simulation_run, void * packet)
{
    Packet_Ptr this_packet, next_packet;
    Buffer_Ptr buffer;
    Time now;
    Simulation_Run_Data_Ptr data;
    Channel_Ptr channel;

    data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);
    channel = data->data_channel;
    now = simulation_run_get_time(simulation_run);
    this_packet = (Packet_Ptr) packet;
    buffer = data->buffer;

    data->number_of_packets_processed++;

    /* This station has stopped transmitting. */
    decrement_transmitting_stn_count(channel);

    /* Transmission was a success. The channel is now IDLE. */
    set_channel_state(channel, IDLE);/* This station has stopped transmitting. */

    TRACE(printf("Success.\n"););

    /* Collect statistics. */
    (data->stations+this_packet->station_id)->packet_count++;
    (data->stations+this_packet->station_id)->accumulated_delay += now - this_packet->arrive_time;

    data->number_of_collisions += this_packet->collision_count;
    data->accumulated_delay += now - this_packet->arrive_time;

    output_blip_to_screen(simulation_run);

    /* This packet is done. */
    free((void*) fifoqueue_get(buffer));

    /* See if there is another packet at this station. If so, enable
    it for transmission. We will transmit immediately. */
    if(fifoqueue_size(buffer) > 0) {
        next_packet = fifoqueue_see_front(buffer);

        schedule_transmission_queue_event(simulation_run,
                                          now,
                                          (void*) next_packet);
    }
}




