/*

Copyright (C) 2013 Matevž Bošnak (matevz@poscope.com)

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

*/

#include "PoKeysLib.h"
#include "PoKeysLibCore.h"

#ifdef WIN32
    #include "windows.h"
   // #include "Winsock.h"
    #include "conio.h"
    #include <iphlpapi.h>
#else
    #include <unistd.h>
    #include <stdlib.h>
    #include <sys/socket.h>
    #include <netdb.h>
    #include <netinet/in.h>
    #include <net/if.h>
    #include <sys/ioctl.h>
#endif
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "PoKeysLib.h"

//#define DEBUG_PoKeysLibSockets

#ifdef DEBUG_PoKeysLibSockets
    #define debug_printf printf
#else
    #define debug_printf (void)
#endif


// Get a list of interfaces - source: Jeremy @ http://developerweb.net/viewtopic.php?id=5085
typedef unsigned long uint32;

#if defined(__FreeBSD__) || defined(BSD) || defined(__APPLE__) || defined(__linux__)
    #define USE_GETIFADDRS 1
    #define SOCKADDR struct sockaddr

    #include <ifaddrs.h>

    uint32 SockAddrToUint32(struct sockaddr * a)
    {
       return ((a)&&(a->sa_family == AF_INET)) ? ntohl(((struct sockaddr_in *)a)->sin_addr.s_addr) : 0;
    }
#endif

uint32 * GetBroadcastAddresses()
{
    uint32 * list = malloc(sizeof(uint32) * 100);
    uint32* ptr = list;
#if defined(WIN32)
    uint32 i;
    MIB_IPADDRTABLE * ipTable = NULL;
    IP_ADAPTER_INFO * pAdapterInfo = NULL;
    ULONG bufLen = 0;
    uint32 ipRet;
    uint32 apRet;

    MIB_IPADDRROW * row;

    uint32 ipAddr;
    uint32 netmask;
    uint32 baddr;

#endif
#if defined(USE_GETIFADDRS)
    uint32 tmp;
#endif

    *ptr = 0;
    

#if defined(USE_GETIFADDRS)
    // BSD-style implementation
    struct ifaddrs * ifap;
    if (getifaddrs(&ifap) == 0)
    {
        struct ifaddrs * p = ifap;
        while(p)
        {
            uint32 ifaAddr  = SockAddrToUint32(p->ifa_addr);

            if (ifaAddr > 0)
            {
				tmp = ((struct sockaddr_in *)p->ifa_broadaddr)->sin_addr.s_addr;
				if (tmp) *(ptr++) = tmp;
			}
            p = p->ifa_next;
        }
        freeifaddrs(ifap);
    }
#elif defined(WIN32)
    // Windows XP style implementation

    // Adapted from example code at http://msdn2.microsoft.com/en-us/library/aa365917.aspx
    // Now get Windows' IPv4 addresses table.  Once again, we gotta call GetIpAddrTable()
    // multiple times in order to deal with potential race conditions properly.
    {
      for (i=0; i<5; i++)
      {
         ipRet = GetIpAddrTable(ipTable, &bufLen, 0);
         if (ipRet == ERROR_INSUFFICIENT_BUFFER)
         {
            free(ipTable);  // in case we had previously allocated it
            ipTable = (MIB_IPADDRTABLE *) malloc(bufLen);
         }
         else if (ipRet == NO_ERROR) break;
         else
         {
            free(ipTable);
            ipTable = NULL;
            break;
         }
     }
   }

   if (ipTable)
   {
      // Try to get the Adapters-info table, so we can given useful names to the IP
      // addresses we are returning.  Gotta call GetAdaptersInfo() up to 5 times to handle
      // the potential race condition between the size-query call and the get-data call.
      // I love a well-designed API :^P
      {
         for (i=0; i<5; i++)
         {
            apRet = GetAdaptersInfo(pAdapterInfo, &bufLen);
            if (apRet == ERROR_BUFFER_OVERFLOW)
            {
               free(pAdapterInfo);  // in case we had previously allocated it
               pAdapterInfo = (IP_ADAPTER_INFO *) malloc(bufLen);
            }
            else if (apRet == ERROR_SUCCESS) break;
            else
            {
               free(pAdapterInfo);
               pAdapterInfo = NULL;
               break;
            }
         }
      }

      for (i=0; i < ipTable->dwNumEntries; i++)
      {
          // -----------------------------------------------------------------------------------------
         row = &ipTable->table[i];
         // -----------------------------------------------------------------------------------------

         ipAddr  = (row->dwAddr);
         netmask = (row->dwMask);
         baddr   = ipAddr & netmask;
         if (row->dwBCastAddr) baddr |= ~netmask;

		 if (baddr) *(ptr++) = baddr;
      }

      free(pAdapterInfo);
      free(ipTable);
   }
#else
   // Dunno what we're running on here!
#  error "Don't know how to implement GetBroadcastAddresses() on this OS!"
#endif

	*ptr = 0;
    return list;
}

#ifdef WIN32
    WSADATA wsaData;

    int32_t InitWinsock()
    {
            // Initialize Winsock - version 2.2
            if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
            {
                    debug_printf("Winsock startup failed!\n");
                    return -1;
            }
            return 0;
    }

    int32_t TerminateWinsock()
    {
            WSACleanup();
            return 0;
    }
#endif


int32_t PK_EnumerateNetworkDevices(sPoKeysNetworkDeviceSummary * devices, uint32_t timeout)
{
    return PK_SearchNetworkDevices(devices, timeout, 0);
}

int32_t PK_SearchNetworkDevices(sPoKeysNetworkDeviceSummary * devices, uint32_t timeout, uint32_t serialNumberToFind)
{
    //Broadcast the message
    int32_t t; // 100 ms timeout
#ifdef WIN32
    struct sockaddr_in remoteEP;
    SOCKET txSocket;
#else
    int txSocket;
    struct sockaddr_in remoteEP;
    fd_set fds;
    struct timeval stimeout;
#endif
    int UDPbroadcast = 1;
    int status = 0;
    int BufLen = 0;
    char SendBuf[1];
    uint32 * addr;
    uint32 * addrPtr;
    uint32 a;
    unsigned char rcvbuf[500];
    int nrOfDetectedBoards = 0;
    sPoKeysNetworkDeviceSummary * device;

    t = timeout;

    debug_printf("Enumerating network PoKeys devices for %d ms...\n", timeout);

#ifdef WIN32
	if (InitWinsock() != 0) 
	{
		debug_printf("InitWinsock error!");
		return 0;
    }
#endif
	
    // Create socket for discovery packet
    if ((txSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        debug_printf("Error creating socket\n");
        return 0;
    }
    if (setsockopt( txSocket, SOL_SOCKET, SO_BROADCAST, (char *)&UDPbroadcast, sizeof UDPbroadcast) == -1)
    {
        debug_printf("Error setting broadcast option\n");
        return 0;
    }

#ifdef WIN32
    if (setsockopt( txSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&t, sizeof(t)) == -1)
    {
        debug_printf("Error setting SO_RCVTIMEO option\n");
        return 0;
    }
    if (setsockopt( txSocket, SOL_SOCKET, SO_SNDTIMEO, (char *)&t, sizeof(t)) == -1)
    {
        debug_printf("Error setting SO_SNDTIMEO option\n");
        return 0;
    }
#endif


    debug_printf("Sending discovery request...\n");

    addr = GetBroadcastAddresses();
    addrPtr = addr;

    while(*addr)
    {
        a = *addr;
        debug_printf("Sending request to %d.%d.%d.%d... ", (a&0xFF),(a>>8)&0xFF, (a>>16)&0xFF, (a>>24)&0xFF);

        // Send discovery request...
        remoteEP.sin_family = AF_INET;
        remoteEP.sin_port = htons(20055);
        remoteEP.sin_addr.s_addr = a; // inet_addr("255.255.255.255");

        if (sendto(txSocket, SendBuf, BufLen, 0, (SOCKADDR *)&remoteEP, sizeof(remoteEP)) == -1)
        {
            debug_printf("Failed\n");
#ifdef WIN32
            closesocket(txSocket);
            WSACleanup();
#else
            close(txSocket);
#endif
            return 0;
        }
        debug_printf(" done\n");
        addr++;
    }
    free(addrPtr);


    debug_printf("Waiting for responses...\n");



#ifdef WIN32

#else
    // Wait for discovery response...
    stimeout.tv_sec = 0;
    stimeout.tv_usec = 1000 * timeout;

    FD_ZERO(&fds);
    FD_SET(txSocket, &fds);
#endif


    while(nrOfDetectedBoards < 16)
    {
        // Receive response from devices
#ifdef WIN32
#else
        if (select(txSocket + 1, &fds, NULL, NULL, &stimeout) < 0)
        {
            debug_printf("Error in select...\n");
            close(txSocket);
            return 0;
        }

        if (FD_ISSET(txSocket, &fds) == 0) break;
#endif

        debug_printf("Retrieving data...\n");
        status = recv(txSocket, (char *)rcvbuf, sizeof(rcvbuf), 0);

        if (status < 0)
        {
#ifdef WIN32
            int iError = WSAGetLastError();

            if (iError != WSAECONNRESET) {
                debug_printf("Error receiving data %d / %ld...\n", status, iError);
                break;
            }
#else
            break;
#endif
        }

        // Get IP address and receive message
        if (status > 0)
        {
            if (status == 14)
            {
                // Parse the response
                debug_printf("Received response...\n");

                debug_printf("  User ID: %u\n", rcvbuf[0]);
                debug_printf("  Serial: %u\n", (int)(256 * (int)rcvbuf[1] + rcvbuf[2]));
                debug_printf("  Version: %u.%u\n", rcvbuf[3], rcvbuf[4]);
                debug_printf("  IP address: %u.%u.%u.%u\n", rcvbuf[5], rcvbuf[6], rcvbuf[7], rcvbuf[8]);
                debug_printf("  DHCP: %u\n", rcvbuf[9]);
                debug_printf("  Host IP address: %u.%u.%u.%u\n", rcvbuf[10], rcvbuf[11], rcvbuf[12], rcvbuf[13]);

                // Save the device info
                device = &devices[nrOfDetectedBoards];
                //device->userID = rcvbuf[0];
                device->SerialNumber = (int)(256 * (int)rcvbuf[1] + rcvbuf[2]);
                device->FirmwareVersionMajor = rcvbuf[3];
                device->FirmwareVersionMinor = rcvbuf[4];
                memcpy(device->IPaddress, rcvbuf + 5, 4);
                device->DHCP = rcvbuf[9];
                memcpy(device->hostIP, rcvbuf + 10, 4);
                device->HWtype = 0;

                nrOfDetectedBoards++;
                status = 0;

                if (serialNumberToFind == device->SerialNumber) break;
            } else if (status == 19)
            {
                // Parse the response
                debug_printf("Received response from 58 series device...\n");

                // Save the device info
                device = &devices[nrOfDetectedBoards];
                //device->userID = rcvbuf[0];
                device->SerialNumber = (int)rcvbuf[14] + ((int)rcvbuf[15] << 8) + ((int)rcvbuf[16] << 16) + ((int)rcvbuf[17] << 24);
                device->FirmwareVersionMajor = rcvbuf[3];
                device->FirmwareVersionMinor = rcvbuf[4];
                memcpy(device->IPaddress, rcvbuf + 5, 4);
                device->DHCP = rcvbuf[9];
                memcpy(device->hostIP, rcvbuf + 10, 4);
                device->HWtype = rcvbuf[18];

                debug_printf("  User ID: %u\n", rcvbuf[0]);
                debug_printf("  Serial: %u\n", device->SerialNumber);
                debug_printf("  Version: %u.%u\n", rcvbuf[3], rcvbuf[4]);
                debug_printf("  IP address: %u.%u.%u.%u\n", rcvbuf[5], rcvbuf[6], rcvbuf[7], rcvbuf[8]);
                debug_printf("  DHCP: %u\n", rcvbuf[9]);
                debug_printf("  Host IP address: %u.%u.%u.%u\n", rcvbuf[10], rcvbuf[11], rcvbuf[12], rcvbuf[13]);

                nrOfDetectedBoards++;
                status = 0;

                if (serialNumberToFind == device->SerialNumber) break;
            }
        }
		else
		{
			if (nrOfDetectedBoards == 0)
			{
				debug_printf("\n No Boards detected\n");
			}
		}


    }


#ifdef WIN32
    closesocket(txSocket);
#else
    close(txSocket);
#endif

    return nrOfDetectedBoards;
}

sPoKeysDevice* PK_ConnectToNetworkDevice(sPoKeysNetworkDeviceSummary * device)
{
    int result;
#ifdef WIN32
    int t = 100; // 100 ms timeout
#endif
    // Create target endpoint
    //struct sockaddr_in * remoteEP;
    uint32 addr ;

    // Create temporary device object
    sPoKeysDevice * tmpDevice;

    if (device == NULL) return NULL;

    tmpDevice = (sPoKeysDevice*)malloc(sizeof(sPoKeysDevice));
    tmpDevice->devHandle2 = malloc(sizeof(struct sockaddr_in));

    tmpDevice->connectionType = PK_DeviceType_NetworkDevice; // Network device
    tmpDevice->connectionParam = device->useUDP;

    addr = (uint32)device->IPaddress[0] + ((uint32)device->IPaddress[1] << 8) + ((uint32)device->IPaddress[2] << 16) + ((uint32)device->IPaddress[3] << 24);
    // Set up the RecvAddr structure with the broadcast ip address and correct port number
    ((struct sockaddr_in*)tmpDevice->devHandle2)->sin_family = AF_INET;
    ((struct sockaddr_in*)tmpDevice->devHandle2)->sin_port = htons(20055);
    ((struct sockaddr_in*)tmpDevice->devHandle2)->sin_addr.s_addr = addr;

#ifdef WIN32    
    if (InitWinsock() != 0)
    {
        debug_printf("InitWinsock error!");
        return 0;
    }


    if (tmpDevice->connectionParam == PK_ConnectionParam_UDP)
    {
        tmpDevice->devHandle = (void*)socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    } else
    {
        tmpDevice->devHandle = (void*)socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    }

    if ((SOCKET)tmpDevice->devHandle == -1)
#else
    tmpDevice->devHandle = malloc(sizeof(int));
    if (tmpDevice->devHandle == NULL) return NULL;
    
    if (tmpDevice->connectionParam == PK_ConnectionParam_UDP)
        *(int *)tmpDevice->devHandle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    else
        *(int *)tmpDevice->devHandle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	if ((*(int *)tmpDevice->devHandle) == -1)
#endif
    {
        //CleanDevice(tmpDevice);
#ifndef WIN32
        free(tmpDevice->devHandle);
#endif
        free(tmpDevice);

        return NULL; // Couldn't create the socket
    }

#ifdef WIN32
	// Set socket options
	setsockopt((SOCKET)tmpDevice->devHandle, SOL_SOCKET, SO_RCVTIMEO, (char *)&t, sizeof(t));
    setsockopt((SOCKET)tmpDevice->devHandle, SOL_SOCKET, SO_SNDTIMEO, (char *)&t, sizeof(t));
#endif


	// Connect to target IP
#ifdef WIN32
    if (tmpDevice->connectionParam != PK_ConnectionParam_UDP)
        result = connect((SOCKET)tmpDevice->devHandle, (SOCKADDR *)tmpDevice->devHandle2, sizeof(struct sockaddr_in));
    else
        result = 0;
#else
	result = 0;
    if (tmpDevice->connectionParam != PK_ConnectionParam_UDP)
        result = connect(*(int *)tmpDevice->devHandle, (struct sockaddr *)tmpDevice->devHandle2, sizeof(struct sockaddr_in));
	else
        result = 0;
		
#endif

    if (result == -1)
    {
		debug_printf(" ERROR");
        //CleanDevice(tmpDevice);
#ifdef WIN32
        closesocket((SOCKET)tmpDevice->devHandle);
#else
        close(*(int *)tmpDevice->devHandle);
        free(tmpDevice->devHandle);
#endif
        free(tmpDevice);

        return NULL; // Couldn't connect
    }

#ifdef WIN32
    if (tmpDevice->connectionParam == PK_ConnectionParam_UDP)
    {
        // Setup socket to be unblocking
        u_long iMode = TRUE;
        ioctlsocket((SOCKET)tmpDevice->devHandle, FIONBIO, &iMode);
    }
#endif

	debug_printf(" Connected\n");

    debug_printf("Initializing the device object... ");
    InitializeNewDevice(tmpDevice);
    debug_printf("done\n");


	//printf("Connected to PoKeys device at %s", addrBuf);
	return tmpDevice;
}

void PK_DisconnectNetworkDevice(sPoKeysDevice* device)
{
    if (device == NULL) return;
	if (device->connectionType != PK_DeviceType_NetworkDevice) return;

    debug_printf("\nClosing connection...");

#ifdef WIN32
    if ((SOCKET)device->devHandle)
		closesocket((SOCKET)device->devHandle);    
#else
    close(*(int *)device->devHandle);
    free(device->devHandle);
#endif

    // Release secondary device handle
    if (device->devHandle2)
        free(device->devHandle2);
}


int32_t SendEthRequest(sPoKeysDevice* device)
{    
    uint32_t retries1 = 0;
    uint32_t retries2 = 0;
    int result;

    fd_set fds;
    struct timeval stimeout;

    if (device == NULL) return PK_ERR_GENERIC;
    if (device->connectionType != PK_DeviceType_NetworkDevice) return PK_ERR_GENERIC;
    if (device->devHandle == NULL) return PK_ERR_GENERIC;

    while (1)
    {
        // Form the request packet
        device->requestID++;

        device->request[0] = 0xBB;
        device->request[6] = device->requestID;
        device->request[7] = getChecksum(device->request);

        //memcpy(requestBuffer, device->request, 64);

        debug_printf("\nSending...");
        // Send the data

        if (device->connectionParam == PK_ConnectionParam_UDP)
        {
#ifdef WIN32
            if (sendto((SOCKET)device->devHandle, (char *)device->request, 64, 0, (SOCKADDR *)device->devHandle2, sizeof(struct sockaddr_in)) == -1)
#else
            if (sendto(*(int*)device->devHandle, (char *)device->request, 64, 0, (SOCKADDR *)device->devHandle2, sizeof(struct sockaddr_in)) == -1)
#endif
            {
                debug_printf("Error sending UDP report\nAborting...\n");
                return -1;
            }
        } else
        {
#ifdef WIN32
            if (send((SOCKET)device->devHandle, (char *)device->request, 64, 0) != 64)
#else
            if (send(*(int*)device->devHandle, (char *)device->request, 64, 0) != 64)
#endif
            {
                debug_printf("Error sending TCP report\nAborting...\n");
                return -1;
            }
        }

		// Wait for the response
		while(1)
		{           
#ifdef WIN32
            FD_ZERO(&fds);
            FD_SET((SOCKET)device->devHandle, &fds);

            stimeout.tv_sec = 0;
			stimeout.tv_usec = 1000 * device->socketTimeout;

            result = select((SOCKET)device->devHandle + 1, &fds, NULL, NULL, &stimeout);

            if (result == 0 || result == -1)
            {
                // Timeout...
                debug_printf("Timeout!");
                if (++retries1 > device->readRetries) break;
                continue;
            }

            //device->reserved64 = GetTickCount64();
            result = recv((SOCKET)device->devHandle, (char *)device->response, 64, 0);
            //device->reserved64 = GetTickCount64() - device->reserved64;
            //printf("Time: %u\n", (uint32_t)device->reserved64);
#else
            FD_ZERO(&fds);
            FD_SET(*(int*)device->devHandle, &fds);

            stimeout.tv_sec = 0;
            stimeout.tv_usec = 1000 * device->socketTimeout;

            result = select(*(int*)device->devHandle + 1, &fds, NULL, NULL, &stimeout);

            if (result == 0 || result == -1)
            {
                // Timeout...
                debug_printf("Timeout!");
                if (++retries1 > 10) break;
                continue;
            }

            //if (FD_ISSET(*(int*)device->devHandle, &fds) == 0)

            result = recv(*(int*)device->devHandle, (char *)device->response, 64, 0);
#endif

			// 64 bytes received?
			if (result == 64)
			{
				if (device->response[0] == 0xAA && device->response[6] == device->requestID)
				{
					if (device->response[7] == getChecksum(device->response))
					{
						debug_printf(" Received!");
						return PK_OK;
					} else 
					{
						debug_printf("!! Wrong checksum...");
					}
				} else
				{
					debug_printf("!! Wrong response received!");
					break;
				}
			}
			else if (result == 0)
				debug_printf("Connection closed\n");
			else
#ifdef WIN32
                debug_printf("recv failed: %d\n", WSAGetLastError());
#else
                debug_printf("recv failed\n");
#endif

            
            if (++retries1 > device->readRetries) break;
		}

        if (retries2++ > device->sendRetries) break;
    }

	debug_printf("Error - timeout...");
	return PK_ERR_TRANSFER;

}

int32_t SendEthRequest_NoResponse(sPoKeysDevice* device)
{    
    uint32_t retries1 = 0;
    uint32_t retries2 = 0;
    //int result;

    //fd_set fds;
    //struct timeval stimeout;

    if (device == NULL) return PK_ERR_GENERIC;
    if (device->connectionType != PK_DeviceType_NetworkDevice) return PK_ERR_GENERIC;
    if (device->devHandle == NULL) return PK_ERR_GENERIC;

    while (1)
    {
        // Form the request packet
        device->requestID++;

        device->request[0] = 0xBB;
        device->request[6] = device->requestID;
        device->request[7] = getChecksum(device->request);

        // Send the data
        if (device->connectionParam == PK_ConnectionParam_UDP)
        {
#ifdef WIN32
            if (sendto((SOCKET)device->devHandle, (char *)device->request, 64, 0, (SOCKADDR *)device->devHandle2, sizeof(struct sockaddr_in)) == -1)
#else
            if (sendto(*(int*)device->devHandle, (char *)device->request, 64, 0, (SOCKADDR *)device->devHandle2, sizeof(struct sockaddr_in)) == -1)
#endif
            {
                debug_printf("Error sending UDP report\nAborting...\n");
                return -1;
            }
        } else
        {
#ifdef WIN32
            if (send((SOCKET)device->devHandle, (char *)device->request, 64, 0) != 64)
#else
            if (send(*(int*)device->devHandle, (char *)device->request, 64, 0) != 64)
#endif
            {
                debug_printf("Error sending TCP report\nAborting...\n");
                return -1;
            }
        }
		
        if (retries2++ > device->sendRetries) break;
    }

	debug_printf("Error - timeout...");
	return PK_ERR_TRANSFER;

}

int32_t SendEthRequestBig(sPoKeysDevice* device)
{
    uint32_t retries1 = 0;
    uint32_t retries2 = 0;
    int result;
	uint32_t i;

    fd_set fds;
    struct timeval stimeout;

    uint8_t * requestBuffer;
    uint8_t * requestBufferPtr = 0;

    if (device == NULL) return PK_ERR_GENERIC;
    if (device->connectionType != PK_DeviceType_NetworkDevice) return PK_ERR_GENERIC;
    if (device->devHandle == NULL) return PK_ERR_GENERIC;

    if (device->multiPartBuffer == 0) return PK_ERR_GENERIC;
    requestBuffer = device->multiPartBuffer;

    while (1)
    {
        // Form the request packet
        for (i = 0; i < 8; i++)
        {
            requestBufferPtr = &requestBuffer[i * 64];

            memcpy(requestBufferPtr, device->request, 8);
            requestBufferPtr[0] = 0xBB;

            // Put packet ID and flags here...
            requestBufferPtr[2] = i;
            if (i == 0)
                requestBufferPtr[2] |= (1<<3);
            else if (i == 7)
                requestBufferPtr[2] |= (1<<4);

            requestBufferPtr[6] = ++device->requestID;
            requestBufferPtr[7] = getChecksum(requestBufferPtr);

            memcpy(requestBufferPtr + 8, device->multiPartData + i*56, 56);
        }

        debug_printf("\nSending...");
        // Send the data

        if (device->connectionParam == PK_ConnectionParam_UDP)
        {
#ifdef WIN32
            if (sendto((SOCKET)device->devHandle, (char *)requestBuffer, 512, 0, (SOCKADDR *)device->devHandle2, sizeof(struct sockaddr_in)) == -1)
#else
            if (sendto(*(int*)device->devHandle, (char *)requestBuffer, 512, 0, (SOCKADDR *)device->devHandle2, sizeof(struct sockaddr_in)) == -1)
#endif
            {
                debug_printf("Error sending UDP report\nAborting...\n");
                return -1;
            }
        } else
        {
#ifdef WIN32
            if (send((SOCKET)device->devHandle, (char *)requestBuffer, 512, 0) != 512)
#else
            if (send(*(int*)device->devHandle, (char *)requestBuffer, 512, 0) != 512)
#endif
            {
                debug_printf("Error sending TCP report\nAborting...\n");
                return -1;
            }
        }

        // Wait for the response
        while(1)
        {
#ifdef WIN32
            FD_ZERO(&fds);
            FD_SET((SOCKET)device->devHandle, &fds);

            stimeout.tv_sec = 0;
            stimeout.tv_usec = 1000 * device->socketTimeout;

            result = select((SOCKET)device->devHandle + 1, &fds, NULL, NULL, &stimeout);

            if (result == 0 || result == -1)
            {
                // Timeout...
                debug_printf("Timeout!");
                if (++retries1 > device->readRetries) break;
                continue;
            }

            result = recv((SOCKET)device->devHandle, (char *)device->response, 64, 0);
#else
            FD_ZERO(&fds);
            FD_SET(*(int*)device->devHandle, &fds);

            stimeout.tv_sec = 0;
            stimeout.tv_usec = 1000 * device->socketTimeout;

            result = select(*(int*)device->devHandle + 1, &fds, NULL, NULL, &stimeout);

            if (result == 0 || result == -1)
            {
                // Timeout...
                debug_printf("Timeout!");
                if (++retries1 > 10) break;
                continue;
            }

            result = recv(*(int*)device->devHandle, (char *)device->response, 64, 0);
#endif

            // 64 bytes received?
            if (result == 64)
            {
                if (device->response[0] == 0xAA && device->response[6] == device->requestID)
                {
                    if (device->response[7] == getChecksum(device->response))
                    {
                        debug_printf(" Received!");
                        return PK_OK;
                    } else
                    {
                        debug_printf("!! Wrong checksum...");
                    }
                } else
                {
                    debug_printf("!! Wrong response received!");
                    break;
                }
            }
            else if (result == 0)
                debug_printf("Connection closed\n");
            else
#ifdef WIN32
                debug_printf("recv failed: %d\n", WSAGetLastError());
#else
                debug_printf("recv failed\n");
#endif


            if (++retries1 > device->readRetries) break;
        }

        if (retries2++ > device->sendRetries) break;
    }

    debug_printf("Error - timeout...");
    return PK_ERR_TRANSFER;

}


void PK_SetEthernetRetryCountAndTimeout(sPoKeysDevice * device, uint32_t sendRetries, uint32_t readRetries, uint32_t timeout)
{
    device->sendRetries = sendRetries;
    device->readRetries = readRetries;
    device->socketTimeout = timeout;
}

