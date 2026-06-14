# Specyfikacja Techniczna — Szachy w C

Ten dokument opisuje architekturę techniczną, struktury danych oraz algorytmy zaimplementowanej gry w szachy w języku C przy użyciu biblioteki graficznej SDL2.

---

## 1. Architektura Modułowa

Projekt został podzielony na moduły, gdzie każdy komponent logiczny znajduje się w osobnych plikach:

* `src/main.c` — Punkt wejścia programu. Zarządza główną pętlą gry (event loop), obsługuje zdarzenia myszy i okna oraz inicjalizuje/zamyka system graficzny.
* `src/game_logic.c` / `include/game_logic.h` — Rdzeń silnika szachowego. Odpowiada za początkowe ustawienie planszy, walidację ruchów figur oraz wykrywanie szachów, matów i patów.
* `src/gui.c` / `include/gui.h` — Graficzny interfejs użytkownika. Rysuje szachownicę, ładuje i wyświetla sprite'y figur, rysuje przyciski panelu bocznego (New Game, Undo, Redo, Load) oraz obsługuje ekran końca gry (overlay).
* `src/file_operations.c` / `include/file_operations.h` — Operacje na plikach. Zapisuje aktualny stan gry do binarnych plików zapisu i loguje historię ruchów do pliku tekstowego.

---

## 2. Kluczowe Struktury Danych

### Figura Szachowa (`ChessPiece`)
Definiuje typ figury (pawn, knight, bishop, rook, queen, king lub empty square) oraz jej kolor:
```c
typedef struct {
    PieceType type;
    PieceColor color;
} ChessPiece;
```

### Stan Gry (`ChessState`)
Reprezentuje pełny chwilowy stan gry szachowej, który jest serializowany podczas zapisu:
```c
typedef struct {
    ChessPiece board[8][8];
    PieceColor turn;
} ChessState;
```

### Dynamiczna Historia Ruchów (`HistoryNode`)
Węzeł dwukierunkowej listy powiązanej, śledzący stany gry na potrzeby operacji Undo/Redo:
```c
typedef struct HistoryNode {
    ChessState state;
    struct HistoryNode* prev;
    struct HistoryNode* next;
} HistoryNode;
```

---

## 3. Algorytmy i Reguły Gry

* **Walidacja Ruchu**: Funkcja `is_move_valid` tymczasowo wykonuje ruch na planszy, sprawdza za pomocą `is_king_checked`, czy ruch nie wystawia króla na szach, cofa ruch i zwraca wynik walidacji.
* **Mat i Pat**: Funkcja `check_game_status` przechodzi przez wszystkie aktywne figury gracza wykonującego ruch, aby znaleźć choć jeden legalny ruch. Jeśli brak legalnych ruchów:
  * Jeśli król jest szachowany — następuje Mat (wygrana przeciwnika).
  * Jeśli król nie jest szachowany — następuje Pat (remis).
