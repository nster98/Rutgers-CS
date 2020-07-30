# Nathan Glikman
# Stephen Scott
# Project 2
# ts1.py

import threading
import socket as mysoc
import os
import sys
import time

# Set up server socket
def serverT1():
        if len(sys.argv) != 2:
                print("ERROR: PLEASE INPUT LISTEN PORT")
		
        try:
                ts = mysoc.socket(mysoc.AF_INET, mysoc.SOCK_STREAM)
                print("TS1 server created successfully")
        except mysoc.error as err:
                print('{} \n'.format("Error creating TS1 socket", err))

        # Bind TS server to port specified by user and enable listening
        server_binding = ('', int(sys.argv[1]))
        ts.bind(server_binding)
        ts.listen(1)

        # Wait for ls to connect
        clientid, addr = ts.accept()

        #Set up dnsTable
        dnsTable ={}
        query = clientid.recv(1024).decode('utf-8')
        queryList = query.split('_')
        info =[]
        file = open(os.path.join(sys.path[0],"PROJ2-DNSTS1.txt"),"r")
        if file.mode == 'r':
            listTable = [i.rstrip() for i in file]
            for entry in listTable:
                tempList = entry.split(' ')
                dnsTable.update({tempList[0]:[tempList[1],tempList[2]]})
            #Get the key matching the query and send the data to ls
            for i in range(0, len(queryList)):
                    if queryList[i] in dnsTable:
                            info.append(queryList[i]+' '+dnsTable.get(queryList[i])[0]+' '+dnsTable.get(queryList[i])[1])
                            toSend =(queryList[i]+' '+dnsTable.get(queryList[i])[0]+' '+dnsTable.get(queryList[i])[1])+"_"
                            clientid.send(toSend.encode('utf-8'))
                            
    
t3 = threading.Thread(name='serverT', target=serverT1)
t3.start()
time.sleep(1)

input("Hit ENTER to exit")

exit()
