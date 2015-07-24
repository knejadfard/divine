#ifndef TRANSPORTER_H
#define TRANSPORTER_H

#include <fstream>
#include <stdexcept>
#include "constants.h"
#include "helper.h"

/*****************************************************
 * Reads contents of a file and writes to an output  *
 * file. In order to minimize memory usage, I/O is   *
 * performed by utilizing a buffer.                  *
 * Buffer's size is defined in constants.h           *
 ****************************************************/
void transfer(std::istream& in, std::ostream& out, unsigned long to_read) {
    if(!in || !out) {
        throw std::runtime_error("transfer function error: one of the I/O streams is not open.");
    }
    unsigned long actual_buffer_size = buffer_size > to_read ? to_read : buffer_size;
    char *buffer = new char[actual_buffer_size];
    do {
        //try to read enough data to fill the buffer
        in.read(buffer, actual_buffer_size);
        //only writes as much as read in previous line
        //this way there is no need for an extra iteration
        //at the end of the loop for possible remaining data.
        out.write(buffer, in.gcount());
        to_read -= in.gcount();
    } while(to_read > 0 && in.gcount() > 0);
    delete[] buffer;
}

#endif
