#include"sendrecv.h"

static pthread_t pthread_send = -1;
static pthread_t pthread_recv = -1;

static void *thread_socket_send(void *g_socket);

int send_start(data4G::Command )
{
  
}
