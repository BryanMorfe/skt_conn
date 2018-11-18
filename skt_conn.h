/*
 * SKT_CONN allows for easy connection to server. It can be configured to be a
 * server or a client. It allows the use of TCP and UDP connections, IPv4 and IPv6
 */

#ifndef SKT_CONN_H
#define SKT_CONN_H

/* Headers */
#include <sys/socket.h>
#include <stdint.h>

/* Constants */
#define MX_HN_LENGTH    100
#define MX_IP_LENGTH    18


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

union sc_addr
{
    char hostname[MX_HN_LENGTH];
    char ip_addr[MX_IP_LENGTH];
};

struct sc_meta
{
    union sc_addr addr;
    port_t        port;
};

struct conn_status
{
    uint8_t          conn;
    struct           serv_meta serv;
    enum ip_prot_ver ip_ver;
    enum trans_prot  trans_prot;
};

struct sc_clt_meta
{
    clt_id_t        clt_id;
    struct sc_meta  clt_meta;
    conn_time_t     time_conn;
}

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
 =                              serv_listen                             =
 ========================================================================
 = This creates a socket, binds it, starts listening, and accepting     =
 = connections. This uses a thread approach that allows simultaneous    =
 = receiving of data from multiple clients in a non-blocking way.       =
 = If handlers are used for new clients, they will be executed after    =
 = the events occur. The function itself is blocking and handlers must  =
 = be set if a response-per-event is required.                          =
 ========================================================================
 */
int serv_listen(struct sc_meta serv_meta, enum ip_prot_ver ip_ver,
                enum trans_prot trans_prot);
int serv_listen(char *addr, char *port, enum ip_prot_ver ip_ver,
                enum trans_prot trans_prot);

/*
 ========================================================================
 =                              serv_dat_recv                           =
 ========================================================================
 = Receive data from connected clients. If an event handler was         =
 = specified for this event, it will be called upon successful or       =
 = unsuccessful completion of message                                   =
 ========================================================================
 */
int serv_dat_recv(void *data);

/*
 ========================================================================
 =                              serv_dat_send                           =
 ========================================================================
 = Send data to specified connected client                              =
 ========================================================================
 */
int serv_dat_send(struct sc_clt_meta *clt, void *data);

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

/* Client-server handler */
int evt_new_msg_hdlr(void (*hdlr)(void *));
int evt_msg_snt_hdlr(void (*hdlr)(void *));

/* Server-only handler */
int evt_new_clt_hdlr(void (*hdlr)(struct sc_clt_meta *, int *));
int evt_clt_dis_hdlr(void (*hdlr)(struct sc_clt_meta *));

/* Client-only handler */
int evt_contd_hdlr(void (*hdlr)(struct sc_meta *, int));
int evt_disconn_hdlr(void (*hdlr)(struct sc_meta *));

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
int clt_conn(struct sc_meta serv_meta, enum ip_prot_ver *ip_ver,
             enum trans_prot *trans_prot);
int clt_conn(char *addr, char *port, enum ip_prot_ver *ip_ver,
             enum trans_prot *trans_prot);

/*
 ========================================================================
 =                              clt_dat_send                            =
 ========================================================================
 = Send data to connected server. If event handler was set it is called =
 ========================================================================
 */
int clt_dat_send(void *data);

/*
 ========================================================================
 =                              clt_dat_recv                            =
 ========================================================================
 = Receive data from connected server. If event handler was set, it is  =
 = called and received data is passed.                                  =
 ========================================================================
 */
int clt_dat_recv();

/*
 ========================================================================
 =                              clt_close_conn                          =
 ========================================================================
 = Cleanup and close connection to server. Handler called if set        =
 ========================================================================
 */
int clt_close_conn();

/* Retrieve connection status */
void get_conn_stat(struct conn_status *conn_stat);

#endif /* SKT_CONN_H */
