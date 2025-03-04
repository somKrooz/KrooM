import KrooM
import subprocess
import threading

text = ""
isAppled = False

def Update():
    global text, message
    val = KrooM.CreateInput(50, 400, 300, 40)
    text = KrooM.GetTex(val)

    if isAppled:
        message = "Applied"
    else:
        message = "Apply Wallpapers"

    KrooM.CreateText(message, 110, 300)

def resetMessage():
    global isAppled
    isAppled = False

def Applywallpaper(text: str):
    global isAppled
    isAppled = True
    subprocess.run(["wall", text])

    threading.Timer(2.0, resetMessage).start()

KrooM.Init("Krooz Window", 400, 500)

KrooM.Updator(Update)

btn0 = KrooM.CreateButton("Apply", 90, 350, 200, 30)
KrooM.setCallback(btn0, lambda: Applywallpaper(text))

KrooM.RunWindow()
