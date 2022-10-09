
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

#ifndef _PACKET_TRANSMISSION_H_
#define _PACKET_TRANSMISSION_H_

/******************************************************************************/

#include "main.h"

/******************************************************************************/

/*
 * Function prototypes
 */

void
start_transmission_on_link(Simulation_Run_Ptr, Packet_Ptr, Server_Ptr);

void
start_transmission_on_link2(Simulation_Run_Ptr, Packet_Ptr, Server_Ptr);

void
start_transmission_on_link3(Simulation_Run_Ptr, Packet_Ptr, Server_Ptr);

void
start_transmission_on_link12(Simulation_Run_Ptr, Packet_Ptr, Server_Ptr);

void
start_transmission_on_link13(Simulation_Run_Ptr, Packet_Ptr, Server_Ptr);

void
end_packet_transmission_event_wireless(Simulation_Run_Ptr, void*);

void
end_packet_transmission_event_wireless12(Simulation_Run_Ptr, void*);

void
end_packet_transmission_event_wireless13(Simulation_Run_Ptr, void*);

void
end_packet_transmission_event_2(Simulation_Run_Ptr, void*);

void
end_packet_transmission_event_3(Simulation_Run_Ptr, void*);

double
get_packet_transmission_time(void);

/******************************************************************************/

#endif /* packet_transmission.h */



