import Krooz   # type: ignore

def Follow():
    Krooz.create_text("Follow Me" ,Krooz.getMouse()[0] ,Krooz.getMouse()[1] ,35 , 6)

def Update():
    Krooz.set_calls(Follow)

