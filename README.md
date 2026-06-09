# Symulacja Roju Robotów
 
Aplikacja symulująca zachowanie roju autonomicznych robotów w środowisku 2D. Zaimplementowane algorytmy opierają się na klasycznym modelu flockingu Reynoldsa, rozszerzonym o formowanie wzorców.
 
Projekt powstał w **C++/CLI** z wykorzystaniem **Windows Forms**.
 
---
 
## Wymagania
 
- Visual Studio 2019 lub nowszy z pakietem **Desktop development with C++** oraz obsługą **C++/CLI**
---
 
## Uruchomienie
 
1. Otwórz plik rozwiązania `SymulacjaRojuRobotow.sln` w Visual Studio.
2. Ustaw `SymulacjaRojuRobotowFrontend` jako projekt startowy (prawy klik → *Set as Startup Project*).
3. Skompiluj i uruchom (`F5`).
 
## Instrukcja obsługi
 
### Podstawowe sterowanie
 
| Klawisz | Akcja |
|---|---|
| `P` | Pauza / wznów symulację |
| `A` | Dodaj robota w losowym miejscu |
| `X` | Usuń losowego robota |
| `+` | Przyspiesz symulację (maks. 5×) |
| `-` | Zwolnij symulację (min. 0.25×) |
 
### Sterowanie myszą
 
| Przycisk | Akcja |
|---|---|
| `LPM` na obszarze | Dodaj robota w miejscu kliknięcia |
| `PPM` na obszarze | Usuń robota najbliższego kursora |
 
| `C` | Zacznij rysować własną formację |
 
W trybie rysowania własnej formacji (`C`):
- **LPM** — dodaj punkt formacji (czerwona kropka)
- **PPM** — zatwierdź i zapisz formację
Po zatwierdzeniu aktywuj formację klawiszem `4`.
 
---
 
## Zachowania roju
 
### Flocking (klawisz `1`)
 
Domyślny tryb oparty na trzech regułach Reynoldsa:
 
- **Kohezja** — roboty przyciągają się do środka masy lokalnej grupy
- **Separacja** — roboty odpychają się gdy są zbyt blisko siebie
- **Wyrównanie** — roboty dopasowują kierunek ruchu do sąsiadów
### Formacje (klawisze `2`–`4`)
 
| Klawisz | Formacja |
|---|---|
| `2` | Koło — roboty równomiernie na okręgu |
| `3` | Siatka hexagonalna |
| `4` | Własna formacja zdefiniowana przez użytkownika |
 
Zmiana formacji podczas symulacji jest możliwa w dowolnym momencie.
 
---
 
## HUD
 
W lewym górnym rogu ekranu wyświetlane są na bieżąco:
 
- liczba aktywnych robotów
- aktualna prędkość symulacji
---
 
## Algorytm formowania wzorców
 
Przypisanie robotów do punktów formacji realizowane jest algorytmem dopasowania: każdy robot otrzymuje najbliższy wolny punkt docelowy. Wyniki są buforowane i przeliczane tylko przy zmianie trybu lub liczby robotów, co eliminuje niestabilność wynikającą z przeliczania co klatkę.

---

## Udział w pracy

- Mikołaj Krok: część graficzna CLI, w tym wyświetlanie robotów, skróty klawiszowe i elementy GUI
- Adam Michalik: pattern forming, w tym formacja kołowa i siatka sześciokątna
- Tobiasz Kobza: zachowanie stadne (flocking), w tym zachowania rojowe kohezji, separacji i wyrównania
