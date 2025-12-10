/*
 Kaun Banega Crorepati?  — Console C Program
 ---------------------------------------------------
 This program simulates the popular quiz game "Kaun Banega Crorepati?"
 in a console environment using C.

 The player answers 12 multiple-choice questions, can use lifelines, and can
 walk away with guaranteed money.

 Author:B.Siva Shasank
 Compile: gcc kaun_banega_crorepati.c -o kaun_banega_crorepati
 Run: ./kaun_banega_crorepati
*/

#include <stdio.h>      // for printf, scanf, fgets
#include <stdlib.h>     // for rand, srand
#include <string.h>     // for string operations
#include <time.h>       // for time(), used to seed RNG
#include <ctype.h>      // for isspace(), toupper()

#define MAX_Q 12        // total number of questions in the game
#define BUF 128         // buffer size for input reading

/*
 Structure representing one question.
 Includes:
 - question text
 - 4 options (A, B, C, D)
 - index of correct answer (0 to 3)
 - prize money for answering correctly
*/
typedef struct {
    char question[256];
    char option[4][128];
    int correct;
    int prize;
} Question;

/*
 flush_stdin()
 ----------------
 Clears any remaining characters in the input buffer.
 This avoids issues when fgets or scanf leave trailing characters.
*/
void flush_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

/*
 get_choice_line()
 ------------------
 Reads an entire line of input using fgets().

 Returns:
 - the first non-space character, converted to uppercase.
 - '\0' if input fails.

 Purpose:
 Ensures robust input even if user types extra spaces or characters.
*/
char get_choice_line() {
    char buf[BUF];

    // Read a full line
    if (!fgets(buf, sizeof(buf), stdin)) return '\0';

    // Scan string for first non-space character
    for (int i = 0; buf[i]; ++i) {
        if (!isspace((unsigned char)buf[i])) {
            return toupper((unsigned char)buf[i]);
        }
    }
    return '\0';
}

/*
 show_question()
 ----------------
 Displays the question text and each of the four answer options.

 Parameters:
 - q       → pointer to the Question struct to display
 - shown[] → array of 4 integers (1 = visible option, 0 = hidden option)

 This allows the 50:50 lifeline to hide two incorrect options.
*/
void show_question(const Question *q, int shown[4]) {
    printf("\nQuestion for Rs %d:\n", q->prize);
    printf("%s\n", q->question);

    char labels[4] = {'A','B','C','D'};

    // Print each option only if it's marked as visible
    for (int i = 0; i < 4; ++i) {
        if (shown[i])
            printf("  %c) %s\n", labels[i], q->option[i]);
        else
            printf("  %c) ---\n", labels[i]); // hidden option
    }
}

/* ===========================================================
   LIFELINES
   =========================================================== */

/*
 lifeline_5050()
 ----------------
 Implements the 50:50 lifeline by hiding two incorrect options.

 Procedure:
 - Identify all wrong answers.
 - Randomly shuffle them.
 - Hide the first two wrong answers.
*/
void lifeline_5050(const Question *q, int shown[4]) {

    int wrongs[3];  // indices of wrong answers
    int wcount = 0;

    // Collect all indices except the correct one
    for (int i = 0; i < 4; ++i)
        if (i != q->correct)
            wrongs[wcount++] = i;

    // Random shuffle of wrong answer indices
    for (int i = 0; i < 3; ++i) {
        int r = rand() % 3;
        int tmp = wrongs[i];
        wrongs[i] = wrongs[r];
        wrongs[r] = tmp;
    }

    // Hide two of the wrong answers
    shown[wrongs[0]] = 0;
    shown[wrongs[1]] = 0;

    printf("50:50 lifeline used. Two wrong answers removed.\n");
}

/*
 lifeline_phone()
 -----------------
 Implements the "Phone a Friend" lifeline.

 Logic:
 - 70% chance friend gives the correct answer.
 - 30% chance friend picks a random visible wrong answer.
 - If no other options available, defaults to correct answer.
*/
void lifeline_phone(const Question *q, int shown[4]) {

    int chance = rand() % 100;
    int suggested;

    if (chance < 70) {
        // Friend likely knows the correct answer
        suggested = q->correct;
    } else {
        // Select a random wrong but still-shown option
        int choices[4], c = 0;

        for (int i = 0; i < 4; ++i)
            if (i != q->correct && shown[i])
                choices[c++] = i;

        // If no wrong visible options remain (rare in 50:50)
        if (c == 0)
            suggested = q->correct;
        else
            suggested = choices[rand() % c];
    }

    printf("You called a friend. They think the answer is: %c\n",
           'A' + suggested);
}

/*
 lifeline_audience()
 ---------------------
 Implements the "Ask the Audience" lifeline.

 Logic:
 - Every shown option gets a base 20%.
 - Correct answer receives a bonus (bias).
 - Remaining percentages are distributed randomly among visible options.
*/
void lifeline_audience(const Question *q, int shown[4]) {

    int perc[4] = {0,0,0,0};   // percentage per option
    int base = 20;             // baseline value
    int remaining = 100;

    int shownCount = 0;
    for (int i = 0; i < 4; ++i)
        if (shown[i])
            shownCount++;

    // Assign base percentage to each visible option
    for (int i = 0; i < 4; ++i) {
        if (shown[i]) {
            perc[i] = base;
            remaining -= base;
        }
    }

    // Add bonus bias to correct answer
    int bonus = 50;  // audience strongly favors correct option
    perc[q->correct] += bonus;
    remaining -= bonus;

    // Distribute remaining percentages
    while (remaining > 0) {
        int i = rand() % 4;
        if (shown[i]) {
            perc[i]++;
            remaining--;
        }
    }

    // Display results
    printf("Audience poll results (percent):\n");
    for (int i = 0; i < 4; ++i) {
        if (shown[i])
            printf("  %c: %d%%\n", 'A'+i, perc[i]);
        else
            printf("  %c: --\n", 'A'+i);
    }
}

/* ===========================================================
   MAIN GAME LOOP
   =========================================================== */

int main() {

    srand((unsigned int)time(NULL));  // seed RNG for randomness

    /*
     Initialize all questions.
     Each entry includes:
     - question text
     - 4 options
     - correct answer index
     - prize value
    */
    Question qlist[MAX_Q] = {
        {"What is the capital of India?", {"New Delhi","Mumbai","Chennai","Kolkata"}, 0, 1000},
        {"Which language is primarily used to write Linux kernel?", {"Python","C","Java","Ruby"}, 1, 2000},
        {"Who wrote the play 'Romeo and Juliet'?", {"Charles Dickens","William Shakespeare","Mark Twain","Leo Tolstoy"}, 1, 3000},
        {"Which planet is known as the Red Planet?", {"Venus","Saturn","Mars","Jupiter"}, 2, 5000},
        {"Which data structure uses LIFO?", {"Queue","Stack","Heap","Graph"}, 1, 10000},
        {"In C, which header is needed for printf?", {"stdlib.h","stdio.h","string.h","math.h"}, 1, 20000},
        {"Which element has chemical symbol 'O'?", {"Gold","Oxygen","Silver","Iron"}, 1, 40000},
        {"Which company created the iPhone?", {"Google","Microsoft","Apple","Samsung"}, 2, 80000},
        {"Which is the smallest prime number?", {"1","2","3","0"}, 1, 160000},
        {"Which year did the World Wide Web become publicly available?", {"1983","1989","1991","1995"}, 2, 320000},
        {"Which mathematician is known for the three laws of motion?", {"Newton","Einstein","Gauss","Euler"}, 0, 640000},
        {"Which programming paradigm uses objects and classes?", {"Procedural","Functional","Object-Oriented","Logical"}, 2, 1000000}
    };

    int total_questions = 12;

    // Flags: each becomes 1 once used
    int lifeline_5050_used = 0;
    int lifeline_phone_used = 0;
    int lifeline_audience_used = 0;

    int guaranteed_money = 0;  // minimum money the user keeps after milestones
    int current_prize = 0;     // amount won so far

    printf("Welcome to Who Wants to Be a Millionaire? (C Console Version)\n");
    printf("Rules: Type A/B/C/D to answer. Type L to use a lifeline. Type Q to walk away.\n");

    /*
     Loop through each question one by one.
     idx = current question number.
    */
    for (int idx = 0; idx < total_questions; ++idx) {

        Question *q = &qlist[idx];
        int shown[4] = {1,1,1,1};   // All options initially visible
        int answered = 0;           // becomes 1 when player answers correctly

        while (!answered) {

            // show full question & visible choices
            show_question(q, shown);

            printf("\nEnter your choice (A/B/C/D), L for lifeline, Q to quit and take winnings: ");
            char ch = get_choice_line();   // read user input safely

            if (ch == '\0') {
                printf("Invalid input. Try again.\n");
                continue;
            }

            /* ---------------------------------------------------
               User chooses to quit and walk away
               --------------------------------------------------- */
            if (ch == 'Q') {
                printf("You chose to walk away with Rs %d. Congratulations!\n",
                       current_prize);
                return 0;
            }

            /* ---------------------------------------------------
               Lifeline handling
               --------------------------------------------------- */
            else if (ch == 'L') {

                printf("\nAvailable lifelines:\n");
                if (!lifeline_5050_used)     printf("  1) 50:50\n");
                if (!lifeline_phone_used)    printf("  2) Phone a friend\n");
                if (!lifeline_audience_used) printf("  3) Ask the audience\n");

                printf("Select lifeline number (or any other key to cancel): ");
                char pick = get_choice_line();

                // Apply chosen lifeline
                if (pick == '1' && !lifeline_5050_used) {
                    lifeline_5050_used = 1;
                    lifeline_5050(q, shown);
                }
                else if (pick == '2' && !lifeline_phone_used) {
                    lifeline_phone_used = 1;
                    lifeline_phone(q, shown);
                }
                else if (pick == '3' && !lifeline_audience_used) {
                    lifeline_audience_used = 1;
                    lifeline_audience(q, shown);
                }
                else {
                    printf("No lifeline used.\n");
                }

                continue;  // re-display question after lifeline
            }

            /* ---------------------------------------------------
               Player answers a question
               --------------------------------------------------- */
            else if (ch >= 'A' && ch <= 'D') {

                int ans = ch - 'A';  // convert A-D → 0-3

                // If option was removed by 50:50
                if (!shown[ans]) {
                    printf("That option is not available. Choose a shown option.\n");
                    continue;
                }

                // Correct answer
                if (ans == q->correct) {

                    current_prize = q->prize;

                    printf("Correct! You have won Rs %d.\n", current_prize);

                    // Update guaranteed amounts (milestones)
                    if (idx == 2) guaranteed_money = q->prize;
                    if (idx == 6) guaranteed_money = q->prize;

                    answered = 1; // proceed to next question
                }

                // Wrong answer
                else {

                    printf("Wrong answer. The correct answer was %c) %s\n",
                           'A' + q->correct,
                           q->option[q->correct]);

                    printf("You leave with Rs %d (guaranteed).\n",
                           guaranteed_money);

                    return 0; // game ends on wrong answer
                }
            }

            else {
                printf("Invalid choice. Use A/B/C/D, L, or Q.\n");
            }
        } // end question loop
    }

    // If all questions answered correctly
    printf("\nCONGRATULATIONS! You answered all questions and won Rs %d!\n",
           current_prize);

    return 0;
}