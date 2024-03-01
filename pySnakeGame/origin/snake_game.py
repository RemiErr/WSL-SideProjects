import tkinter as tk
import random

# 定義貪吃蛇類別
class Snake(tk.Canvas):
    def __init__(self, master):
        super().__init__(master, width=600, height=400, background="black", highlightthickness=0)
        self.init_game()

    def init_game(self):
        self.snake_positions = [(100, 100), (80, 100), (60, 100)]
        self.food_position = self.set_new_food_position()
        self.direction = "Right"
        self.score = 0
        self.game_over = False
        self.speed = 200  # 設定初始速度為200毫秒

        self.load_assets()
        self.create_objects()

        self.bind_all("<Key>", self.on_key_press)
        self.after(self.speed, self.perform_actions)  # 使用速度變數設定定時器間隔

    def load_assets(self):
        try:
            self.snake_body_image = tk.PhotoImage(file="./snake_body.png")
            self.snake_food_image = tk.PhotoImage(file="./snake_food.png")
        except Exception as e:
            print(f"載入資源時發生錯誤: {e}")
            self.quit()

    def create_objects(self):
        self.create_text(35, 12, text=f"分數: {self.score}", tag="score", fill="#fff", font=("TkDefaultFont", 14))
        for x_position, y_position in self.snake_positions:
            self.create_image(x_position, y_position, image=self.snake_body_image, tag="snake")
        self.create_image(*self.food_position, image=self.snake_food_image, tag="food")
        self.create_rectangle(7, 27, 593, 393, outline="#525d69")

    def move_snake(self):
        head_x_position, head_y_position = self.snake_positions[0]

        if self.direction == "Left":
            new_head_position = (head_x_position - 20, head_y_position)
        elif self.direction == "Right":
            new_head_position = (head_x_position + 20, head_y_position)
        elif self.direction == "Up":
            new_head_position = (head_x_position, head_y_position - 20)
        elif self.direction == "Down":
            new_head_position = (head_x_position, head_y_position + 20)

        self.snake_positions = [new_head_position] + self.snake_positions[:-1]

        for segment, position in zip(self.find_withtag("snake"), self.snake_positions):
            self.coords(segment, position)

    def perform_actions(self):
        if self.check_collisions():
            self.end_game()
            return
        self.check_food_collision()
        self.move_snake()
        self.after(self.speed, self.perform_actions)  # 使用速度變數設定定時器間隔

    def check_collisions(self):
        head_x_position, head_y_position = self.snake_positions[0]
        return (
            head_x_position in (0, 600)
            or head_y_position in (20, 400)
            or (head_x_position, head_y_position) in self.snake_positions[1:]
        )

    def on_key_press(self, event):
        new_direction = event.keysym
        all_directions = ("Up", "Down", "Left", "Right")
        opposites = ({"Up", "Down"}, {"Left", "Right"})

        if (
            new_direction in all_directions
            and {new_direction, self.direction} not in opposites
        ):
            self.direction = new_direction

    def check_food_collision(self):
        if self.snake_positions[0] == self.food_position:
            self.score += 1
            self.snake_positions.append(self.snake_positions[-1])

            if self.score % 5 == 0:
                self.speed -= 10  # 每吃到五個食物速度加快10毫秒
                if self.speed < 50:  # 限制最快速度為50毫秒
                    self.speed = 50

            self.create_image(*self.snake_positions[-1], image=self.snake_body_image, tag="snake")
            self.food_position = self.set_new_food_position()
            self.coords(self.find_withtag("food"), self.food_position)
            self.itemconfig(self.find_withtag("score"), text=f"分數: {self.score}", tag="score")

    def set_new_food_position(self):
        while True:
            x_position = random.randint(1, 29) * 20
            y_position = random.randint(3, 19) * 20
            food_position = (x_position, y_position)

            if food_position not in self.snake_positions:
                return food_position

    def end_game(self):
        self.game_over = True
        self.delete(tk.ALL)
        self.create_text(
            self.winfo_width() / 2,
            self.winfo_height() / 2,
            text=f"{'遊戲結束！':>17}\n\n{'結算分數: ':>17}{self.score}\n\n按下空白鍵重新開始遊戲",
            fill="#fff",
            font=("TkDefaultFont", 24)
        )
        self.bind_all("<Key>", self.restart_game)

    def restart_game(self, event):
        if event.keysym == "space":
            self.delete(tk.ALL)
            self.init_game()

# 建立主視窗
root = tk.Tk()
root.title("貪吃蛇遊戲")
root.resizable(False, False)

# 加入遊戲區域
board = Snake(root)
board.pack()

# 執行主迴圈
root.mainloop()