import socket

bind_port = 12000
endian = 'little'

server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)
server_socket.bind(('192.168.1.10', 12001))


while True:
    # message, address = server_socket.recvfrom(1024)
    # print(message)
    i = int(input("Ch: "))
    
    marker = 0xAAAAAAAA.to_bytes(4, endian)
    cmd = i.to_bytes(2, endian)
    port = bind_port.to_bytes(2, endian)
    crc = 0x0000.to_bytes(2, endian)


    data = marker + cmd + port + crc

    server_socket.sendto(data, ('192.168.1.50', 44000))


