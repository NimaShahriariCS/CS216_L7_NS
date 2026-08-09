// Lab #7
// Nima Shahriari
// Objective: learn to create child classes and manipulate an array of objects consisting of objects of various subtypes.

#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <limits>
#include <fstream>
#include <ctime>
using namespace std;

const string DEFAULT_CREATURE_NAME = "n/a";
const string DEFAULT_ARMY_NAME = "n/a";
const string INVALID_CREATURE_NAME = "";
const string INVALID_ARMY_NAME = "";
const string NAMES_FILE = "in_names.txt";
const int DEFAULT_STRENGTH = 45;
const int DEFAULT_HEALTH = 45;
const int INVALID_NUMBER = -1;
const int ARMY_MIN_STRENGTH = 45;
const int ARMY_MAX_STRENGTH = 275;
const int RESET_MIN_VALUE = 30;
const int RESET_MAX_VALUE = 150;
const int DEMON_CHANCE_RANGE = 100;
const int DEMON_CHANCE = 15;
const int DEMON_BONUS = 40;
const int ELF_MAGIC_ROLL = 20;
const int ELF_DAMAGE_MULTIPLIER = 2;
const int CYBERELF_CHANCE_RANGE = 100;
const int CYBERELF_CHANCE = 30;
const int CYBERELF_BONUS = 50;
const int MIN_ARMY_NAME_CHARS = 3;
const int NAME_WIDTH = 18;
const int TYPE_WIDTH = 14;
const int NUMBER_WIDTH = 12;
const int ATTACKER_WIDTH = 24;
const int ARMY_WIDTH = 16;
const int ROUND_WIDTH = 8;
const int PAIR_WIDTH = 7;
const int DAMAGE_WIDTH = 10;
const int HEALTH_WIDTH = 12;
const int BATTLE_SPACING = 2;

string formatLeft(const string& text, int width);
string formatRight(int value, int width);
void clearCin(const string& message);
bool validArmyName(const string& name);
int randomNumber(int low, int high);

enum MainMenu { PLAY_GAME = 1, QUIT };
enum CreatureKind { BASE_CREATURE = 0, DEMON, BALROG, ELF, CYBERELF };
enum CreatureError { INVALID_CREATURE_DATA = 1 };
enum ArmyError { INVALID_ARMY_DATA = 1, NAME_FILE_ERROR, ARMY_MEMORY_ERROR };

class Creature
{
private:
    int strength = DEFAULT_STRENGTH;
    int health = DEFAULT_HEALTH;
    string name = DEFAULT_CREATURE_NAME;

public:
    Creature();
    Creature(const string& newName, int newStrength, int newHealth);
    virtual ~Creature();

    void setCreature(const string& newName, int newStrength, int newHealth);
    void setHealth(int newHealth);
    void reset();

    int getHealth() const;
    int getStrength() const;
    string getId() const;
    virtual string getName() const;
    virtual string getType() const;
    virtual CreatureKind getKind() const;
    virtual int getDamage() const;
    string to_String() const;
};

class Demon : public Creature
{
public:
    Demon();
    Demon(const string& newName, int newStrength, int newHealth);

    string getName() const;
    string getType() const;
    CreatureKind getKind() const;
    int getDamage() const;
};

class Balrog : public Creature
{
public:
    Balrog();
    Balrog(const string& newName, int newStrength, int newHealth);

    string getName() const;
    string getType() const;
    CreatureKind getKind() const;
    int getDamage() const;
};

class Elf : public Creature
{
public:
    Elf();
    Elf(const string& newName, int newStrength, int newHealth);

    string getName() const;
    string getType() const;
    CreatureKind getKind() const;
    int getDamage() const;
};

class Cyberelf : public Elf
{
public:
    Cyberelf();
    Cyberelf(const string& newName, int newStrength, int newHealth);

    string getName() const;
    string getType() const;
    CreatureKind getKind() const;
    int getDamage() const;
};

class Army
{
private:
    Creature** ppCreatures = nullptr;
    int size = 0;
    string name = DEFAULT_ARMY_NAME;

    void setArmy(string& newName, int newSize, Creature**& newList);
    void generateArmy(const string& newName, int newSize, ifstream& input);
    void copyArmy(const Army& rhs);
    Creature* makeCreature(CreatureKind kind, const string& creatureName, int strength, int health) const;
    Creature* copyCreature(const Creature& source) const;
    void deallocateCreatures(Creature**& list, int count);

public:
    Army();
    Army(const Army& rhs);
    Army(const string& newName, int newSize, ifstream& input);
    ~Army();

    Army& operator=(const Army& rhs);

    string getName() const;
    int getSize() const;
    int getTotalHealth() const;
    Creature* getCreature(int index);
    bool isReady() const;
    void print(const string& heading) const;
};

class Game
{
private:
    string getArmyName(const string& prompt) const;
    int getArmySize() const;
    void battle(Army& armyOne, Army& armyTwo) const;
    void battlePair(Creature& creatureOne, Creature& creatureTwo, const string& armyOneName, const string& armyTwoName, int pairNumber) const;
    void attack(Creature& attacker, Creature& defender, const string& attackerArmy, const string& defenderArmy, int pairNumber, int roundNumber) const;
    void printBattleHeader() const;
    void announceWinner(const Army& armyOne, const Army& armyTwo) const;

public:
    Game();

    void play() const;
};

int main()
{
    int menuChoice = 0;
    Game game;

    srand(static_cast<unsigned int>(time(0)));

    do
    {
        cout << "\n\nMenu:\n"
                "1. Play game\n"
                "2. Quit\n"
                "Select from the menu above: ";
        cin >> menuChoice;

        if (cin.fail())
        {
            clearCin("Invalid menu choice.");
            menuChoice = 0;
        }
        else
        {
            switch (menuChoice)
            {
            case PLAY_GAME:
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                try
                {
                    game.play();
                }
                catch (const bad_alloc&)
                {
                    cout << "Memory allocation failed. The game was not started." << endl;
                }
                break;

            case QUIT:
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Goodbye." << endl;
                break;

            default:
                clearCin("Invalid menu choice.");
            }
        }
    } while (menuChoice != QUIT);

    return 0;
}

string formatLeft(const string& text, int width)
{
    string result = text;

    if (static_cast<int>(result.length()) < width)
    {
        result += string(width - static_cast<int>(result.length()), ' ');
    }

    return result;
}

string formatRight(int value, int width)
{
    string result = to_string(value);

    if (static_cast<int>(result.length()) < width)
    {
        result = string(width - static_cast<int>(result.length()), ' ') + result;
    }

    return result;
}

void clearCin(const string& message)
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << message << endl;
}

bool validArmyName(const string& name)
{
    bool valid = true;
    int alphaCount = 0;

    for (int index = 0; index < static_cast<int>(name.length()) && valid; ++index)
    {
        if ((name[index] >= 'A' && name[index] <= 'Z') || (name[index] >= 'a' && name[index] <= 'z'))
        {
            ++alphaCount;
        }
        else if (name[index] != ' ')
        {
            valid = false;
        }
    }

    valid = valid && alphaCount >= MIN_ARMY_NAME_CHARS;
    return valid;
}

int randomNumber(int low, int high)
{
    return (rand() % (high - low + 1)) + low;
}

Creature::Creature()
{
    setCreature(DEFAULT_CREATURE_NAME, DEFAULT_STRENGTH, DEFAULT_HEALTH);
}

Creature::Creature(const string& newName, int newStrength, int newHealth)
{
    setCreature(newName, newStrength, newHealth);
}

Creature::~Creature()
{
    strength = INVALID_NUMBER;
    health = INVALID_NUMBER;
    name = INVALID_CREATURE_NAME;
}

void Creature::setCreature(const string& newName, int newStrength, int newHealth)
{
    try
    {
        if (newName.empty() || newStrength <= 0 || newHealth < 0)
        {
            throw INVALID_CREATURE_DATA;
        }

        name = newName;
        strength = newStrength;
        health = newHealth;
    }
    catch (CreatureError)
    {
        cout << "Invalid creature data. The creature was not changed." << endl;
    }
}

void Creature::setHealth(int newHealth)
{
    setCreature(name, strength, newHealth);
}

void Creature::reset()
{
    setCreature(name, randomNumber(RESET_MIN_VALUE, RESET_MAX_VALUE), randomNumber(RESET_MIN_VALUE, RESET_MAX_VALUE));
}

int Creature::getHealth() const
{
    return health;
}

int Creature::getStrength() const
{
    return strength;
}

string Creature::getId() const
{
    return name;
}

string Creature::getName() const
{
    return name + " the Creature";
}

string Creature::getType() const
{
    return "Creature";
}

CreatureKind Creature::getKind() const
{
    return BASE_CREATURE;
}

int Creature::getDamage() const
{
    return (rand() % strength) + 1;
}

string Creature::to_String() const
{
    string result = formatLeft(name, NAME_WIDTH) + formatLeft(getType(), TYPE_WIDTH) + formatRight(strength, NUMBER_WIDTH) + formatRight(health, NUMBER_WIDTH);
    return result;
}

Demon::Demon() : Creature()
{
}

Demon::Demon(const string& newName, int newStrength, int newHealth) : Creature(newName, newStrength, newHealth)
{
}

string Demon::getName() const
{
    return getId() + " the Demon";
}

string Demon::getType() const
{
    return "Demon";
}

CreatureKind Demon::getKind() const
{
    return DEMON;
}

int Demon::getDamage() const
{
    int damage = Creature::getDamage();

    if ((rand() % DEMON_CHANCE_RANGE) < DEMON_CHANCE)
    {
        damage += DEMON_BONUS;
    }

    return damage;
}

Balrog::Balrog() : Creature()
{
}

Balrog::Balrog(const string& newName, int newStrength, int newHealth) : Creature(newName, newStrength, newHealth)
{
}

string Balrog::getName() const
{
    return getId() + " the Balrog";
}

string Balrog::getType() const
{
    return "Balrog";
}

CreatureKind Balrog::getKind() const
{
    return BALROG;
}

int Balrog::getDamage() const
{
    return Creature::getDamage() + Creature::getDamage();
}

Elf::Elf() : Creature()
{
}

Elf::Elf(const string& newName, int newStrength, int newHealth) : Creature(newName, newStrength, newHealth)
{
}

string Elf::getName() const
{
    return getId() + " the Elf";
}

string Elf::getType() const
{
    return "Elf";
}

CreatureKind Elf::getKind() const
{
    return ELF;
}

int Elf::getDamage() const
{
    int damage = Creature::getDamage();

    if ((rand() % ELF_MAGIC_ROLL) == 0)
    {
        damage *= ELF_DAMAGE_MULTIPLIER;
    }

    return damage;
}

Cyberelf::Cyberelf() : Elf()
{
}

Cyberelf::Cyberelf(const string& newName, int newStrength, int newHealth) : Elf(newName, newStrength, newHealth)
{
}

string Cyberelf::getName() const
{
    return getId() + " the Cyberelf";
}

string Cyberelf::getType() const
{
    return "Cyberelf";
}

CreatureKind Cyberelf::getKind() const
{
    return CYBERELF;
}

int Cyberelf::getDamage() const
{
    int damage = Elf::getDamage();

    if ((rand() % CYBERELF_CHANCE_RANGE) < CYBERELF_CHANCE)
    {
        damage += CYBERELF_BONUS;
    }

    return damage;
}

Army::Army()
{
    string tempName = DEFAULT_ARMY_NAME;
    Creature** tempList = nullptr;
    setArmy(tempName, 0, tempList);
}

Army::Army(const Army& rhs)
{
    copyArmy(rhs);
}

Army::Army(const string& newName, int newSize, ifstream& input)
{
    generateArmy(newName, newSize, input);
}

Army::~Army()
{
    deallocateCreatures(ppCreatures, size);
    size = 0;
    name = INVALID_ARMY_NAME;
}

Army& Army::operator=(const Army& rhs)
{
    if (this != &rhs)
    {
        copyArmy(rhs);
    }

    return *this;
}

void Army::setArmy(string& newName, int newSize, Creature**& newList)
{
    deallocateCreatures(ppCreatures, size);
    name.swap(newName);
    ppCreatures = newList;
    newList = nullptr;
    size = newSize;
}

void Army::generateArmy(const string& newName, int newSize, ifstream& input)
{
    Creature** tempList = nullptr;
    bool complete = true;
    ArmyError failure = NAME_FILE_ERROR;
    string creatureName = DEFAULT_CREATURE_NAME;
    string tempName = newName;

    try
    {
        if (!validArmyName(newName) || newSize <= 0)
        {
            throw INVALID_ARMY_DATA;
        }

        tempList = new Creature*[newSize];

        for (int index = 0; index < newSize; ++index)
        {
            tempList[index] = nullptr;
        }

        try
        {
            for (int index = 0; index < newSize && complete; ++index)
            {
                if (getline(input, creatureName) && !creatureName.empty())
                {
                    CreatureKind kind = static_cast<CreatureKind>(randomNumber(DEMON, CYBERELF));
                    int strength = randomNumber(ARMY_MIN_STRENGTH, ARMY_MAX_STRENGTH);
                    int health = randomNumber(ARMY_MIN_STRENGTH, ARMY_MAX_STRENGTH);
                    tempList[index] = makeCreature(kind, creatureName, strength, health);
                    complete = tempList[index] != nullptr;
                }
                else
                {
                    complete = false;
                }
            }
        }
        catch (const bad_alloc&)
        {
            complete = false;
            failure = ARMY_MEMORY_ERROR;
        }

        if (!complete)
        {
            deallocateCreatures(tempList, newSize);
            throw failure;
        }

        setArmy(tempName, newSize, tempList);
    }
    catch (ArmyError error)
    {
        if (tempList != nullptr)
        {
            deallocateCreatures(tempList, newSize);
        }

        if (error == INVALID_ARMY_DATA)
        {
            cout << "Invalid army data. The army was not changed." << endl;
        }
        else if (error == NAME_FILE_ERROR)
        {
            cout << "The names file did not contain enough valid names. The army was not changed." << endl;
        }
        else
        {
            cout << "A creature could not be allocated. The army was not changed." << endl;
        }
    }
}

void Army::copyArmy(const Army& rhs)
{
    Creature** tempList = nullptr;
    bool complete = true;
    string tempName = rhs.name;

    if (rhs.size == 0)
    {
        setArmy(tempName, 0, tempList);
    }
    else
    {
        tempList = new Creature*[rhs.size];

        for (int index = 0; index < rhs.size; ++index)
        {
            tempList[index] = nullptr;
        }

        try
        {
            for (int index = 0; index < rhs.size && complete; ++index)
            {
                tempList[index] = copyCreature(*rhs.ppCreatures[index]);
                complete = tempList[index] != nullptr;
            }
        }
        catch (const bad_alloc&)
        {
            complete = false;
            cout << "The army copy could not be completed. The destination army was not changed." << endl;
        }

        if (complete)
        {
            setArmy(tempName, rhs.size, tempList);
        }
        else
        {
            deallocateCreatures(tempList, rhs.size);
        }
    }
}

Creature* Army::makeCreature(CreatureKind kind, const string& creatureName, int strength, int health) const
{
    Creature* creature = nullptr;

    switch (kind)
    {
    case BASE_CREATURE:
        creature = new Creature(creatureName, strength, health);
        break;

    case DEMON:
        creature = new Demon(creatureName, strength, health);
        break;

    case BALROG:
        creature = new Balrog(creatureName, strength, health);
        break;

    case ELF:
        creature = new Elf(creatureName, strength, health);
        break;

    case CYBERELF:
        creature = new Cyberelf(creatureName, strength, health);
        break;
    }

    return creature;
}

Creature* Army::copyCreature(const Creature& source) const
{
    return makeCreature(source.getKind(), source.getId(), source.getStrength(), source.getHealth());
}

void Army::deallocateCreatures(Creature**& list, int count)
{
    if (list != nullptr)
    {
        for (int index = 0; index < count; ++index)
        {
            if (list[index] != nullptr)
            {
                delete list[index];
                list[index] = nullptr;
            }
        }

        delete [] list;
        list = nullptr;
    }
}

string Army::getName() const
{
    return name;
}

int Army::getSize() const
{
    return size;
}

int Army::getTotalHealth() const
{
    int total = 0;

    for (int index = 0; index < size; ++index)
    {
        total += ppCreatures[index]->getHealth();
    }

    return total;
}

Creature* Army::getCreature(int index)
{
    Creature* creature = nullptr;

    if (index >= 0 && index < size)
    {
        creature = ppCreatures[index];
    }

    return creature;
}

bool Army::isReady() const
{
    return ppCreatures != nullptr && size > 0;
}

void Army::print(const string& heading) const
{
    cout << "\n" << heading << "\n";
    cout << formatLeft("Name", NAME_WIDTH) + formatLeft("Type", TYPE_WIDTH) + formatLeft("Strength", NUMBER_WIDTH) + formatLeft("Health", NUMBER_WIDTH) << endl;
    cout << setfill('-') << setw(NAME_WIDTH + TYPE_WIDTH + NUMBER_WIDTH + NUMBER_WIDTH) << "" << setfill(' ') << endl;

    for (int index = 0; index < size; ++index)
    {
        cout << ppCreatures[index]->to_String() << endl;
    }

    cout << "Overall health: " << getTotalHealth() << endl;
}

Game::Game()
{
}

string Game::getArmyName(const string& prompt) const
{
    string armyName = DEFAULT_ARMY_NAME;
    bool valid = false;

    do
    {
        cout << prompt;
        getline(cin, armyName);
        valid = validArmyName(armyName);

        if (!valid)
        {
            cout << "Army names must contain at least " << MIN_ARMY_NAME_CHARS << " alphabetic characters and may contain spaces." << endl;
        }
    } while (!valid);

    return armyName;
}

int Game::getArmySize() const
{
    int armySize = 0;
    bool valid = false;

    do
    {
        cout << "Enter the number of creatures for both armies: ";
        cin >> armySize;

        if (cin.fail())
        {
            clearCin("Enter a positive whole number.");
            armySize = 0;
        }
        else if (armySize <= 0)
        {
            clearCin("Enter a positive whole number.");
        }
        else
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            valid = true;
        }
    } while (!valid);

    return armySize;
}

void Game::play() const
{
    string armyOneName = getArmyName("Enter Army #1 name: ");
    string armyTwoName = getArmyName("Enter Army #2 name: ");
    int armySize = getArmySize();
    ifstream input(NAMES_FILE.c_str());

    if (input)
    {
        Army armyOne(armyOneName, armySize, input);
        Army armyTwo(armyTwoName, armySize, input);

        if (armyOne.isReady() && armyTwo.isReady())
        {
            armyOne.print("Army #1 Stats before the Battle");
            armyTwo.print("Army #2 Stats before the Battle");
            battle(armyOne, armyTwo);
            armyOne.print("Army #1 Stats after the Battle");
            armyTwo.print("Army #2 Stats after the Battle");
            announceWinner(armyOne, armyTwo);
        }
        else
        {
            cout << "Both armies could not be created. The battle was canceled." << endl;
        }

        input.close();
    }
    else
    {
        cout << "Unable to open " << NAMES_FILE << ". The battle was not started." << endl;
    }
}

void Game::battle(Army& armyOne, Army& armyTwo) const
{
    int armySize = armyOne.getSize();

    printBattleHeader();

    for (int index = 0; index < armySize; ++index)
    {
        battlePair(*armyOne.getCreature(index), *armyTwo.getCreature(index), armyOne.getName(), armyTwo.getName(), index + 1);
    }
}

void Game::battlePair(Creature& creatureOne, Creature& creatureTwo, const string& armyOneName, const string& armyTwoName, int pairNumber) const
{
    bool armyOneTurn = randomNumber(0, 1) == 0;
    int roundNumber = 1;

    while (creatureOne.getHealth() > 0 && creatureTwo.getHealth() > 0)
    {
        if (armyOneTurn)
        {
            attack(creatureOne, creatureTwo, armyOneName, armyTwoName, pairNumber, roundNumber);
        }
        else
        {
            attack(creatureTwo, creatureOne, armyTwoName, armyOneName, pairNumber, roundNumber);
        }

        armyOneTurn = !armyOneTurn;
        ++roundNumber;
    }
}

void Game::attack(Creature& attacker, Creature& defender, const string& attackerArmy, const string& defenderArmy, int pairNumber, int roundNumber) const
{
    int beforeHealth = defender.getHealth();
    int damage = attacker.getDamage();
    int afterHealth = beforeHealth - damage;

    if (afterHealth < 0)
    {
        afterHealth = 0;
    }

    defender.setHealth(afterHealth);
    cout << left << setw(PAIR_WIDTH) << pairNumber << setw(ROUND_WIDTH) << roundNumber << setw(ATTACKER_WIDTH) << attacker.getName() << setw(ARMY_WIDTH) << attackerArmy << right << setw(DAMAGE_WIDTH) << damage << "  " << left << setw(ATTACKER_WIDTH) << defender.getName() << setw(ARMY_WIDTH) << defenderArmy << right << setw(HEALTH_WIDTH) << beforeHealth << setw(HEALTH_WIDTH) << afterHealth << endl;
}

void Game::printBattleHeader() const
{
    cout << "\nBattle Details\n";
    cout << left << setw(PAIR_WIDTH) << "Pair" << setw(ROUND_WIDTH) << "Round" << setw(ATTACKER_WIDTH) << "Attacker" << setw(ARMY_WIDTH) << "Army" << right << setw(DAMAGE_WIDTH) << "Damage" << "  " << left << setw(ATTACKER_WIDTH) << "Defender" << setw(ARMY_WIDTH) << "Army" << right << setw(HEALTH_WIDTH) << "Before" << setw(HEALTH_WIDTH) << "After" << endl;
    cout << setfill('-') << setw(PAIR_WIDTH + ROUND_WIDTH + ATTACKER_WIDTH + ARMY_WIDTH + DAMAGE_WIDTH + ATTACKER_WIDTH + ARMY_WIDTH + HEALTH_WIDTH + HEALTH_WIDTH + BATTLE_SPACING) << "" << setfill(' ') << endl;
}

void Game::announceWinner(const Army& armyOne, const Army& armyTwo) const
{
    int armyOneHealth = armyOne.getTotalHealth();
    int armyTwoHealth = armyTwo.getTotalHealth();

    cout << "\nFinal overall health:\n" << armyOne.getName() << ": " << armyOneHealth << "\n" << armyTwo.getName() << ": " << armyTwoHealth << endl;

    if (armyOneHealth > armyTwoHealth)
    {
        cout << armyOne.getName() << " wins the battle." << endl;
    }
    else if (armyTwoHealth > armyOneHealth)
    {
        cout << armyTwo.getName() << " wins the battle." << endl;
    }
    else
    {
        cout << "The battle ends in a tie." << endl;
    }
}

/*
TEST RUNS

Test 1 - nonnumeric and out-of-range menu input
Input: x
Output: Invalid menu choice.
Input: 9
Output: Invalid menu choice.

Test 2 - invalid Army name and invalid Army size
Input Army #1 name: A1
Output: Army names must contain at least 3 alphabetic characters and may contain spaces.
Input Army #1 name: Red Army
Input Army #2 name: Blue Army
Input size: abc
Output: Enter a positive whole number.
Input size: -5
Output: Enter a positive whole number.
Input size: 3

Actual battle output from the same run:
Army #1 Stats before the Battle
Name              Type          Strength    Health
--------------------------------------------------------
Aldric            Elf                    271         153
Brenna            Demon                   88          99
Cedric            Elf                    146         273
Overall health: 525

Army #2 Stats before the Battle
Name              Type          Strength    Health
--------------------------------------------------------
Daria             Balrog                 152         101
Eamon             Demon                  259         156
Freya             Elf                    176         188
Overall health: 445

Battle Details
Pair   Round   Attacker                Army                Damage  Defender                Army                  Before       After
-----------------------------------------------------------------------------------------------------------------------------------
1      1       Aldric the Elf          Red Army               238  Daria the Balrog        Blue Army                101           0
2      1       Eamon the Demon         Blue Army              107  Brenna the Demon        Red Army                  99           0
3      1       Cedric the Elf          Red Army                17  Freya the Elf           Blue Army                188         171
3      2       Freya the Elf           Blue Army              108  Cedric the Elf          Red Army                 273         165
3      3       Cedric the Elf          Red Army               109  Freya the Elf           Blue Army                171          62
3      4       Freya the Elf           Blue Army              168  Cedric the Elf          Red Army                 165           0

Army #1 Stats after the Battle
Name              Type          Strength    Health
--------------------------------------------------------
Aldric            Elf                    271         153
Brenna            Demon                   88           0
Cedric            Elf                    146           0
Overall health: 153

Army #2 Stats after the Battle
Name              Type          Strength    Health
--------------------------------------------------------
Daria             Balrog                 152           0
Eamon             Demon                  259         156
Freya             Elf                    176          62
Overall health: 218

Final overall health:
Red Army: 153
Blue Army: 218
Blue Army wins the battle.

Test 3 - missing input file
Output: Unable to open in_names.txt. The battle was not started.

Test 4 - insufficient names in input file
Output: The names file did not contain enough valid names. The army was not changed.
Output: Both armies could not be created. The battle was canceled.

Test 5 - Rule of Three and strong exception guarantee test harness
Original first health: 117
Original first after copy changed: 117
Copied first after change: 0
Original second after assigned changed: 181
Assigned second after change: 0
Invalid creature data. The creature was not changed.
Creature after invalid set: Tester 80 90
Invalid creature data. The creature was not changed.
Creature health after invalid health set: 90
Reset strength in range: 1
Reset health in range: 1
D the Demon damage: 50
B the Balrog damage: 91
E the Elf damage: 91
C the Cyberelf damage: 64

Test 6 - quit
Input: 2
Output: Goodbye.

AddressSanitizer and UndefinedBehaviorSanitizer test:
No memory leaks, invalid deletes, or undefined-behavior errors reported.
*/
