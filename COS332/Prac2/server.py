import socket
import sys
import threading
import signal
import os
from datetime import datetime

FRIENDS_FILE = "./friends.txt"
friends = {}

RESET = "\033[0m"
GREEN = "\033[92m"
RED = "\033[91m"
BLUE = "\033[94m"
YELLOW = "\033[93m"
CYAN = "\033[96m"
CLEAR_SCREEN = "\033[2J"
MOVE_CURSOR_TOP = "\033[H"

def load_friends():
    global friends
    if os.path.exists(FRIENDS_FILE):
        with open(FRIENDS_FILE, "r") as file:
            for line in file:
                name, phone, birthday = line.strip().split(",")
                friends[name] = {"phone": phone, "birthday": birthday}

def save_friends():
    with open(FRIENDS_FILE, "w") as file:
        for name, data in friends.items():
            file.write(f"{name},{data['phone']},{data['birthday']}\n")

def handle_client(conn, addr):
    conn.sendall((CLEAR_SCREEN + MOVE_CURSOR_TOP).encode())
    conn.sendall(f"{CYAN}Welcome to the Friends Database!{RESET}\n".encode())
    
    today = datetime.now().strftime("%m-%d")
    for name, data in friends.items():
        if data["birthday"] == today:
            conn.sendall(f"{YELLOW}Today is {name}'s birthday!{RESET}\n".encode())
    
    menu = (f"{GREEN}1.{RESET} List all friends\n"
            f"{GREEN}2.{RESET} Search for a friend\n"
            f"{GREEN}3.{RESET} Add a new friend\n"
            f"{GREEN}4.{RESET} Delete a friend\n"
            f"{GREEN}5.{RESET} Update a friend's phone number\n"
            f"{GREEN}6.{RESET} Exit\n")
    
    while True:
        conn.sendall(menu.encode())
        conn.sendall(b"Enter your choice: ")
        choice = conn.recv(1024).decode().strip()
        
        if choice == "1":
            if friends:
                conn.sendall(b"\nFriends List:\n")
                for name, data in friends.items():
                    conn.sendall(f"{YELLOW}{name}{RESET}: {data['phone']} (Birthday: {data['birthday']})\n".encode())
            else:
                conn.sendall(b"No friends found.\n")
        
        elif choice == "2":
            conn.sendall(b"Enter friend's name: ")
            name = conn.recv(1024).decode().strip()
            if name in friends:
                conn.sendall(f"Phone number: {friends[name]['phone']}\n".encode())
                conn.sendall(f"Birthday: {friends[name]['birthday']}\n".encode())
            else:
                conn.sendall(f"{RED}Friend not found.{RESET}\n".encode())
        
        elif choice == "3":
            conn.sendall(b"Enter friend's name: ")
            name = conn.recv(1024).decode().strip()
            conn.sendall(b"Enter phone number: ")
            phone = conn.recv(1024).decode().strip()
            conn.sendall(b"Enter birthday (MM-DD): ")
            birthday = conn.recv(1024).decode().strip()
            friends[name] = {"phone": phone, "birthday": birthday}
            save_friends()
            conn.sendall(f"{GREEN}Friend added.{RESET}\n".encode())
        
        elif choice == "4":
            conn.sendall(b"Enter friend's name: ")
            name = conn.recv(1024).decode().strip()
            if name in friends:
                del friends[name]
                save_friends()
                conn.sendall(f"{GREEN}Friend deleted.{RESET}\n".encode())
            else:
                conn.sendall(f"{RED}Friend not found.{RESET}\n".encode())
        
        elif choice == "5":
            conn.sendall(b"Enter friend's name: ")
            name = conn.recv(1024).decode().strip()
            if name in friends:
                conn.sendall(b"Enter new phone number: ")
                phone = conn.recv(1024).decode().strip()
                friends[name]["phone"] = phone
                save_friends()
                conn.sendall(f"{GREEN}Phone number updated.{RESET}\n".encode())
            else:
                conn.sendall(f"{RED}Friend not found.{RESET}\n".encode())
        
        elif choice == "6":
            conn.sendall(f"{CYAN}Goodbye!{RESET}\n".encode())
            break
        else:
            conn.sendall(f"{RED}Invalid choice. Try again.{RESET}\n".encode())
    
    conn.close()

def start_server(port):
    load_friends()
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind(("0.0.0.0", port))
    server.listen(5)
    print(f"{GREEN}Server started on port {port}...{RESET}")
    
    def shutdown_server(sig, frame):
        print(f"\n{RED}Shutting down server...{RESET}")
        server.close()
        sys.exit(0)
    
    signal.signal(signal.SIGINT, shutdown_server)
    
    while True:
        conn, addr = server.accept()
        print(f"{BLUE}Connected to {addr}{RESET}")
        threading.Thread(target=handle_client, args=(conn, addr)).start()

if len(sys.argv) != 2:
    print(f"{RED}Usage: python telnet_server.py <port>{RESET}")
    sys.exit(1)
port = int(sys.argv[1])
start_server(port)