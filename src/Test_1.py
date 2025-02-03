import Krooz  

def Check():
    size = float(Krooz.expose(0))
    size2 = float(Krooz.expose(1))
    if(size >= 60):
        size = 60

    if(size2>=60):
        size2 = 60  

    Krooz.create_text("Hello", 100, 100, size2 , 1)
    Krooz.create_text("CHill", 700, 100, size,2)
    Krooz.create_text("Japan", 300, 100, size2,3)
    Krooz.create_text("Pop", 500, 100, size2,5)
    

def Update():
    Krooz.updateAttr(2.0, 10)  
    Krooz.updateAttr(1.0,20)  
    Krooz.set_calls(Check)
