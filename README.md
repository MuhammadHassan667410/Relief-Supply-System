# EARTHQUAKE RELIEF SUPPLY SYSTEM

## INTRODUCTION OF THE SYSTEM

This program is built to give relief supply to earthquake-affected areas. Due to very limited resources, we had to be very careful in building the system and managing the supplies, so the system is intelligent enough to determine how deserving the area is of the relief need and how much supply should be given to that area. We built a need score calculation system, so we can calculate the need score according to the user’s request and give the resources according to that need score.

## CODE and LOGIC EXPLANATION

So the code starts by defining all the constants and variables that will be used again and again in the program. If we somehow decide to change some values or need score logic, we can easily change it here without touching anything else and without making a lot of changes.

After that, we printed the instructions to show the user how this system works, how to use the system, how it calculates the need score and which factors have more effect.

After it, the system prints the initial values of trucks, supply packs and medical staff. Then it asks the user to input area name, area type, number of affected families, the need for medical aid, if the roads are blocked or not, if there is a danger of aftershocks or not and if the aid is already given before or not.

**Working of points and need score calculation:** Now, the logic of giving points is described below.

- If the area type is plain, we gave +10 points and +15 for mountainous or coastal, as there is more damage of the earthquake in mountainous or coastal than in the plain area.
- If the number of affected families is between 0-20, we gave +5 points and +10 and +15 for 20 – 50 and 50+ respectively because the more the families, the more the need for help.
- If medical aid is needed, we gave +15 points and +5 points if not needed.
- If the route is open, we gave +10 points and if the route is not open/damaged, in which case we cannot deliver them the supplies, we gave -15 points as, if we cannot give them help due to road blockage, we should focus on the ones we can easily give, and when the roads are good to go, then we can give them relief.
- If aftershocks are expected, we gave +10 points and if not, we gave +15 points because if there are expected aftershocks, there is a danger of losing vulnerable supplies, so the areas with less expectation of aftershocks are given priority.
- For the damage level, we gave +5, +10 and +15 points for low, medium and high damage level respectively, as the more the damage level, the more the need for the relief.
- If the aid is given before, the area will be given fewer points of -20 and less priority, as we need to give more priority to the areas that have not received aid before, which is +10.

Now, the points are calculated as stated; now the maximum and minimum range of need score is set to 100 and 0 respectively, so no matter how many points, they will be converted according to the range of 0-100.

**Need score and relief level:** The logic of giving the relief level according to the need score is explained below:

- Need score  90 - 100 : Full Relief  : 3 trucks, 50 supply packs, 5 medical staff
- Need score  70 -  89 : Standard Relief  : 2 trucks, 30 supply packs, 3 medical staff
- Need score  50 -  69 : Mid-Level Relief  : 1 truck,  20 supply packs, 2 medical staff
- Need score  20 -  49 : Low-Level Relief  : 1 truck,  10 supply packs, 1 medical staff
- Need score   0 -  19 : Defer : 0 trucks,  0 supply packs, 1 medical staff

We designed the system to be such that when relief is given, it deducts that relief from the original amount of available trucks, supply packs, and medical staff, so when a new request comes, it gives the relief from the left resources.

Now, we added logic that if the resources are less than the required resources in the calculated relief level, the program drops the relief level to one less level and keeps on dropping until the resources are met according to the relief level, and if the resources are not sufficient for any relief level, then defer is selected because we do not have any resources to give.

After everything, the program prints the whole summary of the request, everything the user entered, the calculation of the need score, the need score, the selected relief level and the resources left. The output and testing files will be attached with other files.

## GROUP MEMBERS AND CONTRIBUTIONS:

We made a group of 3: Fatima Nasir (598914), Azka Qasim (587467) and Muhammad Hassan (587571).

We divided the whole project among each other. The flowchart was made by Fatima, and the code was written by Hassan and Azka. All three of us designed the whole logic of the program, the point system, need score calculation and relief levels with mutual discussion and thinking at the very start; after that, the tasks were divided between us as described, and we did our part with full effort.

GitHub repo link: <https://github.com/MuhammadHassan667410/Relief-Supply-System.git>

## USE OF AI:

As described before, the whole logic, point system, need score calculation, relief level, and the whole working of the system were designed by our mutual discussion with absolutely zero use of AI. We wrote the initial code using our logic ourselves with no use of AI, everything we knew and were taught till now in our classes and labs, which worked according to our logic, but for improvement, we asked AI to improve our code and use good programming techniques which will make our code look good and have less chance of error; the AI improved the code very much, but there was nothing much it added that we already didn’t know; it just improved the output styling and input strings, but the new thing it added was the use of cin.eof, cin.fail, cin.clear and cin.ignore, which were added in the code where we needed to ask the user to input information about their area. Then we learned their logic, why it used them and what difference they make; they were actually error-handling techniques so that the program can handle any error, for instance, if the user enters a string where he needs to input an integer, they will handle it and ask the user the same question again until the user enters a correct value instead of stopping the whole program and breaking everything. The flowchart was also built with zero help of AI. After building the complete flowchart, we verified the flowchart from AI if we built it correctly or not, and there was no error or mistake in the flowchart.