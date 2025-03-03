# KrooM UI Library

A lightweight, immediate-mode UI library for OpenGL applications, built as a personal learning project.

## Features

- Uses old OpenGL immediate mode
- Immediate mode UI paradigm for simplicity
- Core widgets: buttons, sliders, text rendering
- Windows-only (not cross-platform)
- Native Python bindings

```python
import KrooM
KrooM.Init("Krooz Window",500 ,600)

btn = KrooM.CreateButton("Cool Button",30,500,100,30)
KrooM.setCallback(btn,lambda: print("Hello World"))

KrooM.RunWindow()
