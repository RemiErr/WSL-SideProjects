# 定義貪吃蛇類別
class Snake():
    def __init__(self, master):
        self.image = None
        self.init_snake()

    def init_snake(self):
        self.direction = "Right"            # 方向
        self.direction_queue = ["Right"]    # 方向控制佇列
        self.speed = 200                    # 設定初始速度間隔為200毫秒
        self.positions = [(100, 100), (80, 100), (60, 100)]

    def move_queue(self):
        head_x_position, head_y_position = self.positions[0]

        if self.direction_queue:
            self.direction = self.direction_queue.pop(0)

        if self.direction == "Left":
            new_head_position = (head_x_position - 20, head_y_position)
        elif self.direction == "Right":
            new_head_position = (head_x_position + 20, head_y_position)
        elif self.direction == "Up":
            new_head_position = (head_x_position, head_y_position - 20)
        elif self.direction == "Down":
            new_head_position = (head_x_position, head_y_position + 20)

        self.positions = [new_head_position] + self.positions[:-1]

    def check_snake_collisions(self):
        head_x_position, head_y_position = self.positions[0]
        return (
            head_x_position in (0, 600)
            or head_y_position in (20, 400)
            or (head_x_position, head_y_position) in self.positions[1:]
        )

    def on_key_press(self, event):
        new_direction = event.keysym
        all_directions = ("Up", "Down", "Left", "Right")
        opposites = ({"Up", "Down"}, {"Left", "Right"})

        if (
            new_direction in all_directions
            and {new_direction, self.direction} not in opposites
        ):
            self.direction_queue.append(new_direction)