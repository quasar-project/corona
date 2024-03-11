import socket
import ctypes as ct
import struct

class MyStruct(ct.Structure):
    _pack_ = 1
    _fields_ = (('head', ct.c_uint16),
                ('ch', ct.c_uint8),
                ('en', ct.c_uint8),
                ('v', ct.c_uint32),
                ('i', ct.c_uint32))
    size = 12
    
def from_bytes(bin_bytes):
    return MyStruct.from_buffer_copy(bin_bytes)

def chunks(lst, n):
    """Yield successive n-sized chunks from lst."""
    for i in range(0, len(lst), n):
        yield lst[i:i + n]

bind_port = 12000
endian = 'little'

server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)
server_socket.bind(('192.168.1.10', 12000))


marker = 0xAAAAAAAA.to_bytes(4, endian)
cmd = 0x0000.to_bytes(2, endian)
port = bind_port.to_bytes(2, endian)
crc = 0x0000.to_bytes(2, endian)


data = marker + cmd + port + crc

# server_socket.sendto(data, ('192.168.1.50', 44000))

while True:
    message, address = server_socket.recvfrom(1024)
    
    for mess in chunks(message, 12):
        data = from_bytes(mess)
        # print('head', hex(data.head))
        s = 'Ch{}: {} {:6d} {:6d}'.format(data.ch, data.en, data.v, data.i)
        print(s)
    print('')


