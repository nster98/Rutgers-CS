# Nathan Glikman
# Stephen Scott
# Project 2
# client.py

import threading
import time
import socket as mysoc
import os
import sys

# Set up client sockets for LS
def client():
        if len(sys.argv) != 3:
            print("ERROR: PLEASE INPUT THE LS SERVER AND PORT")
        
        try:
            ls_socket = mysoc.socket(mysoc.AF_INET, mysoc.SOCK_STREAM)
            print("DEBUG: Client ls socket created\n")
        except mysoc.error as err:
            print('{} \n'.format("Client ls socket open error", err))

        port = int(sys.argv[2])

        server_binding = (sys.argv[1], port)
        ls_socket.connect(server_binding)

        inputFile = open(os.path.join(sys.path[0],"PROJ2-HNS.txt"),"r")

        if inputFile.mode == 'r':

            # Set up query to send to LS server
            inputList = [i.rstrip() for i in inputFile]
            inputString = '_'.join([str(j) for j in inputList])
            query = inputString.lower()

            # Send query to LS server
            ls_socket.send(query.encode('utf-8'))

            # Recieve response from LS server in form <hostname ip flag>
            returnQuery = ls_socket.recv(1024).decode('utf-8')

            # Split each part by spaces and put in list
            listData = returnQuery.split('_')

            outputFile = open(os.path.join(sys.path[0],"RESOLVED.txt"), "w")               
            for i in range(0, len(listData)):
                print(listData[i])
                outputFile.write(listData[i] + "\n")
            outputFile.close()                
                
                                        
t1 = threading.Thread(name='client', target=client)
t1.start()
time.sleep(3)

input("Hit ENTER to exit")

exit()



