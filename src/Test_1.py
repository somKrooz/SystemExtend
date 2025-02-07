import Krooz

class MyEntity:
    def __init__(self):
        self.pox = 0

    def update(self, dt):
        self.pox += 10 *dt

    def draw(self):
        Krooz.create_text("KroozIsCool", self.pox , 100 , 30, 100 )


class MyEntity2:
    def __init__(self):
        self.pox = 0

    def update(self, dt):
        self.pox += 10 *dt

    def draw(self):
        Krooz.create_text("Shit", self.pox , 500 , 30, 130 )


class MyEntity3:
    def __init__(self):
        self.pox = 0

    def update(self, dt):
        self.pox += 10 *dt

    def draw(self):
        Krooz.create_text("Hello", self.pox , 300 , 30, 234 )

krooz = MyEntity()
krooz2 = MyEntity2()
krooz= MyEntity3()
Krooz.RegisterEntity(krooz)
Krooz.RegisterEntity(krooz2)


# import Krooz  

# def Check():
#     size = float(Krooz.expose(0))
#     size2 = float(Krooz.expose(1))
#     if(size >= 60):
#         size = 60

#     if(size2>=60):
#         size2 = 60  

#     Krooz.create_text("Hello", 1280/2, 700/2, size2 , 1)
    
# def Update():
#     Krooz.updateAttr(2.0, 10)  
#     Krooz.updateAttr(1.0,20)  
#     Krooz.set_calls(Check)
