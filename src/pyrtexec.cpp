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
        

/*
if (!Py_IsInitialized()) {
        std::cerr << "Python interpreter not initialized!" << std::endl;
        return;
    }

    // The Python code we're going to run
    //const char* scriptName = "script";  // Assume "script.py" is in the same directory

    // Import the Python module
    PyObject* pName = PyUnicode_DecodeFSDefault(scriptName);
    PyObject* pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (!pModule) {
        PyErr_Print();
        std::cerr << "Failed to load the Python script: " << scriptName << std::endl;
        return;
    }

    // Call a function in the Python script that asks for input (using input())
    PyObject* pFunc = PyObject_GetAttrString(pModule, "ask_for_name");
    if (!pFunc || !PyCallable_Check(pFunc)) {
        if (PyErr_Occurred()) PyErr_Print();
        std::cerr << "Cannot find the 'ask_for_name' function." << std::endl;
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
        return;
    }

    // Prepare arguments for the function if necessary (e.g., pass some default argument)
    PyObject* pArgs = PyTuple_New(0);  // No arguments passed for this function

    // Run the function that asks for input
    PyObject* pValue = PyObject_CallObject(pFunc, pArgs);
    Py_DECREF(pArgs);

    if (pValue) {
        // If the function returned a value, print it (this could be the name inputted by the user)
        std::cout << "Received input from Python: " << PyUnicode_AsUTF8(pValue) << std::endl;
        Py_DECREF(pValue);
    } else {
        PyErr_Print();
        std::cerr << "Call to 'ask_for_name' failed." << std::endl;
    }

    // Clean up
    Py_XDECREF(pFunc);
    Py_DECREF(pModule);

*/