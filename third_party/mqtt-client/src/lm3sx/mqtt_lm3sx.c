/*******************************************************************************
 * Copyright (c) 2014 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Allan Stockdill-Mander - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "mqtt_lm3sx.h"



void TimerInit(Timer* timer)
{}

char TimerIsExpired(Timer* timer)
{}

void TimerCountdownMS(Timer* timer, unsigned int ms)
{}

void TimerCountdown(Timer*  timer, unsigned int second){}

int TimerLeftMS(Timer* ms){}



int linux_read(Network* network , unsigned char* buf, int len , int timeout)
{}

int linux_write(Network* network , unsigned char* buf, int len, int timeout)
{
  err_t err;
  if (network->pcb == NULL){
    return 0;
  }
  
  /* We cannot send more data than space available in the send
     buffer. */
  if (tcp_sndbuf(network->pcb) < len) {
    len = tcp_sndbuf(network->pcb);
  }
  
  if(len > (2*network->pcb->mss))
  {
    len = 2*network->pcb->mss;
  }
  
  
  do {
    err = tcp_write(network->pcb, buf, len, 0);
    if (err == ERR_MEM) {
      len /= 2;
    }
  } while (err == ERR_MEM && len > 1);
  
  if (err != ERR_OK || ERR_OK != tcp_output(network->pcb))
    return 0;
  else
    return 1;
}

void NetworkInit(Network* network)
{
  network->pcb = tcp_new();
  network->mqttread = linux_read;
  network->mqttwrite = linux_write;
}

static void
conn_err(void *arg, err_t err)
{
  Network *network;

  if(arg)
  {
      network = (Network *)arg;
      /*
      if(false) {
        fs_close(hs->handle);
        hs->handle = NULL;
      }
      mem_free(hs);
      */
  }
}

static void
close_conn( Network* network)
{
  tcp_arg(network->pcb, NULL);
  tcp_sent(network->pcb, NULL);
  tcp_recv(network->pcb, NULL);
  /*
  if(hs->handle) {
    fs_close(hs->handle);
    hs->handle = NULL;
  }
  mem_free(hs);
  */
  tcp_close(network->pcb);
}


/*-----------------------------------------------------------------------------------*/
static err_t
mqtt_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
  int i;
  char *data;
  Network *network;
  network = arg;
  if (err == ERR_OK && p != NULL) {
      /* Inform TCP that we have taken the data. */
      tcp_recved(pcb, p->tot_len);
  } else {
      pbuf_free(p);
  }

  if (err == ERR_OK && p == NULL) {
    close_conn(network);
  }
  return ERR_OK;
}

int NetworkConnect(Network* network, char* addr, int port)
{
  struct ip_addr remote_ip_addr;
  if(network->pcb != NULL){
    IP4_ADDR( &remote_ip_addr, 192, 168, 115, 130 ); 
    tcp_bind(network->pcb, IP_ADDR_ANY, 9999);
    tcp_connect(network->pcb,&remote_ip_addr,port,NULL);
    tcp_arg(network->pcb, network);
    /* Tell TCP that we wish to be informed of incoming data by a call
     to the http_recv() function. */
    tcp_recv(network->pcb, mqtt_recv);
    tcp_err(network->pcb, conn_err);
    //tcp_poll(network->pcb, mqtt_poll, 4);
    return 1;
  }else{
    return 0;    
  }
}

void NetworkDisconnect(Network* network){
   close_conn(network);
}