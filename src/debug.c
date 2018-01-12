/*
 * Copyright 2018 Asseco Poland SA
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

int glob_debug_level = 0;

void debug_set_level(int glob_debug_level_in)
{
  glob_debug_level = glob_debug_level_in;
}

int debug_get_interval(struct timespec *start, struct timespec *end, const char *desc)
{
	int elapsed = (end->tv_sec - start->tv_sec) * 1000000000 +  (end->tv_nsec - start->tv_nsec);
  //STOMP_PRINT_DESC_INT(desc, elapsed)
  return elapsed;
}

