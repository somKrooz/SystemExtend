#include <Python.h>
#include <filesystem>
#include <iostream>
#include <vector>
#include <string>
#include <raylib.h>
#include <Loader.h>

void Log(std::string Message){
    std::cout<<Message<<"\n";
}

static PyObject* py_Log(PyObject* self  , PyObject* args){
    const char* Message;
    if(!PyArg_ParseTuple(args ,"s",&Message)){
        return nullptr;
    }
    Log(Message);
    Py_RETURN_NONE;
}

void Krooz(std::string Message){
    std::cout<<Message + "Krooz"<<"\n";
}

static PyObject* py_Krooz(PyObject* self  , PyObject* args){
    const char* Message;
    if(!PyArg_ParseTuple(args ,"s",&Message)){
        return nullptr;
    }
    Krooz(Message);
    Py_RETURN_NONE;
}

static PyMethodDef Methods[100];
static PyModuleDef Module = {
    PyModuleDef_HEAD_INIT,
    "Krooz",
    nullptr,
    -1,
    Methods
};

PyMODINIT_FUNC Engine() {
    static PyMethodDef Krooz[] = {
        {"log", py_Log, METH_VARARGS, "Do His Thing"},
        {"krooz", py_Krooz, METH_VARARGS, "Do Shit"},
        {NULL, NULL, 0, NULL}  
    };
    
    int i = 0;
    for(auto &ent : Krooz)
    {
        Methods[i] = Krooz[i];
        i++; 
    }

    return PyModule_Create(&Module);
}


int main() {
    InitWindow(1200, 600, "Krooz Engine");
    SetTargetFPS(70);
    PyImport_AppendInittab("Krooz" , &Engine);
    Py_Initialize();    
    Loader RuntimeLoader;

    std::string source = "src";
    if(!RuntimeLoader.Get_RuntimeModules(source)){
        Py_Finalize();
        return -1;
    }
    RuntimeLoader.ExecuteBatch();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        if(IsKeyPressed(KEY_W)){
            RuntimeLoader.Reload_RuntimeModules();
        }

        EndDrawing();
    }


    Py_Finalize();  
    return 0;
}
