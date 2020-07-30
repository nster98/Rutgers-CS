# Nathan Glikman
# Stephen Scott
# Project 2
# ls.py

import threading
import socket as mysoc
import os
import sys
import time
import select

# Set up server socket
def serverL():
        if len(sys.argv) != 6:
                print("ERROR: PLEASE INPUT ARGUMENTS")


        try:
                ls = mysoc.socket(mysoc.AF_INET, mysoc.SOCK_STREAM)
                print("lS server created successfully")
        except mysoc.error as err:
                print('{} \n'.format("Error creating lS socket", err))
                
        try:
            ts1_socket = mysoc.socket(mysoc.AF_INET, mysoc.SOCK_STREAM)
            print("DEBUG: Client ts1 socket created\n")
        except mysoc.error as err:
            print('{} \n'.format("Client ts1 socket open error", err))

        try:
            ts2_socket = mysoc.socket(mysoc.AF_INET, mysoc.SOCK_STREAM)
            print("DEBUG: Client ts2 socket created\n")
        except mysoc.error as err:
            print('{} \n'.format("Client ts1 socket open error", err))

        ts1_port = int(sys.argv[3])
        ts2_port = int(sys.argv[5])

        ts1_server_binding = (sys.argv[2], ts1_port)
        ts1_socket.connect(ts1_server_binding)
        ts2_server_binding = (sys.argv[4], ts2_port)
        ts2_socket.connect(ts2_server_binding)

        # Bind LS server to port specified by user and enable listening
        ls_server_binding = ('', int(sys.argv[1]))
        ls.bind(ls_server_binding)
        ls.listen(1)

        # Wait for client to connect
        clientid, addr = ls.accept()

        query = clientid.recv(1024).decode('utf-8')

        clientQuery = query.split('_')

		# SET NO BLOCKING HERE
        ts1_socket.setblocking(0);
        ts2_socket.setblocking(0);
        
        ts1_socket.send(query.encode('utf-8'))
        ts2_socket.send(query.encode('utf-8'))

        readyTS1 = select.select([ts1_socket], [], [], 10)
        readyTS2 = select.select([ts2_socket], [], [], 10)


        string1 = ''
        string2 = ''

        if readyTS1[0]:
            string1 += (ts1_socket.recv(1024).decode('utf-8'))
        else:
            print("TS1 timed out\n")

        time.sleep(2)
   	
        if readyTS2[0]:	
            string2 += (ts2_socket.recv(1024).decode('utf-8'))
        else:
            print("TS2 timed out\n")

        list1 = string1.split("_")
        list2 = string2.split("_")
        
        

        toSendList = ['none']*(len(clientQuery))
        sublist1 = [None]*len(list1)
        sublist2 = [None]*len(list2)

        for i in range(0, len(list1)):
            sublist1[i] = list1[i].split(' ')

        for i in range(0, len(list2)):
            sublist2[i] = list2[i].split(' ')

        for i in range(0, len(clientQuery)):
            flag = 0
            for j in range(0, len(list1)):
                if clientQuery[i] == sublist1[j][0]:
                    toSendList[i] = list1[j]
                    flag = 1
            for j in range(0, len(list2)):
                if clientQuery[i] == sublist2[j][0] and flag == 0:
                    toSendList[i] = list2[j]
                    flag = 1
            #print("i: " + str(i) + "Flag: " + str(flag) + "\n")
            if flag == 0:
                toSendList[i] = str(clientQuery[i]) + " - Error:HOST NOT FOUND"

        #for i in range(0, len(list1)): 
         #   sublist1[i] = list1[i].split(' ')
            #sublist2[i] = list2[i].split(' ')
            #if sublist1[i][2] != 'Error:HOST':
		#	flag = false
		#	for (j in range(0, len(clientQuery))):
         #       if sublist[i][0] == clientQuery[j]:
          #          toSendList[i] = list1[i]
           #         flag = true
            #if flag == false:
             #   toSendList[i] = sublist1[i][0] + " - Error:HOST NOT FOUND"

        #for i in range(0, len(list2)):
         #   sublist2[i] = list2[i].split(' ')
          #  if len(sublist2[i]) == 3:
           #     toSendList[i + len(list1)] = list2[i]
            #else:
             #   toSendList[i + len(list1)] = sublist2[i][0] + " - Error:HOST NOT FOUND"
       
        toSend = '_'.join([str(j) for j in toSendList])

        clientid.send(toSend.encode('utf-8'))
    
t2 = threading.Thread(name='serverL', target=serverL)
t2.start()
time.sleep(1)

input("Hit ENTER to exit")

exit()
