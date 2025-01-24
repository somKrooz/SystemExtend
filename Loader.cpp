#include "Loader.h"
#include <filesystem> 
#include <iostream>  

bool Loader::Get_RuntimeModules(std::string Path){
    namespace fs = std::filesystem;
    for(auto &ent : fs::directory_iterator(Path)){
        PyObject* System_Path = PySys_GetObject("path");
        PyObject* ExtPath = PyUnicode_FromString(Path.c_str()); 
        PyList_Append(System_Path , ExtPath);
        Py_DECREF(ExtPath);
        
        if(ent.path().extension() == ".py"){
            const std::string GoodPath = ent.path().stem().string(); 
            ModuleName.push_back(GoodPath);
            PyObject* ModName = PyUnicode_FromString(GoodPath.c_str());
            PyObject* module = PyImport_Import(ModName);

            Py_DECREF(module);
            if(module){
                RuntimeModules.push_back(module);
            }
            else{
                return false;
                PyErr_Print();
            }
        }
    }
    return true;
}

void Loader::ExecuteBatch(){
    for(PyObject* mods : RuntimeModules){
        PyObject* Krooz = PyObject_GetAttrString(mods, "Update");
        if (Krooz && PyCallable_Check(Krooz)) {
            PyObject* Res = PyObject_CallObject(Krooz, nullptr);
            Py_DECREF(Res);
        } 
        else{
            PyErr_Print();
            exit(0);
        }
    }
}


void Loader::LookForNew(const std::string Path) {
    namespace fs = std::filesystem;

    for (const auto& ent : fs::directory_iterator(Path)) {
        if (ent.path().extension() == ".py") {
            const std::string GoodPath = ent.path().stem().string();
            
            auto it = std::find(ModuleName.begin(), ModuleName.end(), GoodPath);
            if (it == ModuleName.end()) {  
                std::cout<<GoodPath<<" Added This New Module"<<"\n";
                PyObject* ModName = PyUnicode_FromString(GoodPath.c_str());
                PyObject* module = PyImport_Import(ModName);
                Py_DECREF(ModName);

                if (module) {
                    RuntimeModules.push_back(module);   
                    ModuleName.push_back(GoodPath);    
                } else {
                    PyErr_Print();
                }
            }
        }
    }
}

int Loader::GetLength(){
    return RuntimeModules.size();   
}

std::vector<PyObject*> Loader::GetModules(){
    return RuntimeModules;
}

void Loader::Reload_RuntimeModules(){
    if(!RuntimeModules.empty()){
        for(auto &module : RuntimeModules){
            PyObject* ReloadMods = PyImport_ReloadModule(module);
            if(!ReloadMods){
                PyErr_Print();
            }
            else{
                Py_DECREF(module);
                module = ReloadMods;
            }
        }
        LookForNew("src");
        Loader::ExecuteBatch();
    }
}
