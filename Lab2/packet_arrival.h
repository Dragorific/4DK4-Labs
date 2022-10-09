
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

#ifndef _Packet_ARRIVAL_H_
#define _Packet_ARRIVAL_H_

/******************************************************************************/

#include "simlib.h"

/******************************************************************************/

/*
 * Function prototypes
 */

void
packet_arrival_event(Simulation_Run_Ptr, void*);

long
schedule_packet_arrival_event(Simulation_Run_Ptr, double);

void
packet_arrival_event_2(Simulation_Run_Ptr, void*);

long
schedule_packet_arrival_event_2(Simulation_Run_Ptr, double);

void
packet_arrival_event_3(Simulation_Run_Ptr, void*);

long
schedule_packet_arrival_event_3(Simulation_Run_Ptr, double);

void
packet_arrival_event_wireless(Simulation_Run_Ptr, Packet_Ptr packet, void*);

long
schedule_packet_arrival_event_wireless(Simulation_Run_Ptr, double, Packet_Ptr packet);

/******************************************************************************/

#endif /* packet_arrival.h */




