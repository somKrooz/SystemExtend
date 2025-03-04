#include <Python.h>
#include <filesystem>
#include <iostream>
#include <vector>
#include <string>
#include <raylib.h>
#include <Loader.h>
#include <memory>
#include <Entity.h>
#include <Application.h>


Application app;

class PythonEntity : public Entity {
    private:
        PyObject* pyInstance;
        PyObject* drawMethod;
        PyObject* updateMethod;
    
    public:
        PythonEntity(PyObject* instance) : pyInstance(instance) {
            Py_XINCREF(pyInstance);
            drawMethod = PyObject_GetAttrString(pyInstance, "draw");
            updateMethod = PyObject_GetAttrString(instance, "update");
        }
    
        ~PythonEntity() {
            Py_XDECREF(pyInstance);
            Py_XDECREF(drawMethod);
            Py_XDECREF(updateMethod);
        }
    
        void Draw() override {
            if (drawMethod && PyCallable_Check(drawMethod)) {
                PyObject* result = PyObject_CallObject(drawMethod, nullptr);
                if (!result) {
                    PyErr_Print();
                } else {
                    Py_DECREF(result);
                }
            }
        }
    
        void Update(float dt) override {
            if (updateMethod && PyCallable_Check(updateMethod)) {
                PyObject* args = Py_BuildValue("(f)", dt);
                PyObject* result = PyObject_CallObject(updateMethod, args);
                Py_DECREF(args);
                if (!result) {
                    PyErr_Print();
                } else {
                    Py_DECREF(result);
                }
            }
        }
};

struct Text {
    const char* Text;
    float x;
    float y;
    float size;
    int id;
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

struct AttrUpdate{
    float val;
    float UpdateVal;
};

std::vector<Text> GlobalText; 
std::vector<Button> GlobalButtons; 
std::vector<CallBack> CallBacks;
std::vector<Pixels> GlobalPixels;
std::vector<AttrUpdate> GlobalAttrUpdate;
std::vector<std::unique_ptr<Entity>> ENT;

static PyObject* py_Expose(PyObject* self, PyObject* args) {
    int value;
    if (!PyArg_ParseTuple(args, "i", &value)) {
        return nullptr;
    }
    float res = GlobalAttrUpdate[value].val;
    return PyFloat_FromDouble(res); 
}

void K_AttrUpdate(const AttrUpdate& AU) {
    bool found = false;
    for (auto &ent : GlobalAttrUpdate) {
        if (ent.val == AU.val && ent.UpdateVal == AU.UpdateVal) {
            ent.val = AU.val;
            ent.UpdateVal = AU.UpdateVal;
            found = true;
            break;
        }
    }

    if (!found) {
        GlobalAttrUpdate.push_back(AU);
    }
}

static PyObject* py_AttrUpdate(PyObject* self, PyObject* args) {
    float val, UpdateVal;
    if (!PyArg_ParseTuple(args, "ff", &val, &UpdateVal)) {
        return nullptr;
    }

    AttrUpdate AU = {val, UpdateVal};
    K_AttrUpdate(AU);
    Py_RETURN_NONE; 
}

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
    for (auto& ent : GlobalButtons) {
        if (ent.Onclick) {
            Py_XDECREF(ent.Onclick);  
        }
    }
    GlobalPixels.clear();
    GlobalAttrUpdate.clear();
    GlobalButtons.clear();
    GlobalText.clear();

}


static PyObject* py_Button(PyObject* self, PyObject* args) {
    const char* Label;
    float x1, y1, wid, height;
    PyObject* callback  = nullptr;
    
    if (!PyArg_ParseTuple(args, "sffff|O", &Label, &x1, &y1, &wid, &height,&callback)) {
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
        if (text.id == Params.id) {
            text.Text = Params.Text;
            text.x = Params.x;
            text.y = Params.y;
            text.size = Params.size;
            text.id = Params.id;
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
    int id;
    
    if (!PyArg_ParseTuple(args, "sfffi", &text, &x, &y, &size , &id)) {
        return nullptr; 
    }
    
    Text textComp = {text, x, y, size , id};
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

static PyObject* GetMouseData(PyObject* self , PyObject* args){
    PyObject* Attrs = PyList_New(3);
    if(!Attrs){
        return nullptr;
    }

    float x = GetMousePosition().x;
    float y = GetMousePosition().y;
    
    PyList_SetItem(Attrs , 0 ,PyFloat_FromDouble(x));
    PyList_SetItem(Attrs , 1 ,PyFloat_FromDouble(y));

    return Attrs;
}

static PyObject* py_NativeData(PyObject* self, PyObject* args) {
    PyObject* Attrs = PyList_New(3);
    if (!Attrs) {
        return nullptr; 
    }

    float Height = GetScreenHeight();
    float Width = GetScreenWidth(); 

    PyList_SetItem(Attrs, 0, PyFloat_FromDouble(Height)); 
    PyList_SetItem(Attrs, 1, PyFloat_FromDouble(Width));  

    
    return Attrs;
}

static PyObject* py_RegisterEntity(PyObject* self, PyObject* args) {
    PyObject* entityInstance;
    if (!PyArg_ParseTuple(args, "O", &entityInstance)) {
        return nullptr;
    }

    if (!PyObject_HasAttrString(entityInstance, "draw") || 
        !PyObject_HasAttrString(entityInstance, "update")) {
        PyErr_SetString(PyExc_TypeError, "Entity must have draw() and update(dt) methods");
        return nullptr;
    }
        auto entity = std::make_unique<PythonEntity>(entityInstance); 
        app.AddEntity(std::move(entity)); 
    
    Py_RETURN_NONE;
}


static PyMethodDef Methods[] = {
    {"log", py_Log, METH_VARARGS, "Logs The Data"},
    {"create_text", py_Text, METH_VARARGS, "Creates a Text"},
    {"set_calls", py_CallBack, METH_VARARGS, "Create Calls"},
    {"create_button", py_Button, METH_VARARGS, "Creates a Button"},
    {"create_pix", py_Pixel, METH_VARARGS, "Creates a Textured Object"},
    {"updateAttr", py_AttrUpdate, METH_VARARGS, "Creates a Textured Object"},
    {"expose", py_Expose, METH_VARARGS, "Creates a AttrObject"},
    {"getAttrs", py_NativeData, METH_VARARGS, "get native Attrs"},
    {"getMouse" , GetMouseData , METH_NOARGS , "Get Mouse Data"},
    {"RegisterEntity", py_RegisterEntity ,METH_VARARGS , "RegesterME"},
    {NULL, NULL, 0, NULL}  
};

static PyModuleDef Module = {
    PyModuleDef_HEAD_INIT,
    "Krooz",
    nullptr,
    -1,
    Methods
};

PyMODINIT_FUNC PyInit_Krooz(void) {
    return PyModule_Create(&Module);
}


int main() {
    InitWindow(1200, 600, "Krooz Engine");
    SetTargetFPS(60);
    
    PyImport_AppendInittab("Krooz", &PyInit_Krooz);
    Py_Initialize();    
    Loader RuntimeLoader;
    std::string source = "src";


    if (!RuntimeLoader.Get_RuntimeModules(source)) {
        Py_Finalize();
        return -1;
    }

    RuntimeLoader.ExecuteBatch(); 
    int times = 100;
    while (!WindowShouldClose()) {
        float Delta = GetFrameTime();

        app.UpdateEntity(Delta);
        if (IsKeyPressed(KEY_W)) {
            Rest();
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

        for(auto& ent : GlobalAttrUpdate){
            ent.val += ent.UpdateVal*Delta;
        }
        app.DrawEntity();
        EndDrawing();
    }

    Py_Finalize();
  
    return 0;
}

