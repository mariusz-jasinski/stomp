/*
 * Copyright 2013 Evgeni Dobrev <evgeni_dobrev@developer.bg>
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
#ifndef FRAME_H
#define FRAME_H

#include <stddef.h>
#include "stomp.h"

enum read_state {
	RS_INIT,
	RS_CMD,
	RS_HDR,
	RS_HDR_ESC,
	RS_BODY,
	RS_DONE,
	RS_ERR
};

/** Structer used for pointers set in frame buffer (buf) with offsets*/
struct frame_hdr {
	ptrdiff_t key_offset;
	size_t key_len;
	ptrdiff_t val_offset;
	size_t val_len;
};


/** Structer _frame used for keeping
    data in buffer _buf like command, headers, stomp headres, body, etc */
struct _frame {
	void *buf;           /* data buffer */
	size_t buf_len;      /* length of data in buf in bytes */
	size_t buf_capacity; /* allocated size in bytes */

	ptrdiff_t cmd_offset; /* offset in buff to the start of the cmd string */
	size_t cmd_len;       /* length of cmd string in bytes */

	struct frame_hdr *hdrs; /* array of struct frame_hdr elements */
	size_t hdrs_len;        /* number of elements in the array */
	size_t hdrs_capacity;   /* allocated number of struct frame_hdr elements */

	struct stomp_hdr *stomp_hdrs; /* array of struct stomp_hdr elements */
	size_t stomp_hdrs_len;        /* number of elements in the array */
	size_t stomp_hdrs_capacity;   /* allocated number of struct stomp_hdr elements */

	ptrdiff_t body_offset; /* offset in buff to the start of the body */
	size_t body_len;       /* length of body in bytes */

	enum read_state read_state; /* current state of the frame reading state mashine */
	ptrdiff_t tmp_offset;       /* current position within buf while reading an incomming frame */
	size_t tmp_len;             /* amount of bytes read while reading an incomming frame */
};

/* number of bytes to increase session->buf by
 * when adding more data to the frame */
#define BUFINCLEN 512

/* number of struct stomp_hdr structures to add to session->hdrs 
 * when adding more data to the frame */
#define HDRINCLEN 4

typedef struct _frame frame_t;

static int parse_content_length(const char *s, size_t *len);

/** Creates new frame object */
frame_t *frame_new();

/** Destroys frame memory */
void frame_free(frame_t *f);

/** Clears frame memory, by filling it by zeros till capacity */
void frame_reset(frame_t *f);

/** Returns length of buffer data, interpreting '\r', '\n', ':', '\\' as 2 bytes */ 
static size_t buflene(const void *data, size_t len);

/** Realocates frame memory (buf) by BUFINCLEN ,
    returns place where data ends in buffer */
static void *frame_alloc(frame_t *f, size_t len);

/** Appends data to frame buffer
    returns place where data ends in buffer */
static void *frame_bufcat(frame_t *f, const void *data, size_t len);

/** Appends data to frame buffer considering '\r', '\n', ':', '\\'
    returns place where data ends in buffer */
static void *frame_bufcate(frame_t *f, const void *data, size_t len);

/** Sets frame command, adds command to frame buffer,
    returns 0 if succeeds -1 in otherwise -1 */
int frame_cmd_set(frame_t *f, const char *cmd);

/** Adds 1 header to header array identified with key and val
    returns 0 if succeeds -1 in otherwise -1 */
int frame_hdr_add(frame_t *f, const char *key, const char *val);

/** Adds several (hdrc) headers to header array
    returns 0 if succeeds -1 in otherwise -1 */
int frame_hdrs_add(frame_t *f, size_t hdrc, const struct stomp_hdr *hdrs);

/** Adds body to frame buffer
    returns 0 if succeeds -1 in otherwise -1  */
int frame_body_set(frame_t *f, const void *body, size_t len);

/** Writes frame f to stream fd,
    returns number of bytes written to stream */
ssize_t frame_write(int fd, frame_t *f);

/** Reads 1 char c, sets state RS_DONE if \0
    returns read state */
static enum read_state frame_read_body(frame_t *f, char c);

/** Reads 1 char c, sets state RS_CMD if C, E, R, M or RS_DONE if \n
    returns read state */
static enum read_state frame_read_init(frame_t *f, char c);

/** Reads 1 char c, sets command offset and state RS_HDR when command ends
    returns read state */
static enum read_state frame_read_cmd(frame_t *f, char c);

/** Reads 1 char c, allocates frame header and sets key and value offsets 
    sets RS_BODY or RS_HDR_ESC when header ends, returns read state */
static enum read_state frame_read_hdr(frame_t *f, char c);

/** Reads 1 char c, sets RS_HDR
    returns read state */
static enum read_state frame_read_hdr_esc(frame_t *f, char c);

/** Reads from stream fd to frame f, depending read state 
    returns 0 if succeeds -1 in otherwise -1  */
int frame_read(int fd, frame_t *f);

/** Gets command to cmd pointer, returns commands length */
size_t frame_cmd_get(frame_t *f, const char **cmd);

/** Gets stomp structer to hdrs pointer, returns stomp header length */
size_t frame_hdrs_get(frame_t *f, const struct stomp_hdr **hdrs, int *err);

/** Gets body to body pointer, returns body length */
size_t frame_body_get(frame_t *f, const void **body);



#endif /* FRAME_H */
