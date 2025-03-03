import Krooz

def Check():
    size = float(Krooz.expose(0))
    size2 = float(Krooz.expose(1))
    if(size >= 60):
        size = 60

    if(size2>=60):
        size2 = 60

    Krooz.create_text("Hello", 1280/2, 700/2, size2 , 1)

def Update():
    Krooz.updateAttr(2.0, 10)
    Krooz.updateAttr(1.0,20)
    Krooz.set_calls(Check)
