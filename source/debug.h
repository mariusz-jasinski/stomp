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
#ifndef DEBUG_H
#define DEBUG_H

#define DEBUG_LEVEL_NONE            0
#define DEBUG_LEVEL_SESSION         1
#define DEBUG_LEVEL_STOMP           2
#define DEBUG_LEVEL_STOMP_SESSION   3
#define DEBUG_LEVEL_FRAME           4

#define DEBUG_LEVEL_ALL             7

#define LEVEL_SS            0
#define LEVEL_STOMP         1
#define LEVEL_FRAME         2


extern int glob_debug_level;

/** Set debug level */
void debug_set_level(int glob_debug_level_in);

/** Get interval between end and start in nanosecounds */
int debug_get_interval_inns(struct timespec *start, struct timespec *end, const char *desc);

/** Get interval between end and start in milisecounds */
int debug_get_interval_inms(struct timespec *start, struct timespec *end, const char *desc);

#define IF_LEVEL_ENOUGH(file_level) \
  if(   glob_debug_level&DEBUG_LEVEL_FRAME         && file_level == LEVEL_FRAME \
     || glob_debug_level&DEBUG_LEVEL_STOMP         && file_level == LEVEL_STOMP \
     || glob_debug_level&DEBUG_LEVEL_SESSION       && file_level == LEVEL_SS \
  )

#define STOMP_DEBUG_FUNCTION(file_level) \
  IF_LEVEL_ENOUGH(file_level) \
  { \
    char text[256] = ""; \
    sprintf(text,"DEBUG: STOMP %8s: %d Function: %s \n",__FILE__, __LINE__, __FUNCTION__); \
    fprintf(stderr, text); \
  }

#define STOMP_DEBUG_STR(file_level, debug_text) \
  IF_LEVEL_ENOUGH(file_level) \
  { \
    char text[256] = ""; \
    sprintf(text,"DEBUG: STOMP %8s: %d Function: %s: %s\n",__FILE__, __LINE__, __FUNCTION__, debug_text); \
    fprintf(stderr, text); \
  }

#define STOMP_DEBUG_DESC_STR(file_level, desc, debug_text) \
  IF_LEVEL_ENOUGH(file_level) \
  { \
    char text[256] = ""; \
    sprintf(text,"DEBUG: STOMP %8s: %d Function: %s: %s = %s\n",__FILE__, __LINE__, __FUNCTION__, desc, debug_text); \
    fprintf(stderr, text); \
  }

#define STOMP_DEBUG_DESC_INT(file_level, desc, debug_numer) \
  IF_LEVEL_ENOUGH(file_level) \
  { \
    char text[256] = ""; \
    sprintf(text,"DEBUG: STOMP %8s: %d Function: %s: %s = %d\n",__FILE__, __LINE__, __FUNCTION__, desc, debug_numer); \
    fprintf(stderr, text); \
  }

#define STOMP_PRINT_DESC_INT(desc, debug_numer) \
  { \
    char text[256] = ""; \
    sprintf(text,"DEBUG: STOMP %8s: %d Function: %s: %s = %d\n",__FILE__, __LINE__, __FUNCTION__, desc, debug_numer); \
    fprintf(stderr, text); \
  } 

#define STOMP_DEBUG_DESC_SESSION(file_level, desc, session) \
  IF_LEVEL_ENOUGH(file_level) \
  { \
    char text[256] = ""; \
    sprintf(text,"DEBUG: STOMP %8s: %d Function: %s: %s, b_fd = %d, cl_id = %d, run = %d \n", \
      __FILE__, __LINE__, __FUNCTION__, desc, session->broker_fd, session->client_id, session->run); \
    fprintf(stderr, text); \
    STOMP_DEBUG_DESC_FRAME(file_level, "frame_in",session->frame_in) \
    STOMP_DEBUG_DESC_FRAME(file_level, "frame_out",session->frame_out) \
  }


#define STOMP_DEBUG_DESC_FRAME(file_level, desc, frame) \
  IF_LEVEL_ENOUGH(file_level) \
  { \
    char text[256] = ""; \
    sprintf(text,"DEBUG: STOMP %8s: %d Function: %s: %s, buffer = (%s,%d,%d) \n", \
      __FILE__, __LINE__, __FUNCTION__, desc, frame->buf, frame->buf_len, frame->buf_capacity); \
    fprintf(stderr, text); \
  }
  
#define STOMP_DEBUG_DESC_HDRS(file_level, desc, hdrc, hdrs) \
  IF_LEVEL_ENOUGH(file_level) \
  { \
    int id; \
    char text[256] = ""; \
    for (id=0; id < hdrc; id++) { \
      sprintf(text,"DEBUG: STOMP %8s: %d Function: %s: %s, hdr = (%s,%s) \n", \
        __FILE__, __LINE__, __FUNCTION__, desc,  hdrs[id].key, hdrs[id].val); \
      fprintf(stderr, text); \
    }  \
  }

  
#endif /* DEBUG_H */
