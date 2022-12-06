
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

#include "simparameters.h"
#include "call_duration.h"

/*******************************************************************************/

double get_call_duration(Simulation_Run_Ptr simulation_run)
{
    Simulation_Run_Data_Ptr sim_data;
    sim_data = simulation_run_data(simulation_run);

    return exponential_generator((double) sim_data->call_duration);
}

double get_wait_duration(Simulation_Run_Ptr simulation_run)
{
    Simulation_Run_Data_Ptr sim_data;
    sim_data = simulation_run_data(simulation_run);

    return exponential_generator((double) sim_data->queue_duration);
}



