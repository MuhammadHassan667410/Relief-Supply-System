#include <iostream>
using namespace std;

int main()
{
    // Initial inventory
    int trucks = 10;
    int supplyPacks = 200;
    int medicalStaff = 20;

    char anotherRequest = 'y';

    while (anotherRequest == 'y' || anotherRequest == 'Y')
    {
        // Input variables
        string areaType;
        int affectedFamilies;
        string medicalAid;
        string roads;
        string expectedAftershocks;
        string damageLevel;
        string aidReceived;

        int needScore = 0;

        cout << "\n====================================\n";
        cout << "       EARTHQUAKE RELIEF REQUEST\n";
        cout << "====================================\n";

        // -------- INPUT --------

        cout << "\nArea type (Plain/Mountainous/Coastal): ";
        cin >> areaType;

        cout << "Affected families (0-100): ";
        cin >> affectedFamilies;

        cout << "Medical aid required? (Yes/No): ";
        cin >> medicalAid;

        cout << "Roads (Open/Blocked): ";
        cin >> roads;

        cout << "Expected aftershocks? (Yes/No): ";
        cin >> expectedAftershocks;

        cout << "Damage level (Low/Medium/High): ";
        cin >> damageLevel;

        cout << "Aid already received? (Yes/No): ";
        cin >> aidReceived;


        // -------- NEED SCORE --------

        // Area type
        if (areaType == "Plain" || areaType == "plain")
            needScore += 10;
        else if (areaType == "Mountainous" || areaType == "mountainous")
            needScore += 20;
        else if (areaType == "Coastal" || areaType == "coastal")
            needScore += 15;


        // Affected families
        if (affectedFamilies >= 0 && affectedFamilies < 20)
            needScore += 5;
        else if (affectedFamilies >= 20 && affectedFamilies < 50)
            needScore += 15;
        else if (affectedFamilies >= 50 && affectedFamilies <= 100)
            needScore += 10;


        // Medical aid
        if (medicalAid == "Yes" || medicalAid == "yes")
            needScore += 10;
        else
            needScore += 5;


        // Roads
        if (roads == "Open" || roads == "open")
            needScore += 10;
        else
            needScore -= 15;


        // Expected aftershocks
        if (expectedAftershocks == "Yes" ||
            expectedAftershocks == "yes")
            needScore += 5;
        else
            needScore += 15;


        // Damage level
        if (damageLevel == "Low" || damageLevel == "low")
            needScore += 5;
        else if (damageLevel == "Medium" || damageLevel == "medium")
            needScore += 10;
        else if (damageLevel == "High" || damageLevel == "high")
            needScore += 20;


        // Aid received
        if (aidReceived == "Yes" || aidReceived == "yes")
            needScore -= 20;
        else
            needScore += 10;


        // Keep score between 0 and 100
        if (needScore < 0)
            needScore = 0;

        if (needScore > 100)
            needScore = 100;


        // -------- RELIEF REQUIREMENT --------

        int requiredTrucks = 0;
        int requiredPacks = 0;
        int requiredMedical = 0;

        if (needScore >= 90)
        {
            cout << "\nRelief Level: FULL RELIEF\n";

            requiredTrucks = 3;
            requiredPacks = 50;
            requiredMedical = 5;
        }
        else if (needScore >= 70)
        {
            cout << "\nRelief Level: STANDARD RELIEF\n";

            requiredTrucks = 2;
            requiredPacks = 30;
            requiredMedical = 3;
        }
        else if (needScore >= 50)
        {
            cout << "\nRelief Level: MID-LEVEL RELIEF\n";

            requiredTrucks = 1;
            requiredPacks = 20;
            requiredMedical = 2;
        }
        else if (needScore >= 20)
        {
            cout << "\nRelief Level: LOW-LEVEL RELIEF\n";

            requiredTrucks = 1;
            requiredPacks = 10;
            requiredMedical = 1;
        }
        else
        {
            cout << "\nRelief Level: MINIMAL RELIEF\n";

            requiredTrucks = 0;
            requiredPacks = 0;
            requiredMedical = 1;
        }


        cout << "Need Score: " << needScore << "/100\n";

        cout << "\nRequested resources:\n";
        cout << "Trucks: " << requiredTrucks << endl;
        cout << "Supply Packs: " << requiredPacks << endl;
        cout << "Medical Staff: " << requiredMedical << endl;


        // -------- CHECK INVENTORY --------

        if (requiredTrucks > trucks ||
            requiredPacks > supplyPacks ||
            requiredMedical > medicalStaff)
        {
            cout << "\n*** INSUFFICIENT INVENTORY ***\n";
            cout << "Relief cannot be provided for this request.\n";
            cout << "Program stopped.\n";

            break;
        }


        // -------- PROVIDE RELIEF --------

        trucks -= requiredTrucks;
        supplyPacks -= requiredPacks;
        medicalStaff -= requiredMedical;

        cout << "\n*** RELIEF PROVIDED SUCCESSFULLY ***\n";

        cout << "\nRemaining Inventory:\n";
        cout << "Trucks: " << trucks << endl;
        cout << "Supply Packs: " << supplyPacks << endl;
        cout << "Medical Staff: " << medicalStaff << endl;


        // -------- CHECK IF INVENTORY EMPTY --------

        if (trucks == 0 && supplyPacks == 0 && medicalStaff == 0)
        {
            cout << "\n*** INVENTORY IS EMPTY ***\n";
            cout << "No more relief can be provided.\n";
            break;
        }


        // Ask for another request
        cout << "\nDo you want to process another request? (Y/N): ";
        cin >> anotherRequest;
    }

    cout << "\n====================================\n";
    cout << "       FINAL INVENTORY\n";
    cout << "====================================\n";

    cout << "Trucks remaining: " << trucks << endl;
    cout << "Supply packs remaining: " << supplyPacks << endl;
    cout << "Medical staff remaining: " << medicalStaff << endl;

    return 0;
}