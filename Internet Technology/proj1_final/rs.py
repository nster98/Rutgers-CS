# Nathan Glikman
# Stephen Scott
# Project 1
# rs.py

import threading
import socket as mysoc
import os
import sys
import time

# Set up server socket
def serverR():
        if len(sys.argv) != 2:
                print("ERROR: PLEASE INPUT LISTEN PORT")


        try:
                rs = mysoc.socket(mysoc.AF_INET, mysoc.SOCK_STREAM)
                print("RS server created successfully")
        except mysoc.error as err:
                print('{} \n'.format("Error creating RS socket", err))

        # Bind RS server to port specified by user and enable listening
        server_binding = ('', int(sys.argv[1]))
        rs.bind(server_binding)
        rs.listen(1)

        # Wait for client to connect
        clientid, addr = rs.accept()

        #Set up dnsTable
        dnsTable ={}
        query = clientid.recv(1024).decode('utf-8')
        queryList = query.split("_")
        info = []
        file = open(os.path.join(sys.path[0],"PROJI-DNSRS.txt"),"r")
        if file.mode == 'r':
            listTable = [i.rstrip() for i in file]
            for entry in listTable:
                tempList = entry.split(' ')
                if tempList[2] == 'A':
                        dnsTable.update({tempList[0]:[tempList[1],tempList[2]]})
                else:
                        tsHostname = tempList[0]
            #Get the keys matching the queries and send the data to client
            for i in range(0, len(queryList)):
                    if queryList[i] in dnsTable:
                            info.append(queryList[i]+' '+dnsTable.get(queryList[i])[0]+' '+dnsTable.get(queryList[i])[1])
                    else:
                            info.append(tsHostname+' - NS')
            toSend = '_'.join([str(j) for j in info])
            clientid.send(toSend.encode('utf-8'))
    
t2 = threading.Thread(name='serverR', target=serverR)
t2.start()
time.sleep(1)

input("Hit ENTER to exit")

exit()
