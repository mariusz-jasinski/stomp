/*
 * Copyright 2013 Evgeni Dobrev <evgeni_dobrev@developer.bg>
 * Changes 2018 by Asseco Poland SA
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <time.h>
#include <stdio.h>
#include "debug.h"

int glob_debug_level = DEBUG_LEVEL_NONE;

void debug_set_level(int glob_debug_level_in)
{
  glob_debug_level = glob_debug_level_in;
}

int debug_get_interval_inns(struct timespec *start, struct timespec *end, const char *desc)
{
	int elapsed = (end->tv_sec - start->tv_sec) * 1000000000 +  (end->tv_nsec - start->tv_nsec);
  return elapsed;
}

int debug_get_interval_inms(struct timespec *start, struct timespec *end, const char *desc)
{
	int elapsed = (end->tv_sec - start->tv_sec) * 1000 +  (end->tv_nsec - start->tv_nsec)/ 1000000;
  return elapsed;
}
