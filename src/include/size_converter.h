#ifndef SIZE_CONVERTER_H
#define SIZE_CONVERTER_H

#include <string>
#include <locale>
#include <map>
#include "constants.h"

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
    std::locale loc;
    char postfix = str.back();
    if( isalpha(postfix, loc) ) {
        value = convert_postfix(postfix) * stoul(str);
    } else {
        value = stoul(str);
    }
    return value;
}

#endif
