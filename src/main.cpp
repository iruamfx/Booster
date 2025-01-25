#include <iostream>
#include "pyrtexec.h"

int main()
{
    PyRtExec pyrtexec;
    std::cout << "Waking up Boostify!" << std::endl;
    pyrtexec.execPyInterfaced("script.py");
    return 0;
}
