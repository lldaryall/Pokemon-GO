#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <cctype>

using namespace std;

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

string toLower(string str);
void printMenu();
int getRandomInRange(int min, int max);
bool findPokemonByNumber(Pokemon* dex, int size, int number, int& index);
bool findPokemonByName(Pokemon* dex, int size, string name, int& index);
void printInventory(int pokeballs, int greatballs, int ultraballs);
void printStats(Pokemon* dex, int size);
void printCaught(Pokemon* dex, int size, string trainerName);

int main() {
    srand(time(0));
    Pokemon* dex = nullptr;
    ifstream fin("poke.txt");

    if (!fin) {
        cout << "POKE.TXT FILE FAILED TO OPEN, ENDING PROGRAM." << endl;
        return 1;
    }

    int size;
    fin >> size;
    fin.ignore();
    dex = new Pokemon[size];

    for (int i = 0; i < size; i++) {
        string line;
        getline(fin, line);
        size_t pos = 0;

        pos = line.find(",");
        dex[i].number = stoi(line.substr(0, pos));
        line = line.substr(pos + 1);

        pos = line.find(",");
        dex[i].name = line.substr(0, pos);
        line = line.substr(pos + 1);

        pos = line.find(",");
        dex[i].type = line.substr(0, pos);
        line = line.substr(pos + 1);

        pos = line.find(",");
        dex[i].region = line.substr(0, pos);
        line = line.substr(pos + 1);

        pos = line.find(",");
        dex[i].catchRate = stoi(line.substr(0, pos));
        line = line.substr(pos + 1);

        dex[i].dexEntry = line;

        dex[i].caught = 0;
        dex[i].seen = 0;
    }
    fin.close();

    string trainerName;
    cout << "Welcome trainer! What is your name? ";
    getline(cin, trainerName);

    int pokeballs = 10, greatballs = 10, ultraballs = 10;
    string region;

    while (true) {
        cout << "Choose a region to hunt in (Kanto, Johto, Hoenn, Sinnoh): ";
        getline(cin, region);
        string regLower = toLower(region);
        if (regLower == "kanto" || regLower == "johto" || regLower == "hoenn" || regLower == "sinnoh") {
            region = regLower;
            break;
        } else {
            cout << "Invalid region. Try again.\n";
        }
    }

    cout << "Traveling to the " << region << " region...\n";
    printMenu();

    string command;
    while (true) {
        cout << "\nSelection > ";
        cin >> command;
        string cmd = toLower(command);

        if (cmd == "menu") {
            printMenu();
        } else if (cmd == "inventory" || cmd == "bag" || cmd == "items") {
            printInventory(pokeballs, greatballs, ultraballs);
        } else if (cmd == "statistics") {
            printStats(dex, size);
        } else if (cmd == "pokemon") {
            printCaught(dex, size, trainerName);
        } else if (cmd == "dex") {
            string search;
            getline(cin, search);
            if (search.length() > 0 && search[0] == ' ') search = search.substr(1);
            int index;
            if (findPokemonByName(dex, size, search, index)) {
                cout << "\nNAME: " << dex[index].name << "\nType: " << dex[index].type << "\n" << dex[index].dexEntry << endl;
            } else {
                cout << "\nNo Pokedex entry for " << toLower(search) << endl;
            }
        } else if (cmd == "hunt") {
            int min = 1, max = 151;
            if (region == "johto") { min = 152; max = 251; }
            else if (region == "hoenn") { min = 252; max = 386; }
            else if (region == "sinnoh") { min = 387; max = 493; }

            int randNo = getRandomInRange(min, max);
            int idx;
            if (!findPokemonByNumber(dex, size, randNo, idx)) continue;

            cout << "\nA wild " << dex[idx].name << " has appeared!\n";

            dex[idx].seen++;
            if (dex[idx].catchRate == 0) {
                cout << "Oh! That's a baby Pokemon! We'll take them to the daycare.\n";
                dex[idx].caught++;
                continue;
            }

            int ballChoice;
            while (true) {
                cout << "Choose a ball to throw:\n1 - Poke Ball (" << pokeballs << ")\n2 - Great Ball (" << greatballs << ")\n3 - Ultra Ball (" << ultraballs << ")\nSelection > ";
                cin >> ballChoice;
                if (ballChoice == 1 && pokeballs > 0) {
                    pokeballs--;
                    break;
                } else if (ballChoice == 2 && greatballs > 0) {
                    greatballs--;
                    break;
                } else if (ballChoice == 3 && ultraballs > 0) {
                    ultraballs--;
                    break;
                } else {
                    cout << "Invalid selection or no balls left. Try again.\n";
                }
            }

            int bonus = (ballChoice == 1) ? 0 : (ballChoice == 2) ? 20 : 40;
            int finalRate = dex[idx].catchRate + bonus;
            int roll = getRandomInRange(1, 100);

            if (roll <= finalRate) {
                cout << "You caught " << dex[idx].name << "!\n";
                dex[idx].caught++;
            } else {
                cout << dex[idx].name << " ran away...\n";
            }
        } else if (cmd == "exit" || cmd == "quit") {
            cout << "\nFinal Results:\n";
            printCaught(dex, size, trainerName);
            printStats(dex, size);
            break;
        } else {
            cout << "Invalid option. Try again.\n";
        }

        if (pokeballs + greatballs + ultraballs == 0) {
            cout << "\nYou're out of Pokeballs!\n";
            printCaught(dex, size, trainerName);
            printStats(dex, size);
            break;
        }
    }

    delete[] dex;
    return 0;
}

string toLower(string str) {
    for (char& c : str) c = tolower(c);
    return str;
}

void printMenu() {
    cout << "\nHUNT\t\t(Go hunting for Pokemon!)\nPOKEMON\t(See the Pokemon you've caught.)\nSTATISTICS\t(See your catch statistics.)\nINVENTORY\t(See your current inventory.)\nDEX NAME\t(Example: DEX BULBASAUR)\nMENU\t\t(Reprint this menu.)\nEXIT\t\t(Quit the game.)\n";
}

int getRandomInRange(int min, int max) {
    return rand() % (max - min + 1) + min;
}

bool findPokemonByNumber(Pokemon* dex, int size, int number, int& index) {
    for (int i = 0; i < size; i++) {
        if (dex[i].number == number) {
            index = i;
            return true;
        }
    }
    return false;
}

bool findPokemonByName(Pokemon* dex, int size, string name, int& index) {
    string nameLower = toLower(name);
    for (int i = 0; i < size; i++) {
        if (toLower(dex[i].name) == nameLower) {
            index = i;
            return true;
        }
    }
    return false;
}

void printInventory(int pokeballs, int greatballs, int ultraballs) {
    cout << "You have:\n" << pokeballs << " PokeBalls\n" << greatballs << " Great Balls\n" << ultraballs << " Ultra Balls\n";
}

void printStats(Pokemon* dex, int size) {
    int caught = 0, seen = 0;
    for (int i = 0; i < size; i++) {
        caught += dex[i].caught;
        seen += dex[i].seen;
    }
    double rate = (seen == 0) ? 0 : (double)caught / seen * 100;
    cout << fixed << setprecision(2);
    cout << "Total Pokemon Caught: " << caught << "\nTotal Pokemon Seen: " << seen << "\nOverall Catch Rate: " << rate << "%\n";
}

void printCaught(Pokemon* dex, int size, string trainerName) {
    cout << trainerName << "'s POKEMON:\n";
    for (int i = 0; i < size; i++) {
        if (dex[i].caught > 0) {
            cout << "no " << dex[i].number << "\t" << dex[i].name << endl;
        }
    }
}

