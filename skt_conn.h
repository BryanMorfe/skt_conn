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
int serv_listen(struct sc_meta serv_meta, enum ip_prot_ver *ip_ver, enum trans_prot *trans_prot);
int serv_listen(char *hostname, char *ip_addr, enum ip_prot_ver *ip_ver, enum trans_prot *trans_prot);
int serv_new_clt(struct sc_clt_meta *clts, int n_clts, int max_clts, void *hdlr);
int send_dat_rec(void *data, void *hdlr);
int serv_dat_send(struct sc_clt_meta *clt, void *data);
int stop_listen();

/*
 * If configured as a client:
 * 1) Prepare for communication
 * 2) Send data
 * 3) When done, close communication
 */

/* If configured as a client, use these functions */
int clt_comm_prep(struct sc_meta serv_meta, enum ip_prot_ver *ip_ver, enum trans_prot *trans_prot);
int clt_comm_prep(char *hostname, char *ip_addr, enum ip_prot_ver *ip_ver, enum trans_prot *trans_prot);
int send_data(void *data);
int close_comm();

/* Retrieve connection status */
void get_conn_stat(struct conn_status *conn_stat);

#endif /* SKT_CONN_H */
