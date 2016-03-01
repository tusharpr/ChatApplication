/**
 * @tusharpr_assignment1
 * @author  Tushar Baraiya <tusharpr@buffalo.edu>
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * This contains the main function. Add further description here....
 */

/*code refrences : beej's guide
		   list.h Pintos OS project
**/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<list.h>
#include<list.c>

#include "../include/global.h"
#include "../include/logger.h"
#include "../include/clientinfo.h"
/**
 * main function
 *
 * @param  argc Number of arguments
 * @param  argv The argument list
 * @return 0 EXIT_SUCCESS
 */
void get_login_clients(char *);
void get_stats(char *);
void get_blocked_byip(char *,char *);
void get_login_clients_log(char *);
struct list loginlist;


bool
large(const struct list_elem *a,
                     const struct list_elem *b,
                     void *aux )
{
  struct client *la = list_entry (a, struct client, elem);
  struct client *lb = list_entry (b, struct client, elem);
  return la->cport < lb->cport;
}

int main(int argc, char **argv)
{
	/*Init. Logger*/
	cse4589_init_log(argv[2]);
	/*Clear LOGFILE*/
	fclose(fopen(LOGFILE, "w"));
	/*Start Here*/
if(argc!=3) {printf("USAGE: ./assignment1 <c/s> <PORT>");return 0;}
char option = argv[1][0];
char *port = argv[2];
char ipstr[INET_ADDRSTRLEN];
int t=0;
fd_set read_clients,read_fds;
FD_ZERO(&read_fds);
FD_ZERO(&read_clients);
FD_SET(fileno(stdin), &read_fds);
int fdmax = fileno(stdin);
bool l_flag= false;
char in[300];
char *arg;
char *rest;
int inlen;
int j=0;
if(!(option=='c'||option=='s')) {printf("USAGE: ./assignment1 <c/s> <PORT>");return 0;}

if(option=='c')
{

int newfilesockfd;
struct list list_clients;
char log_l[500];
bool login_suc = false;
bool login_again = false;

struct addrinfo hints,*res,*p,hints2,*res2,*p2;
	int status,sockfd,new_fd;
	struct sockaddr_storage their_addr;
	socklen_t addr_size;
int sockfd2;

memset(&hints2,0,sizeof(hints2));
hints2.ai_family=AF_INET;
hints2.ai_socktype=SOCK_STREAM;
hints2.ai_flags=AI_PASSIVE;
if(getaddrinfo(NULL,port,&hints2,&res2)!=0)
{
	fprintf(stderr,"getaddrinfo error: %s\n",gai_strerror(status));
	return 0;
}

for(p2=res2;p2!=NULL;p2=p2->ai_next)
{
if((sockfd2=socket(p2->ai_family,p2->ai_socktype,p2->ai_protocol))<0)
{
perror("socket() error:");
continue;
}
FD_SET(sockfd2,&read_fds);
if(sockfd2>fdmax)
{
fdmax = sockfd2;
}
if(bind(sockfd2,p2->ai_addr,p2->ai_addrlen)<0)
{
perror("bind() error:");
continue;
}
break;
}
freeaddrinfo(res2);
if(listen(sockfd2,10)<0)
{
perror("listen() error:");
}
while(1)
{
read_clients = read_fds;
if (select(fdmax+1, &read_clients, NULL, NULL, NULL) == -1) {
            perror("select");
	    return 0;
        }

for(j=0;j<=fdmax;j++)
{
if(FD_ISSET(j,&read_clients))
{
bool ft = false;
if(j==sockfd2)
{
printf("Filetransfer");

		addr_size = sizeof(their_addr);
		if((newfilesockfd=accept(sockfd2, (struct sockaddr *)&their_addr, &addr_size))<0)
		{
		perror("accept() error:");
		}
		FD_SET(newfilesockfd, &read_fds); // add to master set
                        if (newfilesockfd > fdmax) {    // keep track of the max
                            fdmax = newfilesockfd;
                        }
		ft=true;
		}

else if(j==newfilesockfd && ft)
{
bool newt = true;
char buff[100];
FILE *fp;
if(newt) 
{
recv(j,buff,100,0);
int l = strlen(buff);
buff[l] = '\0';
newt= false;
fp = fopen(buff,"w+");
}
else
{
recv(j,buff,100,0);
fputs(buff,fp);	
}
}

else if(j==0)
{
fgets(in,300,stdin);
if(in[0]!='\n')
{
arg = strtok_r(in," \n",&rest);

if(strcmp(arg,"EXIT")==0)  
{
send(sockfd,"EXT ",4,0);
cse4589_print_and_log("[EXIT:SUCCESS]\n");
cse4589_print_and_log("[EXIT:END]\n");
return 0;
}
else if(strcmp(arg,"AUTHOR")==0)
	{
cse4589_print_and_log("[AUTHOR:SUCCESS]\n");
cse4589_print_and_log("I, tusharpr, have read and understood the course academic integrity policy.\n");
cse4589_print_and_log("[AUTHOR:END]\n");
	}
else if(strcmp(arg,"IP")==0)
	{
	if(myip(ipstr)==-1){printf("Problem getting ip\n"); continue;}	
	cse4589_print_and_log("[IP:SUCCESS]\n");
	cse4589_print_and_log("IP:%s\n",ipstr);	
	cse4589_print_and_log("[IP:END]\n");
	}
else if(strcmp(arg,"PORT")==0)
	{
	cse4589_print_and_log("[PORT:SUCCESS]\n");
	cse4589_print_and_log("PORT:%s\n",port);	
	cse4589_print_and_log("[PORT:END]\n");
	}
else if(strcmp(arg,"LOGIN")==0)
	{
if(!login_again && !login_suc)
{	
memset(&hints,0,sizeof(hints));
hints.ai_family=AF_UNSPEC;
hints.ai_socktype=SOCK_STREAM;
char *ip = strtok(rest," \n");
char *servport = strtok(NULL," \n");
if(getaddrinfo(ip,servport,&hints,&res)!=0)
{	
	cse4589_print_and_log("[LOGIN:ERROR]\n");
	cse4589_print_and_log("[LOGIN:END]\n");
	fprintf(stderr,"getaddrinfo error: %s\n",gai_strerror(status));
}
for(p=res;p!=NULL;p=p->ai_next)
{
if((sockfd=socket(p->ai_family,p->ai_socktype,p->ai_protocol))<0)
{
cse4589_print_and_log("[LOGIN:ERROR]\n");
cse4589_print_and_log("[LOGIN:END]\n");
perror("socket() error:");
break;
}
FD_SET(sockfd, &read_fds);
fdmax=sockfd;
if(connect(sockfd,p->ai_addr,p->ai_addrlen)<0)
{
cse4589_print_and_log("[LOGIN:ERROR]\n");
cse4589_print_and_log("[LOGIN:END]\n");
perror("connect() error:");
break;
}
login_again  = true;
login_suc = true;
break;
}
if(login_again && login_suc){
char myname[30];
char mynameport[50];
strcpy(mynameport,"CON ");
gethostname(myname,sizeof(myname));
strcat(mynameport,port);
strcat(mynameport," ");
strcat(mynameport,myname);
freeaddrinfo(res);
send(sockfd,mynameport,strlen(mynameport),0);
l_flag=true;
}

}
else if(!login_suc)
{
send(sockfd,"LIA ",4,0);
login_suc = true;
}
else
{

}
}
else if(strcmp(arg,"SEND")==0)
{
char temp[300];
strcpy(temp,rest);
char *temp_ip;
temp_ip = strtok(temp," \n");
bool valid = false;
struct list_elem *e;
for(e=list_begin(&list_clients);e!=NULL;e=list_next(e))
{
struct client_list *a = list_entry (e, struct client_list, elem);
if(strcmp(a->ip,temp_ip)==0)
{
valid=true;
}
}

if(login_suc && valid)
{

char rmsg[300];
strcpy(rmsg,"REL ");
strcat(rmsg,rest);
int rmsg_len = strlen(rmsg);
rmsg[rmsg_len]='\0';
send(sockfd,rmsg,rmsg_len,0);

}
else
{
cse4589_print_and_log("[SEND:ERROR]\n");
cse4589_print_and_log("[SEND:END]\n");
}

}

else if(strcmp(arg,"REFRESH")==0)
{
if(login_suc)
{
send(sockfd,"LIS ",4,0);
}
else
{
}
}

else if(strcmp(arg,"LIST")==0)
{
if(login_suc)
{
char temp_list[500];
strcpy(temp_list,log_l);
cse4589_print_and_log("[LIST:SUCCESS]\n");
int i=0;
char *elem; 
strtok_r(temp_list," \n",&elem);
char *lname;
for(i=1;;i++)
{
if(i==1)lname = strtok(elem," \n");
else lname = strtok(NULL," \n");
if(strcmp(lname,"end")==0) break;
char *lip = strtok(NULL," \n");
char *lport = strtok(NULL," \n");
cse4589_print_and_log("%-5d%-35s%-20s%-8d\n",i,lname,lip,atoi(lport));
}
cse4589_print_and_log("[LIST:END]\n");
}
else
{
}
} 

else if(strcmp(arg,"BLOCK")==0)
{

char temp[300];
strcpy(temp,rest);
char *temp_ip;
temp_ip = strtok(temp," \n");
bool valid = false;
struct list_elem *e;
for(e=list_begin(&list_clients);e!=NULL;e=list_next(e))
{
struct client_list *a = list_entry (e, struct client_list, elem);
if(strcmp(a->ip,temp_ip)==0)
{
valid=true;
}
}
if(login_suc && valid)
{
char bmsg[100];
strcpy(bmsg,"BLK ");
strcat(bmsg,rest);
int bmsg_len = strlen(bmsg);
bmsg[bmsg_len]='\0';
send(sockfd,bmsg,bmsg_len,0);
}
else
{
cse4589_print_and_log("[BLOCK:ERROR]\n");
cse4589_print_and_log("[BLOCK:END]\n");
}
}

else if(strcmp(arg,"UNBLOCK")==0)
{
char temp[300];
strcpy(temp,rest);
char *temp_ip;
temp_ip = strtok(temp," \n");
bool valid = false;
struct list_elem *e;
for(e=list_begin(&list_clients);e!=NULL;e=list_next(e))
{
struct client_list *a = list_entry (e, struct client_list, elem);
if(strcmp(a->ip,temp_ip)==0)
{
valid=true;
}
}



if(login_suc && valid)
{
char ubmsg[100];
strcpy(ubmsg,"UBL ");
strcat(ubmsg,rest);
int ubmsg_len = strlen(ubmsg);
ubmsg[ubmsg_len]='\0';
send(sockfd,ubmsg,ubmsg_len,0);
}
else
{
cse4589_print_and_log("[UNBLOCK:ERROR]\n");
cse4589_print_and_log("[UNBLOCK:END]\n");
}
}



else if(strcmp(arg,"TEST")==0)
{
char bmsg[4];
strcpy(bmsg,"TES ");
send(sockfd,bmsg,4,0);
}


else if(strcmp(arg,"LOGOUT")==0)
{
if(login_suc)
{
login_suc = false;
char bmsg[4];
strcpy(bmsg,"LOT ");
send(sockfd,bmsg,4,0);
cse4589_print_and_log("[LOGOUT:SUCCESS]\n");
cse4589_print_and_log("[LOGOUT:END]\n");
}
else
{
}
}
else if(strcmp(arg,"BROADCAST")==0)
{

if(login_suc)
{
char bcmsg[300];
strcpy(bcmsg,"BRD ");
strcat(bcmsg,rest);
int bcmsg_len = strlen(bcmsg);
bcmsg[bcmsg_len]='\0';
send(sockfd,bcmsg,bcmsg_len,0);
}
else
{
}
}

else if(strcmp(arg,"SENDFILE")==0)
{
printf("filetr");
if(login_suc)
{
printf("filetr");
char *sndip = strtok(rest," \n");
char *filename = strtok(NULL," \n");
bool valid = false;
char sndport[5];
int sockfd3;	
struct list_elem *e;
for(e=list_begin(&list_clients);e!=NULL;e=list_next(e))
{
struct client_list *a = list_entry (e, struct client_list, elem);
if(strcmp(a->ip,sndip)==0)
{
sprintf(sndport, "%d", a->port);
printf(" %s ",sndport);
valid=true;
break;
}
}
	if(valid)
		{
	printf("filetr2");
		
			if(getaddrinfo(sndip,sndport,&hints,&res)!=0)
			{	
				fprintf(stderr,"getaddrinfo error: %s\n",gai_strerror(status));
			}
			for(p=res;p!=NULL;p=p->ai_next)
			{
			if((sockfd3=socket(p->ai_family,p->ai_socktype,p->ai_protocol))<0)
			{
			perror("socket() error:");
			break;
			}
			if(connect(sockfd3,p->ai_addr,p->ai_addrlen)<0)
			{
			perror("connect() error:");
			break;
			}
			}
			FILE *fp;
			fp = fopen(filename,"r");
			if(fp!=NULL)
				{
				printf("filetr3");
				send(sockfd3,filename,strlen(filename),0);
				char buffer[100];				
				while((fgets(buffer,100,fp)))
				{
				//send(sockfd3,buffer,strlen(buffer),0);
				}
				fclose(fp);
				}
				
		}

}
else
{

}
}


}
}
else if(login_suc)
{
char log_list[1000];
int rcv_log_list = recv(sockfd,log_list,1000,0);
log_list[rcv_log_list] = '\0';
char temp[1000];
strcpy(temp,log_list);
char *rcv;
char *rarg = strtok_r(log_list," \n",&rcv);
if(strcmp(rarg,"LIS")==0)
{
list_init(&list_clients);
strcpy(log_l,temp);
cse4589_print_and_log("[REFRESH:SUCCESS]\n");
int i=0;
char *elem; 
strtok_r(temp," \n",&elem);
char *lname;
for(i=1;;i++)
{
if(i==1)lname = strtok(elem," \n");
else lname = strtok(NULL," \n");
if(strcmp(lname,"end")==0) break;
char *lip = strtok(NULL," \n");
char *lport = strtok(NULL," \n");
//cse4589_print_and_log("%-5d%-35s%-20s%-8d\n",i,lname,lip,atoi(lport));
struct client_list *cl = malloc(sizeof(struct client_list));
strcpy(cl->name,lname);
strcpy(cl->ip,lip);
cl->port = atoi(lport);
list_insert(list_end(&list_clients),&cl->elem);
}
cse4589_print_and_log("[REFRESH:END]\n");
}
else if(strcmp(rarg,"LOG")==0)
{
strcpy(log_l,temp);
cse4589_print_and_log("[LOGIN:SUCCESS]\n");
int i=0;
char *elem; 
strtok_r(temp," \n",&elem);
char *lname;
list_init(&list_clients);
for(i=1;;i++)
{
if(i==1)lname = strtok(elem," \n");
else lname = strtok(NULL," \n");
if(strcmp(lname,"end")==0) break;
char *lip = strtok(NULL," \n");
char *lport = strtok(NULL," \n");
//cse4589_print_and_log("%-5d%-35s%-20s%-8d\n",i,lname,lip,atoi(lport));
struct client_list *cl = malloc(sizeof(struct client_list));
strcpy(cl->name,lname);
strcpy(cl->ip,lip);
cl->port = atoi(lport);
list_insert(list_end(&list_clients),&cl->elem);
}
cse4589_print_and_log("[LOGIN:END]\n");

char *dcd = strtok(NULL," \n");
if(strcmp(dcd,"RCV")==0)
{
while(1)
{
char *smsg =  strtok(NULL," \n");
if(strcmp(smsg,"RCV")==0){smsg =  strtok(NULL," \n");}
else if(strcmp(smsg,"endm")==0){break;}
char *sip = strtok(NULL," \n");
cse4589_print_and_log("[RECEIVED:SUCCESS]\n");
//cse4589_print_and_log("msg from:%s\n[msg]:%s\n",sip, smsg);
cse4589_print_and_log("[RECEIVED:END]\n");
}
}

}


else if(strcmp(rarg,"BFD")==0)
{
if(strcmp(strtok(rcv," \n"),"RCV")==0)
{
while(1)
{
char *m,*ipb;
m = strtok(NULL," \n");
if(strcmp(m,"endm")==0)break;
else if(strcmp(m,"RCV")==0)m = strtok(NULL," \n");
ipb = strtok(NULL," \n");
cse4589_print_and_log("[RECEIVED:SUCCESS]\n");
cse4589_print_and_log("msg from:%s\n[msg]:%s\n",ipb, m);
cse4589_print_and_log("[RECEIVED:END]\n");
}
}
}



else if(strcmp(rarg,"SNDS")==0)
{
cse4589_print_and_log("[SEND:SUCCESS]\n");
cse4589_print_and_log("[SEND:END]\n");
}
else if(strcmp(rarg,"SNDF")==0)
{
cse4589_print_and_log("[SEND:ERROR]\n");
cse4589_print_and_log("[SEND:END]\n");
}


else if(strcmp(rarg,"BRS")==0)
{
cse4589_print_and_log("[BROADCAST:SUCCESS]\n");
cse4589_print_and_log("[BROADCAST:END]\n");
}

else if(strcmp(rarg,"BS")==0)
{
cse4589_print_and_log("[BLOCK:SUCCESS]\n");
cse4589_print_and_log("[BLOCK:END]\n");
}
else if(strcmp(rarg,"BE")==0)
{
cse4589_print_and_log("[BLOCK:ERROR]\n");
cse4589_print_and_log("[BLOCK:END]\n");
}

else if(strcmp(rarg,"UBS")==0)
{
cse4589_print_and_log("[UNBLOCK:SUCCESS]\n");
cse4589_print_and_log("[UNBLOCK:END]\n");
}
else if(strcmp(rarg,"UBF")==0)
{
cse4589_print_and_log("[UNBLOCK:ERROR]\n");
cse4589_print_and_log("[UNBLOCK:END]\n");
}

else if(strcmp(rarg,"RCV")==0)
{
cse4589_print_and_log("[RECEIVED:SUCCESS]\n");
char *msg = strtok(rcv," \n");
char *ip = strtok(NULL," \n");
cse4589_print_and_log("msg from:%s\n[msg]:%s\n",ip, msg);
cse4589_print_and_log("[RECEIVED:END]\n");
}

}
}
}
}
return 0;
}

else
{

char in2[100];
char ipstr2[10][INET_ADDRSTRLEN];
int socket_client[10];
struct addrinfo hints,*res,*p;
int status,sockfd,newfd;
int i;
fd_set master,read_fds;
int fdmax;
void *add;
char buf[300];
int rcvlen;
char *rcomand;
char *rrest;
int rport;
char *rname;
struct client *a[10];
struct block_ips *bi;
list_init(&loginlist);
int login_count=0;
int ipcount=0;
struct sockaddr_storage their_addr;
socklen_t addr_size;

memset(&hints,0,sizeof(hints));
hints.ai_family=AF_INET;
hints.ai_socktype=SOCK_STREAM;
hints.ai_flags=AI_PASSIVE;

if(getaddrinfo(NULL,port,&hints,&res)!=0)
{
	fprintf(stderr,"getaddrinfo error: %s\n",gai_strerror(status));
	return 0;
}

for(p=res;p!=NULL;p=p->ai_next)
{
if((sockfd=socket(p->ai_family,p->ai_socktype,p->ai_protocol))<0)
{
perror("socket() error:");
continue;
}
if(bind(sockfd,p->ai_addr,p->ai_addrlen)<0)
{
perror("bind() error:");
continue;
}
break;
}
freeaddrinfo(res);

if(listen(sockfd,10)<0)
{
perror("listen() error:");
}
FD_SET(fileno(stdin),&master);
FD_SET(sockfd,&master);
fdmax=sockfd;
char *read;

for(;;)
{
read_fds=master;
        if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
        }
for(i=0;i<=fdmax;i++)
{

	if(FD_ISSET(i,&read_fds))
	{
	
		if(i==sockfd)
		{
		addr_size = sizeof(their_addr);
		if((newfd=accept(sockfd, (struct sockaddr *)&their_addr, &addr_size))<0)
		{
		perror("accept() error:");
		}
		socket_client[ipcount] = newfd;
		FD_SET(newfd, &master); // add to master set
                        if (newfd > fdmax) {    // keep track of the max
                            fdmax = newfd;
                        }
		add=&(((struct sockaddr_in*)(struct sockaddr *)&their_addr)->sin_addr);
		if(inet_ntop(their_addr.ss_family,add,ipstr2[ipcount],sizeof(ipstr2[ipcount]))<0)
		{
		perror("ntop() error:");
		}

			++ipcount;				
		}
		else if(i>0)
		{
		if ((rcvlen = recv(i, buf, sizeof(buf), 0)) <= 0) {
                        if (rcvlen == 0) {
                        } else {
                            perror("recv");
                        }
                        FD_CLR(i, &master);
		}
	else
	{
	buf[rcvlen]='\0';
//	printf("msg %s\n",buf);	
	rcomand = strtok_r(buf," \n",&rrest);
	if(!strcmp(rcomand,"CON"))
		{
		char login_string[600];
		char *cport_string=strtok(rrest," \n"); 
		rport = atoi(cport_string);
		rname = strtok(NULL," \n");				
		a[login_count] = malloc(sizeof(struct client));
		a[login_count]->index =login_count;
		a[login_count]->client_status =1;
		a[login_count]->rcount =0;
		a[login_count]->scount =0;		
		strcpy(a[login_count]->name,rname);
		strcpy(a[login_count]->cport_s,cport_string);
		strcpy(a[login_count]->ip,ipstr2[login_count]);
		a[login_count]->cport =rport;
		a[login_count]->sock =socket_client[login_count];
		list_insert_ordered (&loginlist, &a[login_count]->elem,large, NULL);
		list_init(&a[login_count]->block_list);
		list_init(&a[login_count]->msgbuf);			
		++login_count;	
		get_login_clients_log(login_string);
		strcat(login_string," endm");		
		send(i,login_string,strlen(login_string),0);				
		}
	else if(!strcmp(rcomand,"LIA"))
	{
		char login_string[500];
		int my_i = get_index_bysockfd(i);
		a[my_i]->client_status = 1;
		get_login_clients_log(login_string);
		strcat(login_string," endm");
		int slen = strlen(login_string);				
		send(i,login_string,slen,0);		
		char buffdmsg[1000];
		char dtemp[1000];
		
		strcpy(buffdmsg,"BFD");		
		if(!list_empty(&a[my_i]->msgbuf))
		{
		int my_i = get_index_bysockfd(i);
		struct list_elem *e;
		for(e=list_begin(&a[my_i]->msgbuf);list_next(e)!=NULL;e=list_next(e))
		{
			
			struct buff_msgs *b = list_entry(e,struct buff_msgs,elem);
			if(b->valid==1)
			{
			strcat(buffdmsg," ");
			strcat(buffdmsg,b->bmsg);			
			++a[my_i]->rcount;
			list_remove(e);
			}
		}
		}		
		strcat(buffdmsg," endm");
		strcpy(dtemp,buffdmsg);


		strtok(dtemp," \n");
		char *dcd = strtok(NULL," \n");
		if(strcmp(dcd,"RCV")==0)
		{
		while(1)
		{
		char *smsg =  strtok(NULL," \n");
		if(strcmp(smsg,"RCV")==0){smsg =  strtok(NULL," \n");}
		else if(strcmp(smsg,"endm")==0){break;}
		char *sip = strtok(NULL," \n");
			cse4589_print_and_log("[RELAYED:SUCCESS]\n");
			cse4589_print_and_log("msg from:%s, to:%s\n[msg]:%s\n",sip,a[my_i]->ip, smsg);
			cse4589_print_and_log("[RELAYED:END]\n");
					}
		}
		slen = strlen(buffdmsg);
		send(i,buffdmsg,slen,0);	
}

	else if(!strcmp(rcomand,"LIS"))
	{
	char login_string[600];
	get_login_clients(login_string);
	send(i,login_string,strlen(login_string),0);
	}
	
	else if(!strcmp(rcomand,"UBL"))
	{
	bool ublk = false;	
	char *ublk_ip =strtok(rrest," \n\0");
	int ublk_ip_len  = strlen(ublk_ip);
	ublk_ip[ublk_ip_len] = '\0';
	int my_i = get_index_bysockfd(i);	
		struct list_elem *e1;
		for(e1=list_begin(&a[my_i]->block_list);e1!=NULL;e1=list_next(e1))
		{
		struct block_ips *la1 = list_entry (e1, struct block_ips, elem);
		if(strcmp(la1->bip,ublk_ip)==0)
		{
		ublk = true;
		list_remove(e1);
		}
		}
if(ublk)
	{
	send(i,"UBS ",4,0);
	}
	else
	{
	send(i,"UBF ",4,0);
	}	
	}


	
	else if(!strcmp(rcomand,"BLK"))
	{
	char *blk_ip =strtok(rrest," \n\0");
	int blk_ip_len  = strlen(blk_ip);
	blk_ip[blk_ip_len] = '\0'; 
	bool berror = false; 
	int my_i = get_index_bysockfd(i);
	struct list_elem *e1;
	for(e1=list_begin(&loginlist);e1!=NULL;e1=list_next(e1))
		{
		struct client *la1 = list_entry (e1, struct client, elem);
		if(strcmp(la1->ip,blk_ip)==0)
		{
		berror = true;
		}
		}
		if(berror)		
		{		
		struct list_elem *e;
		for(e=list_begin(&a[my_i]->block_list);e!=NULL;e=list_next(e))
		{
		struct block_ips *la1 = list_entry (e, struct block_ips, elem);
		if(strcmp(la1->bip,blk_ip)==0)
		{
		berror = false;
		}
		}
		}
	if(berror)
	{
	bi = malloc(sizeof(struct block_ips));
	strcpy(bi->bip,blk_ip);
	list_insert(list_end(&a[my_i]->block_list),&bi->elem);
	}
	if(berror)
	{
	send(i,"BS ",3,0);
	}
	else
	{
	send(i,"BE ",3,0);
	}
	}
	else if(!strcmp(rcomand,"REL"))
	{

	bool isblkd = false;
	char snd[500];
	strcpy(snd,"RCV ");	
	char *ipto = strtok(rrest," \n");
	char *msg = strtok(NULL," \n");	
	strcat(snd,msg);
	strcat(snd," ");	
	int send_i = get_index_byip(ipto);
	int my_i = get_index_bysockfd(i);
	strcat(snd,a[my_i]->ip);
	int sndlen = strlen(snd);
	snd[sndlen] = '\0'; 
	if(send_i!=-1) 
		{
		struct list_elem *e1;
		for(e1=list_begin(&a[my_i]->block_list);e1!=NULL;e1=list_next(e1))
		{
		struct block_ips *la1 = list_entry (e1, struct block_ips, elem);
		if(strcmp(la1->bip,a[send_i]->ip)==0)
		{
		isblkd = true;
		}
		}
		}	

	if(send_i!=-1) 
		{
		if(a[send_i]->client_status != 0)
		{
		if(!isblkd)
			{++a[send_i]->rcount;
			send(a[send_i]->sock,snd,sndlen,0);
			cse4589_print_and_log("[RELAYED:SUCCESS]\n");
			cse4589_print_and_log("msg from:%s, to:%s\n[msg]:%s\n",a[my_i]->ip, a[send_i]->ip, msg);
			cse4589_print_and_log("[RELAYED:END]\n");
			}
		}
		else
		{
		if(!isblkd)
		{ 
		struct buff_msgs *bm = malloc(sizeof(struct buff_msgs));
		strcpy(bm->bmsg,snd);
		bm->valid=1;
		list_insert(list_end(&a[send_i]->msgbuf),&bm->elem);
		}
		}
		++a[my_i]->scount;
		send(i,"SNDS ",5,0);		
		}
	else
		{
		send(i,"SNDF ",5,0);
		}	
	}
	
	else if(!strcmp(rcomand,"BRD"))
	{
	char snd[500];
	strcpy(snd,"RCV ");
	char *msg = strtok(rrest," \n");
	strcat(snd,msg);
	strcat(snd," ");	
	int my_i = get_index_bysockfd(i);
	strcat(snd,a[my_i]->ip);
	int sndlen = strlen(snd);
	snd[sndlen] = '\0';
	cse4589_print_and_log("[RELAYED:SUCCESS]\n");
	cse4589_print_and_log("msg from:%s, to:255.255.255.255\n[msg]:%s\n",a[my_i]->ip, msg);
	cse4589_print_and_log("[RELAYED:END]\n");
	struct list_elem *e;
	++a[my_i]->scount;
	for(e=list_begin(&loginlist);e!=NULL;e=list_next(e))
	{
	struct client *c = list_entry(e, struct client, elem);		
		if(c->sock!=i && c->sock!=0)
		{
		bool isblkd = false;		
		struct list_elem *e1;
		for(e1=list_begin(&a[my_i]->block_list);e1!=NULL;e1=list_next(e1))
		{
		struct block_ips *la1 = list_entry (e1, struct block_ips, elem);
		if(strcmp(la1->bip,c->ip)==0)
		{
		isblkd = true;
		}
		}
			if(c->client_status != 0)
			{
			if(!isblkd){++c->rcount;send(c->sock,snd,sndlen,0);}
			}
			else
			{
			if(!isblkd)
			{
			struct buff_msgs *bm = malloc(sizeof(struct buff_msgs));
			strcpy(bm->bmsg,snd);
			bm->valid=1;
			list_insert(list_end(&c->msgbuf),&bm->elem);
			}
			}
		}
	}
	send(i,"BRS ",4,0);
	}
	else if(!strcmp(rcomand,"LOT"))
	{
	int ind = get_index_bysockfd(i);
		if(ind!=-1)
		{
			a[ind]->client_status = 0;
		}	
	}

	else if(strcmp(rcomand,"EXT")==0)
	{
	close(i);
	FD_CLR(i,&master);	
	struct list_elem *e;
	for(e=list_begin(&loginlist);e!=NULL;e=list_next(e))
	{
	struct client *la1 = list_entry (e, struct client, elem);
	if(la1->sock==i)
	{
	free(a[la1->index]);
	list_remove(e);
	}
	}	
	}

}
}
else if(i==0)
{
fgets(in2,300,stdin);
int in2len = strlen(in2);
in2[in2len] = '\0';
char *res;
char *ar = strtok_r(in2," \n",&res);
if(!strcmp(ar,"IP"))
{
char s_ip[INET_ADDRSTRLEN];
myip(s_ip);
cse4589_print_and_log("[IP:SUCCESS]\n");
cse4589_print_and_log("IP:%s\n",s_ip);
cse4589_print_and_log("[IP:END]\n");
} 
else if(!strcmp(ar,"PORT"))
{
cse4589_print_and_log("[PORT:SUCCESS]\n");
cse4589_print_and_log("PORT:%s\n",port);
cse4589_print_and_log("[PORT:END]\n");
}

else if(strcmp(ar,"AUTHOR")==0)
	{
	cse4589_print_and_log("[AUTHOR:SUCCESS]\n");
	cse4589_print_and_log("I, tusharpr, have read and understood the course academic integrity policy.\n");
	cse4589_print_and_log("[AUTHOR:END]\n");
	}
else if(!strcmp(ar,"LIST"))
{
char login_string[600];
get_login_clients(login_string);
cse4589_print_and_log("[LIST:SUCCESS]\n");
int i=0;
char *elem; 
strtok_r(login_string," \n",&elem);
char *lname;
for(i=1;;i++)
{
if(i==1)lname = strtok(elem," \n");
else lname = strtok(NULL," \n");
if(strcmp(lname,"end")==0) break;
char *lip = strtok(NULL," \n");
char *lport = strtok(NULL," \n");
cse4589_print_and_log("%-5d%-35s%-20s%-8d\n",i,lname,lip,atoi(lport));
}
cse4589_print_and_log("[LIST:END]\n");
}

else if(strcmp(ar,"STATISTICS")==0)
	{
	char stats[600];
	get_stats(stats);
	cse4589_print_and_log("[STATISTICS:SUCCESS]\n");
	int i=0;
	char *elem; 
	strtok_r(stats," \n",&elem);
	char *lname;
	for(i=1;;i++)
	{
	if(i==1)lname = strtok(elem," \n");
	else lname = strtok(NULL," \n");
	if(strcmp(lname,"end")==0) break;
	char *ls = strtok(NULL," \n");
	char *lr = strtok(NULL," \n");
	char *status = strtok(NULL," \n");
	cse4589_print_and_log("%-5d%-35s%-8d%-8d%-8s\n", i, lname, atoi(ls), atoi(lr), status);
	}
	cse4589_print_and_log("[STATISTICS:END]\n");
	}

else if(strcmp(ar,"EXIT")==0)
{
cse4589_print_and_log("[EXIT:SUCCESS]\n");
cse4589_print_and_log("[EXIT:END]\n");
return 0;
}
else if(strcmp(ar,"BLOCKED")==0)
	{
		char blkd[200];
		get_blocked_byip(res,blkd);
		int i=0;
		int al[5]={-1,-1,-1,-1,-1};
		char *elem; 
		strtok_r(blkd," \n",&elem);
		char *blip;
		if(strcmp(strtok(elem," \n"),"valid")==0)
		{
		cse4589_print_and_log("[BLOCKED:SUCCESS]\n");
		for(i=0;;)
		{
		blip = strtok(NULL," \n");
		if(strcmp(blip,"end")==0)break;
		int in = get_index_byip(blip);
		if(in!=-1)
			{
			al[i] = in;	
			++i;
			}
		}
		struct list_elem *e;
		int j =1;		
		for(e=list_begin(&loginlist);e!=NULL;e=list_next(e))
		{
		struct client *la1 = list_entry (e, struct client, elem);
			for(i = 0;i<5;i++)
			{
				if(la1->index == al[i] && al[i]!=-1)
					{
					cse4589_print_and_log("%-5d%-35s%-20s%-8d\n",j++,a[al[i]]->name,a[al[i]]->ip,a[al[i]]->cport);
					al[i]=-1;
					}
			}

		}
	cse4589_print_and_log("[BLOCKED:END]\n");
	}
else
{
cse4589_print_and_log("[BLOCKED:ERROR]\n");
cse4589_print_and_log("[BLOCKED:END]\n");
}
}


}
}
}
}
return 0;
}	
return 0;
}

int myip(char *ipstr)
{
struct addrinfo hints,*res,*p;
int status;
char name[50];
if(gethostname(name,sizeof(name))==-1)
{
perror("Error in gethostname()");
return -1;
}
memset(&hints,0,sizeof(hints));
hints.ai_family=AF_INET;
hints.ai_socktype=SOCK_DGRAM;
if(getaddrinfo(name,NULL,&hints,&res)!=0)
{
        fprintf(stderr,"getaddrinfo error: %s\n",gai_strerror(status));
        return -1;
}
for(p=res;p!=NULL;p=p->ai_next)
{
void *addr;
if(p->ai_family==AF_INET)

{
struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
addr = &(ipv4->sin_addr);
}
else
{
continue;
}

if(inet_ntop(p->ai_family,addr,ipstr,INET_ADDRSTRLEN)<=0)
{
printf("error in inet_ntop\n");
return -1;
}
freeaddrinfo(res);
return 1;
}
freeaddrinfo(res);
return -1;
}

void get_login_clients(char *login_string)
{
//memset(login_string,0,strlen(login_string));
strcpy(login_string,"LIS ");
struct list_elem *e;
for(e=list_begin(&loginlist);e!=NULL;e=list_next(e))
{
struct client *la1 = list_entry (e, struct client, elem);
if(la1->client_status == 1)
{
strcat(login_string,la1->name);
strcat(login_string," ");
strcat(login_string,la1->ip);
strcat(login_string," ");
strcat(login_string,la1->cport_s);
strcat(login_string," ");
}
}
strcat(login_string," end");
int len = strlen(login_string);
login_string[len]='\0';
}

void get_login_clients_log(char *login_string)
{
//memset(login_string,0,strlen(login_string));
strcpy(login_string,"LOG ");
struct list_elem *e;
for(e=list_begin(&loginlist);e!=NULL;e=list_next(e))
{
struct client *la1 = list_entry (e, struct client, elem);
if(la1->client_status == 1)
{
strcat(login_string,la1->name);
strcat(login_string," ");
strcat(login_string,la1->ip);
strcat(login_string," ");
strcat(login_string,la1->cport_s);
strcat(login_string," ");
}
}
strcat(login_string," end");
int len = strlen(login_string);
login_string[len]='\0';
}
void get_stats(char *login_string)
{
strcpy(login_string,"STS ");
struct list_elem *e;
for(e=list_begin(&loginlist);e!=NULL;e=list_next(e))
{
struct client *la1 = list_entry (e, struct client, elem);
if(la1->cport!=0)
{
char snd[5];
sprintf(snd, "%d", la1->scount);
char rcv[5];
sprintf(rcv, "%d", la1->rcount);
strcat(login_string,la1->name);
strcat(login_string," ");
strcat(login_string,snd);
strcat(login_string," ");
strcat(login_string,rcv);
strcat(login_string," ");
if(la1->client_status == 1){strcat(login_string,"online");}
else {strcat(login_string,"offline");}
strcat(login_string," ");
}
}
strcat(login_string," end");
int len = strlen(login_string);
login_string[len]='\0';
}
int get_index_byip(char *ip)
{
struct list_elem *e;
for(e=list_begin(&loginlist);e!=NULL;e=list_next(e))
{
struct client *la1 = list_entry (e, struct client, elem);
if(!strcmp(ip,la1->ip))
{
return la1->index;
}
}
return -1;
}

int in_login_list(char *ip)
{
struct list_elem *e;
for(e=list_begin(&loginlist);e!=NULL;e=list_next(e))
{
struct client *la1 = list_entry(e, struct client, elem);
if(!strcmp(ip,la1->ip))
{
return 1;
}
}
return -1;
}

void get_blocked_byip(char *ip,char *s)
{
strcpy(s,"BLKD ");
struct list_elem *e;
for(e=list_begin(&loginlist);e!=NULL;e=list_next(e))
{
struct client *la1 = list_entry (e, struct client, elem);
if(!strcmp(ip,la1->ip)==0)
{
strcat(s,"valid ");
	struct list_elem *e1;
	for(e1=list_begin(&la1->block_list);e1!=NULL;e1=list_next(e1))
	{
			struct block_ips *la2 = list_entry (e1, struct block_ips, elem);
			strcat(s,la2->bip);
			strcat(s," ");
	}
}
}
strcat(s," end");
int l = strlen(s);
s[l] = '\0';
}

int get_index_bysockfd(int sfd)
{
struct list_elem *e;
for(e=list_begin(&loginlist);e!=NULL;e=list_next(e))
{
struct client *la1 = list_entry (e, struct client, elem);
if(sfd==la1->sock)
{
return la1->index;
}
}
return -1;
}
