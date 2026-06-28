# Jarvis Voice Assistant

Jarvis is a Windows-friendly Python assistant that can open apps, search the web,
check weather, manage simple notes, and handle common desktop commands.

## Features

- Open Chrome
- Play music
- Tell weather
- Open VS Code
- Open YouTube
- Open Notepad
- Open Calculator
- Open Desktop, Documents, Downloads, Music, or Pictures
- Search Google
- Search Wikipedia
- Tell time and date
- Take notes and read notes
- Tell a joke
- Shutdown PC with confirmation

## Requirements

- Python 3
- Windows for app-launching commands such as Notepad, Calculator, and shutdown

Optional packages:

```powershell
python -m pip install pyttsx3 SpeechRecognition PyAudio
```

If `PyAudio` is not installed, Jarvis still works with typed commands.

## How to Run

From the project folder:

```powershell
python jarvis.py
```

Or double-click:

```text
Run_Jarvis.bat
```

## Commands

```text
open chrome
play music
tell weather
open vs code
open youtube
open youtube lo fi music
open notepad
open calculator
open desktop
open documents
open downloads
open music
open pictures
search google python tutorial
search wikipedia artificial intelligence
time
date
take note complete homework
read notes
joke
shutdown pc
help
exit
```

## Notes

- Shutdown requires typing `YES` before the command runs.
- Notes are saved locally in `jarvis_notes.txt`.
- `jarvis_notes.txt` is ignored by Git so personal notes are not uploaded.
