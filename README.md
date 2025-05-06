Pokémon Catching Simulator (C++)

A fun, text-based Pokemon catching simulator built with C++. Inspired by the nostalgia of the early games and the mechanics of Pokémon Go, this project lets you explore regions, encounter wild Pokemon, and try your luck at catching them.


Game Highlights
- Choose your region: Kanto, Johto, Hoenn, and Sinnoh
- Randomly encounter wild Pokemon, each with unique stats
- Some Pokemon take multiple throws to catch or might escape!
- Automatically catch baby Pokémon and send them to daycare
- Use different types of Poke Balls with different catch bonuses
- Track every Pokemon you've seen or caught in your personal Pokedex
- Search Pokedex entries by name

---

Input File Format
The game reads Pokemon data from a file called `poke.txt`. 
Make sure it’s in the same directory as your executable.

** Example format:**
```
<number_of_records>
<number>,<name>,<type>,<region>,<catchRate>,<dexEntry>
```
Each line contains one Pokemon's full data.

---

How to Run the Game
Step 1: Compile the program
```bash
g++ -std=c++11 poke.cpp -o poke
```

Step 2: Run the game
```bash
./poke
```

Make sure `poke.txt` is in the same folder.

---

Commands During Gameplay
Here’s what you can type during the game:
- `HUNT` – Search for a random Pokémon
- `POKEMON` – View the Pokémon you've caught so far
- `STATISTICS` – See your total seen, caught, and catch rate
- `INVENTORY` – See how many balls you have left
- `DEX NAME` – Look up any Pokémon's Pokédex info by name
- `MENU` – Display the list of commands again
- `EXIT` – End the game and see your final stats

---

Project Scope
This project was built for a Computer Science course to practice:
- Structs and dynamic arrays
- File I/O and data parsing
- Random number generation and probability logic
- Clean, readable procedural programming
- Basic user input handling


---

About the Developer
Darya Pylypenko 
Computer Science Major @ Florida State University  
Looking for Summer 2025 internships in software development!

---

License
This project is open for educational use. If you fork or build on it, a shoutout would be appreciated!
