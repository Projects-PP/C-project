# 🕹️ Interaktywne Szachy w C (SDL2)

Interaktywna, graficzna gra w szachy zaimplementowana w całości w języku C. Oferuje modułową architekturę, dynamiczne zarządzanie pamięcią dla historii ruchów (Undo/Redo), solidną obsługę błędów, zapisywanie/wczytywanie gier oraz logowanie zdarzeń.

Projekt jest zgodny z wymaganiami projektu zaliczeniowego dla przedmiotu **Programowanie niskopoziomowe w języku C**.

---

## 🧩 Szczegóły Projektu

* **Przedmiot**: Programowanie niskopoziomowe w języku C
* **Rok akademicki**: 2025/2026
* **Prowadzący**: Dr. Agnieszka Rybarczyk
* **Format**: Projekt zaliczeniowy
* **Wykonane przez**: Rostyslav Kovalenko, Przemysław Afelt
* **Środowisko i System**: Kompilowane i testowane w środowisku **VS Code** na systemach **macOS** oraz **Windows 10/11**

---

## 🏗️ Funkcjonalności

1. **Pełna Walidacja Zasad Gry**:
   - Walidacja podstawowych ruchów dla pawn, knight, bishop, rook, queen, i king.
   - Wykrywanie szacha, mata oraz pata.
2. **Interaktywny Interfejs Graficzny (GUI)**:
   - Okno graficzne (800x600 px) zbudowane przy użyciu biblioteki SDL2.
   - Renderowanie przezroczystych wektorowych grafik figur.
   - Wizualne wyróżnienie aktualnie wybranej figury.
3. **Dynamiczna Historia Ruchów w Pamięci (Undo/Redo)**:
   - Dynamiczna lista dwukierunkowa (`HistoryNode`) śledząca wszystkie stany planszy.
   - Przyciski na panelu bocznym do cofania (`Undo`) oraz ponawiania (`Redo`) ruchów w historii gry.
4. **Operacje na Plikach**:
   - **Wczytywanie gry**: Kliknięcie przycisku "Load" otwiera natywne okno wyboru plików systemu macOS/Windows w celu wznowienia zapisanej gry z pliku binarnego `.sav`.
   - **Automatyczny zapis z sygnaturą czasową**: Automatycznie zapisuje grę przy zamknięciu okna do folderu `saves/` z unikalną sygnaturą czasową (np. `game_20260614_190000.sav`).
   - **Logowanie zdarzeń**: Ruchy są zapisywane do pliku `logs/chess.log` w standardowej notacji algebraicznej.

---

## 💾 Struktura Repozytorium

```
.
├── assets/
│   ├── images/         
│   └── fonts/          
├── docs/
│   ├── project_specification.md 
│   └── user_manual.md            
├── include/
│   ├── game_logic.h    
│   ├── gui.h          
│   └── file_operations.h 
├── src/
│   ├── game_logic.c    
│   ├── gui.c           
│   ├── file_operations.c 
│   └── main.c          
├── logs/               
├── saves/              
├── tests/
│   ├── test_logic.c    
│   └── test_gui.c      
├── Makefile            
├── .gitignore          
└── README.md           
```

---

## ⚙️ Kompilacja i Uruchamianie

### Wymagania
Upewnij się, że masz zainstalowane wymagane biblioteki SDL2:
- **macOS**: `brew install sdl2 sdl2_image sdl2_ttf pkg-config`
- **Ubuntu/Debian**: `sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev pkg-config`
- **Windows (MSYS2)**: `pacman -S mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_image mingw-w64-x86_64-SDL2_ttf pkg-config` (lub pobierz oficjalne paczki deweloperskie ze stron SDL2)

### Przepisy Kompilacji
Użyj poleceń `Makefile` w głównym katalogu:
- **Kompilacja i Uruchomienie**:
  ```bash
  make run
  ```
- **Czyszczenie kompilacji**:
  ```bash
  make clean
  ```
- **Uruchomienie Testów Jednostkowych**:
  ```bash
  make test
  ```
