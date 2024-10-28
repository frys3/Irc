IRC Server in C++
This project is an implementation of an IRC (Internet Relay Chat) server in C++. It provides a basic server capable of handling multiple clients through the use of TCP sockets and the poll function. The server listens for incoming connections, processes user input, and manages various IRC commands such as NICK, JOIN, PRIVMSG, and more.

Features:
Socket-based connection handling with support for multiple clients.
IRC command processing, including:
NICK: Set or change a user's nickname.
JOIN: Join a channel.
PRIVMSG: Send private messages to users or channels.
KICK, INVITE, TOPIC, MODE, and more.
Client management with connection tracking and event-based communication using the poll function.
Basic error handling and socket options for server optimization.
How It Works:
The server creates a listening socket and waits for client connections. Once connected, the server processes user input and dispatches commands to the appropriate handlers. Channels can be created, users can send messages, and various IRC commands are executed based on user requests.

The project is designed for learning and educational purposes, showcasing how to build a simple IRC server from scratch in C++.
