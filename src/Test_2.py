import Krooz   # type: ignore

size = 10
def Small():
    global size
    size = 20


def Dynamic():
    global size    
    if size >= 200:
        size = 200

    Krooz.create_text("Som", 100, 100, size/3)
    Krooz.create_text("Krooz", 200, 100, size/3)
    Krooz.create_text("Japan", 300, 100, size/3)
    
    size += 1

def Update():
    global size
    Krooz.set_calls(Dynamic)
    Krooz.create_button("Small",200, 200, 200, 50,Small)
