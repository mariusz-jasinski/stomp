/*
 * Copyright 
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

extern int glob_debug_level;
void debug_set_level(int glob_debug_level_in);

/** Get interval between end and start in nanosecounds */
int debug_get_interval(struct timespec *start, struct timespec *end, const char *desc);

#define STOMP_DEBUG_FUNCTION \
  if(glob_debug_level){ \
    char text[256] = ""; \
    sprintf(text,"DEBUG: STOMP %8s: %d Function: %s \n",__FILE__, __LINE__, __FUNCTION__); \
    fprintf(stderr, text); \
  }

#define STOMP_DEBUG_STR(debug_text) \
  if(glob_debug_level){ \
    char text[256] = ""; \
    sprintf(text,"DEBUG: STOMP %8s: %d Function: %s: %s\n",__FILE__, __LINE__, __FUNCTION__, debug_text); \
    fprintf(stderr, text); \
  }

#define STOMP_DEBUG_DESC_STR(desc, debug_text) \
  if(glob_debug_level){ \
    char text[256] = ""; \
    sprintf(text,"DEBUG: STOMP %8s: %d Function: %s: %s = %s\n",__FILE__, __LINE__, __FUNCTION__, desc, debug_text); \
    fprintf(stderr, text); \
  }

#define STOMP_DEBUG_DESC_INT(desc, debug_numer) \
  if(glob_debug_level){ \
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

#define STOMP_DEBUG_DESC_SESSION(desc, session) \
  if(glob_debug_level){ \
    char text[256] = ""; \
    sprintf(text,"DEBUG: STOMP %8s: %d Function: %s: %s, b_fd = %d, cl_id = %d, run = %d \n", \
      __FILE__, __LINE__, __FUNCTION__, desc, session->broker_fd, session->client_id, session->run); \
    fprintf(stderr, text); \
    STOMP_DEBUG_DESC_FRAME("frame_in",session->frame_in) \
    STOMP_DEBUG_DESC_FRAME("frame_out",session->frame_out) \
  }

#define STOMP_DEBUG_DESC_FRAME(desc, frame) \
  if(glob_debug_level){ \
    char text[256] = ""; \
    sprintf(text,"DEBUG: STOMP %8s: %d Function: %s: %s, buffer = (%s,%d,%d) \n", \
      __FILE__, __LINE__, __FUNCTION__, desc, frame->buf, frame->buf_len, frame->buf_capacity); \
    fprintf(stderr, text); \
  }
  
#define STOMP_DEBUG_DESC_HDRS(desc, hdrc, hdrs) \
  if(glob_debug_level){ \
    int id; \
    char text[256] = ""; \
    for (id=0; id < hdrc; id++) { \
      sprintf(text,"DEBUG: STOMP %8s: %d Function: %s: %s, hdr = (%s,%s) \n", \
        __FILE__, __LINE__, __FUNCTION__, desc,  hdrs[id].key, hdrs[id].val); \
      fprintf(stderr, text); \
    }  \
  }
  
#define STOMP_DEBUG_FLUSH \
  if(glob_debug_level){ \
   fflush(stderr); \
  }

  
#endif /* DEBUG_H */
