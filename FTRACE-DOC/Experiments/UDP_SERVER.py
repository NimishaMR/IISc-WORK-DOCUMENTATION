import socket

CLIENT_UDP_IP = "10.114.64.162"
SERVER_UDP_IP = "10.114.64.130"
UDP_PORT = 5000

# Create a UDP socket
server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Bind the socket to the server IP and port
server_socket.bind((SERVER_UDP_IP, UDP_PORT))

print("UDP server is listening...")

while True:
    # Receive data from the client and the client address
    data, addr = server_socket.recvfrom(1024)
    print(f"Received message from {addr}: {data.decode()}")
