# Instrukcja Użytkownika — Szachy w C

Ten podręcznik zawiera instrukcje dotyczące kompilacji, uruchamiania i sterowania interaktywną grą w szachy.

---

## 1. Wymagania Systemowe i Zależności

Aby skompilować i uruchomić grę, Twój system musi posiadać:
- Kompilator C (`gcc` lub `clang`) obsługujący standard C99.
- Biblioteki **SDL2**, **SDL2_image**, **SDL2_ttf** oraz narzędzie `pkg-config`.

### Instalacja na macOS:
```bash
brew install sdl2 sdl2_image sdl2_ttf pkg-config
```

### Instalacja na Ubuntu/Debian:
```bash
sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev pkg-config
```

### Instalacja na Windows (MSYS2):
```bash
pacman -S mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_image mingw-w64-x86_64-SDL2_ttf pkg-config
```

---

## 2. Kompilacja i Uruchomienie

Kompilacja projektu jest zarządzana za pomocą pliku `Makefile`:

* **Kompilacja i Uruchomienie**:
  ```bash
  make run
  ```
* **Czyszczenie plików kompilacji**:
  ```bash
  make clean
  ```


---

## 3. Sterowanie i Rozgrywka

* **Wybór figury**: Kliknij lewym przyciskiem myszy na swoją figurę na planszy. Zostanie ona podświetlona na żółto.
* **Wykonanie ruchu**: Po wybraniu figury kliknij na docelowe pole planszy. Jeśli ruch jest dozwolony, figura się tam przesunie.
* **Cofnięcie ruchu (Undo)**: Kliknij zielony przycisk **"Undo"** na panelu bocznym, aby cofnąć ostatni ruch.
* **Powtórzenie ruchu (Redo)**: Kliknij czerwony przycisk **"Redo"**, aby przywrócić cofnięty ruch.
* **Nowa gra (New Game)**: Kliknij niebieski przycisk **"New Game"**, aby zresetować planszę i rozpocząć nową grę (zaczynają białe).
* **Wczytanie gry (Load)**: Kliknij fioletowy przycisk **"Load"**, aby otworzyć systemowe okno dialogowe i wybrać plik zapisu `.sav` do wczytania.

---

## 4. Zapisywanie i Logowanie

* **Autozapis przy wyjściu**: Po zamknięciu okna gry stan planszy jest automatycznie zapisywany w katalogu `saves/` pod nazwą zawierającą znacznik czasu (np. `saves/game_20260614_190000.sav`).
* **Logi ruchów**: Wszystkie udane ruchy są zapisywane w pliku `logs/chess.log` w standardowej notacji szachowej (np. `Move: e2 -> e4`) wraz ze znacznikiem czasu.
