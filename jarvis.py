import os
import random
import shutil
import subprocess
import sys
import urllib.parse
import urllib.request
import webbrowser
from datetime import datetime
from pathlib import Path


try:
    import pyttsx3
except ImportError:
    pyttsx3 = None

try:
    import speech_recognition as sr
except ImportError:
    sr = None

try:
    import pyaudio  # noqa: F401
except ImportError:
    pyaudio = None


ASSISTANT_NAME = "Jarvis"
MUSIC_URL = "https://music.youtube.com"
WEATHER_URL = "https://wttr.in/{city}?format=3"
NOTES_FILE = Path(__file__).with_name("jarvis_notes.txt")
JOKES = [
    "Why do programmers prefer dark mode? Because light attracts bugs.",
    "I told my computer I needed a break, and it said no problem, it would go to sleep.",
    "There are 10 types of people: those who understand binary and those who do not.",
]


def speak(message):
    print(f"{ASSISTANT_NAME}: {message}")
    if pyttsx3 is None:
        return

    try:
        engine = pyttsx3.init()
        engine.say(message)
        engine.runAndWait()
    except Exception:
        pass


def listen():
    if sr is None or pyaudio is None:
        return input("You: ").strip().lower()

    recognizer = sr.Recognizer()
    with sr.Microphone() as source:
        print("Listening...")
        recognizer.adjust_for_ambient_noise(source, duration=0.5)
        audio = recognizer.listen(source)

    try:
        text = recognizer.recognize_google(audio)
        print(f"You: {text}")
        return text.lower()
    except sr.UnknownValueError:
        speak("Sorry, I did not catch that.")
    except sr.RequestError:
        speak("Speech recognition service is unavailable.")

    return ""


def open_chrome():
    chrome_paths = [
        r"C:\Program Files\Google\Chrome\Application\chrome.exe",
        r"C:\Program Files (x86)\Google\Chrome\Application\chrome.exe",
    ]

    for path in chrome_paths:
        if os.path.exists(path):
            subprocess.Popen([path])
            speak("Opening Chrome.")
            return

    if shutil.which("chrome"):
        subprocess.Popen(["chrome"])
        speak("Opening Chrome.")
        return

    webbrowser.open("https://www.google.com")
    speak("Chrome was not found, so I opened Google in your default browser.")


def play_music():
    music_folder = os.path.join(os.path.expanduser("~"), "Music")
    if os.path.isdir(music_folder) and any(os.scandir(music_folder)):
        os.startfile(music_folder)
        speak("Opening your Music folder.")
        return

    webbrowser.open(MUSIC_URL)
    speak("Opening YouTube Music.")


def open_youtube(command):
    query = command.replace("open youtube", "", 1).replace("youtube", "", 1).strip()
    if query:
        webbrowser.open(f"https://www.youtube.com/results?search_query={urllib.parse.quote(query)}")
        speak(f"Searching YouTube for {query}.")
        return

    webbrowser.open("https://www.youtube.com")
    speak("Opening YouTube.")


def tell_weather():
    city = input("City for weather: ").strip()
    if not city:
        speak("Please enter a city name.")
        return

    url = WEATHER_URL.format(city=urllib.parse.quote(city))
    try:
        with urllib.request.urlopen(url, timeout=8) as response:
            weather = response.read().decode("utf-8").strip()
        speak(weather)
    except Exception:
        webbrowser.open(f"https://www.google.com/search?q={urllib.parse.quote('weather ' + city)}")
        speak("I could not fetch the weather directly, so I opened the forecast on Google.")


def open_vs_code():
    if shutil.which("code"):
        subprocess.Popen(["code"])
        speak("Opening VS Code.")
        return

    code_paths = [
        os.path.join(os.environ.get("LOCALAPPDATA", ""), r"Programs\Microsoft VS Code\Code.exe"),
        r"C:\Program Files\Microsoft VS Code\Code.exe",
    ]

    for path in code_paths:
        if path and os.path.exists(path):
            subprocess.Popen([path])
            speak("Opening VS Code.")
            return

    speak("VS Code was not found on this computer.")


def open_notepad():
    subprocess.Popen(["notepad"])
    speak("Opening Notepad.")


def open_calculator():
    subprocess.Popen(["calc"])
    speak("Opening Calculator.")


def open_folder(folder_name):
    folders = {
        "desktop": Path.home() / "Desktop",
        "documents": Path.home() / "Documents",
        "downloads": Path.home() / "Downloads",
        "music": Path.home() / "Music",
        "pictures": Path.home() / "Pictures",
    }
    folder = folders.get(folder_name)

    if folder and folder.exists():
        os.startfile(folder)
        speak(f"Opening {folder_name}.")
    else:
        speak(f"I could not find your {folder_name} folder.")


def search_google(command):
    query = command.replace("search google", "", 1).strip()
    if not query:
        query = input("Search Google for: ").strip()

    if not query:
        speak("Please give me something to search.")
        return

    webbrowser.open(f"https://www.google.com/search?q={urllib.parse.quote(query)}")
    speak(f"Searching Google for {query}.")


def search_wikipedia(command):
    query = command.replace("search wikipedia", "", 1).replace("wikipedia", "", 1).strip()
    if not query:
        query = input("Search Wikipedia for: ").strip()

    if not query:
        speak("Please give me something to search.")
        return

    webbrowser.open(f"https://en.wikipedia.org/wiki/Special:Search?search={urllib.parse.quote(query)}")
    speak(f"Searching Wikipedia for {query}.")


def tell_time():
    speak(f"The time is {datetime.now().strftime('%I:%M %p')}.")


def tell_date():
    speak(f"Today is {datetime.now().strftime('%A, %d %B %Y')}.")


def take_note(command):
    note = command.replace("take note", "", 1).replace("add note", "", 1).strip()
    if not note:
        note = input("Note: ").strip()

    if not note:
        speak("No note was added.")
        return

    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M")
    with NOTES_FILE.open("a", encoding="utf-8") as file:
        file.write(f"[{timestamp}] {note}\n")
    speak("Note saved.")


def read_notes():
    if not NOTES_FILE.exists() or NOTES_FILE.stat().st_size == 0:
        speak("You do not have any notes yet.")
        return

    speak("Here are your notes.")
    print(NOTES_FILE.read_text(encoding="utf-8").strip())


def tell_joke():
    speak(random.choice(JOKES))


def shutdown_pc():
    confirmation = input("Type YES to shutdown this PC: ").strip()
    if confirmation == "YES":
        speak("Shutting down in 10 seconds.")
        subprocess.run(["shutdown", "/s", "/t", "10"], check=False)
    else:
        speak("Shutdown cancelled.")


def show_help():
    print(
        """
Commands:
  open chrome
  play music
  tell weather
  open vs code
  open youtube <optional search>
  open notepad
  open calculator
  open desktop/documents/downloads/music/pictures
  search google <your search>
  search wikipedia <your search>
  time
  date
  take note <your note>
  read notes
  joke
  shutdown pc
  help
  exit
""".strip()
    )


def handle_command(command):
    if not command:
        return True

    if "open chrome" in command:
        open_chrome()
    elif "play music" in command:
        play_music()
    elif "open youtube" in command or command.startswith("youtube"):
        open_youtube(command)
    elif "tell weather" in command or "weather" in command:
        tell_weather()
    elif "open vs code" in command or "open vscode" in command:
        open_vs_code()
    elif "open notepad" in command:
        open_notepad()
    elif "open calculator" in command or "open calc" in command:
        open_calculator()
    elif command.startswith("open ") and command.replace("open ", "", 1) in {
        "desktop",
        "documents",
        "downloads",
        "music",
        "pictures",
    }:
        open_folder(command.replace("open ", "", 1))
    elif "search google" in command or command.startswith("google "):
        search_google(command.replace("google ", "search google ", 1))
    elif "search wikipedia" in command or command.startswith("wikipedia"):
        search_wikipedia(command)
    elif command in {"time", "tell time", "what time is it"}:
        tell_time()
    elif command in {"date", "tell date", "what is the date"}:
        tell_date()
    elif command.startswith("take note") or command.startswith("add note"):
        take_note(command)
    elif command in {"read notes", "show notes", "my notes"}:
        read_notes()
    elif command in {"joke", "tell joke", "tell me a joke"}:
        tell_joke()
    elif "shutdown pc" in command or "shutdown computer" in command:
        shutdown_pc()
    elif command in {"help", "commands"}:
        show_help()
    elif command in {"exit", "quit", "stop"}:
        speak("Goodbye.")
        return False
    else:
        speak("Command not recognized. Say help to see available commands.")

    return True


def main():
    speak("Hello, I am Jarvis. How can I help?")
    show_help()

    while True:
        if not handle_command(listen()):
            break


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        speak("Goodbye.")
        sys.exit(0)
