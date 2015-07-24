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
void transfer(std::istream& in, std::ostream& out) {
    if(!in || !out) {
        throw std::runtime_error("Exception occured in one of the file streams!");
    }
    char *buffer = new char[buffer_size];
    do {
        //try to read enough data to fill the buffer
        in.read(buffer, buffer_size);
        //only writes as much as read in previous line
        //this way there is no need for an extra iteration
        //at the end of the loop for possible remaining data.
        out.write(buffer, in.gcount());
    } while(in.gcount() > 0);
    delete[] buffer;
}

#endif
