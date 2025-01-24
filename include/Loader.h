#ifndef LOADER_H
#define LOADER_H

#include <vector>
#include <string>
#include <any>
#include <Python.h> 

class Loader {
private:
    std::vector<PyObject*> RuntimeModules;
    std::vector<std::string> ModuleName;

public:
    bool Get_RuntimeModules(const std::string Path);
    
    void ExecuteBatch();
    void LookForNew(std::string Path);

    void Reload_RuntimeModules();
    int GetLength();
    std::vector<PyObject*> GetModules(); 

};

#endif
