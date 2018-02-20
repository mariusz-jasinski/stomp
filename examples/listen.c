#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <stomp.h>
#include <debug.h>
#include <unistd.h>

struct ctx {
	const char *destination;
};

void dump_hdrs(int hdrc, const struct stomp_hdr *hdrs)
{
	int i;
	for (i=0; i < hdrc; i++) {
		fprintf(stdout, "%s:%s\n", hdrs[i].key, hdrs[i].val);
	}
}

void _connected(stomp_session_t *s, void *ctx, void *session_ctx)
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

void _message(stomp_session_t *s, void *ctx, void *session_ctx)
{
	struct stomp_ctx_message *e = (stomp_ctx_message *)ctx;
	dump_hdrs(e->hdrc, e->hdrs);
	fprintf(stdout, "message: %s\n", (const char *)e->body);
}

void _error(stomp_session_t *session, void *ctx, void *session_ctx)
{
	struct stomp_ctx_error *e = (struct stomp_ctx_error *)ctx;
	dump_hdrs(e->hdrc, e->hdrs);
	fprintf(stderr, "err: %s\n", (const char *)e->body);
}

void _receipt(stomp_session_t *s, void *ctx, void *session_ctx)
{
	struct stomp_ctx_receipt *e = (struct stomp_ctx_receipt *)ctx;
	dump_hdrs(e->hdrc, e->hdrs);
	fprintf(stdout, "receipt: \n");
}

void _user(stomp_session_t *s, void *ctx, void *session_ctx) 
{
}

int main(int argc, char *argv[]) 
{
	int err;
  char host[256]="",port[256]="";
	struct ctx c;
	stomp_session_t *s;
	struct stomp_hdr hdrs[] = {
		{"login", "guest"},
		{"passcode", "guest"},
		{"accept-version", "1.2"},
		{"heart-beat", "1000,1000"},
	};

	if (argc != 2 && argc != 4) {
    fprintf(stderr, "usage: %s <destination> (host) (port) \n", argv[0]);
		exit(EXIT_FAILURE);
	}

  debug_set_level(1);
  if (argc == 2){
    c.destination = argv[1];
    strcpy(host,"10.90.99.116");
    strcpy(port,"61613");
  }
  else  if (argc == 4){
    c.destination = argv[1];
    strcpy(host,argv[2]);
    strcpy(port,argv[3]);
  }
  
	s = stomp_session_new(&c);
	if (!s) {
		perror("stomp");
		exit(EXIT_FAILURE);
	}
/*
	stomp_callback_set(s, SCB_CONNECTED, _connected);
	stomp_callback_set(s, SCB_ERROR, _error);
	stomp_callback_set(s, SCB_MESSAGE, _message);
	stomp_callback_set(s, SCB_RECEIPT, _receipt);
  */
	stomp_callback_set(s, SCB_USER, _user);


	err = stomp_connect(s, host, port, sizeof(hdrs)/sizeof(struct stomp_hdr), hdrs);
	if (err) {
		perror("stomp");
		stomp_session_free(s);
		exit(EXIT_FAILURE);
	}
  
	err = stomp_run_register(s);
	if (err) {
		perror("stomp");
		stomp_session_free(s);
		exit(EXIT_FAILURE);
	}

	stomp_session_free(s);

	exit(EXIT_SUCCESS);
}



