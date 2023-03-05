import kivy
from kivy.app import App
from kivy.uix.boxlayout import BoxLayout

import random

kivy.require('1.9.0')

class myRoot(BoxLayout):
    def __init__(self):
        super(myRoot, self).__init__()
    def rdnum(self):
        self.rand_label.text=str(random.randint(0, 1000))

class NR(App):
    def build(self):
        return myRoot()
    
NR().run()