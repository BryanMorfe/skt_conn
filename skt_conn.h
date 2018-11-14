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

union serv_addr
{
    char hostname[MX_HN_LENGTH];
    char ip_addr[MX_IP_LENGTH];
};

struct serv_meta
{
    union serv_addr addr;
    uint16_t port;
};

struct conn_status
{
    uint8_t conn;
    struct serv_meta serv;
    enum ip_prot_ver ip_ver;
    enum trans_prot trans_prot;
};

/*** Function prototypes ***/

/*
 * If configured as a server:
 * 1) Listen for connections
 * 2) Receive data
 * 3) When done, stop listening
 */

/* If configured as server, use these functions */
int serv_listen(struct serv_addr serv_addr, enum ip_prot_ver *ip_ver, enum trans_prot *trans_prot);
int serv_listen(char *hostname, char *ip_addr, enum ip_prot_ver *ip_ver, enum trans_prot *trans_prot);

int receive_data(void *data);

int stop_listen();

/*
 * If configured as a client:
 * 1) Prepare for communication
 * 2) Send data
 * 3) When done, close communication
 */

/* If configured as a client, use these functions */
int clt_comm_prep(struct serv_addr serv_addr, enum ip_prot_ver *ip_ver, enum trans_prot *trans_prot);
int clt_comm_prep(char *hostname, char *ip_addr, enum ip_prot_ver *ip_ver, enum trans_prot *trans_prot);

int send_data(void *data);

int close_comm();

/* Retrieve connection status */
void get_conn_stat(struct conn_status *conn_stat);

#endif /* SKT_CONN_H */
