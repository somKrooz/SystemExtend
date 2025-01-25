import Krooz

size = 60

def Dynamic():
    global size
    Krooz.create_text("Som", 120, 100, size/3)
    Krooz.create_text("Krooz", 200, 100, size/3)
    Krooz.create_text("Japan", 300, 100, size/3)
    
    size += 1

def Update():
    Krooz.set_calls(Dynamic)
