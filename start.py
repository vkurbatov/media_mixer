#!/usr/bin/python

import logging
import socket
import select
import time
import random, string
import sys


logging.basicConfig(level=logging.DEBUG,
                    format='%(filename)s(%(lineno)s) [%(asctime)s] (%(threadName)-4s) %(message)s',
                    )

g_alive = True

start_proxy="\
\x02\
\x1e\x00\
\x02\x10\x02\x0a\x32\x75\
\x02\x10\x02\x0a\x34\x75\
\x01\x0d\x03\x00\x10\x00\x01\x01\x02\
\x01\x0d\x0f\x00\x48\x00\x01\x03\x04\
\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\
\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\
\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\
\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\
\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\
\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\
\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\
\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\
\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\
\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\
\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\
\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\
\xff\xff\xff\xff\xff\xff\xff\xff\xff\
\xff\
"

def hex_to_str(byteStr):
    #return ''.join( [ "%02X " % x for x in byteStr ] )
    return ' '.join(hex(ord(x))[2: ] for x in byteStr)


class TcpClient():
    __poll_timeout = 3
    __buffer_size = 1039
    __reconnect_timeout = 0.2

    def __init__(self, remote_address):
        remote_host, remote_port = remote_address.split(":")
        self.__remote_addr = (remote_host, int(remote_port))
        self.__connected = False

    def __is_connected(self):
        return self.__connected

    def run(self):
        logging.debug("run()")
        while g_alive == True:
            if self.__is_connected() == False:
                self.__client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                if self.__connect() == False:
                    time.sleep(self.__reconnect_timeout)
            else:
		self.send(start_proxy)
                readable, writable, exceptional = select.select([self.__client], [], [])
		time.sleep(self.__poll_timeout)
                for self.socket in readable:
                    try:
                        data = self.socket.recv(self.__buffer_size)
                        if len(data) == 0:
                            logging.debug(str(self.__remote_addr) + " has closed connection")
                            self.__on_disconnect()
                            break
                        else:
                            self.__on_recv(data)
                    except Exception as ex:
                        logging.debug("run(): catch exception: " + str(ex))
                        self.__on_disconnect()
        
        self.stop()
    
    def __connect(self):
        try:
            self.__client.connect(self.__remote_addr)
            self.__on_connect()
        except ConnectionRefusedError:
            logging.debug("can't connect to " + str(self.__remote_addr))
        except Exception as ex:
            logging.debug("connect(): catch exception " + str(ex))
        finally:
            return self.__is_connected()
    
    def __on_connect(self):
        logging.debug("connected to " + str(self.__remote_addr))
        self.__connected = True
    
    def __on_disconnect(self):
        if self.__is_connected() == True:
            logging.debug("shutdown connection with " + str(self.__remote_addr))
            self.__connected = False
        try:
            self.__client.shutdown(socket.SHUT_RDWR)
            self.__client.close()
        except Exception as ex:
            logging.debug("on_disconnect(): catch exception: " + str(ex))
    
    def __on_recv(self, data):
        logging.debug(" <-- (" + str(len(data)) + ") " + hex_to_str(data))
#        self.send(status)
    
    def send(self, data):
        if self.__is_connected() == True:
            logging.debug(" --> (" + str(len(data)) + ") " + hex_to_str(data))
            try:
                self.__client.send(data)
            except Exception as ex:
                logging.debug("run(): catch exception: " + str(ex))
                self.__on_disconnect()
    
    def stop(self):
        logging.debug("stop()")
        self.__on_disconnect()


if __name__ == '__main__':
    logging.debug("start!")
    remote_address = "10.2.16.21:6080"
    tcp_client = TcpClient(remote_address)
    
    try:
        while g_alive == True:
            tcp_client.run()
#            time.sleep(1)
#            tcp_client.send(start_proxy)
#	     logginng.debug("sending start_proxy");
    except KeyboardInterrupt:
        g_alive = False
    except Exception as ex:
        logging.debug("catch exception: " + str(ex))
    finally:
        logging.debug("stop!")
        sys.exit()
