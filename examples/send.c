#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <stomp.h>
#include <debug.h>
#include <time.h>
#include <cmath>

struct ctx {
	const char *destination;
};

static void dump_hdrs(int hdrc, const struct stomp_hdr *hdrs)
{
	int i;
	for (i=0; i < hdrc; i++) {
		fprintf(stdout, "%s:%s\n", hdrs[i].key, hdrs[i].val);
	}
}

static void _connected(stomp_session_t *s, void *ctx, void *session_ctx)
{
	struct ctx *c = (struct ctx *)session_ctx;
	struct stomp_hdr hdrs[] = {
		{"destination", c->destination},
	};

	int err = stomp_subscribe(s, sizeof(hdrs)/sizeof(struct stomp_hdr), hdrs);
	if (err<0) {
		perror("stomp");
	}
}

static void _message(stomp_session_t *s, void *ctx, void *session_ctx)
{
	struct stomp_ctx_message *e = (stomp_ctx_message *)ctx;
	dump_hdrs(e->hdrc, e->hdrs);
	fprintf(stdout, "message: %s\n", (const char *)e->body);
}

static void _error(stomp_session_t *session, void *ctx, void *session_ctx)
{
	struct stomp_ctx_error *e = (struct stomp_ctx_error *)ctx;
	dump_hdrs(e->hdrc, e->hdrs);
	fprintf(stderr, "err: %s\n", (const char *)e->body);
}

static void _receipt(stomp_session_t *s, void *ctx, void *session_ctx)
{
	struct stomp_ctx_receipt *e = (struct stomp_ctx_receipt *)ctx;
	dump_hdrs(e->hdrc, e->hdrs);
	fprintf(stdout, "receipt: \n");
}

static void _user(stomp_session_t *s, void *ctx, void *session_ctx) 
{
}

char *read_body_from_file(const char *fileName)
{
  FILE * pFile;
  long lSize;
  char * buffer;
  size_t result;

  pFile = fopen ( fileName , "rb" );
  if (pFile==NULL) {
    fprintf(stderr, "File error (read_body_from_file): \n");
    return NULL;
  }

  // obtain file size:
  fseek (pFile , 0 , SEEK_END);
  lSize = ftell (pFile);
  rewind (pFile);

  // allocate memory to contain the whole file:
  buffer = (char*) malloc (sizeof(char)*lSize);
  if (buffer == NULL) {
    fprintf(stderr, "Memory error (read_body_from_file): \n");
    return NULL;
  }

  // copy the file into the buffer:
  result = fread (buffer,1,lSize,pFile);
  if (result != lSize) {
    fprintf(stderr, "Reading error (read_body_from_file): \n");
    return NULL;
  }

  /* the whole file is now loaded in the memory buffer. */
  // terminate
  fclose (pFile);
  return buffer;
}

int main(int argc, char *argv[]) 
{
	int err,debug=1,isfile=0;
	struct ctx c;
  char host[256]="", port[256]="", paramBody[256] ="", fileName[256]="", *body = NULL;  
	stomp_session_t *s;
  int loopnumber = 1;
  
	struct stomp_hdr hdrs[] = {
		{"login", "guest"},
		{"passcode", "guest"},
		{"accept-version", "1.2"},
		{"heart-beat", "1000,1000"},
	};

  debug_set_level(1);
  
	if (argc != 3 && argc != 9) {
    fprintf(stderr, "usage: %s <destination> <body> (host) (port) (debug) (loopnumber) (isfile) (file) \n", argv[0]);
		exit(EXIT_FAILURE);
	}


  if (argc == 3){
    c.destination = argv[1];
    strcpy(paramBody,argv[2]);
    strcpy(host,"10.90.99.51");
    strcpy(port,"61613");
    debug = 1;
    loopnumber = 1;
    isfile = 0;
  }
  else  if (argc == 9){
    c.destination = argv[1];
    strcpy(paramBody,argv[2]);
    strcpy(host,argv[3]);
    strcpy(port,argv[4]);
    debug = atoi(argv[5]);
    loopnumber = atoi(argv[6]);
    isfile = atoi(argv[7]);
    strcpy(fileName,argv[8]);
  }
  
  debug_set_level(debug);

  if(isfile){
    body = read_body_from_file(fileName);
    if(body == NULL){
      exit(EXIT_FAILURE);
    }
  }
  else{
    body = paramBody;
  }
    
  
	s = stomp_session_new(&c);
	if (!s) {
		perror("stomp");
		exit(EXIT_FAILURE);
	}

	stomp_callback_set(s, SCB_CONNECTED, _connected);
	stomp_callback_set(s, SCB_ERROR, _error);
	stomp_callback_set(s, SCB_MESSAGE, _message);
	stomp_callback_set(s, SCB_RECEIPT, _receipt);
	stomp_callback_set(s, SCB_USER, _user);

  struct timespec begin;
  struct timespec after_connect;
  struct timespec after_send;
  struct timespec after_disconnect;
  
  int sum_connect = 0;
  int sum_send = 0;
  int sum_disconnect = 0;

  for(int i = 0; i<loopnumber; i++){
    
    clock_gettime(CLOCK_REALTIME, &begin);
    err = stomp_connect(s,host, port, sizeof(hdrs)/sizeof(struct stomp_hdr), hdrs);
    if (err) {
      perror("stomp");
      if(body !=NULL) delete body;
      stomp_session_free(s);
      exit(EXIT_FAILURE);
    }

    clock_gettime(CLOCK_REALTIME, &after_connect);
    sum_connect += debug_get_interval_inns(&begin, &after_connect, "CONNECT INTERVAL(ms)");
    
    struct stomp_hdr hdrs2[] = {
      {"destination", c.destination},
    };   

    err = stomp_send(s, sizeof(hdrs2)/sizeof(struct stomp_hdr), hdrs2, body, strlen(body));
    if (err) {
      perror("stomp");
      if(body !=NULL) delete body;
      stomp_session_free(s);
      exit(EXIT_FAILURE);
    }

    clock_gettime(CLOCK_REALTIME, &after_send);
    sum_send += debug_get_interval_inns(&after_connect, &after_send, "SEND INTERVAL(ms)");
    
    struct stomp_hdr hdrs3[] = {
      {"receipt", "77"},
    };   

    err = stomp_disconnect(s, sizeof(hdrs3)/sizeof(struct stomp_hdr), hdrs3);
    if (err) {
      perror("stomp");
      if(body !=NULL) delete body;
      stomp_session_free(s);
      exit(EXIT_FAILURE);
    }

    clock_gettime(CLOCK_REALTIME, &after_disconnect);
    sum_disconnect += debug_get_interval_inns(&after_send, &after_disconnect, "DISCONNECT INTERVAL(ms)");
  }

  STOMP_PRINT_DESC_INT("CONNECT INTERVAL(ns)", sum_connect/loopnumber)
  STOMP_PRINT_DESC_INT("CONNECT INTERVAL(ms)", (int)(ceil((double)sum_connect)/(1000000*loopnumber)))
  
  STOMP_PRINT_DESC_INT("SEND INTERVAL(ns)", sum_send/loopnumber)
  STOMP_PRINT_DESC_INT("SEND INTERVAL(ms)", (int)(ceil((double)sum_send)/(1000000*loopnumber)))
  
  STOMP_PRINT_DESC_INT("DISCONNECT INTERVAL(ns)", sum_disconnect/loopnumber)
  STOMP_PRINT_DESC_INT("DISCONNECT INTERVAL(ms)", (int)(ceil((double)sum_disconnect)/(1000000*loopnumber)))
  
  if(body !=NULL) delete body;
	stomp_session_free(s);

	exit(EXIT_SUCCESS);
}



