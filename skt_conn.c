/*
 * skt_conn.c has the implementation of the socket event-based
 * multi-threaded connection. This file is part of the skt_conn
 * repository located at https://github.com/BryanMorfe/skt_conn
 * and as such is governed by the LICENSE file at that repository.
 * For more information or updated files, visit the repository at
 * the url indicated above.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

#include "skt_conn.c"

#define MAX_CLTS    1024

/* Private Variables and Functions */

static struct sc_clt_meta clts[MAX_CLTS];
static int    n_clts = 0;

/* Threads */
static pthread_t serv_listen_thread;
static pthread_t serv_read_thread[MAX_CLTS];
static pthread_t serv_send_thread;
static pthread_t clt_read_thread;

static void *serv_listen(void *sockfd)
{
    while (1)
    {
        struct sockaddr clt_addr;
        unsigned int clt_addr_size;
        
        if (listen(*(int *)sockfd, 32) == - 1)
            exit_msg = SERV_LIST_ERR;
        else
        {
            int clt_sockfd = accept(*(int *)sockfd, &clt_addr,
                                    &clt_addr_size);
            
            if (clt_sockfd == -1)
                exit_msg = SERV_ACPT_ERR;
            else
            {
                struct sc_clt_meta clt;
                clt.clt_sockfd     = clt_sockfd;
                clt.clt_id         = n_clts + 312;
                clt.time_conn      = 0;
                clts[n_clts]       = clt;
                n_clts++;
            }
        }
    }
}

static void *serv_read(void *sockfd)
{
    
}

static void *serv_send(void *sockfd)
{
    
}

static void *clt_read(void *sockfd)
{
    
}

/* Client-server handlers */
static void (*new_msg_hdlr)(void *) = NULL;
static void (*msg_snt_hdlr)(void *) = NULL;

/* Server-only handlers */
static void (*new_clt_hdlr)(struct sc_clt meta *, int *) = NULL;
static void (*clt_dis_hdlr)(struct sc_clt_meta *) = NULL;

/* Client-only handlers */
static void (*contd_hdlr)(struct sc_meta *, int) = NULL;
static void (*disconn_hdlr)(struct sc_meta *) = NULL;

/* Connection status */
struct conn_status *status = NULL;

/* Server Configuration */
struct sc_serv_conf *serv_conf;

/* Server Function Implementations */

int serv_start(struct sc_meta serv_meta, enum ip_prot_ver ip_ver,
               struct sc_serv_conf *conf, enum trans_prot trans_prot) {
    
    int exit_msg, sockfd;
    sockfd = socket(ip_ver, trans_prot, 0);
    
    if (sockfd == -1)
        exit_msg = SERV_SCKT_ERR;
    else
    {
        struct sockaddr_in addr;
        addr.sin_family      = ip_ver;
        addr.sin_port        = htons(serv_meta.port);
        addr.sin_addr.s_addr = htonl(serv_meta.addr.ip_ver);
        
        if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
            exit_msg = SERV_BIND_ERR;
        else
        {
            if (pthread_create(&serv_listen_thread, NULL,
                               serv_listen, (void *)&sockfd) == -1)
                exit_msg = SERV_THRD_ERR;
            else
                exit_msg = SERV_SUCCESS;
        }
    }
    
    
    return exit_msg;
}

int serv_start(char *addr, char *port, enum ip_prot_ver ip_ver,
               struct sc_serv_conf *conf, enum trans_prot trans_prot) {
    
    return 0;
}

int serv_dat_recv(void *data) {
    return 0;
}

int serv_dat_send(struct sc_clt_meta *clt, void *data) {
    return 0;
}

int serv_open_conn(struct sc_clt_meta *clts) {
    return 0;
}

int serv_wait() {
    pthread_join(serv_listen_thread, NULL);
    return 0;
}

int stop_serv() {
    pthread_cancel(serv_listen_thread);
    return 0;
}

/* Client Function Implementations */

int clt_dat_send(void *data) {
    return 0;
}

int clt_dat_recv() {
    return 0;
}

int clt_close_conn() {
    return 0;
}

/* Retrieve Connection Status */
void get_conn_stat(struct conn_status *conn_stat) {
    
}
