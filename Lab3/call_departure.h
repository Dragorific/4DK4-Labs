
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

#ifndef _Call_DEPARTURE_H_
#define _Call_DEPARTURE_H_

/*******************************************************************************/

/*
 *
 * Function prototypes
 *
 */

void
end_call_on_channel_event(Simulation_Run_Ptr ThisSimulation_Run, void*);

long int
schedule_end_call_on_channel_event(Simulation_Run_Ptr, double, void*);

void
end_call_on_queue_event(Simulation_Run_Ptr ThisSimulation_Run, long int);

long int
schedule_end_call_on_queue_event(Simulation_Run_Ptr, double, long int);

/*******************************************************************************/

#endif /* call_departure.h */



