#ifndef HELPER_H
#define HELPER_H

#include <string>
#include <locale>
#include <map>
#include <fstream>
#include "constants.h"

/******************************************************************
 * Takes the file's size and each part's desired size as argument *
 * and returns the number of parts which are going to be          *
 * created.                                                       *
 ******************************************************************/
unsigned long count_parts(const unsigned long& file_size, const unsigned long& part_size)
{
    return (file_size / part_size) + (file_size % part_size==0 ? 0:1);
}

/*******************************************************************
 * Takes a size postfix character as the only argument and returns *
 * the equivalent number of bytes.                                 *
 *                                                                 *
 * Valid characters for c: b, k, m, g, t                           *
 *******************************************************************/
unsigned long convert_postfix(const char& c) {
    std::map<char, unsigned long> table{
        {'b', 1},
        {'k', kb},
        {'m', mb},
        {'g', gb},
        {'t', tb}
    };
    return table.at(c);
}

/*****************************************************************
 * Converts a size string to the equivalent unsigned long value. *
 *                                                               *
 * String    Value (bytes)                                       *
 * "12"   ->  12                                                 *
 * "12b"  ->  12                                                 *
 * "12k"  ->  12*1024                                            *
 * "12m"  ->  12*1024*1024                                       *
 * "12g"  ->  12*1024*1024*1024                                  *
 * "12t"  ->  12*1024*1024*1024*1024                             *
 *****************************************************************/
unsigned long convert_size(const std::string& str)
{
    unsigned long value = 0;
    char postfix = str.back();
    if( isalpha(postfix, std::locale{}) ) {
        value = convert_postfix(postfix) * stoul(str);
    } else {
        value = stoul(str);
    }
    return value;
}

/**********************************
 * Returns a file's size in bytes *
 *********************************/
unsigned long file_size(const std::string& filename)
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if( !file.is_open() ) {
        throw std::runtime_error("Cannot open file \""+filename+"\"");
    }
    return file.tellg();
}

unsigned long file_size(std::istream& input) {
    std::streampos original_position = input.tellg();
    input.seekg(0, std::ios::end);
    std::streampos end = input.tellg();
    input.seekg(original_position);
    return end;
}

#endif
