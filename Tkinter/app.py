# https://coderslegacy.com/python/ctypes-arrays/
from tkinter import *
from ctypes import *

m = cdll.LoadLibrary('./lib/libtk.so')

# Text Dict
txtDict = {
    "t": "Prime Number",
    "bg": "#323232",
    "fg": "#ECF0F1",
    "lab": ["起始數", "目標數", "A ~ B 質數表\n"],
    "btn": "確認"}


def btnEvent():
    try:
        a, b = int(ent_1.get()), int(ent_2.get())
        if a > b:
            a, b = b, a

        # 型態轉換
        py_getPS = m.getPrimeSheet
        py_getPS.argtypes = [c_int, c_int]
        py_getPS.restype = POINTER(c_int * (b-a+1))  # 陣列指標

        arr = [str(_) for _ in py_getPS(a, b).contents]

        count, chr = 0, txtDict["lab"][2]
        for i in arr:
            chr += i
            if count >= 60:
                chr += '\n'
                count = 0
            count += 1
        lab_3.config(text=chr)

    except:
        print('Error Variable Type.')


# # 傳入 C++ 陣列宣告方式
# values = [_ for _ in range(1, 4)]
# arrs = (c_int * len(values))(*values)


# tkinter gui
win = Tk()
win.title(txtDict["t"])

# 視窗大小
width, height = 600, 400
scr_width = win.winfo_screenwidth()
scr_height = win.winfo_screenheight()
win_x = int((scr_width - width) / 2)
win_y = int((scr_height - height) / 2)

win.resizable(0, 0)
# win.minsize(width, height)
# win.maxsize(int(width*1.5), int(height*1.5))

# 視窗啟動位置
win.geometry(f'{width}x{height}+{win_x}+{win_y}')
win.configure(bg=txtDict["bg"])

# 繪製元件
lab_1 = Label(text=txtDict["lab"][0], height=2)
lab_1.config(bg=txtDict["bg"], fg=txtDict["fg"], font=('', 15))
lab_1.pack()

ent_1 = Entry()
ent_1.pack()

lab_2 = Label(text=txtDict["lab"][1], height=2)
lab_2.config(bg=txtDict["bg"], fg=txtDict["fg"], font=('', 15))
lab_2.pack()

ent_2 = Entry()
ent_2.pack()

Label(text="", bg=txtDict["bg"], height=1).pack()

btn_1 = Button(text=txtDict["btn"], width=12, height=1)
btn_1.config(bg='#16A085', fg=txtDict["fg"])
btn_1.config(command=btnEvent)
btn_1.pack()

lab_3 = Label(text=txtDict["lab"][2], width=100, height=20)
lab_3.config(bg=txtDict["bg"], fg=txtDict["fg"], font=('', 12))
lab_3.pack()

win.mainloop()
