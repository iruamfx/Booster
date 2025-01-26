#ifndef PYRTEXEC
#define PYRTEXEC

#include "pyrtexec.h"

PyRtExec::PyRtExec() {
    Py_Initialize();
    if(!Py_IsInitialized()){
        std::cerr << "Failed to initialize the Python runtime" << std::endl;
    }
}

PyRtExec::~PyRtExec() {
    if(Py_IsInitialized){
        Py_Finalize();
    }
}

void PyRtExec::execPyInterfaced(const char* scriptName) //std::filesystem::path* file)
{
    // Define a custom Python class to handle stdin in Python
        const char* customStdinCode = R"(
class CustomInput:
    def readline(self):
        return input_callback()
import sys
sys.stdin = CustomInput()
)";

        // Define the callback for Python to invoke
        PyRun_SimpleString("from ctypes import *");

        // Load the callback from C++
        auto inputCallback = [](PyObject*, PyObject*) -> PyObject* {
            std::string input;
            std::cout << "(C++: Python input() requested: )";
            std::getline(std::cin, input);
            return PyUnicode_FromString(input.c_str());
        };

        // Wrap the callback into a Python callable object
        PyMethodDef callbackMethod = {"input_callback", inputCallback, METH_NOARGS, nullptr};
        PyObject* callbackFunction = PyCFunction_New(&callbackMethod, nullptr);

        // Add the callback to the Python built-ins
        PyObject* builtins = PyEval_GetBuiltins();
        PyDict_SetItemString(builtins, "input_callback", callbackFunction);

        // Execute the custom stdin script
        PyRun_SimpleString(customStdinCode);

        // Now execute the actual script
        FILE* fp = fopen(scriptName, "r");
        if (fp) {
            PyRun_SimpleFile(fp, scriptName);
            fclose(fp);
        } else {
            std::cerr << "Failed to open script: " << scriptName << std::endl;
        }

        // Cleanup
        PyDict_DelItemString(builtins, "input_callback");
        Py_XDECREF(callbackFunction);
    }
}

#endif
        
