import random as rand
from res import lib
from tkinter import *

m = lib.Test()
win = Tk()
win.title("My Functions")
win.tk.call('wm', 'iconphoto', win, PhotoImage(file='calculator.png'))
# win.iconbitmap('./res/img/calculator.png')

width, height = 600, 100
src_width = win.winfo_screenwidth()
src_height = win.winfo_screenheight()
win_x = (src_width - width) // 2
win_y = (src_height - height) // 2

win.resizable(1,0)

win.geometry(f'{width}x{height}+{win_x}+{win_y}')

txt_1, txt_2 = StringVar(), StringVar()

def chack(N1, N2):
    win.geometry(f'{width}x{height}')

    def flg(Ns):
        ck, typ, p = "0123456789", 's', True
        for n in Ns:
            if n not in ck:
                if n == '.':
                    if p:
                        typ = 'f'
                        p=False
                        continue
                    typ='s'
                    break
                else:
                    typ = 's'
                    break      
            else:
                if p: typ = 'i'
        return typ

    if   flg(N1) == 'i' and flg(N2) == 'i': return int(N1), int(N2)
    elif flg(N1) == 'f' and flg(N2) == 'f': return float(N1), float(N2)
    elif ((flg(N1) == 'i' and flg(N2) == 'f') or
          (flg(N1) == 'f' and flg(N2) == 'i')): return float(N1), float(N2)
    else: return N1, N2

def btn1_event():
    tmp,st1,st2 = '0123456789.', '', ''
    n = rand.randint(5, 10)
    while n:
        st1 += tmp[rand.randint(0,len(tmp))-1]
        st2 += tmp[rand.randint(0,len(tmp))-1]
        if st1[-1] == '.' or st2[-1] == '.': tmp = '0123456789'
        n-=1
    txt_1.set(f'{st1}')
    txt_2.set(f'{st2}')
    ent_1.configure(textvariable=txt_1)
    ent_2.configure(textvariable=txt_2)

def btn2_event():
    tmp,st1,st2 = '0123456789.abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ,~!@#$%^&*()_-+=/<>?', '',''
    n = rand.randint(5, 10)
    while n:
        st1 += tmp[rand.randint(0,len(tmp))-1]
        st2 += tmp[rand.randint(0,len(tmp))-1]
        n-=1
    txt_1.set(f'{st1}')
    txt_2.set(f'{st2}')
    ent_1.configure(textvariable=txt_1)
    ent_2.configure(textvariable=txt_2)    

def btn3_event():
    x, y = chack(ent_1.get(), ent_2.get())
    ans = m.pySum(x, y)
    anstp = str(type(ans))
    lab.configure(text=f'Ans: {ans}\nType: {anstp[8 : -2]}')

def btn4_event():
    x, y = chack(ent_1.get(), ent_2.get())
    ans = m.pySub(x, y)
    anstp = str(type(ans))
    lab.configure(text=f'Ans: {ans}\nType: {anstp[8 : -2]}')


# GUI
frame_1 = Frame(win)
frame_1.pack()
frame_2 = Frame(frame_1)
frame_2.grid(column=0, row=1)

ent_1 = Entry(frame_1)
ent_1.grid(column=0, row=0,ipadx=65, ipady=6)
ent_2 = Entry(frame_1)
ent_2.grid(column=1, row=0,ipadx=65, ipady=6, sticky=W)

btn_1 = Button(frame_2, text="Random Number", width=15, command=btn1_event)
btn_1.grid(column=0, row=0)

btn_2 = Button(frame_2, text="Random String", width=15, command=btn2_event)
btn_2.grid(column=0, row=1)

btn_3 = Button(frame_2, text="+ SUM +", width=15, command=btn3_event)
btn_3.grid(column=1, row=0)

btn_3 = Button(frame_2, text="- SUB -", width=15, command=btn4_event)
btn_3.grid(column=1, row=1)

lab = Label(frame_1, text='')
lab.grid(column=1, row=1)

win.mainloop()