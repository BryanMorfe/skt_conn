# skt_conn
#### The cool networking interface

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
skt_conn is an interface that uses *socket programming* in C to allow *easy* data transfer.

### Features
* Can be used as a client to send data
* Can be used as a server to receive data

### Platforms
N/A

### Installing
N/A

### Server
```C
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
    int dat_stat;
    void data[4096]; // 4Kb of data max
    while ((dat_stat = serv_wait_clt_comm(data, NULL)) != SERV_CONN_ERR)
        // do something with data, handle errors
  
    int status = stop_listen();
  
    // Only get to this point if data error has occured
    // You may choose to handle error inside the loop and continue to receive data
    if (dat_stat == SERV_CONN_ERR) {
        // error with connection
    }
  
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
    struct serv_meta serv;
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
