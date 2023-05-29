// ComputerGuessMyNumber.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <random>
#include <ctime>
#include <chrono>
#include <thread>

namespace settings {
	const int lowRangeVal = 1, 
			  highRangeVal = 100; //low and high value of number range 
	const int outputDelay = 1; //in seconds
}

/// <summary>
/// ask user for a number in a range
/// </summary>
/// <param name="text">instruction or question for user asking for number</param>
/// <param name="lowVal">low value of the range</param>
/// <param name="highVal">high value of the range</param>
/// <returns>number between range</returns>
int askNumber(std::string text, int lowVal, int highVal);

int main()
{
	const int lowVal = settings::lowRangeVal, highVal = settings::highRangeVal; 
	int tries = 0;
	int secretNumber = 0;
	int guess = 0;
	
	int lowGuessVal = lowVal, 
		highGuessVal = highVal; //known low and high value by computer 

	int midProbability = 0; //probability for choosing midrange guess

	int rngNum = 0;

	std::cout << "\tWelcome to Computer Guess My Number\n\n";

	secretNumber = askNumber("Enter a number for the computer to guess", lowVal, highVal);

	//===== generator for normal distribution rng =====
	std::default_random_engine generator;
	generator.seed(static_cast<unsigned int>(time(0))); //seeding the generator
	//=================================================

	//computer guess the number
	do
	{
		//set the probability value
		if (tries == 0)
			midProbability = 90;
		else if (tries == 1)
			midProbability = 50;

		//get the number for probability
		std::uniform_int_distribution<> uniDist(1, 100);
		rngNum = uniDist(generator);

		std::cout << "Guessing...\n";

		if (rngNum <= midProbability)
		{
			//================== using midrange value  ==================
			guess = static_cast<int>((lowGuessVal + highGuessVal) / 2);
			//=============================================================
		}
		else {
			//============================ using normal distribution rng ============================
			// using normal distribution so guess is not the same all the time
			// while ensuring higher chance of picking value around the midrange 
				std::normal_distribution<double> normalDist(((lowGuessVal + highGuessVal) / 2), 2.0);
				do
				{
					guess = static_cast<int>(normalDist(generator));
				} while (guess < lowGuessVal || guess > highGuessVal);
			//=======================================================================================
		}

		//delay the output to simulate thinking
		std::this_thread::sleep_for(std::chrono::seconds(settings::outputDelay));

		tries++;
		std::cout << guess << ".\n";
		if (guess < secretNumber)
		{
			lowGuessVal = guess + 1; //set guess number + 1 as new low range value
			std::cout << "Too Low.\n\n";
		}
		else if (guess > secretNumber)
		{
			highGuessVal = guess - 1; //set guess number - 1 as new high range value
			std::cout << "Too High.\n\n";
		}
		else {
			std::cout << "Guessed it. Got it in " << tries << " tries\n\n";
		}
	} while (guess != secretNumber);

	std::cout << "Press enter to close the program.";

	std::cin.ignore();
	std::cin.get();
}

int askNumber(std::string text, int lowVal, int highVal) {
	int answer = 0;
	do
	{
		std::cout << text << " between " << lowVal << "-" << highVal << ": ";
		std::cin >> answer;
		//user gave non-numeric input
		if (std::cin.fail()) {
			std::cout << "Please input numbers only!\n\n";
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		//user input number outside the range 
		else if (answer < lowVal || answer > highVal)
		{
			std::cout << "Please input number between " << lowVal << "-" << highVal << "!\n\n";
		}
	} while (std::cin.fail() || answer < lowVal || answer > highVal);

	return answer;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
