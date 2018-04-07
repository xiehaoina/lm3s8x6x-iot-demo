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
int linux_write(Network* network , unsigned char* bug, int len, int timeout)
{}

void NetworkInit(Network* network)
{}
int NetworkConnect(Network* network, char* addr, int port)
{}
void NetworkDisconnect(Network* network){}