#include <iostream>
#include <exception>
#include "ui.h"

using namespace std;

int main() {
    try {
        ui app;
        app.run();
    } catch(exception& e) {
        cerr<<"Exception caught: "<<e.what()<<endl;
        return 1;
    }
    return 0;
}
