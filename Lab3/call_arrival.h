
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

#ifndef _CALL_ARRIVAL_H_
#define _CALL_ARRIVAL_H_

/*******************************************************************************/

#include "main.h"

/*******************************************************************************/

/*
 *
 * Function prototypes
 *
 */

Server_Ptr
get_free_channel(Simulation_Run_Ptr, int);

void
call_arrival_event(Simulation_Run_Ptr, void *);

long int
schedule_call_arrival_event(Simulation_Run_Ptr, double);

/*******************************************************************************/

#endif /* call_arrival.h */






