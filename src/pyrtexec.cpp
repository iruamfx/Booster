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
        // std::stringstream inputBuffer;
        
        // // Sample input to be passed to Python input() calls (you can customize this)
        // std::string userInput = "John";  // For example, "John" as input for input()
        
        // // Write the user input to the stringstream (this simulates Python's input)
        // inputBuffer << userInput << std::endl;

        // // Create a custom Python file-like object for sys.stdin
        // PyObject* ioModule = PyImport_ImportModule("io");
        // PyObject* stringIOClass = PyObject_GetAttrString(ioModule, "StringIO");

        // char* strstreamval;
        // inputBuffer >> strstreamval;
        // PyObject* inputBufferObject = PyObject_CallObject(stringIOClass, PyBytes_FromString(strstreamval));
        
        // // Set the inputBuffer as the new sys.stdin
        // PyObject* sysModule = PyImport_ImportModule("sys");
        // PyObject_SetAttrString(sysModule, "stdin", inputBufferObject);

        // // Now execute the Python script
        // //const char* scriptPath = scriptName.c_str();
        // FILE* fp = fopen(scriptName, "r");
        // if (fp) {
        //     PyRun_SimpleFile(fp, scriptName);
        //     fclose(fp);
        // } else {
        //     std::cerr << "Failed to open script: " << scriptName << std::endl;
        // }

        // // Clean up
        // Py_XDECREF(ioModule);
        // Py_XDECREF(stringIOClass);
        // Py_XDECREF(inputBufferObject);
        // Py_XDECREF(sysModule);
//}

#endif
        
