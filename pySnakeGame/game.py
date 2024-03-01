import tkinter as tk
from food import Food
from snake import Snake

# 定義貪吃蛇類別
class Game(tk.Canvas):
    def __init__(self, master):
        super().__init__(master, width=600, height=400, background="black", highlightthickness=0)
        self.snake = Snake(self)
        self.food = Food(self)
        self.init_game()

    def init_game(self):
        self.score = 0
        self.game_over = False

        self.load_assets()
        self.snake.init_snake()
        self.food.set_new_position(self.snake.positions)
        self.create_objects()

        self.bind_all("<Key>", self.snake.on_key_press)
        self.after(self.snake.speed, self.perform_actions)  # 使用速度變數設定定時器間隔

    def load_assets(self):
        try:
            self.snake.image = tk.PhotoImage(file="./img/snake_body.png")
            self.food.image = tk.PhotoImage(file="./img/snake_food.png")
        except Exception as e:
            print(f"載入資源時發生錯誤: {e}")
            self.quit()

    def create_objects(self):
        self.create_text(35, 12, text=f"分數: {self.score}", tag="score", fill="#fff", font=("TkDefaultFont", 14))
        for x_position, y_position in self.snake.positions:
            self.create_image(x_position, y_position, image=self.snake.image, tag="snake")
        self.create_image(*self.food.position, image=self.food.image, tag="food")
        self.create_rectangle(7, 27, 593, 393, outline="#525d69")

    def perform_actions(self):
        if self.snake.check_snake_collisions():
            self.end_game()
            return

        if self.food.check_food_collision(self.snake.positions):
            self.score += 1
            if self.score % 5 == 0:
                self.snake.speed -= 10  # 每吃到五個食物速度加快10毫秒
                if self.snake.speed < 50:  # 限制最快速度為50毫秒
                    self.snake.speed = 50

            self.create_image(*self.snake.positions[-1], image=self.snake.image, tag="snake")
            self.food.set_new_position(self.snake.positions)
            self.coords(self.find_withtag("food"), self.food.position)
            self.itemconfig(self.find_withtag("score"), text=f"分數: {self.score}", tag="score")

        self.snake.move_queue()
        for segment, position in zip(self.find_withtag("snake"), self.snake.positions):
            self.coords(segment, position)
        self.after(self.snake.speed, self.perform_actions)  # 使用速度變數設定定時器間隔

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
board = Game(root)
board.pack()

# 執行主迴圈
root.mainloop()