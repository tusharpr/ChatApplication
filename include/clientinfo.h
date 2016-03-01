struct client 
{
int index;
char name[30];
char ip[INET_ADDRSTRLEN];
int cport;
char cport_s[6];
int client_status;
int sock;
int rcount;
int scount;
struct list block_list;
struct list msgbuf;
struct list_elem elem;
};

struct block_ips
{
char bip[INET_ADDRSTRLEN];
struct list_elem elem;
};

struct buff_msgs
{
int valid;
char bmsg[500];
struct list_elem elem;
};

bool
large (const struct list_elem *,
                     const struct list_elem *,
                     void *);

struct client_list
{
char name[30];
char ip[INET_ADDRSTRLEN];
int port;
struct list_elem elem;
};

