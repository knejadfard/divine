#ifndef UI_H
#define UI_H

#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>
#include "helper.h"
#include "transfer.h"

/**************************************************************************************
 * ui - user inteface                                                                 *
 *                                                                                    *
 * All input/output operations that involve the user are carried out by the ui class. *
 *************************************************************************************/
class ui
{

public:

    /************************************
     * runs all the necessary functions *
     ***********************************/
    void run();

private:

    /*************************
     * introduce the program *
     ************************/
    void introduce();

    /***********************
     * print commands help *
     **********************/
    void show_help();

    /**************************************************************************
     * print a message to screen and return the user's input as a std::string *
     *************************************************************************/
    std::string input(const std::string& message);

    /*****************************************
     * analyze a command and act accordingly *
     ****************************************/
    void act(const std::string& command);

};


/********************************************************
* Function Definitions                                  *
*********************************************************/

void ui::run() {
    introduce();
    show_help();
    act( input("Enter your command:  ") );
}

void ui::introduce() {
    std::cout<<"Welcome to divine-0.1!"<<std::endl;
}

void ui::show_help() {
    std::cout<<"\nHere\'s how to use the program:"<<std::endl;
    std::cout<<"\tsplit filename part_size"<<std::endl;
    std::cout<<"\tmerge filename"<<std::endl;
}

std::string ui::input(const std::string& message) {
    std::cout<<message;
    std::string line;
    if( !std::getline(std::cin, line) ) {
        throw std::runtime_error("User input failed!");
    }
    return line;
}

void ui::act(const std::string& command) {
    std::stringstream sstr{command};
    std::string task, filename; //the two mandatory parts of a command
    if( !(sstr>>task>>filename) ) {
        throw std::runtime_error("Missing file name in command.");
    }
    if(task == "split") {
        std::string part_size;
        if( !(sstr>>part_size) ) {
            throw std::runtime_error("Missing size for file parts.");
        }
        unsigned long size = file_size(filename);
        unsigned long ul_part_size = convert_size(part_size);
        unsigned long parts_count = count_parts(size, ul_part_size);
        std::cout<<"Size of "<<filename<<" -> "<<size<<std::endl;
        std::cout<<"After splitting, "<<parts_count<<" part(s) will be created."<<std::endl;
        std::string answer = input("Proceed? (enter \"y\" to confirm) ");
        if(answer == "y") {
            std::ifstream in(filename, std::ios::binary);
            std::ofstream out;
            if(!in) {
                throw std::runtime_error("Error opening file \""+filename+"\".");
            }
            for(unsigned long i = 1; i <= parts_count; ++i) {
                out.open(filename+"-part"+std::to_string(i), std::ios::binary | std::ios::trunc);
                if(!out) {
                    throw std::runtime_error("Error opening output file for part "+std::to_string(i)+".");
                }
                transfer(in, out, ul_part_size);
                out.close();
            }
            in.close();
        }
    } else if(task == "merge") {
        std::cout<<"Merge command given"<<std::endl;
    } else {
        throw std::runtime_error("Unknown command: "+task);
    }
}

#endif
