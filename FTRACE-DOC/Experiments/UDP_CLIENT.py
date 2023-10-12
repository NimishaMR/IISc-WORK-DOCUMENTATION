import socket

CLIENT_UDP_IP = "10.114.64.130"
SERVER_UDP_IP = "10.114.240.247"
UDP_PORT = 5000

# Create a UDP socket
client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

message = "Hello, world"
# client_socket.bind((CLIENT_UDP_IP, UDP_PORT))
while(1):
	# Encode the message to bytes before sending
	client_socket.sendto(message.encode(), (SERVER_UDP_IP, UDP_PORT))

	print("Message sent")

# Close the client socket after sending the message
client_socket.close()
