Networking Assignment 6 & 7

Coin Grabber
Players run around and grab as many coins as they can.

Known Bugs:
Sending too many Reliable In Order messages clogs up the pipe. In order is waiting for a sequence ID that the sender things got received correctly.
* Limited 1 coin on screen at a time to avoid this issue for now.

Comments:
* Leaks are down from 40 to 2! Woo!
* Implemented synced Net clocks and client prediction.
* Score board for players and how many coins they got
* Players are colored uniquely by their player index


Controls:
WASD - Move Player
Space - Shoot
Esc - Quit

~ - Show Console
When console is open:
PG UP / PG DOWN - Scroll thru console messages
Up Arrow - Get Last command (only does last 1 message and is slightly buggy with the cursor)

Console commands of importance:

//A1
command_server_host = Host a TCP connection
command_server_info = List TCP Status (Host, Client, no connection) and the IPs we are connected to
command_server_join <ip> = Join an IP
command_server_leave = If Client, leave a connection
command_server_stop = If host, stop all connections.
net_list_tcp_addresses <portName> = List all TCP addresses available to us on a port

//A2
net_session_start - Start session
net_session_stop - Stop session
net_ping <ip> <port> <optional_message> - Pings the ip. Receive a pong if successful.

//A3
net_session_create_connection <idx> <guid> <address> <port> - Creates a NetConnection with ID and name. If no args are given, will create two connections with the following default values:
	0 nick 192.168.1.10 4334
	1 bob 192.168.1.10 4335
	This will also create a Player object for each connection
net_session_destroy_connection <idx> - Destroys a Connection with this index. This will also remove the Player Object associated with it.
net_sim_lag <min_additional_lag_sec> [max_additional_lag_sec] - Simulates lag on receiving packets. Some lag has been enabled by default. Note that this uses seconds, not ms.
net_sim_loss <loss_percentage> - Simualtes drop rate of packets. Notes that 0.15 means 15%.


//A4
net_debug - Show the network debug display
test_reliable <connection index> <count> = Sends <count> reliables to connection <connection index>
net_go = Runs 3 commands, a net session and 2 net_session_create_connection that are defaulted to 192.168.1.10 4335 and 192.168.1.10 4334


//A5
net_host <guid> - Starts up a hosting game session and sets host guid to the given guid.
net_join <guid> <ip> <port> - Joins a hosting session at the given ip and tells the host your guid.
net_leave - Disconnects self from host. Does not send a Leave Message to Host atm, but Host will drop connection after timeout.
test_in_order <connection index> <count> = Sends <count>  in order reliables to connection <connection index>
