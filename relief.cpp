//  EARTHQUAKE RELIEF SUPPLY DISTRIBUTION SYSTEM

#include <iostream>
#include <string>
using namespace std;

// First we will initialize and declare all constant variables that will be used throughout the program. 
// These constants will be used to determine the starting stock, menu codes, need-score points, relief level codes, 
// minimum score needed for each relief level, and what each relief level gives.

//1. Starting stock 
const int START_TRUCKS = 10;
const int START_PACKS  = 200;
const int START_STAFF  = 20;

// 2. Menu codes
const int AREA_PLAIN       = 1;
const int AREA_MOUNTAINOUS = 2;
const int AREA_COASTAL     = 3;

const int DAMAGE_LOW    = 1;
const int DAMAGE_MEDIUM = 2;
const int DAMAGE_HIGH   = 3;

// 3. Need-score points
// Area type
const int POINTS_PLAIN       = 10;
const int POINTS_MOUNTAINOUS = 15;
const int POINTS_COASTAL     = 15;

// Affected families: each MAX is the TOP of a band
//   band 1 = 0 to 20, band 2 = 21 to 50, band 3 = 50 and above
const int FAMILIES_BAND1_MAX = 20;
const int FAMILIES_BAND2_MAX = 50;
const int FAMILIES_BAND3_MAX = 10000;
const int POINTS_FAMILIES_BAND1 = 5;
const int POINTS_FAMILIES_BAND2 = 10;
const int POINTS_FAMILIES_BAND3 = 15;
const int MAX_FAMILIES_ALLOWED  = 10000;   // largest number accepted as input

// Medical aid needed?
const int POINTS_MEDICAL_YES = 10;
const int POINTS_MEDICAL_NO  = 5;

// Is the road to the area open?
const int POINTS_ROUTE_OPEN    = 10;
const int POINTS_ROUTE_BLOCKED = -15;

// Are aftershocks expected?
const int POINTS_AFTERSHOCK_YES = 10;
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

//4. Relief level codes
// When stock runs low the program steps down: 0 -> 1 -> 2 -> 3 -> 4
const int FULL_RELIEF     = 0;
const int STANDARD_RELIEF = 1;
const int MID_RELIEF      = 2;
const int LOW_RELIEF      = 3;
const int DEFER           = 4;   // nothing is sent

//5. Minimum score needed for each relief level
const int THRESHOLD_FULL     = 90;
const int THRESHOLD_STANDARD = 70;
const int THRESHOLD_MID      = 50;
const int THRESHOLD_LOW      = 20;
// anything below THRESHOLD_LOW is Defer

//6. What each relief level gives
const int FULL_TRUCKS     = 3;   const int FULL_PACKS     = 50;   const int FULL_STAFF     = 5;
const int STANDARD_TRUCKS = 2;   const int STANDARD_PACKS = 30;   const int STANDARD_STAFF = 3;
const int MID_TRUCKS      = 1;   const int MID_PACKS      = 20;   const int MID_STAFF      = 2;
const int LOW_TRUCKS      = 1;   const int LOW_PACKS      = 10;   const int LOW_STAFF      = 1;
const int DEFER_TRUCKS    = 0;   const int DEFER_PACKS    = 0;    const int DEFER_STAFF    = 1;

// MAIN PROGRAM 
int main() {
    //  WELCOME INSTRUCTIONS
    cout << R"(EARTHQUAKE RELIEF SUPPLY SYSTEM
Welcome. This system distributes limited relief supplies fairly
by calculating a need score for each affected area.
 
HOW TO USE THE SYSTEM
1. Enter the name of the affected area.
2. Choose the area type and enter the number of affected families.
3. Answer each Yes/No question exactly as requested (y or n).
4. Select the damage level (Low, Medium, or High).
5. The system adds the points for every answer and calculates
   the area's total need score (0 to 100).
6. Supplies are assigned from the need score. Whatever is given is
   removed from the stock, so the next area receives what is left.
 
INPUTS AND NEED-SCORE POINTS
Area type:            Plain = +10, Mountainous = +15, Coastal = +15
Affected families:    0-20 = +5, 21-50 = +10, over 50 = +15
Medical aid needed:   Yes = +10, No = +5
Road to the area:     Open = +10, Blocked = -15
Aftershocks expected: Yes = +10, No = +15
Damage level:         Low = +5, Medium = +10, High = +20
Aid already received: Yes = -20, No = +10
 
A higher score means the area has a greater and more urgent need.
Earlier aid reduces the score because that area has already been
helped. The score is always kept between 0 and 100.
 
SUPPLY ALLOCATION BY NEED SCORE
Need score  90 - 100 : Full Relief       3 trucks, 50 supply packs, 5 medical staff
Need score  70 -  89 : Standard Relief   2 trucks, 30 supply packs, 3 medical staff
Need score  50 -  69 : Mid-Level Relief  1 truck,  20 supply packs, 2 medical staff
Need score  20 -  49 : Low-Level Relief  1 truck,  10 supply packs, 1 medical staff
Need score   0 -  19 : Defer             0 trucks,  0 supply packs, 1 medical staff
 
IMPORTANT NOTES
- Do not enter estimates that are not supported by field information.
- Check that all answers and family counts are correct before submitting.
- If supplies are not enough for the level a score deserves, the system
  steps down to the biggest level that the remaining stock can cover.
  Deferred areas must be recorded for the next distribution.
- Confirm road access before dispatching trucks and coordinate medical
  staff with the receiving relief team.
)";

    //STARTING STOCK 
    // These three variables are the "warehouse". They go down every time
    // relief is given.
    int trucksLeft = START_TRUCKS;
    int packsLeft  = START_PACKS;
    int staffLeft  = START_STAFF;

    int  value;    // holds a number typed by the user
    char answer;   // holds the y or n typed by the user

    // The initial stck is printed here so the user can see what is available before entering requests.
    cout << "\nStarting stock: " << START_TRUCKS << " trucks, " << START_PACKS
         << " supply packs, " << START_STAFF << " medical staff.\n";

    //COUNTERS FOR THE FINAL SUMMARY
    int requestCount  = 0;   // how many requests were processed
    int servedFully   = 0;   // got the level their score earned
    int servedReduced = 0;   // got something, but less than earned
    int deferred      = 0;   // got nothing (deferred)

    // PROCESS REQUESTS. It will repeat until the user says stop
    bool anotherRequest = true;
    while (anotherRequest) {
        requestCount++;
        cout << "\nEnter the details of request #" << requestCount << endl;

        // READ THE INPUTS

        // Area name (getline also accepts spaces)
        string areaName;
        cout << "Name of the affected area: ";
        getline(cin, areaName);
        if (areaName == "") {   // user just pressed Enter
            areaName = "Unnamed area";
        }

        // Area type
        int areaType = 0;
        while (true) {   // keep asking until the answer is valid
            cout << "Area type (1=Plain, 2=Mountainous, 3=Coastal): ";
            cin >> value;
            if (cin.eof()) {   // no input left, so stop instead of looping forever
                cout << "\nInput ended. Program stopped.\n";
                return 0;
            }
            if (cin.fail()) {   // user typed letters instead of a number
                cin.clear();    // repair cin so it can read again
                cin.ignore(1000, '\n');  // throw away the bad text
                cout << "  Invalid input. Please type a number.\n";
            } else if (value < AREA_PLAIN || value > AREA_COASTAL) { // number is outside the allowed range
                cin.ignore(1000, '\n'); // remove the leftover Enter key
                cout << "  Please type a number from " << AREA_PLAIN
                     << " to " << AREA_COASTAL << ".\n";
            } else {   // valid answer
                cin.ignore(1000, '\n');     // remove the leftover Enter key
                areaType = value;  // save the answer
                break;    // leave the loop
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

        //CALCULATE THE NEED SCORE (add/subtract the points of every answer)
        int score = 0;

        // Area type
        switch (areaType) {
            case AREA_PLAIN:       score += POINTS_PLAIN;       break;
            case AREA_MOUNTAINOUS: score += POINTS_MOUNTAINOUS; break;
            case AREA_COASTAL:     score += POINTS_COASTAL;     break;
        }

        // Affected families (3 bands, checked from the smallest band up)
        if (families <= FAMILIES_BAND1_MAX) {
            score += POINTS_FAMILIES_BAND1;
        } else if (families <= FAMILIES_BAND2_MAX) {
            score += POINTS_FAMILIES_BAND2;
        } else {
            score += POINTS_FAMILIES_BAND3;
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

        // FIND T THE RELIEF LEVEL THE SCORE EARNS
        //      (checked from the highest limit down, first match wins)
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

        //  CHECK THE STOCK
        //  Start at the earned level. If the stock cannot cover it,
        //  step DOWN one level at a time until something fits.
        //  DEFER is the last step and is always allowed.
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
        int packsBefore = packsLeft;
        int staffBefore = staffLeft;

        // SUBTRACT WHAT WAS USED FROM THE STOCK
        //  The NEXT request will see the new values.
        trucksLeft -= trucksUsed;
        packsLeft  -= packsUsed;
        staffLeft  -= staffUsed;

        // UPDATE THE SUMMARY COUNTERS
        if (givenLevel == DEFER) {
            deferred++;                    // got nothing
        } else if (givenLevel == wantedLevel) {
            servedFully++;                 // got what the score earned
        } else {
            servedReduced++;               // got a smaller level
        }

        // SHOW THE REQUEST DETAILS
        cout << " REQUEST #" << requestCount << " : " << areaName << endl;

        cout << " Area type : ";
        switch (areaType) {
            case AREA_PLAIN: cout << "Plain";       break;
            case AREA_MOUNTAINOUS: cout << "Mountainous"; break;
            case AREA_COASTAL: cout << "Coastal";     break;
        }
        cout << endl;
        cout << " Affected families    : " << families << endl;

        // (condition ? textIfTrue : textIfFalse) is the short if-else
        cout << " Medical aid needed : " << (medicalAid ? "Yes" : "No") << endl;
        cout << " Road to the area : " << (routeOpen ? "Open" : "Blocked") << endl;
        cout << " Aftershocks expected : " << (aftershocks ? "Yes" : "No") << endl;

        cout << " Damage level         : ";
        switch (damage) {
            case DAMAGE_LOW: cout << "Low";    break;
            case DAMAGE_MEDIUM: cout << "Medium"; break;
            case DAMAGE_HIGH: cout << "High";   break;
        }
        cout << endl;

        cout << " Aid already received : " << (aidReceived ? "Yes" : "No") << endl;

        // SHOW THE DECISION
        cout << "------------------------------------------------------------\n";
        cout << " NEED SCORE           : " << score << " / " << MAX_SCORE << endl;
        cout << " Level this score earns: " << wantedName << endl;

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
            cout << endl;
            cout << " >> Stepped down from " << wantedName << " to " << givenName << endl;
        } else if (givenLevel == DEFER) {
            // The score itself was low; this has nothing to do with the stock.
            cout << " >> Reason       : need score is below " << THRESHOLD_LOW
                 << " (low priority)\n";
        }

        cout << " ACTION TAKEN : " << givenName << endl;
        cout << " Trucks used : " << trucksUsed << endl;
        cout << " Supply packs used : " << packsUsed << endl;
        cout << " Medical staff used : " << staffUsed << endl;
        cout << "------------------------------------------------------------\n";
        cout << " REMAINING STOCK  ->  Trucks: " << trucksLeft
             << "\tPacks: " << packsLeft
             << "\tStaff: " << staffLeft << endl;

        // ASK WHETHER TO CONTINUE
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
                cout << "Please type y or n.\n";
            }
        }
    }

    //FINAL SUMMARY
    int handled = servedFully + servedReduced;   // requests that got some relief

    cout << "                        SUMMARY\n";
    cout << " Total requests processed : " << requestCount << endl;
    cout << " Served at the level they earned : " << servedFully << endl;
    cout << " Served with REDUCED relief : " << servedReduced << endl;
    cout << " Deferred / could not be served : " << deferred << endl;
    cout << " Successfully handled (got relief) : " << handled << " of " << requestCount << endl;
    cout << "------------------------------------------------------------\n";
    cout << " Stock left at the end ->\tTrucks: " << trucksLeft
         << "\tPacks: " << packsLeft << "\tStaff: " << staffLeft << endl;
    return 0;   // 0 = finished without errors
}
