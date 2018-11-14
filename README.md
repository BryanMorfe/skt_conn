# skt_conn
#### The cool C networking interface

**Current Version: Beta**

## Index

* 1. [Description](https://github.com/BryanMorfe/skt_conn/README.md#description)
* 2. [Features](https://github.com/BryanMorfe/skt_conn/README.md#features)
* 3. [Platforms](https://github.com/BryanMorfe/skt_conn/README.md#platforms)
* 4. [Installing](https://github.com/BryanMorfe/skt_conn/README.md#installing)
* 5. [Using As Server](https://github.com/BryanMorfe/skt_conn/README.md#server)
* 6. [Using As Client](https://github.com/BryanMorfe/skt_conn/README.md#client)
* 7. [Other Features](https://github.com/BryanMorfe/skt_conn/README.md#other)

### Description
skt_conn is event-driven client-server communication interface for the C programming language.

### Features
skt_conn can be:
* Configured as a client to send messages to a server;
* Configured as a server to receive messages from clients; and,
* Configured for bidirection communication between an skt_conn configured client and an skt_conn configured server.

### Platforms
N/A

### Installing
N/A

### Server
```C
/* Handler */
void *msg_handler(void *data);
int main()
{

    /* Server metadata */
    struct sc_meta serv;
    strcpy(serv.addr.ip_addr, "127.0.0.1");
    serv.port = 3463;
  
    /* Start listener */
    if(serv_listen(serv, ipv4, tcp) == -1) {
        // handle error
    }
  
    /* Receive data */
    void data[4096]; // 4Kb of data max
    serv_msg_rec(msg_handler, data);
  
    return 0;
}
```

### Client

```C
int main()
{

    /* Server metadata */
    struct sc_meta serv;
    strcpy(serv.addr.ip_addr, "192.168.1.65");
    serv.port = 3463;
  
    /* Prepare for Communication */
    if(clt_comm_prep(serv, ipv4, tcp) == -1) {
        // handle error
    }
  
    /* Send data */
    int dat_stat;
    void data[4096]; // 4Kb of data max
  
    // add some data to the buffer
  
    dat_stat = send_data(data);
  
    if (dat_stat == CLT_DATA_ERR) {
        // error sending data
    }
  
    close_comm();
  
    return 0;
}
```

### Other
#### Getting Connection Status

```C
int main()
{
    // Conn is established before
  
    /* Get connection status */
    struct conn_status status;
  
    get_conn_stat(&status);
  
    // Do whatever with status
    if (status.conn == UNS_CONN) {
        printf("Connection unsuccessful\n");
        exit(1);  
    }
}
```

#### Starting an Open Client-Server Bidirectional Communication

##### Server


```C

/* Event handlers */
void *new_clt_handler(struct sc_clt_meta *clts, int *n_clts);
void *msg_handler(void *data);

int main()
{
    /* Server metadata */
    struct sc_meta serv;
    strcpy(serv.addr.ip_addr, "127.0.0.1");
    serv.port = 3463;
  
    /* Start listener */
    if(serv_listen(serv, ipv4, tcp) == -1) {
        // handle error
    }
    
    /* Event handlers */
    struct sc_clt_meta clts[32]; // Max 32 concurrent connections
    int n_clts = 0;
    
    /* Event: new connection; new client. 
     * new_clt_handler called when a new client connects 
     */
    serv_new_clt(clts, n_clts, 32, new_clt_handler);
    
    void data[4096]; // 4kb max in data
    /* Event: new message received from a client. 
     * msg_handler called when a new message (data) comes through 
     */
    serv_msg_rec(msg_handler, data);
    
    void msg[10];
    
    /* Send messages to connected clients */
    for (int i = 0; i < n_clts, i++)
        serv_msg_send(clts[i], msg);
}
```

##### Client


#### Simple 
