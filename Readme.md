# System Extend

A learning project for experimenting with C++/Python integration, runtime scripting, and game dev using Raylib.

## Features
- Embedded Python scripting
- Hot-reload scripts at runtime (W to reload)
- Dynamic module loading & callbacks
- Simple UI elements (text, buttons, textures)
- Simple State Management


```python
import Krooz

class MyEntity:
    def __init__(self):
        self.pox = 0

    def update(self, dt):
        self.pox += 10 *dt

    def draw(self):
        Krooz.create_text("This", self.pox , 100 , 30, 100 )

krooz = MyEntity()
Krooz.RegisterEntity(krooz)

# All The Python Scripts In The Src Folder will be loaded and
# Evaluated By the Engine At Runtime
