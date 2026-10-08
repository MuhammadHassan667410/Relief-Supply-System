#include<iostream>
#include "output.h"
using namespace std;

void printOutput() {
    cout << "EARTHQUAKE RELIEF SUPPLY SYSTEM" << endl;
    cout << "Welcome. This system distributes limited relief supplies fairly" << endl;
    cout << "by calculating a need score for each affected area." << endl;
    cout << endl;

    cout << "HOW TO USE THE SYSTEM" << endl;
    cout << "1. Enter the requested information for the affected area." << endl;
    cout << "2. Enter the population and the number of affected families." << endl;
    cout << "3. Answer each Yes/No question exactly as requested." << endl;
    cout << "4. Select the damage level (Low, Medium, or High)." << endl;
    cout << "5. The system adds the points for every answer and calculates" << endl;
    cout << "   the area's total need score." << endl;
    cout << "6. Supplies are assigned from the highest-priority areas first." << endl;
    cout << endl;

    cout << "INPUTS AND NEED-SCORE POINTS" << endl;
    cout << "Area type:          Plain = +10, Mountainous = +20," << endl;
    cout << "                    Coastal area = +15" << endl;
    cout << "Affected families:  0-20=+5, 20-50 = +10"<<endl;
    cout << "                    50+ = 10" << endl;
    cout << "Medical aid:        Yes = +10, No = +5" << endl;
    cout << "Roads:              Open = +10, Blocked = +15" << endl;
    cout << "Expected aftershock:Yes = +5, No = +15" << endl;
    cout << "Damage level:       Low = +5, Medium = +10, High = +20" << endl;
    cout << "Aid received:       Yes = -20, No = +10" << endl;
    cout << endl;

    cout << "A higher score means that the area has a greater and more urgent" << endl;
    cout << "need. A previous aid receipt reduces the score because that area" << endl;
    cout << "has already received assistance. The possible score is 0 to 200." << endl;
    cout << endl;

    cout << "SUPPLY ALLOCATION BY NEED SCORE" << endl;
    cout << "Need score 90-100 : Full relief     - 3 trucks, 50 supply packs, 5 medical staff" << endl;
    cout << "Need score 70-90  : Standard relief - 2 trucks, 30 supply packs, 3 medical staff" << endl;
    cout << "Need score 50-70  : Mid-level relief- 1 truck,  20 supply packs, 2 medical staff" << endl;
    cout << "Need score 20-50  : Low-level relief - 1 truck,  10 supply packs, 1 medical staff" << endl;
    cout << "Need score 0-20   : Defer           - 0 trucks,  0 supply packs, 1 medical staff" << endl;
    cout << endl;

    cout << "IMPORTANT NOTES" << endl;
    cout << "- Do not enter estimates that are not supported by field information." << endl;
    cout << "- Check that all answers and family counts are correct before submitting." << endl;
    cout << "- If supplies are insufficient for all areas, the highest need scores" << endl;
    cout << "  receive priority. Record deferred areas for the next distribution." << endl;
    cout << "- Confirm road access before dispatching trucks and coordinate medical" << endl;
    cout << "  staff with the receiving relief team." << endl;

}