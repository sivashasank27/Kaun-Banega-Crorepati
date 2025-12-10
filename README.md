 Overview
This project is a simple command-line simulation of the popular Indian quiz show, Kaun Banega Crorepati. The game presents players with multiple-choice questions, and for each correct answer, they win a certain amount of virtual money, progressing up the prize ladder.

The core of the project utilizes fundamental C programming concepts, including:

File Handling: To store and load questions, options, and answers.

Arrays/Structs: To manage the question data efficiently.

Control Structures (Loops and Conditionals): To handle game flow, question display, and answer validation.

 Key Features
Prize Ladder: A pre-defined structure of prize money for each question.

Question Bank: Stores multiple-choice questions with four options and one correct answer.

Lifelines (Basic Implementation): A simplified version of lifelines (e.g., 50:50).

Game Persistence (Optional): Can be extended to save high scores.

 Prerequisites
To compile and run this C program, you will need:

A C Compiler: Such as GCC (GNU Compiler Collection).

A Text Editor or IDE: For editing the source code (e.g., VS Code, Code::Blocks, or any basic text editor).

 Installation and Execution
Follow these steps to get the game running on your local machine:

1. Clone or Download the Project
Make sure you have all the source code files (e.g., kbc.c, questions.txt, etc., if applicable) in a single directory.

2. Compilation (Using GCC)
Open your terminal or command prompt, navigate to the project directory, and compile the source file(s) using the GCC compiler:

Bash

gcc kbc.c -o kbc_game
Note: kbc.c is the primary source file, and -o kbc_game creates the executable file named kbc_game.

3. Execution
Run the compiled executable from the terminal:

Bash

./kbc_game
or (on Windows)

Bash

kbc_game.exe
 How to Play
The game will start and display the first question and the four options (A, B, C, D).

Read the question carefully.

Enter the letter corresponding to your chosen answer (e.g., A, B, C, or D).

The program will inform you if your answer is correct or incorrect and update your winnings.

If correct, you proceed to the next question. If incorrect, the game ends, and your final winnings are displayed.

 Project Structure (Example)
A typical project structure for this game might look like this:

kbc-project/
├── kbc.c             # Main source code file
├── questions.txt     # File containing the list of questions and answers
└── README.md         # This file
 Contribution
This project is a great starting point for learning C! Feel free to:

Add more questions to the question bank.

Implement more lifelines (e.g., Audience Poll, Phone-a-Friend).

Enhance the User Interface (e.g., clearer text formatting, better presentation).

 Author
 B.Siva Shasank
