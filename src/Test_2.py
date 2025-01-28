import Krooz   # type: ignore

def Follow():
    Krooz.create_text("Follow" ,Krooz.getMouse()[0] ,Krooz.getMouse()[1] , 30)

def Update():
    Krooz.set_calls(Follow)