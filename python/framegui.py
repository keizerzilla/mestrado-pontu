from tkinter import *
from tkinter import ttk
from tkinter.scrolledtext import *

# WINDOW
# ------------------------------------------------------------------------------
window = Tk()
window.title("mcalc")
#window.geometry("640x480")
window.resizable(False, False)

# VARIABLES
# ------------------------------------------------------------------------------
cuts = {"whole" : IntVar(),
        "frontal" : IntVar(),
        "sagittal" : IntVar(),
        "transversal" : IntVar(),
        "radial" : IntVar(),
        "upper" : IntVar(),
        "lower" : IntVar()}

moments = {"spheric" : IntVar(),
           "hututu" : IntVar(),
           "hu1980" : IntVar(),
           "husiq" : IntVar(),
           "zernike" : IntVar(),
           "legendre" : IntVar(),
           "chebyshev" : IntVar()}

# MENUS
# ------------------------------------------------------------------------------
menu = Menu(window)
menu.add_command(label="File")
menu.add_command(label="Preferences")
menu.add_command(label="About")
window.config(menu=menu)

# TABS
# ------------------------------------------------------------------------------
tab_control = ttk.Notebook(window)
tab_extraction = ttk.Frame(tab_control)
tab_classification = ttk.Frame(tab_control)
tab_combination = ttk.Frame(tab_control)
tab_control.add(tab_extraction, text="Extraction")
tab_control.add(tab_classification, text="Classification")
tab_control.add(tab_combination, text="Combination")

# TAB:EXTRACTION
# ------------------------------------------------------------------------------
extraction_label = Label(window, text="Cuts")
extraction_label.pack()
i = 0
for key, value in cuts.items():
	Checkbutton(window, text=key, variable=value).pack()
	i = i + 1

# TAB:FIN
# ------------------------------------------------------------------------------
tab_control.pack(expand=1, fill="both")

# CONSOLE
# ------------------------------------------------------------------------------
console = ScrolledText(window, height=8, width=80)
console.config(state="disabled")
console.pack()

window.mainloop()

