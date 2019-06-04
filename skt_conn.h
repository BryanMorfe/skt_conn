/*
 * skt_conn.h contains the prototypes and definitions of types
 * of the SKT_CONN library. This file is part of the skt_conn
 * repository located at https://github.com/BryanMorfe/skt_conn
 * and as such is governed by the LICENSE file at that repository.
 * For more information or updated files, visit the repository at
 * the url indicated above.
 */

#ifndef SKT_CONN_H
#define SKT_CONN_H

/**** Headers ****/
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdint.h>

/**** Constants ****/
#define MX_HN_LENGTH    100
#define MX_IP_LENGTH    18
#define MX_MSG_LENGTH   16384

/*** Error Codes ***/
/* Client Error Codes */
#define CLT_CONN_ERR    40
#define CLT_SEND_ERR    41
#define CLT_RECV_ERR    42
#define CLT_CONC_ERR    43
#define CLT_SCKT_ERR    44

/* Server Error Codes */
#define SERV_INVAL_META 50
#define SERV_LIST_ERR   51
#define SERV_SEND_ERR   52
#define SERV_RECV_ERR   53
#define SERV_STOP_ERR   54
#define SERV_BIND_ERR   55
#define SERV_SCKT_ERR   56
#define SERV_ACPT_ERR   57
#define SERV_THRD_ERR   58
#define SERV_INVL_CFG   59
#define SERV_INVL_DAT   60
#define SERV_INVL_CLT   61
#define SERV_INVL_MSG   62
#define SERV_STAT_ERR   63

/*** Sucess Codes ***/
#define CLT_SUCCESS     20
#define SERV_SUCCESS    30

/*** Other Status Codes ***/
#define CLT_CONNECTED   10
#define CLT_FAILED      11
#define CLT_DISCONN     12


/*** Types ***/
typedef uint16_t    port_t;
typedef uint16_t    clt_id_t;
typedef uint32_t    conn_time_t;

enum ip_prot_ver
{
    ipv4 = AF_INT,
    ipv6 = AF_INT6
};

enum trans_prot
{
    tcp = SOCK_STREAM,
    udp = SOCK_DGRAM
};

enum sc_serv_status
{
    stopped = 0,
    running = 1,
    failed = 2
};

union sc_addr
{
    char hostname[MX_HN_LENGTH];
    char ip_addr[MX_IP_LENGTH];
};

struct sc_msg
{
    /* Headers */
    int  src_sockfd;
    int  dst_sockfd;
    int  msg_size;
    int  trans_timestamp;
    int  msg_type;
    void *ots_hds;
    
    /* Content */
    void *data;
};

struct sc_meta
{
    union sc_addr addr;
    port_t        port;
};

struct sc_serv_conf
{
    uint16_t max_conc_clts;
};

struct sc_conn_status
{
    uint8_t          conn;
    struct sc_meta   serv;
    enum ip_prot_ver ip_ver;
    enum trans_prot  trans_prot;
};

struct sc_serv_meta
{
    int             sockfd;
    struct sc_meta  serv_meta;
    conn_time_t     time_conn;
};

struct sc_clt_meta
{
    int             clt_sockfd;
    clt_id_t        clt_id;
    struct sc_meta  clt_meta;
    conn_time_t     time_conn;
};

/*** Function prototypes ***/

/*
 * If configured as a server:
 * 1) Listen for connections
 * 2) Receive data
 * 3) When done, stop listening
 */

/* If configured as server, use these functions */

/*
 ========================================================================
 =                              serv_start                              =
 ========================================================================
 = This creates a socket, binds it, starts listening, and accepting     =
 = connections. This uses a thread approach that allows simultaneous    =
 = receiving of data from multiple clients in a non-blocking way.       =
 = If handlers are used for new clients, they will be executed after    =
 = the events occur. The function itself is blocking and handlers must  =
 = be set if a response-per-event is required.                          =
 ========================================================================
 */
int serv_start(struct sc_meta serv_meta, enum ip_prot_ver ip_ver,
               struct sc_serv_conf *conf, enum trans_prot trans_prot);
int serv_start(char *addr, port_t port, enum ip_prot_ver ip_ver,
               struct sc_serv_conf *conf, enum trans_prot trans_prot);

/*
 ========================================================================
 =                              serv_dat_recv                           =
 ========================================================================
 = Receive data from connected clients. If an event handler was         =
 = specified for this event, it will be called upon successful or       =
 = unsuccessful completion of message                                   =
 ========================================================================
 */
int serv_dat_recv(struct sc_clt_meta *clt, struct sc_msg *msg);

/*
 ========================================================================
 =                              serv_dat_send                           =
 ========================================================================
 = Send data to specified connected client                              =
 ========================================================================
 */
int serv_dat_send(struct sc_clt_meta *clt, struct sc_msg *msg);

/*
 ========================================================================
 =                              serv_open_conn                          =
 ========================================================================
 = Get the list of connected clients and returns number of clients.     =
 ========================================================================
 */
int serv_open_conn(struct sc_clt_meta *clts);

/*
 ========================================================================
 =                              serv_status                             =
 ========================================================================
 = Get the server status: stopped, running, failed.                     =
 ========================================================================
 */
int serv_status();

/*
 ========================================================================
 =                              serv_wait                               =
 ========================================================================
 = Blocks the main thread and waits for the server to be stopped.       =
 ========================================================================
 */
int serv_wait();

/*
 ========================================================================
 =                              stop_serv                               =
 ========================================================================
 = Clean up and stop listening                                          =
 ========================================================================
 */
int stop_serv();

/*
 * If configured as a client:
 * 1) Prepare for communication
 * 2) Send data
 * 3) When done, close communication
 */

/********** Setting Event handlers **********/

/* Client-server handlers */
void evt_new_msg_hdlr(void (*hdlr)(struct sc_msg *msg));
void evt_msg_snt_hdlr(void (*hdlr)(struct sc_msg *msg));

/* Server-only handlers */
void evt_new_clt_hdlr(void (*hdlr)(struct sc_clt_meta *, int *));
void evt_clt_dis_hdlr(void (*hdlr)(struct sc_clt_meta *));

/* Client-only handlers */
void evt_contd_hdlr(void (*hdlr)(struct sc_meta *, int));
void evt_disconn_hdlr(void (*hdlr)(struct sc_meta *));

/* If configured as a client, use these functions */

/*
 ========================================================================
 =                              clt_conn                                =
 ========================================================================
 = Prepares a client's socket and attempts to connect to the server     =
 = specified in the sc_meta structure. The event handler connected      =
 = is called if it was specified with the evt_contd_hdlr                =
 ========================================================================
 */
int clt_conn(struct sc_meta serv_meta, enum ip_prot_ver ip_ver,
             enum trans_prot trans_prot);
int clt_conn(char *addr, port_t port, enum ip_prot_ver ip_ver,
             enum trans_prot trans_prot);

/*
 ========================================================================
 =                              clt_dat_send                            =
 ========================================================================
 = Send data to connected server. If event handler was set it is called =
 ========================================================================
 */
int clt_dat_send(struct sc_msg *msg);

/*
 ========================================================================
 =                              clt_dat_recv                            =
 ========================================================================
 = Receive data from connected server. If event handler was set, it is  =
 = called and received data is passed.                                  =
 ========================================================================
 */
int clt_dat_recv(struct sc_msg *msg);

/*
 ========================================================================
 =                              clt_close_conn                          =
 ========================================================================
 = Cleanup and close connection to server. Handler called if set        =
 ========================================================================
 */
int clt_close_conn();

/* Retrieve connection status */
void get_conn_stat(struct sc_conn_status *conn_stat);

#endif /* SKT_CONN_H */
