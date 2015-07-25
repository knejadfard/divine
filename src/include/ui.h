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

    /***************
     * member data *
     **************/
    std::string _filename;
    unsigned long _part_size;

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

    /********************************************
     * split a file into parts of specific size *
     ********************************************/
    void split();

    /*********************************************
     * merge file parts to produce a single file *
     ********************************************/
    void merge();

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
    std::string task; //the two mandatory parts of a command
    if( !(sstr>>task>>_filename) ) {
        throw std::runtime_error("Missing file name in command.");
    }
    if(task == "split") {
        std::string str_part_size;
        if( !(sstr>>str_part_size) ) {
            throw std::runtime_error("Missing size for file parts.");
        }
        _part_size = convert_size(str_part_size);
        split();
    } else if(task == "merge") {
        std::cout<<"Analyzing parts..."<<std::endl;
        merge();
    } else {
        throw std::runtime_error("Unknown command: "+task);
    }
}

void ui::split() {
    unsigned long size = file_size(_filename);
    unsigned long parts_count = count_parts(size, _part_size);
    std::cout<<"Size of "<<_filename<<" -> "<<size<<std::endl;
    std::cout<<"After splitting, "<<parts_count<<" part(s) will be created."<<std::endl;
    std::string answer = input("Proceed? (enter \"y\" to confirm) ");
    if(answer == "y") {
        std::ifstream in(_filename, std::ios::binary);
        std::ofstream out;
        if(!in) {
            throw std::runtime_error("Error opening file \""+_filename+"\".");
        }
        for(unsigned long i = 1; i <= parts_count; ++i) {
            out.open(_filename+"-part"+std::to_string(i), std::ios::binary | std::ios::trunc);
            if(!out) {
                throw std::runtime_error("Error opening output file for part "+std::to_string(i)+".");
            }
            transfer(in, out, _part_size);
            out.close();
        }
        in.close();
    }
}

void ui::merge() {
    std::ifstream in;
    //TODO: check for file's existance and ask for confirmation
    std::ofstream out(_filename, std::ios::out | std::ios::trunc);
    unsigned int i = 1;
    while(true) {
        in.open(_filename+"-part"+std::to_string(i), std::ios::binary);
        if(in) {
            std::cout<<"   found part "<<i<<". Writing...";
            transfer(in, out, file_size(in));
            std::cout<<" Done!"<<std::endl;
            ++i;
        } else {
            std::cout<<"No more parts to write. Operation has finished after writing "<<i-1<<" parts."<<std::endl;
            break;
        }
        in.close();
    }
}

#endif
