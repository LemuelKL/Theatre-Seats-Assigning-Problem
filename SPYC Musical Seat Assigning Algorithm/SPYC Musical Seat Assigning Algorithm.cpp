// SPYC Musical Seat Assigning Algorithm.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <vector>
#include <stdio.h>
#include <ctime>
#include <algorithm>
#include <windows.h>
#include <string>
#include "console_ui.h"
#include <stdlib.h>
#include <random>

int NPEOPLE_IN_THEATRE = 500;
int NPEOPLE_IN_THEATRE_WITH_GROUP = 400;
int NPEOPLE_IN_THEATRE_WITH_NO_GROUP = 100;

int POPULATION_IN_GENERATION = 1000;

int INCREMENT_HAPPINESS = 1;

int TOURNAMENT_SELECTION_TOURNAMENT_SIZE = 300;

double CROSSOVER_PROBABILITY_NPOINTS = 0.989;
double MUTATION_RATE = 0.005;

bool prob_true(double p)
{
	return rand() < p * (RAND_MAX + 1.0);
}

void gotoxy(short x, short y)
{
	COORD coord = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

class PERSON	// Gene
{
private:
	int mId;
	int mGroupId;
	int mHappyIndex;

public:
	void SetId(int id)
	{
		mId = id;
	}
	PERSON(void)
	{
		mId = -1;
		SetGroupId(-1);
		SetHappyIndex(-1);
	}
	PERSON(int pId, int gId)
	{
		mId = pId;
		SetGroupId(gId);
		SetHappyIndex(-1);
	}
	int GetGroupId()
	{
		return mGroupId;
	}
	void SetGroupId(int gId)
	{
		mGroupId = gId;
	}
	int GetHappyIndex()
	{
		return mHappyIndex;
	}
	void SetHappyIndex(int hIdx)
	{
		mHappyIndex = hIdx;
	}
	int GetId()
	{
		return mId;
	}
	bool hasGroup()
	{
		if (mGroupId == -1)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	bool IfIsInGroup(int gId)
	{
		if (gId == -1)
			return false;
		if (gId == mGroupId)
			return true;
		return false;
	}
};

class THEATRE	// Chromosome
{
private:
	int mId;
	std::vector<std::vector<PERSON> > mSeats;
	int mFitness;
public:
	static int mGlobalId;
	void Display()
	{
		std::string outStr("");
		int i = 0;
		for (auto& row : mSeats) {
			for (auto& p : row) {
				i++;
				int groupId = p.GetGroupId();
				
				if (std::to_string(groupId).length() > 1)
				{
					outStr += "[" + std::to_string(groupId) + "] ";
				}
				else
				{
					outStr += "[ " + std::to_string(groupId) + "] ";
				}
				if (i % 25 == 0)
					outStr += "\n";
			}
		}
		std::cout << outStr;
	}
	std::vector<PERSON> GetRaw1DSeats()
	{
		std::vector<PERSON> ret;
		for (auto& row : mSeats) {
			for (auto& p : row) {
				ret.push_back(p);
			}
		}
		return ret;
	}
	void SetRaw1DSeats(std::vector<PERSON> persons)
	{
		int i = 0;
		for (auto& row : mSeats) {
			for (auto& p : row) {
				p = persons.at(i);
				i++;
			}
		}
	}
	int Fitness()	// Only return, no calculation done.
	{
		return mFitness;
	}
	int GetId()
	{
		return mId;
	}
	int GetNSeats()
	{
		return mSeats.size();
	}
	PERSON GetSeatPerson(int row, int col)
	{
		return mSeats[row][col];
	}
	int GetFitness()
	{
		CalculateFitness();
		return mFitness;
	}
	void SetSeatPerson(int row, int col, PERSON p)
	{
		mSeats[row][col] = p;
	}
	void GetPersonSeat(int &retRow, int &retCol, int PERSON_id)
	{
		int iRow = 0;
		int iCol = 0;
		for (auto& row : mSeats) {
			iCol = 0;
			for (auto& p : row) {
				if (p.GetId() == PERSON_id)
				{
					retRow = iRow;
					retCol = iCol;
					return;
				}
				iCol++;
			}
			iRow++;
		}
	}
	int GetPersonSeatRow(int PERSON_id)
	{
		int iRow = 0;
		for (auto& row : mSeats) {
			for (auto& p : row) {
				if (p.GetId() == PERSON_id)
				{
					return iRow;
				}
			}
			iRow++;
		}
	}
	int GetPersonSeatCol(int PERSON_id)
	{
		int iCol = 0;
		for (auto& row : mSeats) {
			iCol = 0;
			for (auto& p : row) {
				if (p.GetId() == PERSON_id)
				{
					return iCol;
				}
				iCol++;
			}
		}
	}
	void InitFirstPopulation(std::vector<PERSON> PeopleToBeAssigned)
	{
		int i = 0;
		int iRow = 0;
		int iCol = 0;
		for (auto& row : mSeats) {
			iCol = 0;
			for (auto& p : row) {
				p = PeopleToBeAssigned[i];
				iCol++;
				i++;
			}
			iRow++;
		}
		ShuffleSwap();
	}
	void ShuffleSwap()
	{
		PERSON tPerson;
		int rRow1, rRow2;
		int rCol1, rCol2;
		int i;
		for (i = 0; i < 5000; i++)
		{
			rRow1 = rand() % 20;
			rCol1 = rand() % 25;
			rRow2 = rand() % 20;
			rCol2 = rand() % 25;
			tPerson = mSeats[rRow1][rCol1];
			mSeats[rRow1][rCol1] = mSeats[rRow2][rCol2];
			mSeats[rRow2][rCol2] = tPerson;
		}
	}
	void CalculateFitness()
	{
		mFitness = 0;
		for (auto& row : mSeats)
		{
			for (auto& p : row)
			{
				if (p.GetGroupId() != -1)
				{
					PERSON neighborL;
					neighborL.SetId(-2);
					PERSON neighborR;
					neighborR.SetId(-2);

					if (GetPersonSeatCol(p.GetId()) != 0) {
						neighborL = GetSeatPerson(GetPersonSeatRow(p.GetId()), GetPersonSeatCol(p.GetId()) - 1);
					}
					if (GetPersonSeatCol(p.GetId()) != 24)
					{
						neighborR = GetSeatPerson(GetPersonSeatRow(p.GetId()), GetPersonSeatCol(p.GetId()) + 1);
					}

					int PersonHappiness = 0;

					if (neighborL.GetId() != -2 && neighborL.IfIsInGroup(p.GetGroupId()))
					{
						PersonHappiness = PersonHappiness + INCREMENT_HAPPINESS;
					}
					if (neighborR.GetId() != -2 && neighborR.IfIsInGroup(p.GetGroupId()))
					{
						PersonHappiness = PersonHappiness + INCREMENT_HAPPINESS;
					}

					p.SetHappyIndex(PersonHappiness);
					mFitness = mFitness + p.GetHappyIndex();
				}
			}
		}
	}
	void AssignID()
	{
		mId = ++mGlobalId;
	}

	THEATRE(int id = -1)
	{
		/*

		25 * 20 seats
		STAGE RIGHT                            STAGE LEFT

		+ + + + + + + + + + + + + + + + + + + + + + + + +
		+ + + + + + + + + + + + + + + + + + + +	+ + + + +
		+ + + + + + + + + + + + + + + + + + + +	+ + + + +
		+ + + + + + + + + + + + + + + + + + + +	+ + + + +
		+ + + + + + + + + + + + + + + + + + + +	+ + + + +
		+ + + + + + + + + + + + + + + + + + + +	+ + + + +
		+ + + + + + + + + + + + + + + + + + + +	+ + + + +
		+ + + + + + + + + + + + + + + + + + + +	+ + + + +
		+ + + + + + + + + + + + + + + + + + + +	+ + + + +
		+ + + + + + + + + + + + + + + + + + + +	+ + + + +
		+ + + + + + + + + + + + + + + + + + + +	+ + + + +
		+ + + + + + + + + + + + + + + + + + + +	+ + + + +
		+ + + + + + + + + + + + + + + + + + + +	+ + + + +
		+ + + + + + + + + + + + + + + + + + + +	+ + + + +
		+ + + + + + + + + + + + + + + + + + + +	+ + + + +
		+ + + + + + + + + + + + + + + + + + + +	+ + + + +
		+ + + + + + + + + + + + + + + + + + + +	+ + + + +
		+ + + + + + + + + + + + + + + + + + + +	+ + + + +
		+ + + + + + + + + + + + + + + + + + + +	+ + + + +
		+ + + + + + + + + + + + + + + + + + + +	+ + + + +

		*/

		mId = ++mGlobalId;
		std::vector<std::vector<PERSON>> v(20, std::vector<PERSON>(25, PERSON()));
		mSeats = v;
	}
	THEATRE(std::vector<PERSON> wunD_people)	// Dump in 1D offspring
	{
		if (wunD_people.size() != 500)	// constant
		{
			std::cout << "NOT ENOUGH PEOPLE" << std::endl;
		}
		std::vector<std::vector<PERSON>> v(20, std::vector<PERSON>(25, PERSON()));
		mSeats = v;
		int i = 0;
		for (auto& row : mSeats) 
		{
			for (auto& p : row) 
			{
				p = wunD_people.at(i);
				i++;
			}
		}
	}
};
int THEATRE::mGlobalId = 0;

class GENERATION	// Generation
{
private:
	int mGenerationId;
	int mSize;
	std::vector<THEATRE> mHalls;
	std::vector<THEATRE> mHalls_Parent;
	std::vector<THEATRE> mHalls_Offspring;

	std::vector<PERSON> vecncp(std::vector<PERSON> dest, std::vector<PERSON> scr, int sPos)
	{
		/*
		std::cout << "dest.size(): " << dest.size() << std::endl;
		std::cout << "scr.size(): " << scr.size() << std::endl;
		std::cout << "sPos: " << sPos << std::endl;
		*/
		std::vector<PERSON> ret = dest;
		int i;
		for (i = 0; i < scr.size(); i++)
		{
			ret.at(sPos+i) = scr.at(i);
		}
		return ret;
	}
public:
	void SelectParentsForInternal()
	{
		std::cout << "[ Roulette Wheel Selection ]: " << std::endl;
		for (int i = 0; i < mHalls.size()/2; i++)
		{
			THEATRE selected;
			selected = RouletteWheelSelection();
			mHalls_Parent.push_back(selected);
			//std::cout << "Theatre ID: " << selected.GetId() << "\tFitness: " << selected.Fitness() << std::endl;
		}
	}
	void SelectParentsForInternal_Tournament()
	{
		std::cout << "[ Tournament Selection ]: " << std::endl;
		for (int i = 0; i < mHalls.size() / 2; i++)
		{
			THEATRE selected;
			selected = TournamentSelection();
			mHalls_Parent.push_back(selected);
			//std::cout << "Theatre ID: " << selected.GetId() << "\tFitness: " << selected.Fitness() << std::endl;
		}
	}
	GENERATION(void)
	{
		// Do Nothing;
	}
	GENERATION(int id, std::vector<THEATRE> halls)
	{
		mGenerationId = id;
		mHalls = halls;
		mSize = mHalls.size();
	}
	void MutateOffsprings()
	{
		int nToMutate = MUTATION_RATE * mHalls_Offspring.size();
		int i, which;
		std::vector<int> iMutated;
		std::vector<THEATRE> HallsToMutate;
		for (i = 0; i < nToMutate; i++)
		{
			which = rand() % mHalls_Offspring.size();
			HallsToMutate.push_back(mHalls_Offspring.at(which));
			iMutated.push_back(which);
		}

		PERSON temp;
		int rPos1, rPos2;
		for (i = 0; i< HallsToMutate.size(); i++)
		{
			std::vector<PERSON> seats = HallsToMutate.at(i).GetRaw1DSeats();
			rPos1 = rand() % seats.size();
			rPos2 = rand() % seats.size();
			temp = seats.at(rPos1);
			seats.at(rPos1) = seats.at(rPos2);
			seats.at(rPos2) = temp;

			mHalls_Offspring.at(iMutated.at(i)).SetRaw1DSeats(seats);
		}

	}
	GENERATION BreedNextGeneration()
	{
		//std::cout << "START BREEDING WITH ABOVE PARENTS x " << mHalls_Parent.size() << std::endl;

		std::vector<PERSON> Seats1;
		std::vector<PERSON> Seats2;
		std::vector<PERSON> offspringSeats1;
		std::vector<PERSON> offspringSeats2;
		std::vector<PERSON> offspringSeats1LowerSwap;
		std::vector<PERSON> offspringSeats2LowerSwap;
		std::vector<PERSON> offspringSeats1UpperSwap;
		std::vector<PERSON> offspringSeats2UpperSwap;
		int i;
		int j;
		int t;
		for (j = 0; j < mHalls_Parent.size(); j += 2)
		{
			Seats1 = mHalls_Parent.at(j).GetRaw1DSeats();
			Seats2 = mHalls_Parent.at(j + 1).GetRaw1DSeats();
			offspringSeats1.clear();
			offspringSeats2.clear();
			offspringSeats1LowerSwap.clear();
			offspringSeats2LowerSwap.clear();
			offspringSeats1UpperSwap.clear();
			offspringSeats2UpperSwap.clear();
			offspringSeats1 = Seats1;
			offspringSeats2 = Seats2;
			
			int n = 1;
			bool continueDoing;
			double accumulatedProb = CROSSOVER_PROBABILITY_NPOINTS;
			do {
				continueDoing = false;
				continueDoing = prob_true(accumulatedProb);
				if (continueDoing == true)
				{
					n++;
					accumulatedProb = accumulatedProb * CROSSOVER_PROBABILITY_NPOINTS;
				}
			} while (continueDoing == true);
			
			int lowerPos;
			do {
				lowerPos = rand() % Seats1.size();
			} while (lowerPos + n >= Seats1.size() - 1);

			int upperPos = lowerPos + n;
			int swapAfterPos = (rand() % n) + lowerPos;

			int h = 0;
			for (t = lowerPos; t <= swapAfterPos; t++)
			{
				offspringSeats1LowerSwap.push_back(offspringSeats1.at(t));
				offspringSeats2LowerSwap.push_back(offspringSeats2.at(t));
				h++;
			}
			h = 0;
			for (t = swapAfterPos + 1; t <= upperPos; t++)
			{
				offspringSeats1UpperSwap.push_back(offspringSeats1.at(t));
				offspringSeats2UpperSwap.push_back(offspringSeats2.at(t));
				h++;
			}

			//std::cout << "*********** " << std::endl;

			offspringSeats1 = vecncp(offspringSeats1, offspringSeats1UpperSwap, lowerPos);
			offspringSeats1 = vecncp(offspringSeats1, offspringSeats1LowerSwap, lowerPos + offspringSeats1UpperSwap.size() );
			
			offspringSeats2 = vecncp(offspringSeats2, offspringSeats2UpperSwap, lowerPos);
			offspringSeats2 = vecncp(offspringSeats2, offspringSeats2LowerSwap, lowerPos + offspringSeats2UpperSwap.size() );
			// ???
			//std::cout << "*********** " << std::endl;

			THEATRE offspringTheatre1(offspringSeats1);
			THEATRE offspringTheatre2(offspringSeats2);
			offspringTheatre1.AssignID();
			offspringTheatre2.AssignID();

			mHalls_Offspring.push_back(offspringTheatre1);
			mHalls_Offspring.push_back(offspringTheatre2);
			mHalls_Offspring.push_back(mHalls_Parent.at(j));
			mHalls_Offspring.push_back(mHalls_Parent.at(j + 1));

		}
		// Mutate before calculating fitness or passing on to next generation
		MutateOffsprings();

		int iLargest = 0;
		mHalls_Offspring.at(iLargest).CalculateFitness();
		for (i = 1; i < mHalls_Offspring.size(); i++)
		{
			mHalls_Offspring.at(i).CalculateFitness();
			if (mHalls_Offspring.at(i).Fitness() > mHalls_Offspring.at(iLargest).Fitness())
			{
				iLargest = i;
			}
		}
		
		std::string outStr("");
		gotoxy(0, 0);
		outStr += "\n";
		outStr += "========================================================================================================================\n\n";
		outStr += "BEST Offspring From Generation: " + std::to_string(mGenerationId) + "\n";
		outStr += "THEATRE ID: " + std::to_string(mHalls_Offspring.at(iLargest).GetId()) + " Fitness: " + std::to_string(mHalls_Offspring.at(iLargest).Fitness()) + "   \t" + "\n";
		outStr += "It's index in Population: " + std::to_string(iLargest) + "   \t" + "\n";
		outStr += "\n";
		std::cout << outStr << std::endl;
		
		mHalls_Offspring.at(iLargest).Display();

		std::cout << "\n";
		std::cout << "========================================================================================================================";
		std::cout << std::endl;
		std::cout << std::endl;
		
		GENERATION NextGen(mGenerationId + 1, mHalls_Offspring);
		return NextGen;
	}
	void Init_Take(std::vector<THEATRE> halls)
	{
		mHalls = halls;
		mSize = mHalls.size();
	}
	void CalculateTheatreFitness()
	{
		int i;
		for (i = 0; i < mHalls.size(); i++)
		{
			mHalls.at(i).CalculateFitness();
		}
	}
	void InitializeFirstGeneration(int populationSize, std::vector<PERSON> PeopleToBeAssigned)
	{
		mGenerationId = 1;
		mSize = populationSize;
		int i;
		for (i = 0; i < mSize; i++)
		{
			THEATRE Hall = THEATRE();
			Hall.InitFirstPopulation(PeopleToBeAssigned);
			mHalls.push_back(Hall);
		}
	}
	int GetSize()
	{
		return mHalls.size();
	}
	std::vector<THEATRE> SelectTheFittest_GENERIC(int n)
	{
		for (int i = 0; i < mHalls.size(); i++)
		{
			mHalls.at(i).CalculateFitness();
		}
		SortByFitness();
		std::vector<THEATRE> ret = mHalls;
		ret.resize(n);
		return ret;
	}
	void Swap(THEATRE *xp, THEATRE *yp)
	{
		THEATRE temp = *xp;
		*xp = *yp;
		*yp = temp;
	}
	void SortByFitness()
	{
		int minPos;
		int i, j;
		int size = mHalls.size();
		for (i = 0; i < size - 1; i++)
		{
			for (j = 0; j < size - i - 1; j++)
			{
				if (mHalls.at(j).Fitness() < mHalls.at(j + 1).Fitness())
				{
					Swap(&mHalls.at(j + 1), &mHalls.at(j));
				}
			}
		}
	}
	std::vector<THEATRE> GetTheatres()
	{
		return mHalls;
	}
	THEATRE RouletteWheelSelection()
	{
		int i;
		int totalSum = 0;
		for (i = 0; i < mHalls.size(); i++)
		{
			totalSum += mHalls.at(i).Fitness();	// CARE use Fitness() is only a value-returning function, use GetFitness() instead if calculation of the fitness itself is desired.
		}
		//int rand = std::rand() % totalSum;	// CARE rand gives really bad numbers, really smalle in values...
		std::default_random_engine generator;
		std::uniform_int_distribution<int> distribution(0, totalSum-1);
		int random_int = distribution(generator);
		int partialSum = 0;
		for (i = 0; i < mHalls.size(); i++)
		{
			partialSum += mHalls.at(i).Fitness();	// Care
			if (partialSum >= random_int)
			{
				return mHalls.at(i);
			}
		}
	}
	THEATRE TournamentSelection()
	{
		// This functin simulates 1 Tournament and return only 1, the best participant.
		int iBest = 0;
		int i;
		std::vector<THEATRE> Participants;
		std::vector<THEATRE> Pool = mHalls;
		std::random_shuffle(Pool.begin(), Pool.end());
		for (i = 0; i < TOURNAMENT_SELECTION_TOURNAMENT_SIZE; i++)
		{
			Participants.push_back(Pool.back());
			Pool.pop_back();
		}
		for (i = 0; i < TOURNAMENT_SELECTION_TOURNAMENT_SIZE; i++)
		{
			if (Participants.at(i).Fitness() > Participants.at(iBest).Fitness())
			{
				iBest = i;
			}
		}
		return Participants.at(iBest);
	}
};

void PrintTheatre(THEATRE theatre)
{
	std::cout << "Theatre ID: " << theatre.GetId() << " Fitness: " << theatre.Fitness() << std::endl;
}

void PrintTheatres(std::vector<THEATRE> theatre)
{
	std::cout << "============================================================" << std::endl;
	int i;
	for (i = 0; i < theatre.size(); i++)
	{
		std::cout << "Theatre ID: " << theatre.at(i).GetId() << " Fitness: " << theatre.at(i).Fitness() << std::endl;
	}
	std::cout << "============================================================" << std::endl;
}

std::vector<PERSON> GenerateAudiences()
{
	std::vector<PERSON> RetPeopleToBeAssigned;
	int i, j;
	int a = 0;
	int GroupNo = 1;
	int nPeopleInGroup;

	for (i = 0; i < NPEOPLE_IN_THEATRE_WITH_GROUP; i++)
	{
		nPeopleInGroup = rand() % 9 + 2; // 2 to 10 people per a group
		i = i + nPeopleInGroup - 1;
		for (j = 0; j < nPeopleInGroup; j++)
		{
			PERSON Audience(a, GroupNo);
			RetPeopleToBeAssigned.push_back(Audience);
			a++;
		}
		GroupNo++;
	}
	RetPeopleToBeAssigned.resize(NPEOPLE_IN_THEATRE_WITH_GROUP);

	for (i = 0; i < NPEOPLE_IN_THEATRE_WITH_NO_GROUP; i++)
	{
		PERSON Audience(i + NPEOPLE_IN_THEATRE_WITH_GROUP, -1);
		RetPeopleToBeAssigned.push_back(Audience);
	}
	return RetPeopleToBeAssigned;
}

int GenerateVerifyPin(std::vector<PERSON> People)
{
	int verifyPin = 0;
	for (int i = 0; i < People.size(); i++)
	{
		verifyPin += People.at(i).GetGroupId();
	}
	return verifyPin;
}

int main()
{
	HWND hwnd = GetConsoleWindow();
	if (hwnd != NULL) { MoveWindow(hwnd, 340, 100, 1100, 768, TRUE); }

	srand((unsigned)time(0));

	std::vector<PERSON> PeopleToBeAssigned = GenerateAudiences();
	int verifyPin = GenerateVerifyPin(PeopleToBeAssigned);

	// // // // //

	GENERATION firstGen = GENERATION();
	firstGen.InitializeFirstGeneration(POPULATION_IN_GENERATION, PeopleToBeAssigned);
	firstGen.CalculateTheatreFitness();
	firstGen.SelectParentsForInternal_Tournament();
	GENERATION currGen = firstGen.BreedNextGeneration();

	GENERATION old = currGen;

	console_ui::ClearScreen();
	while (1==1)
	{
		std::cout << "PeopleToBeAssigned ID: " << verifyPin << std::endl;
		old.CalculateTheatreFitness();
		old.SelectParentsForInternal_Tournament();
		currGen = old.BreedNextGeneration();

		if (verifyPin != GenerateVerifyPin(old.GetTheatres().at(0).GetRaw1DSeats()))
		{
			std::cout << GenerateVerifyPin(old.GetTheatres().at(0).GetRaw1DSeats());
			std::cout << " DATA CORRUPTED!" << std::endl;
			return -1;
		}

		old = currGen;
	}
}
