**NG 8/11/2026**
* **To provide you with thorough feedback, I read your code line by line, sometimes multiple times**. Jumping from file to file is too time-consuming. Unless noted otherwise, place your class definitions above main() and all function definitions below main(), all in one file. While the industry standard involves multi-source file programs, we will use a single file format for these labs unless instructed otherwise. Submit a fully completed lab by the due date using the correct GitHub invite link to get a second chance
* **feedback.md is for instructor use only**: please DO NOT change the feedback.md; make a copy if needed; do not add anything; if you do not understand the feedback, let me know asap (mymail.laccd.edu, grigorn@laccd.edu)
* if you do not understand feedback, let me know asap
* if any, items with (-X) - no deductions this time, serve as a warning; please ensure these errors are corrected, as repeating them in future assignments will result in X points being deducted
* in feedback, #N means line number, e.g., 
```text
* #25  global var(s); only constant may be global -10
meaning: see line #25 ; -10 deduction points for the mistake
```
* **class definition style & standards**:  first, member vars  one per line and do not forget to initialize them to state; although private is a default access specifier, still label private members  clearly for better readability; second- functions: separate group of functions  by a blank line; either matching setter/getter pairs or list all setters back to back and list all getters back to back; start with a default c’tor, followed by  copy c’tor (if any), then other c’tors in the order of increased number of arguments, d’tor- right after c’tor(s), overloaded functions- list them back to back in the order of increased number of arguments;  all accessor/getter functions, except for static function,  should be const; all functions with more than one statement should be normal (defined externally, outside of the class and below main()), no inline functions with more than one statement; initialize all member variables to appropriate default values at the time of definition; all member variables must be private; classes must provide a strong exception guarantee; must have default c’tor; implement Rule of Three when necessary;  to promote strong exception guarantee and to reduce redundancy, all setter functions, including c’tors, must call a setter function that sets all member variables
* **strong exception guarantee**: this guarantee acts as a transaction. If the operation successfully completes, all changes take effect; if it fails mid-execution, all partial changes are completely undone. If incoming values are invalid, do not change member variables; provided all member variables have been initialized to a valid state in the class definition and c’tors, the object will always be valid
* **there is no such thing as a partially valid record or set of related data** (let's say, a Creature record in an input file). If any required field (e.g., health, strength) in a record is invalid, then the entire record must be considered invalid. As a developer, your responsibility is to validate the input, not to "fix" it by replacing invalid values with defaults. Doing so changes the user's data and assumes you know what they intended to enter. In general, you should not guess what the user meant or silently modify invalid input. Instead, detect the invalid record and handle it according to the program's requirements (for example, by rejecting it or reporting an error). 
* **classes must completely manage their own failures**; classes must catch and handle internal errors locally to maintain safe, fully encapsulated invariants. In other words, classes must maintain encapsulation by catching and handling internal exceptions locally. According to the C++ Core Guidelines, an object should achieve the no-fail or strong exception guarantee by rolling back state or swallowing expected failures internally so callers aren't burdened with cleanup.
* in  CS 216, **  if a lab submission does not comply with posted instructions, it will result in a loss of an attempt; further feedback will not be provided, and additional attempts will not be granted 
***
* inadequate and/ insufficient test data/ not all possible scenarios and edge cases have been tested; e.g., each record has 4 pieces of data; pow(2,4)->16  possible combinations of valid/invalid and only one of them is a valid record; all menu options should be tested;  no zeros and/or negative numbers, no numbers divisible by both 7 and 5 etc.; big test numbers != good test data; must test all created functions; all possible pathways should be tested -5
* array of pointers and creature allocations should have been handled by a single function to avoid unpredictable behaviors of the program; you can always call a function to determine what creature to create; who will catch bad_alloc? -5
* 197 and like – use setCreature() with 3args for consistency -1
* 197 and like – use setCreature() with 3args for consistency -1* 73 what for?  redundant; unnecessary functions to maintain; you can always  call setCreature() -2
* redundant convoluted menu implementation; invalid menu choice should be handled by default case of the menu switch;  need only one loop and one switch; create a highly reusable function that takes an error message string as a parameter, clears the failed input, and outputs the error message to be called in the default case of the menu switch -5
```text
do {
	cout<<menu;
	cout>>sortChoice;

switch (sortChoice) {
            case SORT_NAME:
            case SORT_HEALTH:
                sortMenu(names, types, health, strength, numCreatures, sortChoice);
                break;
            case BACK:
			//message about going back to the previous menu
                break;
            default:
                clearCin("Invalid menu choice");
        }
}while (sortChoice!=QUIT) 
```

* 208 and like **classes must completely manage their own failures** see above; if an exception is caused by the Army class, the Army class should handle it; the Army class should not rely on others to clean up its own mess and rely on others to notify a user– 10
* #304 redundant convoluted logic with/ unnecessary vars; improper/poor use of try/catch;  try/catch should be used to separate the successful path of execution from the error path of execution; in other words, “if everything goes well” code goes into the try block; -2
* 304 simple if should suffice; 
* 542 do not use swap function -2
*  565 waste of time, should have initialized  with new ; ppCreatures = new Creature *[newSize]{nullptr}; -2
* 803 redundnt convoluted implementation to get a single int above zero -2
*  548, 624 convoluted redundant implementation; hard to read and maintain -5

```text
//pseudocode do demo logic and proper use of  try/catch
Creature **ppTemp=nullptr;
    try {
        ppTemp = new Creature *[newSize]{nullptr};

        for (int i = 0; i < newSize; ++i) {

            ppTemp[i] = new Creature();//call  whatever c'tor is most suitable
        }
        name = newName;         // you can get here only if new does not fail
        size = newSize;         // you can get here only if new does not fail
        ppCreatures = ppTemp;   // you can get here only if new does not fail
    }
    catch (const bad_alloc &error) {
        clearArmy(ppTemp, newSize);     //release already dynamically allocated memory- may be an array of pointers was allocated but not creatures
        cerr << "\nThere is not enough memory to create an Army of " << newSize << " creatures" << endl;
    }


```

***
