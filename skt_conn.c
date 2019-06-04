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

static struct sc_clt_meta _clts[MAX_CLTS];
static int    n_clts = 0;

static enum sc_serv_status _serv_status = stopped;

/* Threads */
static pthread_t serv_listen_thread;
static pthread_t serv_recv_thread[MAX_CLTS];
static pthread_t serv_send_thread;
static pthread_t clt_read_thread;

static void *_serv_listen(void *sockfd)
{
    while (_serv_status == running)
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
                _clts[n_clts]      = clt;
                n_clts++;
            }
        }
    }
}

static void *_serv_recv(void *msg_vd)
{
    struct sc_msg *msg;
    void *data;
    int read_len;
    
    msg = (struct sc_msg *)msg_vd;
    data = (void *)malloc(2048);
    
    while (_serv_status == running)
    {
        while ((read_len = read(msg->src_sockfd, data, 2048)) > 0)
        {
            
        }
        
        if (new_msg_hdlr != NULL)
            
    }
}

static void *_serv_send(void *sockfd)
{
    
}

static void *_clt_read(void *sockfd)
{
    
}

/* Client-server handlers */
static void (*new_msg_hdlr)(struct sc_msg *msg) = NULL;
static void (*msg_snt_hdlr)(struct sc_msg *msg) = NULL;

/* Server-only handlers */
static void (*stat_changed_hdlr)(enum sc_serv_status status) = NULL;
static void (*new_clt_hdlr)(struct sc_clt meta *, int *) = NULL;
static void (*clt_dis_hdlr)(struct sc_clt_meta *) = NULL;

/* Client-only handlers */
static void (*contd_hdlr)(struct sc_meta *, int) = NULL;
static void (*disconn_hdlr)(struct sc_meta *) = NULL;

/* Connection status (client-only) */
struct sc_conn_status *conn_status = NULL;

/* Server Configuration */
struct sc_serv_conf *serv_conf;

/* Server Function Implementations */

int serv_start(struct sc_meta serv_meta, enum ip_prot_ver ip_ver,
               struct sc_serv_conf *conf, enum trans_prot trans_prot) {
    
    int exit_msg, sockfd;
    sockfd = socket(ip_ver, trans_prot, 0);
    
    if (sockfd == -1)
    {
        exit_msg = SERV_SCKT_ERR;
        _serv_status = failed;
        
        if (stat_changed_hdlr != NULL)
            stat_changed_hdlr(_serv_status);
    }
    else
    {
        struct sockaddr_in addr;
        addr.sin_family      = ip_ver;
        addr.sin_port        = htons(serv_meta.port);
        addr.sin_addr.s_addr = htonl(serv_meta.addr.ip_ver);
        
        if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
        {
            exit_msg = SERV_BIND_ERR;
            _serv_status = failed;
            if (stat_changed_hdlr != NULL)
                stat_changed_hdlr(_serv_status);
        }
        else
        {
            if (pthread_create(&serv_listen_thread, NULL,
                               _serv_listen, (void *)&sockfd) == -1)
            {
                exit_msg = SERV_THRD_ERR;
                _serv_status = failed;
                if (stat_changed_hdlr != NULL)
                    stat_changed_hdlr(_serv_status);
            }
            else
            {
                exit_msg = SERV_SUCCESS;
                _serv_status = running;
                if (stat_changed_hdlr != NULL)
                    stat_changed_hdlr(_serv_status);
            }
        }
    }
    
    return exit_msg;
}

int serv_start(char *addr, port_t port, enum ip_prot_ver ip_ver,
               struct sc_serv_conf *conf, enum trans_prot trans_prot) {
    
    int exit_msg;
    
    struct sc_meta serv_meta;
    strcpy(serv_meta.addr.ip_addr, addr);
    serv_meta.port = port;
    
    exit_msg = serv_start(serv_meta, ip_ver, conf, trans_prot);
    
    return exit_msg;
}

int serv_dat_recv(struct sc_clt_meta *clt, struct sc_msg *msg) {
    
    int exit_msg;
    
    if (_serv_status != running)
        exit_msg = SERV_STAT_ERR;
    else
    {
        if (msg == NULL)
            exit_msg = SERV_INVL_MSG;
        else
        {
            if (clt == NULL || clt->clt_sockfd == -1)
                exit_msg = SERV_INVL_CLT;
            else
            {
                msg->src_sockfd = clt->clt_sockfd;
                    
                if (pthread_create(serv_recv_thread[clt->clt_id], NULL,
                                   _serv_recv, (void *)msg) == -1)
                    exit_msg = SERV_THRD_ERR;
                else
                    exit_msg = SERV_SUCCESS;
            }
        }
    }
    
    return exit_msg;
}

int serv_dat_send(struct sc_clt_meta *clt, struct sc_msg *msg) {
    return 0;
}

int serv_open_conn(struct sc_clt_meta *clts) {
    
    if (_serv_status == running)
        memcpy(clts, _clts);
    
    return n_clts;
}

int serv_status() {
    return _serv_status;
}

int serv_wait() {
    
    int exit_msg;
    
    if (_serv_status == running)
    {
        pthread_join(serv_listen_thread, NULL);
        exit_msg = SERV_SUCCESS;
    }
    else
        exit_msg = SERV_STAT_ERR;
    
    return exit_msg;
}

int stop_serv() {
    
    int exit_msg;
    
    if (_serv_status == running)
    {
        pthread_cancel(serv_listen_thread);
        exit_msg = SERV_SUCCESS;
        
        _serv_status = failed;
        
        if (stat_changed_hdlr != NULL)
            stat_changed_hdlr(_serv_status);
    }
    else
    {
        exit_msg = SERV_STAT_ERR;
        
        _serv_status = failed;
        
        if (stat_changed_hdlr != NULL)
            stat_changed_hdlr(_serv_status);
    }
    
    return exit_msg;
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
void get_conn_stat(struct sc_conn_status *conn_stat) {
    
    if (conn_status != NULL)
        memcpy(conn_stat, conn_status);
    else
        conn_stat = NULL;
    
}

/* Client-server handlers */
void evt_new_msg_hdlr(void (*hdlr)(struct sc_msg *msg)) {
    new_msg_hdlr = hdlr;
}

void evt_msg_snt_hdlr(void (*hdlr)(struct sc_msg *msg)) {
    msg_snt_hdlr = hdlr;
}

/* Server-only handlers */
void evt_new_clt_hdlr(void (*hdlr)(struct sc_clt_meta *, int *)) {
    new_clt_hdlr = hdlr;
}

void evt_clt_dis_hdlr(void (*hdlr)(struct sc_clt_meta *)) {
    clt_dis_hdlr = hdlr;
}

void evt_stat_changed_hdlr(void (*hdlr)(enum sc_serv_status status)) {
    stat_changed_hdlr = hdlr;
}

/* Client-only handlers */
void evt_contd_hdlr(void (*hdlr)(struct sc_meta *, int)) {
    contd_hdlr = hdlr;
}

void evt_disconn_hdlr(void (*hdlr)(struct sc_meta *)) {
    disconn_hdlr = hdlr;
}
