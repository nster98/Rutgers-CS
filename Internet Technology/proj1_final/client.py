# Nathan Glikman
# Stephen Scott
# Project 1
# client.py

import threading
import time
import socket as mysoc
import os
import sys

# Set up client sockets for TS and RS
def client():
        if len(sys.argv) != 4:
            print("ERROR: PLEASE INPUT THE RS SERVER AND 2 PORTS")
        
        try:
            rs_socket = mysoc.socket(mysoc.AF_INET, mysoc.SOCK_STREAM)
            print("DEBUG: Client rs socket created\n")
        except mysoc.error as err:
            print('{} \n'.format("Client rs socket open error", err))

        # Define port for the server
        # TODO: Define only the RS server, do the TS server socket only if needed to go to it

        port = int(sys.argv[2])
        port2 = int(sys.argv[3])

        # Connect to server on local machine
        # TODO: Changed RS server to be from user input
        # TODO: Move TS server binding and connect to after needing to connect to it

        server_binding = (sys.argv[1], port)
        #server_binding2 = (sa_sameas_myaddr, port2)
        #ts_socket.connect(server_binding2)
        rs_socket.connect(server_binding)

        inputFile = open(os.path.join(sys.path[0],"PROJI-HNS.txt"),"r")

        if inputFile.mode == 'r':

            # Set up query to send to RS server
            inputList = [i.rstrip() for i in inputFile]
            inputString = '_'.join([str(j) for j in inputList])
            query = inputString.lower()

            # Send query to RS server
            rs_socket.send(query.encode('utf-8'))

            # Recieve response from RS server in form <hostname ip flag>
            returnQuery = rs_socket.recv(1024).decode('utf-8')

            # Split each part by spaces and put in list
            listData = returnQuery.split('_')
            outputList = ["NONE"]*len(listData)
            tsList = []

            # Loop through data and check for A or NS
            # If NS, use hostname to set up TS socket and connect to it
            for i in range(0, len(listData)):
                tempList = listData[i].split(' ')
                if tempList[2] == 'A':
                    outputList[i] = listData[i]
                else:
                    tsHostname = tempList[0]
                    tsList.append(str(i)+' '+inputList[i])
            # Set up TS socket here
            try:
                 ts_socket = mysoc.socket(mysoc.AF_INET, mysoc.SOCK_STREAM)
                 print("DEBUG: Client ts socket created\n")
            except mysoc.error as err:
                print('{} \n'.format("Client ts socket open error", err))
            server_binding2 = (tsHostname, port2)
            ts_socket.connect(server_binding2)

            if len(tsList)>0:

                tsString = '_'.join([str(j) for j in tsList])
                query2 = tsString.lower()
                ts_socket.send(query2.encode('utf-8'))
                returnQuery2 = ts_socket.recv(1024).decode('utf-8')
                returnList = returnQuery2.split('_')

                for i in range(0, len(returnList)):
                    tempList = returnList[i].split(' ')
                    if tempList[3] == 'Error':
                        tempList[3] = 'Error:HOST NOT FOUND'
                    outputList[int(tempList[0])] = (tempList[1]+' '+tempList[2]+' '+tempList[3])

            outputFile = open(os.path.join(sys.path[0],"RESOLVED.txt"), "w")               
            for i in range(0, len(outputList)):
                print(outputList[i])
                outputFile.write(outputList[i] + "\n")
            outputFile.close()         
                                        
t1 = threading.Thread(name='client', target=client)
t1.start()
time.sleep(1)

input("Hit ENTER to exit")

exit()


