#include <Python.h>
#include <filesystem>
#include <iostream>
#include <vector>
#include <string>
#include <raylib.h>
#include <Loader.h>

struct Text {
    const char* Text;
    float x;
    float y;
    float size;
};

struct Button {
    Rectangle Dim;
    const char* Label;
    PyObject* Onclick;
};

struct CallBack{
    PyObject* onUpdate;
};

struct Pixels
{
    Texture2D tex;
    float x;
    float y;
    float scale;
};


std::vector<Text> GlobalText; 
std::vector<Button> GlobalButtons; 
std::vector<CallBack> CallBacks;
std::vector<Pixels> GlobalPixels;

void K_Pixels(const Pixels& pixel){
    bool found = false;

    for(auto &ent: GlobalPixels){
        if(ent.x == pixel.x && ent.y == pixel.y){
            ent.tex = pixel.tex;
            ent.x = pixel.x;
            ent.y = pixel.y;
            ent.scale = pixel.scale;  
            found = true;
            break;
        }
    }

    if(!found){
        GlobalPixels.push_back(pixel);
    }
}


static PyObject* py_Pixel(PyObject* self , PyObject* args){
    const char* tex;
    float x;
    float y ;
    float  scale;

    if(!PyArg_ParseTuple(args , "sfff" ,&tex, &x , &y , &scale)){
        return nullptr;
    }
    Texture2D sprite = LoadTexture(tex);
    Pixels pix = {sprite, x ,y , scale};
    K_Pixels(pix);
    Py_RETURN_NONE;
}

void PixUpdate(){
    for(auto &ent : GlobalPixels){
        Vector2 location = { ent.x , ent.y} ;
        DrawTextureEx(ent.tex,location , 0.0f, ent.scale, WHITE); 
    }
} 

void K_CallBacks(const CallBack& cb){
    CallBacks.push_back(cb);
}

PyObject* py_CallBack(PyObject* self, PyObject* args) {
    PyObject* onUpdate;
    
    if (!PyArg_ParseTuple(args, "O", &onUpdate)) {
        return nullptr;
    }

    if (!PyCallable_Check(onUpdate)) {
        PyErr_SetString(PyExc_TypeError, "Argument must be callable");
        return nullptr;
    }

    Py_INCREF(onUpdate);  
    CallBack cb = {onUpdate};
    K_CallBacks(cb);

    Py_RETURN_NONE;  
}

void K_Button_Push(const Button& btn) {
    GlobalButtons.push_back(btn);
}

bool K_Button() {
    bool anyButtonClicked = false;
    for (auto& ent : GlobalButtons) {
        bool isHovered = CheckCollisionPointRec(GetMousePosition(), ent.Dim); 

        DrawRectangleRec(ent.Dim, isHovered ? Fade(RED, 0.3f) : BLUE);
        Vector2 textSize = MeasureTextEx(GetFontDefault(), ent.Label, 20, 1);
        DrawText(ent.Label, ent.Dim.x + (ent.Dim.width - textSize.x) / 2, ent.Dim.y + (ent.Dim.height - textSize.y) / 2, 20, WHITE);

        if (isHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) 
        {
            if (ent.Onclick) {
                PyObject_CallObject(ent.Onclick, nullptr); 
            }

            anyButtonClicked = true;
        }
    }    
    return anyButtonClicked;
}

void Rest(){
    for(auto &ent : GlobalButtons){
        if (ent.Onclick) {
            Py_XDECREF(ent.Onclick);  
        }
    }
    GlobalPixels.clear();
    GlobalButtons.clear();
    GlobalText.clear();
}


static PyObject* py_Button(PyObject* self, PyObject* args) {
    const char* Label;
    float x1, y1, wid, height;
    PyObject* callback;
    
    if (!PyArg_ParseTuple(args, "sffffO", &Label, &x1, &y1, &wid, &height,&callback)) {
        return nullptr; 
    }

    if(!PyCallable_Check(callback)){
        return nullptr;
    }

    Py_XINCREF(callback);
    Rectangle rect = {x1, y1, wid, height};
    Button buttonComp = {rect, Label,callback}; 
    K_Button_Push(buttonComp);
    Py_RETURN_NONE; 
}

void K_Text(const Text& Params) {
    bool found = false;
    
    for (auto& text : GlobalText) {
        if (text.x == Params.x && text.y == Params.y) {  
            text.Text = Params.Text;
            text.size = Params.size;
            found = true;
            break;
        }
    }
    
    if (!found) {
        GlobalText.push_back(Params);
    }
}

static PyObject* py_Text(PyObject* self, PyObject* args) {
    const char* text;
    float x, y, size;
    
    if (!PyArg_ParseTuple(args, "sfff", &text, &x, &y, &size)) {
        return nullptr; 
    }
    
    Text textComp = {text, x, y, size};
    K_Text(textComp);
    Py_RETURN_NONE; 
}

void Log(std::string Message) {
    std::cout << Message << "\n";
}

static PyObject* py_Log(PyObject* self, PyObject* args) {
    const char* Message;
    if (!PyArg_ParseTuple(args, "s", &Message)) {
        return nullptr;
    }
    Log(Message);
    Py_RETURN_NONE;
}


static PyMethodDef Methods[] = {
    {"log", py_Log, METH_VARARGS, "Logs The Data"},
    {"create_text", py_Text, METH_VARARGS, "Creates a Text"},
    {"set_calls", py_CallBack, METH_VARARGS, "Create Calls"},
    {"create_button", py_Button, METH_VARARGS, "Creates a Button"},
    {"create_pix", py_Pixel, METH_VARARGS, "Creates a Textured Object"},
    {NULL, NULL, 0, NULL}  
};

static PyModuleDef Module = {
    PyModuleDef_HEAD_INIT,
    "Krooz",
    nullptr,
    -1,
    Methods
};


PyMODINIT_FUNC Engine() {
    return PyModule_Create(&Module);
}

int main() {
    InitWindow(1200, 600, "Krooz Engine");
    SetTargetFPS(60);
    PyImport_AppendInittab("Krooz", &Engine);
    Py_Initialize();    
    Loader RuntimeLoader;
    std::string source = "src";

    if (!RuntimeLoader.Get_RuntimeModules(source)) {
        Py_Finalize();
        return -1;
    }

    RuntimeLoader.ExecuteBatch(); 

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_W)) {
            GlobalText.clear();
            Rest();
            GlobalButtons.clear();
            RuntimeLoader.Reload_RuntimeModules();
        }
        BeginDrawing();
        ClearBackground(RAYWHITE);

        for(auto& ent : GlobalPixels){
            PixUpdate();
        }

        for (auto& ent : GlobalText) {
            DrawText(ent.Text, ent.x, ent.y, ent.size, RED);
        }

        for (auto& ent : GlobalButtons) {
            if(K_Button()){};
        }

        for(auto& ent : CallBacks){
            PyObject* result = PyObject_CallObject(ent.onUpdate, nullptr); 
            if (!result) {
                PyErr_Print();
            } else {
                Py_DECREF(result);  
            }
        }

        EndDrawing();
    }

    Py_Finalize();  
    return 0;
}
