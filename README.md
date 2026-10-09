# Earthquake Relief Supply Distribution System - Code Guide

Read this file first. It explains what every file does, what every function
does, where each one is used inside `main()`, and how to change the system
later. After this, the code itself (which has a comment on every line) will
be easy to follow.

**Suggested reading order:** this README -> `config.h` -> `output.h` ->
`main.cpp` -> `output.cpp`.

---

## 1. What the program does

After an earthquake, supplies are limited (trucks, supply packs, medical
staff). Several affected areas ask for help. The program:

1. Asks 8 questions about one affected area.
2. Turns the answers into a **need score** (0 to 100) by adding points.
3. Uses the score to pick a **relief level** (Full, Standard, Mid-Level,
   Low-Level, or Defer). Each level has a fixed number of trucks, packs and
   medical staff.
4. Checks the **remaining stock**. If there is not enough for that level, it
   steps down to the biggest smaller level that the stock can cover.
5. **Subtracts** what was given from the stock, so the next area gets what
   is left.
6. Repeats for as many areas as the user enters, then prints a summary.

---

## 2. Files at a glance

| File | What it is | What is inside | Who uses it |
|---|---|---|---|
| `config.h` | The settings file | Every number: starting stock, points, score limits, relief amounts | Included by `main.cpp` and `output.cpp` |
| `main.cpp` | The brain | Input helpers, score calculation, decision rules, the request loop | This is where the program starts |
| `output.h` | The "menu" of printing functions | Only the *declarations* (names + inputs) of the functions in `output.cpp` | Included by `main.cpp` and `output.cpp` |
| `output.cpp` | The screen | All the `cout` printing: intro, request details, decision, summary | Its functions are called from `main()` |
| `test_set1_input.txt` | Ready-made answers for test set 1 | The 5 test requests, default stock | Fed to the program instead of typing |
| `test_set2_input.txt` | Ready-made answers for test set 2 | Same 5 requests, small stock (3 trucks, 40 packs, 4 staff) | Fed to the program instead of typing |
| `test_set1_output.txt` / `test_set2_output.txt` | Saved program output | What the program printed for each test set | For the report |

**Rule of thumb:** *numbers* live in `config.h`, *thinking* lives in
`main.cpp`, *printing* lives in `output.cpp`. Because of this split, you can
change the points without touching any logic, and change the look of the
output without touching any calculation.

---

## 3. How to build and run

From a terminal inside the project folder (all `.cpp` files must be compiled
together):

```
g++ -std=c++17 main.cpp output.cpp -o relief.exe
.\relief.exe
```

Note: the VS Code "Run" button compiles only the file you have open, which
fails here because the project has two `.cpp` files. Use the terminal command.

To run a test set without typing anything:

```
PowerShell:   Get-Content test_set1_input.txt | .\relief.exe
cmd.exe:      relief.exe < test_set1_input.txt
```

Use `test_set2_input.txt` for the insufficient-resources test.

---

## 4. The big picture: one run of the program

```
START
  |
  v
print welcome + instructions            (printOutput, output.cpp)
  |
  v
set starting stock  (default from config.h, or typed by the user)
  |
  v
+--------------------- LOOP: one pass per request ---------------------+
|  read the 8 answers                       (readInt / readYesNo)      |
|        |                                                             |
|        v                                                             |
|  need score = sum of points, kept in 0..100   (calculateNeedScore)   |
|        |                                                             |
|        v                                                             |
|  wantedLevel = level the score earns    (getReliefLevelFromScore)    |
|        |                                                             |
|        v                                                             |
|  givenLevel = biggest level the stock can cover, starting at         |
|               wantedLevel and stepping down   (chooseAffordableLevel)|
|        |                                                             |
|        v                                                             |
|  work out trucks / packs / staff used  (trucksFor, packsFor,         |
|  staffFor), build the reason if we stepped down                      |
|        |                                                             |
|        v                                                             |
|  SUBTRACT used amounts from the stock                                |
|  update summary counters                                             |
|        |                                                             |
|        v                                                             |
|  print request details + decision    (printRequestInfo,              |
|                                       printDecision)                 |
|        |                                                             |
|        v                                                             |
|  "Enter another request?"  -- yes --> back to the top of the loop    |
+----------------------------- no -------------------------------------+
  |
  v
print summary                             (printSummary, output.cpp)
  |
  v
END
```

---

## 5. `config.h` - the settings file

**Purpose:** one place for every number. To change the points system, the
limits, the relief amounts or the starting stock, you edit a number here and
recompile. Nothing else needs to change.

**Key ideas used in this file**

- `const int X = 10;` makes a **constant**: a named number that cannot change
  while the program runs. Capital letters are the habit for constants.
- `#pragma once` stops the file from being included twice.
- A header file is allowed to hold `const` values; every `.cpp` that includes
  it gets its own copy.

**Its six sections and where each is used**

| Section | Constants | Used in |
|---|---|---|
| 1. Starting stock | `START_TRUCKS`, `START_PACKS`, `START_STAFF` | `main()` (default stock) |
| 2. Menu codes | `AREA_PLAIN/MOUNTAINOUS/COASTAL` (1,2,3), `DAMAGE_LOW/MEDIUM/HIGH` (1,2,3) | `getAreaPoints`, `getDamagePoints` (main.cpp); `areaText`, `damageText` (output.cpp) |
| 3. Points | `POINTS_...`, family band limits, `MIN_SCORE`, `MAX_SCORE`, `MAX_FAMILIES_ALLOWED` | `calculateNeedScore` and its helpers; also printed in the intro by `printOutput` |
| 4. Relief level codes | `FULL_RELIEF`=0, `STANDARD_RELIEF`=1, `MID_RELIEF`=2, `LOW_RELIEF`=3, `DEFER`=4 | decision functions in main.cpp; `levelName` in output.cpp |
| 5. Score limits | `THRESHOLD_FULL` 90, `THRESHOLD_STANDARD` 70, `THRESHOLD_MID` 50, `THRESHOLD_LOW` 20 | `getReliefLevelFromScore`; intro table |
| 6. Relief amounts | `FULL_TRUCKS`, `FULL_PACKS`, `FULL_STAFF`, ... for each level | `trucksFor`, `packsFor`, `staffFor`; intro table |

**Why the relief level codes are 0 to 4 in that order:** the fallback logic
steps down by doing `level++`. Because 0 is the biggest relief and 4 (Defer)
is the smallest, adding 1 always means "one level smaller".

### The points system

| Question | Answer | Points |
|---|---|---|
| Area type | Plain | +10 |
| | Mountainous | +20 |
| | Coastal | +15 |
| Affected families | 0 - 20 | +5 |
| | 21 - 50 | +15 |
| | 51 - 100 | +10 |
| | over 100 | +5 |
| Medical aid needed | Yes | +10 |
| | No | +5 |
| Road to the area | Open | +10 |
| | Blocked | -15 |
| Aftershocks expected | Yes | +5 |
| | No | +15 |
| Damage level | Low | +5 |
| | Medium | +10 |
| | High | +20 |
| Aid already received | Yes | -20 |
| | No | +10 |

The raw total can range from -5 to 100. The program clamps it to **0 to 100**.

### The relief levels

| Need score | Level | Trucks | Supply packs | Medical staff |
|---|---|---|---|---|
| 90 - 100 | Full Relief | 3 | 50 | 5 |
| 70 - 89 | Standard Relief | 2 | 30 | 3 |
| 50 - 69 | Mid-Level Relief | 1 | 20 | 2 |
| 20 - 49 | Low-Level Relief | 1 | 10 | 1 |
| 0 - 19 | Defer | 0 | 0 | 1 |

---

## 6. `output.h` and `output.cpp` - everything that prints

### `output.h` (the declarations)

A header file lists which functions exist, so `main.cpp` can call them
without seeing their code. It declares four functions:

| Function | Called from `main()` | What it prints |
|---|---|---|
| `printOutput()` | once, at the start | Welcome screen, instructions, points table, relief table, notes |
| `printRequestInfo(...)` | once per request (step 3g) | The answers the user typed for that request |
| `printDecision(...)` | once per request (step 3g) | Score, level earned, reason for any step-down, action taken, units used, remaining stock |
| `printSummary(...)` | once, at the end | Counts of served / reduced / deferred, and the stock left |

The header uses `std::string` instead of `using namespace std;` because
headers should not force `using namespace` on every file that includes them.

### `output.cpp` (the real code)

**Small helper functions** (marked `static`, which means "only usable inside
this file"). They are not called from `main()`; the four functions above call
them.

| Helper | What it does | Used by |
|---|---|---|
| `pointsText(int)` | `10` -> `"+10"`, `-20` -> `"-20"` | `printOutput` |
| `yesNoText(bool)` | `true` -> `"Yes"`, `false` -> `"No"` | `printRequestInfo` |
| `areaText(int)` | `1` -> `"Plain"`, `2` -> `"Mountainous"`, `3` -> `"Coastal"` | `printRequestInfo` |
| `damageText(int)` | `1` -> `"Low"`, `2` -> `"Medium"`, `3` -> `"High"` | `printRequestInfo` |
| `levelName(int)` | `0..4` -> `"Full Relief"` ... `"Defer"` | `printOutput`, `printDecision` |
| `printTierLine(...)` | Prints one row of the relief table with aligned columns (`setw`, `left`, `right`) | `printOutput` (5 times) |

**Important design point:** `printOutput` does not contain typed-in numbers.
It prints `POINTS_PLAIN`, `THRESHOLD_FULL`, `FULL_TRUCKS` and so on straight
from `config.h`. So if you change a point value, the instruction screen
changes with it and can never show wrong information.

**Formatting used:** `\n` (new line) and `\t` (tab) escape sequences, `setw`
for column width, `left`/`right` alignment, and the short `? :` form in
`printRequestInfo` to print "Open" or "Blocked".

---

## 7. `main.cpp` - the brain

`main.cpp` has four parts. Parts 1 to 3 are functions; Part 4 is `main()`,
which uses them.

### Part 1 - Safe input helpers

| Function | What it does | Used in `main()` for |
|---|---|---|
| `stopIfInputEnded()` | If the input has ended (Ctrl+Z / Ctrl+D, or an input file ran out), prints a message and exits. Prevents endless "ask again" loops. | Called inside `readInt` and `readYesNo` |
| `readInt(prompt, min, max)` | Shows a question and keeps asking until the user types a whole number from `min` to `max`. | Area type, families, damage level, custom stock |
| `readYesNo(prompt)` | Shows a question with `(y/n)`. Returns `true` for y/Y, `false` for n/N, otherwise asks again. | Default stock, medical aid, road open, aftershocks, aid received, another request |

How `readInt` works: it loops forever with `while (true)`, reads a number
with `cin >> value`, and only leaves the loop with `return value` when the
number is valid. If the user typed letters, `cin.fail()` is true, so it uses
`cin.clear()` (reset the error) and `cin.ignore(1000, '\n')` (throw away the
bad line) and asks again. If the number is outside the range it also asks
again.

### Part 2 - Need score

| Function | What it does |
|---|---|
| `getAreaPoints(areaType)` | `switch` on the area code, returns the points for Plain / Mountainous / Coastal |
| `getFamilyPoints(families)` | `if / else if` chain over the four family bands |
| `getDamagePoints(damage)` | `switch` on the damage code, returns the points |
| `calculateNeedScore(...)` | Starts at 0 and adds the points of all 7 answers using `+=`, then keeps the result inside `MIN_SCORE`..`MAX_SCORE` |

`calculateNeedScore` is called once per request in step 3b of `main()`.
Booleans (medical aid, road open, aftershocks, aid received) are handled
directly inside it with `if / else`.

### Part 3 - Decision rules

| Function | What it does |
|---|---|
| `getReliefLevelFromScore(score)` | Checks the score against the limits from the top (90, 70, 50, 20) and returns the first level that matches. This is the level the score *earns*. |
| `trucksFor(level)`, `packsFor(level)`, `staffFor(level)` | Each uses a `switch` to return how many of that item a level needs |
| `canAfford(level, trucksLeft, packsLeft, staffLeft)` | Returns `true` only if trucks **and** packs **and** staff are all enough (`&&`) |
| `chooseAffordableLevel(wantedLevel, ...)` | Starts at the earned level and does `level++` while the level is not Defer **and** cannot be afforded. Returns the level that fits. Defer is always allowed because it sends no trucks or packs. |
| `buildShortageReason(wantedLevel, ...)` | Builds a sentence such as `trucks (need 2, have 1)  supply packs (need 30, have 10)` naming exactly what was short |

### Part 4 - `main()` step by step

The comments in the code use the same labels (3a, 3b, ...).

**Step 1 - Stock.** Three variables, `trucksLeft`, `packsLeft`, `staffLeft`,
act as the warehouse. They start at the `config.h` values. If the user
answers "n" to "Use the default stock?", they are replaced by typed values
(this is how test set 2 uses a small stock without recompiling).

**Step 2 - Counters.** `requestCount`, `servedFully`, `servedReduced`,
`deferred`: used for the final summary.

**Step 3 - The request loop** (`while (anotherRequest)`):

| Step | What happens |
|---|---|
| 3a | Read the area name with `getline` (so spaces are allowed), then the 7 answers with `readInt` / `readYesNo` |
| 3b | `score` = `calculateNeedScore(...)`; `wantedLevel` = `getReliefLevelFromScore(score)`; `givenLevel` = `chooseAffordableLevel(...)` |
| 3c | `trucksUsed`, `packsUsed`, `staffUsed` come from `trucksFor/packsFor/staffFor(givenLevel)`. A safety check makes sure staff used never exceeds staff left (only matters for Defer, which asks for 1 staff) |
| 3d | If `givenLevel` differs from `wantedLevel`, build the shortage reason. This must be done **before** subtracting, otherwise it would describe the wrong stock |
| 3e | Subtract: `trucksLeft -= trucksUsed;` and the same for packs and staff. The next loop pass sees the smaller stock |
| 3f | Update counters: Defer -> `deferred++`; same level as earned -> `servedFully++`; otherwise -> `servedReduced++` |
| 3g | `printRequestInfo(...)` then `printDecision(...)` |
| 3h | `anotherRequest = readYesNo(...)`; if false, the loop ends |

**Step 4 - Summary.** `printSummary(...)`, then `return 0;`.

### Who calls whom

```
main
 |-- printOutput                      (output.cpp)
 |-- readYesNo, readInt               -> stopIfInputEnded
 |-- calculateNeedScore               -> getAreaPoints, getFamilyPoints, getDamagePoints
 |-- getReliefLevelFromScore
 |-- chooseAffordableLevel            -> canAfford -> trucksFor, packsFor, staffFor
 |-- trucksFor, packsFor, staffFor
 |-- buildShortageReason              -> trucksFor, packsFor, staffFor
 |-- printRequestInfo                 (output.cpp) -> areaText, yesNoText, damageText
 |-- printDecision                    (output.cpp) -> levelName
 `-- printSummary                     (output.cpp)
```

---

## 8. How stock is deducted - worked example (test set 2)

Starting stock: **3 trucks, 40 packs, 4 staff**.

| Request | Score | Level earned | What happens | Given | Stock left (T / P / S) |
|---|---|---|---|---|---|
| R1 Riverside | 45 | Low (1/10/1) | Fits | Low | 2 / 30 / 3 |
| R2 Hilltop | 50 | Mid (1/20/2) | Fits | Mid | 1 / 10 / 1 |
| R3 Lakeside | 80 | Standard (2/30/3) | Standard needs 2 trucks, only 1 left -> no. Mid needs 20 packs, only 10 left -> no. Low (1/10/1) fits | Low | 0 / 0 / 0 |
| R4 Market Town | 60 | Mid | Nothing left. Steps down to Defer | Defer | 0 / 0 / 0 |
| R5 Bridge Colony | 30 | Low | Nothing left. Steps down to Defer | Defer | 0 / 0 / 0 |

Summary: 2 served at the level they earned, 1 served with reduced relief,
2 deferred.

How `chooseAffordableLevel` handled R3: `level` starts at 1 (Standard).
`canAfford` is false -> `level++` (2, Mid). `canAfford` is false ->
`level++` (3, Low). `canAfford` is true -> the loop stops and 3 is returned.

---

## 9. Hand-traced test requests (default stock, test set 1)

Area types are an assumption: the assignment's test table has none, so we
chose them. Flood warning is treated as "aftershocks expected", damaged route
as "road blocked", and medical need as "medical aid needed".

| Request | Area | Families | Medical | Road | Aftershocks | Damage | Aid before | Points added up | Score | Level |
|---|---|---|---|---|---|---|---|---|---|---|
| R1 Riverside | Plain | 150 | Yes | Blocked | Yes | High | No | 10+5+10-15+5+20+10 | 45 | Low |
| R2 Hilltop | Mountainous | 60 | No | Open | No | Medium | Yes | 20+10+5+10+15+10-20 | 50 | Mid |
| R3 Lakeside | Coastal | 90 | Yes | Open | Yes | High | No | 15+10+10+10+5+20+10 | 80 | Standard |
| R4 Market Town | Plain | 200 | No | Open | No | Low | No | 10+5+5+10+15+5+10 | 60 | Mid |
| R5 Bridge Colony | Coastal | 40 | Yes | Blocked | Yes | High | Yes | 15+15+10-15+5+20-20 | 30 | Low |

With the default stock (10 / 200 / 20) all five are served and **4 trucks,
110 packs and 11 staff** remain.

---

## 10. How to change the system later

| I want to... | Do this |
|---|---|
| Change the points for an answer | Edit the `POINTS_...` number in `config.h` |
| Change where the family bands start/end | Edit `FAMILIES_BAND1_MAX`, `BAND2_MAX`, `BAND3_MAX` |
| Change the score needed for a relief level | Edit the `THRESHOLD_...` numbers |
| Change what a relief level gives | Edit that level's `..._TRUCKS`, `..._PACKS`, `..._STAFF` |
| Change the default starting stock | Edit `START_TRUCKS`, `START_PACKS`, `START_STAFF` |
| Change the score range | Edit `MIN_SCORE`, `MAX_SCORE` (also check the thresholds still make sense) |
| Add a new area type or damage level | New menu code and points in `config.h`, a new `case` in `getAreaPoints` / `getDamagePoints` (main.cpp) and `areaText` / `damageText` (output.cpp), and update the menu text and range in `main()` |
| Add a brand-new question | New variable and question in `main()`, new parameter and points in `calculateNeedScore`, and show it in `printRequestInfo` |

The first five rows need **no logic change at all** - only numbers in
`config.h`. The intro screen updates automatically.

---

## 11. C++ concepts used, and where

| Concept (from the assignment) | Where it is used |
|---|---|
| `if`, `if-else`, nested `if / else if` | `getFamilyPoints`, `getReliefLevelFromScore`, `calculateNeedScore`, `readInt`, step 3f in `main()` |
| `switch` | `getAreaPoints`, `getDamagePoints`, `trucksFor`, `packsFor`, `staffFor`, and the text helpers in output.cpp |
| Relational operators `<  <=  >=  ==  !=` | `getFamilyPoints`, `getReliefLevelFromScore`, `canAfford`, `givenLevel != wantedLevel` |
| Logical operators `&&  \|\|  !` | `canAfford` (AND), `readInt` range check (OR), `readYesNo` (OR), `chooseAffordableLevel` (AND, NOT), "use default stock?" (NOT) |
| Arithmetic operators `+  -` | `FAMILIES_BAND1_MAX + 1` and `THRESHOLD_FULL - 1` in the printed tables |
| Assignment operators `+=  -=  ++` | `score +=` in `calculateNeedScore`; `trucksLeft -=` in step 3e; `requestCount++`, `level++`, counters |
| Repetition | `while (anotherRequest)` over requests; `while (true)` in `readInt` / `readYesNo`; `while` in `chooseAffordableLevel` |
| Formatted output with escape sequences | `\n`, `\t`, `setw`, `left`, `right` in output.cpp |
| Functions and header files | Everything is split into small functions; `output.h` / `config.h` |

---

## 12. Design decisions and assumptions

- **Road blocked = -15.** A blocked road lowers the score. With this value
  the highest possible total is exactly 100, which matches the maximum
  score in our notes.
- **Aftershocks:** "No" scores +15 and "Yes" scores +5 (as in our notes). The
  group must be ready to explain why.
- **Score clamp:** the raw total can be as low as -5, so the program keeps the
  score between 0 and 100.
- **Limits that touch (90, 70, 50, 20):** the higher level wins (`>=`). For
  example, a score of exactly 50 is Mid-Level Relief.
- **Family bands:** 0-20, 21-50, 51-100, over 100.
- **Defer sends 1 medical staff** (as in our notes). If no staff is left, it
  sends 0 so the stock never goes below zero.
- **Step-down rule:** if the earned level is too big for the stock, the
  program tries each smaller level in turn and gives the first that fits.
- **Summary counts:** "Deferred" counts both low-need deferrals and
  stock-shortage deferrals; the printed reason tells them apart.
- **The assignment requires a written reason for every weight.** That reasoning
  must come from the group; it is not written here.

---

## 13. Likely viva questions

1. **Why do we use `const` instead of typing numbers in the code?** One place
   to change them, names explain themselves, and nothing can change them by accident.
2. **What happens if the stock is too low for the level the score earns?**
   `chooseAffordableLevel` steps down one level at a time until one fits; the
   reason is printed.
3. **Why is Defer always allowed?** It uses 0 trucks and 0 packs, so it
   cannot run out. The loop stops at Defer so it can never go past the end.
4. **Why is the shortage reason built before subtracting?** After subtracting,
   the stock numbers would no longer be the ones that caused the step-down.
5. **What does `cin.ignore(1000, '\n')` do?** Throws away the rest of the
   typed line, including the Enter key, so the next read starts clean.
6. **Why `getline` for the name?** `cin >>` stops at a space; `getline` reads
   the whole line, so "Riverside Village" works.
7. **Why is the score clamped?** The raw total can go below 0 (down to -5);
   the system defines the score range as 0 to 100.
8. **What is a header file and why `#pragma once`?** A header declares
   things for other files; `#pragma once` prevents including it twice.
9. **Why does `printOutput` read `config.h` instead of using typed numbers?**
   So the instructions can never disagree with the real points.
10. **What does `static` mean on the helper functions in output.cpp?** They
    are private to that file.
11. **Predict: R2 with "aid already received" changed to No.** The -20 becomes
    +10, so the score goes from 50 to 80 (Standard Relief).
12. **Why are the relief levels numbered 0 to 4 in that order?** So that
    `level++` always means one level smaller.

---

## 14. To fill in before submitting

| Item | Your entry |
|---|---|
| Group members and contributions | `[Name 1 - what they did]`, `[Name 2 - ...]`, `[Name 3 - ...]` |
| GitHub repository link | `[link]` |
| Written reason for every weight and threshold | `[group to write]` |

**AI-use note (edit so it is true for your group):** *Our group designed the
inputs, the points system, the relief levels and the starting stock on
paper. We used an AI assistant (Claude) to turn that design into C++ code,
to explain the code line by line, and to write this guide. We read, tested
and can explain all of the code ourselves.*
