import random

# 定義食物類別
class Food():
    def __init__(self, master):
        self.image = None
        self.position = None
        self.eat = False

    def check_food_collision(self, snake_positions:list):
        if snake_positions[0] == self.position:
            self.eat = True
            snake_positions.append(snake_positions[-1])

        return self.eat

    def set_new_position(self, snake_positions:list):
        while True:
            x_position = random.randint(1, 29) * 20
            y_position = random.randint(3, 19) * 20
            position = (x_position, y_position)

            if position not in snake_positions:
                self.eat = False
                self.position = position
                return position