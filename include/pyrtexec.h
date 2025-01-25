#ifndef PYRTEXEC_H
#define PYRTEXEC_H

#include <Python.h>
#include <filesystem>
#include <iostream>
#include <sstream>

class PyRtExec {
    public:
        PyRtExec();
        ~PyRtExec();
        void execPyInterfaced(const char* scriptName);//std::filesystem::path* file);
        //void* 
};

#endif
