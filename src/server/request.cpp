// Authors:
// Abner Eduardo Silveira Santos - NUSP 10692012
// João Pedro Uchôa Cavalcante - NUSP 10801169
// Luís Eduardo Rozante de Freitas Pereira - NUSP 10734794

# include "request.hpp"

# include <string>

// ==============================================================================================================================================================
// Constructors/destructors =====================================================================================================================================
// ==============================================================================================================================================================

request::request() {

    this->origin_socket = -1;
    this->r_type = rt_Invalid;
    this->data = "/none";

}

request::request(int origin_socket, request_type r_type, const std::string data) {

    this->origin_socket = origin_socket;
    this->r_type = r_type;
    this->data = data;

}

// ==============================================================================================================================================================
// Getters ======================================================================================================================================================
// ==============================================================================================================================================================

int request::get_origin_socket() const { return this->origin_socket; }

request_type request::get_type() const { return this->r_type; }

std::string request::get_data() const { return this->data; }