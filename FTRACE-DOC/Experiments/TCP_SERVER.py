import socket

SERVER_TCP_IP = "10.114.64.133"
TCP_PORT = 5000

# Create a TCP socket
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Bind the socket to the server IP and port
server_socket.bind((SERVER_TCP_IP, TCP_PORT))

# Listen for incoming connections
server_socket.listen()

print("TCP server is listening...")

while True:
    # Accept a new connection from a client
    client_socket, client_addr = server_socket.accept()
    print(f"Accepted connection from {client_addr}")
    
    while True:
        # Receive data from the client
        data = client_socket.recv(1024)
        if not data:
            break
        print(f"Received message from {client_addr}: {data.decode()}")
        
    client_socket.close()

