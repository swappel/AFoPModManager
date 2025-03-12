from tkinter import *
from tkinter import ttk

root = Tk()
root.title("FoP Mod Manager")
root.minsize(1000, 600)

root.option_add("*tearOff", FALSE)

menubar = Menu(root)

menunames = ["test", "Holy beautiful", "bruh"]

filemenu = Menu(menubar, tearoff=0)

profileMenu = Menu(filemenu)
for name in menunames:
    profileMenu.add_command(label=name)


filemenu.add_cascade(label="Profiles", menu=profileMenu)

menubar.add_cascade(label="File", menu=filemenu)

mainframe = ttk.Frame(root, padding="3 3 12 12")
mainframe.grid(column=0, row=0, sticky=(N, W, E, S))
root.columnconfigure(0, weight=1)
root.rowconfigure(0, weight=1)

for child in mainframe.winfo_children():
    child.grid_configure(padx=5, pady=5)

root.config(menu=menubar)

root.config(background="black")
root.mainloop()