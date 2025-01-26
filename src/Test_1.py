import Krooz  

def Check():
    size = float(Krooz.expose(0))
    if(size >= 200):
        size = 200

    Krooz.log(str(Krooz.getAttrs()[1]))
    Krooz.create_text("Som", 100, 100, size)
    Krooz.create_text("Krooz", 200, 100, size)
    Krooz.create_text("Japan", 300, 100, size)
    

def Update():
    Krooz.updateAttr(2.0, 200)  
    Krooz.set_calls(Check)
