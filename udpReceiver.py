import socket
import threading
import time

# Global flag to control sending
should_send = True
send_lock = threading.Lock()

ipString = "192.168.137.110"

def send_searching(target_ip=ipString, send_port=1234):
    # Create UDP socket for sending
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    
    while True:
        with send_lock:
            if not should_send:
                print("Stopping search - connection established")
                break
            
            try:
                # Send searching message
                message = b'searching'
                sock.sendto(message, (target_ip, send_port))
                print(f'Sent searching to {target_ip}:{send_port}')
            except Exception as e:
                print(f"Error sending message: {e}")
                break
        
        time.sleep(1)  # Wait 1 second between sends
    
    sock.close()

def receive_messages(receive_port=5000):
    global should_send
    # Create UDP socket for receiving
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind(('', receive_port))
    
    print(f'Listening for UDP messages on port {receive_port}...')
    
    while True:
        try:
            # Receive message
            data, addr = sock.recvfrom(1024)
            message = data.decode()
            # print(f'\nReceived message from {addr}:')
            print(f'Received data: {message}')
            
            # Check if we received "connected"
            if message == "connected":
                print("Received 'connected' message - stopping search")
                with send_lock:
                    should_send = False
                break
                
        except Exception as e:
            print(f"Error receiving message: {e}")
            break
    
    sock.close()

if __name__ == '__main__':

    print("Qual IP?")
    newIpString = input(f"Digite o IP (default: {ipString}):")
    if newIpString.strip():
        ipString = newIpString.strip()

    # Start sender thread
    sender_thread = threading.Thread(target=send_searching, daemon=True)
    sender_thread.start()
    
    # Start receiver in main thread
    receive_messages()
