import Krooz   # type: ignore

def Follow():
    Krooz.create_text("Mytthh" ,Krooz.getMouse()[0] ,Krooz.getMouse()[1] , 100 , 6)

def Update():
    Krooz.set_calls(Follow)