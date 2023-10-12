import socket

SERVER_TCP_IP = "10.114.64.133"
TCP_PORT = 5000

# Create a TCP socket
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect to the server
client_socket.connect((SERVER_TCP_IP, TCP_PORT))

message = "Hello, world"
# Encode the message to bytes before sending
client_socket.send(message.encode())

print("Message sent")

# Close the client socket after sending the message
client_socket.close()
