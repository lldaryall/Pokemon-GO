#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <cctype>

using namespace std;

struct Inventory {
    int pokeBalls;
    int greatBalls;
    int ultraBalls;
};
struct Pokemon {
    int number;
    string name;
    string type;
    string region;
    int catchRate;
    string dexEntry;
    int caught;
    int seen;
};

string toUpper(string str);
int getRegionMin(string region);
int getRegionMax(string region);
void loadData(Pokemon *&dex, int &size);
void displayMenu();
void huntPokemon(Pokemon *dex, int size, string region, int &pokeBalls, int &greatBalls, int &ultraBalls);
void showCaughtPokemon(Pokemon *dex, int size, string trainerName);
void showStatistics(Pokemon *dex, int size);
void showInventory(int pokeBalls, int greatBalls, int ultraBalls);
void searchDex(Pokemon *dex, int size);

int main() {
    srand(time(0));
    Pokemon* dex;
    int size;
    string trainerName;
    string region;
    string choice;
    Inventory inventory = {10, 10, 10};
    
    loadData(dex, size); 
    
    cout << "Welcome trainer! What is your name? ";
    getline(cin, trainerName);
    
    cout << "Choose a region to hunt (Kanto, Johto, Hoenn, Sinnoh): ";
    getline(cin, region);

    while (toUpper(region) != "KANTO" && toUpper(region) != "JOHTO" && toUpper(region) != "HOENN" && toUpper(region) != "SINNOH") {
        cout << "Invalid region. Choose again: ";
        getline(cin, region);
    }

    cout << "Traveling to " << region << " region...\n";
    displayMenu();

    do {
        cout << "\nSelection > ";
        cin >> choice;
        choice = toUpper(choice);

        if (choice == "HUNT") {
            if (inventory.pokeBalls + inventory.greatBalls + inventory.ultraBalls == 0) {
                cout << "No Pokeballs left!\n";
                break;
            }
            huntPokemon(dex, size, region, inventory);
        } else if (choice == "POKEMON") {
            showCaughtPokemon(dex, size, trainerName);
        } else if (choice == "STATISTICS") {
            showStatistics(dex, size);
        } else if (choice == "INVENTORY") {
            showInventory(inventory);
        } else if (choice == "DEX") {
            searchDex(dex, size);
        } else if (choice == "MENU") {
            displayMenu();
        } else if (choice != "EXIT") {
            cout << "Invalid option. Try again.\n";
        }

    } while (choice != "EXIT" && (inventory.pokeBalls + inventory.greatBalls + inventory.ultraBalls > 0));

    showCaughtPokemon(dex, size, trainerName);
    showStatistics(dex, size);
    delete[] dex;

    cout << "Thank you for playing!\n";
    return 0;
}
string toUpper(string str) {
    for (int i = 0; i < str.length(); i++) {
        str[i] = toupper(str[i]);
    }
    return str;
}
int getRegionMin(string region) {
    region = toUpper(region);
    if (region == "KANTO") return 1;
    if (region == "JOHTO") return 152;
    if (region == "HOENN") return 252;
    if (region == "SINNOH") return 387;
    return 1;
}

int getRegionMax(string region) {
    region = toUpper(region);
    if (region == "KANTO") return 151;
    if (region == "JOHTO") return 251;
    if (region == "HOENN") return 386;
    if (region == "SINNOH") return 493;
    return 151;
}

void loadData(Pokemon *&dex, int &size) {
    ifstream fin;
    string line;
    int comma1, comma2, comma3, comma4, comma5;

    fin.open("poke.txt");
    if (!fin) {
        cout << "POKE.TXT FILE FAILED TO OPEN, ENDING PROGRAM.\n";
        exit(1);
    }

    fin >> size;
    fin.ignore();
    dex = new Pokemon[size];

    for (int i = 0; i < size; i++) {
        getline(fin, line);
        comma1 = line.find(',');
        comma2 = line.find(',', comma1 + 1);
        comma3 = line.find(',', comma2 + 1);
        comma4 = line.find(',', comma3 + 1);
        comma5 = line.find(',', comma4 + 1);

        dex[i].number = stoi(line.substr(0, comma1));
        dex[i].name = line.substr(comma1 + 1, comma2 - comma1 - 1);
        dex[i].type = line.substr(comma2 + 1, comma3 - comma2 - 1);
        dex[i].region = line.substr(comma3 + 1, comma4 - comma3 - 1);
        dex[i].catchRate = stoi(line.substr(comma4 + 1, comma5 - comma4 - 1));
        dex[i].dexEntry = line.substr(comma5 + 1);
        dex[i].numCaught = 0;
        dex[i].numSeen = 0;
    }
    fin.close();
}

void displayMenu() {
    cout << "\nChoose an option:\n";
    cout << "HUNT\t\t(Go hunting for Pokemon!)\n";
    cout << "POKEMON\t\t(See the Pokemon you've caught.)\n";
    cout << "STATISTICS\t(See your catch statistics.)\n";
    cout << "INVENTORY\t(See your current inventory.)\n";
    cout << "DEX NAME\t(View a Pokedex entry.)\n";
    cout << "MENU\t\t(Reprint this menu.)\n";
    cout << "EXIT\t\t(Exit the game.)\n";
}

void huntPokemon(Pokemon *dex, int size, string region, Inventory &inventory) {
    int minNum = getRegionMin(region);
    int maxNum = getRegionMax(region);
    int wildNumber = (rand() % (maxNum - minNum + 1)) + minNum;
    int foundIndex = -1;
    for (int i = 0; i < size; i++) {
        if (dex[i].number == wildNumber) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1) {
        cout << "No Pokemon found!\n";
        return;
    }

    cout << "\nA wild " << dex[foundIndex].name << " has appeared!\n";
    if (dex[foundIndex].catchRate == 0) {
        cout << "Oh! That's a baby Pokemon! We'll take them to the daycare.\n";
        dex[foundIndex].numSeen++;
        dex[foundIndex].numCaught++;
        return;
    }

    bool caught = false;
    bool ran = false;
    while (!caught && !ran && (inventory.pokeBalls + inventory.greatBalls + inventory.ultraBalls > 0)) {
        int choice;
        int bonus = 0;
        cout << "Choose a ball to throw:\n";
        cout << "1 - Poke Ball   (You have: " << inventory.pokeBalls << ")\n";
        cout << "2 - Great Ball  (You have: " << inventory.greatBalls << ")\n";
        cout << "3 - Ultra Ball  (You have: " << inventory.ultraBalls << ")\n";
        cout << "Selection > ";
        cin >> choice;

        while (choice < 1 || choice > 3 ||
               (choice == 1 && inventory.pokeBalls == 0) ||
               (choice == 2 && inventory.greatBalls == 0) ||
               (choice == 3 && inventory.ultraBalls == 0)) {
            cout << "Invalid choice. Try again: ";
            cin >> choice;
        }

        if (choice == 1) { inventory.pokeBalls--; bonus = 0; }
        if (choice == 2) { inventory.greatBalls--; bonus = 20; }
        if (choice == 3) { inventory.ultraBalls--; bonus = 40; }

        int finalCatchRate = dex[foundIndex].catchRate + bonus;
        int chance = (rand() % 100) + 1;

        if (chance <= finalCatchRate) {
            cout << "You caught " << dex[foundIndex].name << "!\n";
            dex[foundIndex].numCaught++;
            caught = true;
        } else {
            cout << "Oh no! " << dex[foundIndex].name << " broke free!\n";
            int fleeChance = (rand() % 100) + 1;
            if (fleeChance <= 30) {
                cout << dex[foundIndex].name << " ran away!\n";
                ran = true;
            } else {
                cout << dex[foundIndex].name << " is still here! Try again.\n";
            }
        }
    }
    dex[foundIndex].numSeen++;
}

void showCaughtPokemon(Pokemon *dex, int size, string trainerName) {
    cout << trainerName << "'s POKEMON:\n";
    for (int i = 0; i < size; i++) {
        if (dex[i].numCaught > 0) {
            cout << "no " << dex[i].number << "   " << dex[i].name << '\n';
        }
    }
}

void showStatistics(Pokemon *dex, int size) {
    int totalCaught = 0, totalSeen = 0;
    for (int i = 0; i < size; i++) {
        totalCaught += dex[i].numCaught;
        totalSeen += dex[i].numSeen;
    }
    cout << fixed << setprecision(2);
    cout << "Total Pokemon Caught: " << totalCaught << '\n';
    cout << "Total Pokemon Seen: " << totalSeen << '\n';
    if (totalSeen == 0)
        cout << "Overall Catch Rate: 0.00%\n";
    else
        cout << "Overall Catch Rate: " << (static_cast<double>(totalCaught) / totalSeen) * 100 << "%\n";
}

void showInventory(Inventory inventory) {
    cout << "You have:\n";
    cout << inventory.pokeBalls << " PokeBalls\n";
    cout << inventory.greatBalls << " Great Balls\n";
    cout << inventory.ultraBalls << " Ultra Balls\n";
}

void searchDex(Pokemon *dex, int size) {
    string searchName;
    bool found = false;
    cin.ignore();
    getline(cin, searchName);
    searchName = toUpper(searchName);

    for (int i = 0; i < size; i++) {
        if (toUpper(dex[i].name) == searchName) {
            cout << "NAME: " << dex[i].name << '\n';
            cout << "Type: " << dex[i].type << '\n';
            cout << '"' << dex[i].dexEntry << '"' << '\n';
            found = true;
            break;
        }
    }
    if (!found) {
        cout << "No Pokedex entry for " << searchName << '\n';
    }
}

