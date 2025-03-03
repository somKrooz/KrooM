import KrooM

def Print():
  print("Hello")

def CreateFolder():
  print("Folder Has been Created")

def Log():
  for i in range(100):
    print("Krooz is Cooking\n")

def Update():
  val = KrooM.Getvalue(slider)
  KrooM.CreateText(str(round(val, 2)),50,320)


KrooM.Init("Krooz Window",500 ,600)

slider = KrooM.CreateSlider(50,300,200)
KrooM.Updator(Update)


btn0 = KrooM.CreateButton("Krooz",30,500,100,30)
KrooM.setCallback(btn0,Print)

btn1 = KrooM.CreateButton("Cool",30,450,130,30)
KrooM.setCallback(btn1,Log)

btn2 = KrooM.CreateButton("UI",30,400,200,30)
KrooM.setCallback(btn2,CreateFolder)


KrooM.RunWindow()
