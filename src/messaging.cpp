# include "messaging.hpp"

# include <iostream>
# include <string>

# include <cstdlib>
# include <cstring>

# include <errno.h>

# include <fcntl.h>

# include <sys/types.h>
# include <sys/socket.h>

// Sends data to a socket.
void send_message(int socket, const std::string &message) {

    // Gets the message in the format of an array.
    const char * c_str = message.c_str();
    char *message_data = new char[message.size() + 1];
    for(size_t i = 0; i < message.size() + 1; i++)
        message_data[i] = c_str[i];

    // Breaks the message into blocks of a maximum size.
    size_t sent = 0;
    while (sent < (message.length() + 1)) {

        // Calculates how much data to send in this block.
        int bytes_to_send;
        if(message.length() + 1 - sent < max_block_size) {
            bytes_to_send = message.length() + 1 - sent;
        } else {
            bytes_to_send = max_block_size;
        }

        // Sends the data.
        sent += send(socket, message_data + sent, bytes_to_send, 0);

    }

    delete[] message_data;

}

// Tries receiving data from a socket and storing it on a buffer.
std::string check_message(int socket, int *const status, int need_to_acknowledge) {

    // Stores the received message.
    std::string response_message;

    // Ensures the socket is set to non-blocking.
    int flags = fcntl(socket, F_GETFL);
    flags |= O_NONBLOCK;
    fcntl(socket, F_SETFL, flags);

    // Receives the message.
    char temp_buffer[max_block_size];
    int bytes_received = 0;
    int received_now = 0;
    do {

        // Tries receiving data.        
        received_now = recv(socket, temp_buffer, max_block_size, 0);

        // Handles no data received.
        if(received_now == 0) { // The server or client has disconnected in a ordenerly way.
                *status = -1;
                return std::string("\0"); // Returns empty string.
        } else if (received_now < 0) {

            if(errno == EAGAIN || errno == EWOULDBLOCK) { 
            
                if(bytes_received == 0) { // No new message.
                    *status = 1;
                    return std::string("\0"); // Returns empty string.
                } else // Continues waiting for the message.
                    continue;     
                
            } else { // Error.
                *status = -1;
                return std::string("\0"); // Returns empty string.
            }
            
        }

        // Adds the received data to the response string and counts the amout of data received.
        response_message += std::string(temp_buffer);
        bytes_received += received_now;

    } while (received_now < 0 || (received_now == max_block_size && temp_buffer[max_block_size - 1] == '\0'));
    

    // Sends an acknowledgement that the message has being received if necessary.
    if(need_to_acknowledge) {
        // The acknowledge message.
        std::string ack(acknowledge_message);
        // Sends the ack
        send_message(socket, ack);
    }

    return response_message;

}