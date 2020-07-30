# Nathan Glikman
# Stephen Scott
# Project 1
# ts.py

import threading
import socket as mysoc
import os
import sys
import time

# Set up server socket
def serverT():
        if len(sys.argv) != 2:
                print("ERROR: PLEASE INPUT LISTEN PORT")
		
        try:
                ts = mysoc.socket(mysoc.AF_INET, mysoc.SOCK_STREAM)
                print("TS server created successfully")
        except mysoc.error as err:
                print('{} \n'.format("Error creating TS socket", err))

        # Bind TS server to port specified by user and enable listening
        server_binding = ('', int(sys.argv[1]))
        ts.bind(server_binding)
        ts.listen(1)

        # Wait for client to connect
        clientid, addr = ts.accept()

        #Set up dnsTable
        dnsTable ={}
        query = clientid.recv(1024).decode('utf-8')
        queryList = query.split('_')
        info =[]
        file = open(os.path.join(sys.path[0],"PROJI-DNSTS.txt"),"r")
        if file.mode == 'r':
            listTable = [i.rstrip() for i in file]
            for entry in listTable:
                tempList = entry.split(' ')
                dnsTable.update({tempList[0]:[tempList[1],tempList[2]]})
            #Get the key matching the query and send the data to client
            for i in range(0, len(queryList)):
                    tempList = queryList[i].split(' ')
                    if tempList[1] in dnsTable:
                            info.append(tempList[0]+' '+tempList[1]+' '+dnsTable.get(tempList[1])[0]+' '+dnsTable.get(tempList[1])[1])
                    else:
                            info.append(tempList[0]+' '+tempList[1]+' - Error')
            toSend = '_'.join([str(j) for j in info])
            clientid.send(toSend.encode('utf-8'))
            
    
t3 = threading.Thread(name='serverT', target=serverT)
t3.start()
time.sleep(1)

input("Hit ENTER to exit")

exit()
