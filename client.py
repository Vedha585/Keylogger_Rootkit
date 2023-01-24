import socket


host = '10.0.0.3'
port = 12365
print(host)

s=socket.socket()
print("Socket Created")
s.connect((host,port))
print("Socket Connected")
msg = input("Enter 1 to ask the server to send the data: ")
s.send(msg.encode())

savefilename = 'key_logger.txt'
with s,open(savefilename,'w') as file:
    while True:
        recvfile = s.recv(4096)
        recvfile=recvfile.decode()
        file.write(recvfile)
        file.flush()
        file.close()
        break
    s.close()
print("File has been received")
