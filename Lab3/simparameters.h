
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

#ifndef _SIMPARAMETERS_H_
#define _SIMPARAMETERS_H_

/*******************************************************************************/

#define List_ARRIVALRATE 3              /* calls/minute */
#define MEAN_CALL_DURATION 2, 4, 6, 8, 10   /* minutes */ //DONT CHANGE
#define RUNLENGTH 5e6                   /* number of successful calls */
#define BLIPRATE 1e3
#define LIST_NUMBER_OF_CHANNELS 6, 8, 10, 12, 14
#define t 5
// DO, 2, 3, 4
#define MEAN_QUEUE_DURATION 2 /* minutes */ // FOR EACH OF THIS VALUE, MAKE A SEPARATE GRAPH/DATA SET

/* Comma separated list of random seeds to run. */
#define RANDOM_SEED_LIST 400167784

/*******************************************************************************/

#endif /* simparameters.h */




