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
};

std::vector<Text> GlobalText; 
std::vector<Button> GlobalButtons; 

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

        if (isHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            anyButtonClicked = true;
        }
    }    
    return anyButtonClicked;
}


static PyObject* py_Button(PyObject* self, PyObject* args) {
    const char* Label;
    float x1, y1, wid, height;
    
    if (!PyArg_ParseTuple(args, "sffff", &Label, &x1, &y1, &wid, &height)) {
        return nullptr; 
    }
    Rectangle rect = {x1, y1, wid, height};
    Button buttonComp = {rect, Label}; 
    K_Button_Push(buttonComp);
    Py_RETURN_NONE; 
}

void K_Text(const Text& Params) {
    GlobalText.push_back(Params); 
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
    {"create_button", py_Button, METH_VARARGS, "Creates a Button"},
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
            GlobalButtons.clear();
            RuntimeLoader.Reload_RuntimeModules();
        }
        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (auto& ent : GlobalText) {
            DrawText(ent.Text, ent.x, ent.y, ent.size, RED);
        }

        for (auto& ent : GlobalButtons) {
            if(K_Button()){};
        }

        EndDrawing();
    }

    Py_Finalize();  
    return 0;
}
