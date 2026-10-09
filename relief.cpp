// =====================================================================
//  EARTHQUAKE RELIEF SUPPLY DISTRIBUTION SYSTEM   (course-level version)
// =====================================================================
//  Uses only what we studied: cin, cout, variables, const, if / else if,
//  switch, ternary (?:), while / for loops, break.
//
//  HOW TO CHANGE THE SYSTEM
//  ------------------------
//  Every number lives in the SETTINGS section below.
//  Change a number there and recompile. You do NOT need to touch any
//  other part of the program: the welcome screen, the score and the
//  relief decisions all read their numbers from the settings.
// =====================================================================

#include <iostream>
#include <string>
using namespace std;

// =====================================================================
//  SETTINGS  (the only part you need to edit)
// =====================================================================

// ---- 1. Starting stock ----
const int START_TRUCKS = 10;
const int START_PACKS  = 200;
const int START_STAFF  = 20;

// Largest numbers accepted when the user types a custom stock
const int MAX_CUSTOM_TRUCKS = 1000;
const int MAX_CUSTOM_PACKS  = 100000;
const int MAX_CUSTOM_STAFF  = 1000;

// ---- 2. Menu codes (the number typed for each menu choice) ----
const int AREA_PLAIN       = 1;
const int AREA_MOUNTAINOUS = 2;
const int AREA_COASTAL     = 3;

const int DAMAGE_LOW    = 1;
const int DAMAGE_MEDIUM = 2;
const int DAMAGE_HIGH   = 3;

// ---- 3. Need-score points (a negative number takes points away) ----
// Area type
const int POINTS_PLAIN       = 10;
const int POINTS_MOUNTAINOUS = 20;
const int POINTS_COASTAL     = 15;

// Affected families: each MAX is the TOP of a band
//   band 1 = 0 to 20, band 2 = 21 to 50, band 3 = 51 to 100, band 4 = above
const int FAMILIES_BAND1_MAX = 20;
const int FAMILIES_BAND2_MAX = 50;
const int FAMILIES_BAND3_MAX = 100;
const int POINTS_FAMILIES_BAND1 = 5;
const int POINTS_FAMILIES_BAND2 = 15;
const int POINTS_FAMILIES_BAND3 = 10;
const int POINTS_FAMILIES_BAND4 = 5;
const int MAX_FAMILIES_ALLOWED  = 10000;   // largest number accepted as input

// Medical aid needed?
const int POINTS_MEDICAL_YES = 10;
const int POINTS_MEDICAL_NO  = 5;

// Is the road to the area open?
const int POINTS_ROUTE_OPEN    = 10;
const int POINTS_ROUTE_BLOCKED = -15;

// Are aftershocks expected?
const int POINTS_AFTERSHOCK_YES = 5;
const int POINTS_AFTERSHOCK_NO  = 15;

// Damage level
const int POINTS_DAMAGE_LOW    = 5;
const int POINTS_DAMAGE_MEDIUM = 10;
const int POINTS_DAMAGE_HIGH   = 20;

// Has the area already received aid?
const int POINTS_AID_RECEIVED_YES = -20;
const int POINTS_AID_RECEIVED_NO  = 10;

// The final score is always kept between these two numbers
const int MIN_SCORE = 0;
const int MAX_SCORE = 100;

// ---- 4. Relief level codes (0 = biggest relief, 4 = smallest) ----
// When stock runs low the program steps down: 0 -> 1 -> 2 -> 3 -> 4
const int FULL_RELIEF     = 0;
const int STANDARD_RELIEF = 1;
const int MID_RELIEF      = 2;
const int LOW_RELIEF      = 3;
const int DEFER           = 4;   // nothing is sent, area is monitored

// ---- 5. Minimum score needed for each relief level ----
const int THRESHOLD_FULL     = 90;
const int THRESHOLD_STANDARD = 70;
const int THRESHOLD_MID      = 50;
const int THRESHOLD_LOW      = 20;
// anything below THRESHOLD_LOW is Defer

// ---- 6. What each relief level gives ----
const int FULL_TRUCKS     = 3;   const int FULL_PACKS     = 50;   const int FULL_STAFF     = 5;
const int STANDARD_TRUCKS = 2;   const int STANDARD_PACKS = 30;   const int STANDARD_STAFF = 3;
const int MID_TRUCKS      = 1;   const int MID_PACKS      = 20;   const int MID_STAFF      = 2;
const int LOW_TRUCKS      = 1;   const int LOW_PACKS      = 10;   const int LOW_STAFF      = 1;
const int DEFER_TRUCKS    = 0;   const int DEFER_PACKS    = 0;    const int DEFER_STAFF    = 1;

// =====================================================================
//  PROGRAM  (no need to edit anything below this line)
// =====================================================================
int main() {

    // =================================================================
    //  PART 1: WELCOME SCREEN
    //  All numbers printed here come from the settings above, so this
    //  screen updates by itself when a setting is changed.
    // =================================================================
    cout << "============================================================\n";
    cout << "             EARTHQUAKE RELIEF SUPPLY SYSTEM\n";
    cout << "============================================================\n";
    cout << "Welcome. This system distributes limited relief supplies fairly\n";
    cout << "by calculating a need score for each affected area.\n\n";

    cout << "HOW TO USE THE SYSTEM\n";
    cout << "1. Enter the name of the affected area.\n";
    cout << "2. Choose the area type and enter the number of affected families.\n";
    cout << "3. Answer each Yes/No question exactly as requested (y or n).\n";
    cout << "4. Select the damage level (Low, Medium, or High).\n";
    cout << "5. The system adds the points for every answer and calculates\n";
    cout << "   the area's total need score (" << MIN_SCORE << " to " << MAX_SCORE << ").\n";
    cout << "6. Supplies are assigned from the need score. Whatever is given is\n";
    cout << "   removed from the stock, so the next area receives what is left.\n\n";

    // showpos makes cout print a + sign in front of positive numbers
    // (+10, +5 ...). Negative numbers always show their own - sign.
    // noshowpos switches the + sign off again.
    cout << "INPUTS AND NEED-SCORE POINTS\n";
    cout << showpos;
    cout << "Area type:           Plain = " << POINTS_PLAIN
         << ", Mountainous = " << POINTS_MOUNTAINOUS
         << ", Coastal = " << POINTS_COASTAL << "\n";

    // The family line mixes plain numbers (the bands) with points (+5),
    // so the + sign is switched off and on around each part.
    cout << "Affected families:   0-" << noshowpos << FAMILIES_BAND1_MAX << " = "
         << showpos << POINTS_FAMILIES_BAND1 << ", "
         << noshowpos << FAMILIES_BAND1_MAX + 1 << "-" << FAMILIES_BAND2_MAX << " = "
         << showpos << POINTS_FAMILIES_BAND2 << ",\n                     "
         << noshowpos << FAMILIES_BAND2_MAX + 1 << "-" << FAMILIES_BAND3_MAX << " = "
         << showpos << POINTS_FAMILIES_BAND3 << ", over "
         << noshowpos << FAMILIES_BAND3_MAX << " = "
         << showpos << POINTS_FAMILIES_BAND4 << "\n";

    cout << "Medical aid needed:  Yes = " << POINTS_MEDICAL_YES
         << ", No = " << POINTS_MEDICAL_NO << "\n";
    cout << "Road to the area:    Open = " << POINTS_ROUTE_OPEN
         << ", Blocked = " << POINTS_ROUTE_BLOCKED << "\n";
    cout << "Aftershocks expected: Yes = " << POINTS_AFTERSHOCK_YES
         << ", No = " << POINTS_AFTERSHOCK_NO << "\n";
    cout << "Damage level:        Low = " << POINTS_DAMAGE_LOW
         << ", Medium = " << POINTS_DAMAGE_MEDIUM
         << ", High = " << POINTS_DAMAGE_HIGH << "\n";
    cout << "Aid already received: Yes = " << POINTS_AID_RECEIVED_YES
         << ", No = " << POINTS_AID_RECEIVED_NO << "\n\n";
    cout << noshowpos;   // from here on, no + signs

    cout << "A higher score means the area has a greater and more urgent need.\n";
    cout << "Earlier aid reduces the score because that area has already been\n";
    cout << "helped. The score is always kept between " << MIN_SCORE << " and " << MAX_SCORE << ".\n\n";

    // ---- The relief table: one row per relief level ----
    // The loop runs once for each level (0 to 4). The switch fills in the
    // numbers for that row, then ONE block of code prints the row.
    cout << "SUPPLY ALLOCATION BY NEED SCORE\n";
    for (int tier = FULL_RELIEF; tier <= DEFER; tier++) {
        int fromScore = 0;
        int toScore = 0;
        int tierTrucks = 0;
        int tierPacks = 0;
        int tierStaff = 0;
        string tierName = "";   // names are padded with spaces to 18 letters

        switch (tier) {
            case FULL_RELIEF:
                fromScore = THRESHOLD_FULL;     toScore = MAX_SCORE;
                tierTrucks = FULL_TRUCKS;       tierPacks = FULL_PACKS;     tierStaff = FULL_STAFF;
                tierName = "Full Relief       ";
                break;
            case STANDARD_RELIEF:
                fromScore = THRESHOLD_STANDARD; toScore = THRESHOLD_FULL - 1;
                tierTrucks = STANDARD_TRUCKS;   tierPacks = STANDARD_PACKS; tierStaff = STANDARD_STAFF;
                tierName = "Standard Relief   ";
                break;
            case MID_RELIEF:
                fromScore = THRESHOLD_MID;      toScore = THRESHOLD_STANDARD - 1;
                tierTrucks = MID_TRUCKS;        tierPacks = MID_PACKS;      tierStaff = MID_STAFF;
                tierName = "Mid-Level Relief  ";
                break;
            case LOW_RELIEF:
                fromScore = THRESHOLD_LOW;      toScore = THRESHOLD_MID - 1;
                tierTrucks = LOW_TRUCKS;        tierPacks = LOW_PACKS;      tierStaff = LOW_STAFF;
                tierName = "Low-Level Relief  ";
                break;
            default:   // DEFER
                fromScore = MIN_SCORE;          toScore = THRESHOLD_LOW - 1;
                tierTrucks = DEFER_TRUCKS;      tierPacks = DEFER_PACKS;    tierStaff = DEFER_STAFF;
                tierName = "Defer             ";
                break;
        }

        // Print the row. The small "if" lines add spaces in front of
        // short numbers so that the columns line up.
        cout << "Need score ";
        if (fromScore < 100) cout << " ";
        if (fromScore < 10)  cout << " ";
        cout << fromScore << " - ";
        if (toScore < 100) cout << " ";
        if (toScore < 10)  cout << " ";
        cout << toScore << " : " << tierName << tierTrucks << " trucks, ";
        if (tierPacks < 10) cout << " ";
        cout << tierPacks << " supply packs, " << tierStaff << " medical staff\n";
    }
    cout << "\n";

    cout << "IMPORTANT NOTES\n";
    cout << "- Do not enter estimates that are not supported by field information.\n";
    cout << "- Check that all answers and family counts are correct before submitting.\n";
    cout << "- If supplies are not enough for the level a score deserves, the system\n";
    cout << "  steps down to the biggest level that the remaining stock can cover.\n";
    cout << "  Deferred areas must be recorded for the next distribution.\n";
    cout << "- Confirm road access before dispatching trucks and coordinate medical\n";
    cout << "  staff with the receiving relief team.\n";

    // =================================================================
    //  PART 2: STARTING STOCK
    // =================================================================
    // These three variables are the "warehouse". They go down every time
    // relief is given.
    int trucksLeft = START_TRUCKS;
    int packsLeft  = START_PACKS;
    int staffLeft  = START_STAFF;

    int  value;    // holds a number typed by the user
    char answer;   // holds the y or n typed by the user

    cout << "\nDefault stock: " << START_TRUCKS << " trucks, " << START_PACKS
         << " supply packs, " << START_STAFF << " medical staff.\n";

    // ---- Ask: use the default stock? (keeps asking until y or n) ----
    bool useDefault = true;
    while (true) {
        cout << "Use the default stock? (y/n): ";
        cin >> answer;
        if (cin.eof()) {                       // no more input: stop the program
            cout << "\nInput ended. Program stopped.\n";
            return 0;
        }
        cin.ignore(1000, '\n');                // throw away the rest of the line

        if (answer == 'y' || answer == 'Y') {
            useDefault = true;
            break;                             // valid answer: leave the loop
        } else if (answer == 'n' || answer == 'N') {
            useDefault = false;
            break;
        } else {
            cout << "  Please type y or n.\n";
        }
    }

    // ---- Custom stock (useful for testing "not enough supplies") ----
    if (!useDefault) {

        // Trucks
        while (true) {
            cout << "  Trucks available        : ";
            cin >> value;
            if (cin.eof()) {
                cout << "\nInput ended. Program stopped.\n";
                return 0;
            }
            if (cin.fail()) {                  // user typed letters, not a number
                cin.clear();                   // reset cin's error state
                cin.ignore(1000, '\n');        // throw away the bad line
                cout << "  Invalid input. Please type a number.\n";
            } else if (value < 0 || value > MAX_CUSTOM_TRUCKS) {
                cin.ignore(1000, '\n');
                cout << "  Please type a number from 0 to " << MAX_CUSTOM_TRUCKS << ".\n";
            } else {
                cin.ignore(1000, '\n');
                trucksLeft = value;
                break;
            }
        }

        // Supply packs
        while (true) {
            cout << "  Supply packs available  : ";
            cin >> value;
            if (cin.eof()) {
                cout << "\nInput ended. Program stopped.\n";
                return 0;
            }
            if (cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "  Invalid input. Please type a number.\n";
            } else if (value < 0 || value > MAX_CUSTOM_PACKS) {
                cin.ignore(1000, '\n');
                cout << "  Please type a number from 0 to " << MAX_CUSTOM_PACKS << ".\n";
            } else {
                cin.ignore(1000, '\n');
                packsLeft = value;
                break;
            }
        }

        // Medical staff
        while (true) {
            cout << "  Medical staff available : ";
            cin >> value;
            if (cin.eof()) {
                cout << "\nInput ended. Program stopped.\n";
                return 0;
            }
            if (cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "  Invalid input. Please type a number.\n";
            } else if (value < 0 || value > MAX_CUSTOM_STAFF) {
                cin.ignore(1000, '\n');
                cout << "  Please type a number from 0 to " << MAX_CUSTOM_STAFF << ".\n";
            } else {
                cin.ignore(1000, '\n');
                staffLeft = value;
                break;
            }
        }
    }

    // =================================================================
    //  PART 3: COUNTERS FOR THE FINAL SUMMARY
    // =================================================================
    int requestCount  = 0;   // how many requests were processed
    int servedFully   = 0;   // got the level their score earned
    int servedReduced = 0;   // got something, but less than earned
    int deferred      = 0;   // got nothing (deferred)

    // =================================================================
    //  PART 4: PROCESS REQUESTS (repeat until the user says stop)
    // =================================================================
    bool anotherRequest = true;
    while (anotherRequest) {
        requestCount++;
        cout << "\n--- Enter the details of request #" << requestCount << " ---\n";

        // -------------------------------------------------------------
        //  4a. READ THE INPUTS
        // -------------------------------------------------------------

        // Area name (getline also accepts spaces)
        string areaName;
        cout << "Name of the affected area: ";
        getline(cin, areaName);
        if (areaName == "") {                  // user just pressed Enter
            areaName = "Unnamed area";
        }

        // Area type
        int areaType = 0;
        while (true) {
            cout << "Area type (1=Plain, 2=Mountainous, 3=Coastal): ";
            cin >> value;
            if (cin.eof()) {
                cout << "\nInput ended. Program stopped.\n";
                return 0;
            }
            if (cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "  Invalid input. Please type a number.\n";
            } else if (value < AREA_PLAIN || value > AREA_COASTAL) {
                cin.ignore(1000, '\n');
                cout << "  Please type a number from " << AREA_PLAIN
                     << " to " << AREA_COASTAL << ".\n";
            } else {
                cin.ignore(1000, '\n');
                areaType = value;
                break;
            }
        }

        // Number of affected families
        int families = 0;
        while (true) {
            cout << "Number of affected families: ";
            cin >> value;
            if (cin.eof()) {
                cout << "\nInput ended. Program stopped.\n";
                return 0;
            }
            if (cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "  Invalid input. Please type a number.\n";
            } else if (value < 0 || value > MAX_FAMILIES_ALLOWED) {
                cin.ignore(1000, '\n');
                cout << "  Please type a number from 0 to " << MAX_FAMILIES_ALLOWED << ".\n";
            } else {
                cin.ignore(1000, '\n');
                families = value;
                break;
            }
        }

        // Is medical aid needed?
        bool medicalAid = false;
        while (true) {
            cout << "Is medical aid needed? (y/n): ";
            cin >> answer;
            if (cin.eof()) {
                cout << "\nInput ended. Program stopped.\n";
                return 0;
            }
            cin.ignore(1000, '\n');
            if (answer == 'y' || answer == 'Y') {
                medicalAid = true;
                break;
            } else if (answer == 'n' || answer == 'N') {
                medicalAid = false;
                break;
            } else {
                cout << "  Please type y or n.\n";
            }
        }

        // Is the road to the area open?
        bool routeOpen = false;
        while (true) {
            cout << "Is the road to the area open? (y/n): ";
            cin >> answer;
            if (cin.eof()) {
                cout << "\nInput ended. Program stopped.\n";
                return 0;
            }
            cin.ignore(1000, '\n');
            if (answer == 'y' || answer == 'Y') {
                routeOpen = true;
                break;
            } else if (answer == 'n' || answer == 'N') {
                routeOpen = false;
                break;
            } else {
                cout << "  Please type y or n.\n";
            }
        }

        // Are aftershocks expected?
        bool aftershocks = false;
        while (true) {
            cout << "Are aftershocks expected? (y/n): ";
            cin >> answer;
            if (cin.eof()) {
                cout << "\nInput ended. Program stopped.\n";
                return 0;
            }
            cin.ignore(1000, '\n');
            if (answer == 'y' || answer == 'Y') {
                aftershocks = true;
                break;
            } else if (answer == 'n' || answer == 'N') {
                aftershocks = false;
                break;
            } else {
                cout << "  Please type y or n.\n";
            }
        }

        // Damage level
        int damage = 0;
        while (true) {
            cout << "Damage level (1=Low, 2=Medium, 3=High): ";
            cin >> value;
            if (cin.eof()) {
                cout << "\nInput ended. Program stopped.\n";
                return 0;
            }
            if (cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "  Invalid input. Please type a number.\n";
            } else if (value < DAMAGE_LOW || value > DAMAGE_HIGH) {
                cin.ignore(1000, '\n');
                cout << "  Please type a number from " << DAMAGE_LOW
                     << " to " << DAMAGE_HIGH << ".\n";
            } else {
                cin.ignore(1000, '\n');
                damage = value;
                break;
            }
        }

        // Has the area already received aid?
        bool aidReceived = false;
        while (true) {
            cout << "Has the area already received aid? (y/n): ";
            cin >> answer;
            if (cin.eof()) {
                cout << "\nInput ended. Program stopped.\n";
                return 0;
            }
            cin.ignore(1000, '\n');
            if (answer == 'y' || answer == 'Y') {
                aidReceived = true;
                break;
            } else if (answer == 'n' || answer == 'N') {
                aidReceived = false;
                break;
            } else {
                cout << "  Please type y or n.\n";
            }
        }

        // -------------------------------------------------------------
        //  4b. CALCULATE THE NEED SCORE (add the points of every answer)
        // -------------------------------------------------------------
        int score = 0;

        // Area type
        switch (areaType) {
            case AREA_PLAIN:       score += POINTS_PLAIN;       break;
            case AREA_MOUNTAINOUS: score += POINTS_MOUNTAINOUS; break;
            case AREA_COASTAL:     score += POINTS_COASTAL;     break;
        }

        // Affected families (4 bands, checked from the smallest band up)
        if (families <= FAMILIES_BAND1_MAX) {
            score += POINTS_FAMILIES_BAND1;
        } else if (families <= FAMILIES_BAND2_MAX) {
            score += POINTS_FAMILIES_BAND2;
        } else if (families <= FAMILIES_BAND3_MAX) {
            score += POINTS_FAMILIES_BAND3;
        } else {
            score += POINTS_FAMILIES_BAND4;
        }

        // Medical aid
        if (medicalAid) {
            score += POINTS_MEDICAL_YES;
        } else {
            score += POINTS_MEDICAL_NO;
        }

        // Road
        if (routeOpen) {
            score += POINTS_ROUTE_OPEN;
        } else {
            score += POINTS_ROUTE_BLOCKED;     // this one is negative
        }

        // Aftershocks
        if (aftershocks) {
            score += POINTS_AFTERSHOCK_YES;
        } else {
            score += POINTS_AFTERSHOCK_NO;
        }

        // Damage level
        switch (damage) {
            case DAMAGE_LOW:    score += POINTS_DAMAGE_LOW;    break;
            case DAMAGE_MEDIUM: score += POINTS_DAMAGE_MEDIUM; break;
            case DAMAGE_HIGH:   score += POINTS_DAMAGE_HIGH;   break;
        }

        // Aid already received
        if (aidReceived) {
            score += POINTS_AID_RECEIVED_YES;  // negative: already helped
        } else {
            score += POINTS_AID_RECEIVED_NO;
        }

        // Keep the score inside the allowed range
        if (score < MIN_SCORE) {
            score = MIN_SCORE;
        }
        if (score > MAX_SCORE) {
            score = MAX_SCORE;
        }

        // -------------------------------------------------------------
        //  4c. FIND THE RELIEF LEVEL THE SCORE EARNS
        //      (checked from the highest limit down, first match wins)
        // -------------------------------------------------------------
        int wantedLevel;
        if (score >= THRESHOLD_FULL) {
            wantedLevel = FULL_RELIEF;
        } else if (score >= THRESHOLD_STANDARD) {
            wantedLevel = STANDARD_RELIEF;
        } else if (score >= THRESHOLD_MID) {
            wantedLevel = MID_RELIEF;
        } else if (score >= THRESHOLD_LOW) {
            wantedLevel = LOW_RELIEF;
        } else {
            wantedLevel = DEFER;
        }

        // What does the level the score EARNED need?
        // (used later to explain a shortage)
        int wantTrucks = 0;
        int wantPacks  = 0;
        int wantStaff  = 0;
        string wantedName = "";
        switch (wantedLevel) {
            case FULL_RELIEF:
                wantTrucks = FULL_TRUCKS;     wantPacks = FULL_PACKS;     wantStaff = FULL_STAFF;
                wantedName = "Full Relief";
                break;
            case STANDARD_RELIEF:
                wantTrucks = STANDARD_TRUCKS; wantPacks = STANDARD_PACKS; wantStaff = STANDARD_STAFF;
                wantedName = "Standard Relief";
                break;
            case MID_RELIEF:
                wantTrucks = MID_TRUCKS;      wantPacks = MID_PACKS;      wantStaff = MID_STAFF;
                wantedName = "Mid-Level Relief";
                break;
            case LOW_RELIEF:
                wantTrucks = LOW_TRUCKS;      wantPacks = LOW_PACKS;      wantStaff = LOW_STAFF;
                wantedName = "Low-Level Relief";
                break;
            default:   // DEFER
                wantTrucks = DEFER_TRUCKS;    wantPacks = DEFER_PACKS;    wantStaff = DEFER_STAFF;
                wantedName = "Defer";
                break;
        }

        // -------------------------------------------------------------
        //  4d. CHECK THE STOCK
        //      Start at the earned level. If the stock cannot cover it,
        //      step DOWN one level at a time until something fits.
        //      DEFER is the last step and is always allowed.
        // -------------------------------------------------------------
        int givenLevel = wantedLevel;
        int trucksUsed = 0;
        int packsUsed  = 0;
        int staffUsed  = 0;
        string givenName = "";

        bool levelFound = false;
        while (!levelFound) {
            // What does the level we are trying need?
            switch (givenLevel) {
                case FULL_RELIEF:
                    trucksUsed = FULL_TRUCKS;     packsUsed = FULL_PACKS;     staffUsed = FULL_STAFF;
                    givenName = "Full Relief";
                    break;
                case STANDARD_RELIEF:
                    trucksUsed = STANDARD_TRUCKS; packsUsed = STANDARD_PACKS; staffUsed = STANDARD_STAFF;
                    givenName = "Standard Relief";
                    break;
                case MID_RELIEF:
                    trucksUsed = MID_TRUCKS;      packsUsed = MID_PACKS;      staffUsed = MID_STAFF;
                    givenName = "Mid-Level Relief";
                    break;
                case LOW_RELIEF:
                    trucksUsed = LOW_TRUCKS;      packsUsed = LOW_PACKS;      staffUsed = LOW_STAFF;
                    givenName = "Low-Level Relief";
                    break;
                default:   // DEFER
                    trucksUsed = DEFER_TRUCKS;    packsUsed = DEFER_PACKS;    staffUsed = DEFER_STAFF;
                    givenName = "Defer";
                    break;
            }

            if (givenLevel == DEFER) {
                levelFound = true;                     // Defer is always allowed
            } else if (trucksUsed <= trucksLeft && packsUsed <= packsLeft && staffUsed <= staffLeft) {
                levelFound = true;                     // stock covers trucks AND packs AND staff
            } else {
                givenLevel++;                          // not enough: try the next smaller level
            }
        }

        // Never use more staff than we have (only possible for Defer)
        if (staffUsed > staffLeft) {
            staffUsed = staffLeft;
        }

        // Remember the stock BEFORE subtracting (needed for the shortage message)
        int trucksBefore = trucksLeft;
        int packsBefore  = packsLeft;
        int staffBefore  = staffLeft;

        // -------------------------------------------------------------
        //  4e. SUBTRACT WHAT WAS USED FROM THE STOCK
        //      The NEXT request will see the new values.
        // -------------------------------------------------------------
        trucksLeft -= trucksUsed;
        packsLeft  -= packsUsed;
        staffLeft  -= staffUsed;

        // -------------------------------------------------------------
        //  4f. UPDATE THE SUMMARY COUNTERS
        // -------------------------------------------------------------
        if (givenLevel == DEFER) {
            deferred++;                    // got nothing
        } else if (givenLevel == wantedLevel) {
            servedFully++;                 // got what the score earned
        } else {
            servedReduced++;               // got a smaller level
        }

        // -------------------------------------------------------------
        //  4g. SHOW THE REQUEST DETAILS
        // -------------------------------------------------------------
        cout << "\n============================================================\n";
        cout << " REQUEST #" << requestCount << " : " << areaName << "\n";
        cout << "============================================================\n";

        cout << " Area type            : ";
        switch (areaType) {
            case AREA_PLAIN:       cout << "Plain";       break;
            case AREA_MOUNTAINOUS: cout << "Mountainous"; break;
            case AREA_COASTAL:     cout << "Coastal";     break;
        }
        cout << "\n";

        cout << " Affected families    : " << families << "\n";

        // (condition ? textIfTrue : textIfFalse) is the short if-else
        cout << " Medical aid needed   : " << (medicalAid ? "Yes" : "No") << "\n";
        cout << " Road to the area     : " << (routeOpen ? "Open" : "Blocked") << "\n";
        cout << " Aftershocks expected : " << (aftershocks ? "Yes" : "No") << "\n";

        cout << " Damage level         : ";
        switch (damage) {
            case DAMAGE_LOW:    cout << "Low";    break;
            case DAMAGE_MEDIUM: cout << "Medium"; break;
            case DAMAGE_HIGH:   cout << "High";   break;
        }
        cout << "\n";

        cout << " Aid already received : " << (aidReceived ? "Yes" : "No") << "\n";

        // -------------------------------------------------------------
        //  4h. SHOW THE DECISION
        // -------------------------------------------------------------
        cout << "------------------------------------------------------------\n";
        cout << " NEED SCORE           : " << score << " / " << MAX_SCORE << "\n";
        cout << " Level this score earns: " << wantedName << "\n";

        if (givenLevel != wantedLevel) {
            // The stock was too low, so a smaller level was given. Say WHY.
            cout << " >> STOCK SHORTAGE    : ";
            if (wantTrucks > trucksBefore) {
                cout << "trucks (need " << wantTrucks << ", have " << trucksBefore << ")  ";
            }
            if (wantPacks > packsBefore) {
                cout << "supply packs (need " << wantPacks << ", have " << packsBefore << ")  ";
            }
            if (wantStaff > staffBefore) {
                cout << "medical staff (need " << wantStaff << ", have " << staffBefore << ")  ";
            }
            cout << "\n";
            cout << " >> Stepped down from " << wantedName << " to " << givenName << "\n";
        } else if (givenLevel == DEFER) {
            // The score itself was low; this has nothing to do with the stock.
            cout << " >> Reason            : need score is below " << THRESHOLD_LOW
                 << " (low priority)\n";
        }

        cout << " ACTION TAKEN         : " << givenName << "\n";
        cout << " Trucks used          : " << trucksUsed << "\n";
        cout << " Supply packs used    : " << packsUsed << "\n";
        cout << " Medical staff used   : " << staffUsed << "\n";
        cout << "------------------------------------------------------------\n";
        cout << " REMAINING STOCK  ->  Trucks: " << trucksLeft
             << "\tPacks: " << packsLeft
             << "\tStaff: " << staffLeft << "\n";

        // -------------------------------------------------------------
        //  4i. ASK WHETHER TO CONTINUE
        // -------------------------------------------------------------
        while (true) {
            cout << "\nEnter another request? (y/n): ";
            cin >> answer;
            if (cin.eof()) {
                cout << "\nInput ended. Program stopped.\n";
                return 0;
            }
            cin.ignore(1000, '\n');
            if (answer == 'y' || answer == 'Y') {
                anotherRequest = true;
                break;
            } else if (answer == 'n' || answer == 'N') {
                anotherRequest = false;
                break;
            } else {
                cout << "  Please type y or n.\n";
            }
        }
    }

    // =================================================================
    //  PART 5: FINAL SUMMARY
    // =================================================================
    int handled = servedFully + servedReduced;   // requests that got some relief

    cout << "\n============================================================\n";
    cout << "                        SUMMARY\n";
    cout << "============================================================\n";
    cout << " Total requests processed        : " << requestCount << "\n";
    cout << " Served at the level they earned : " << servedFully << "\n";
    cout << " Served with REDUCED relief      : " << servedReduced << "\n";
    cout << " Deferred / could not be served  : " << deferred << "\n";
    cout << " Successfully handled (got relief): " << handled << " of " << requestCount << "\n";
    cout << "------------------------------------------------------------\n";
    cout << " Stock left at the end ->\tTrucks: " << trucksLeft
         << "\tPacks: " << packsLeft << "\tStaff: " << staffLeft << "\n";
    cout << "============================================================\n";

    return 0;   // 0 = finished without errors
}
