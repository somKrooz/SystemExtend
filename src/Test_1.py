import Krooz

class MyEntity:
    def __init__(self):
        self.pox = 0

    def update(self, dt):
        self.pox += 10 *dt

    def draw(self):
        Krooz.create_text("This", self.pox , 100 , 30, 100 )


class MyEntity2:
    def __init__(self):
        self.pox = 0

    def update(self, dt):
        self.pox += 10 *dt

    def draw(self):
        Krooz.create_text("is Cool", self.pox , 500 , 30, 130 )


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


