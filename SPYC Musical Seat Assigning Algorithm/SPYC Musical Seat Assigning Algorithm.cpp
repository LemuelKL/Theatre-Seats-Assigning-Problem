// SPYC Musical Seat Assigning Algorithm.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <vector>
#include <stdio.h>
#include <ctime>
#include <algorithm>

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
		for (i = 0; i < 2500; i++)
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
					PersonHappiness = PersonHappiness + 5;
				}

				if (neighborR.GetId() != -2 && neighborR.IfIsInGroup(p.GetGroupId()))
				{
					PersonHappiness = PersonHappiness + 5;
				}

				p.SetHappyIndex(PersonHappiness);
				mFitness = mFitness + p.GetHappyIndex();
			}
		}
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

		mId = id;
		std::vector<std::vector<PERSON>> v(20, std::vector<PERSON>(25, PERSON()));
		mSeats = v;
	}
};

class GENERATION	// Generation
{
private:
	int mSize;
	std::vector<THEATRE> mHalls;

public:
	void CalculateTheatreFitness()
	{
		int i;
		for (i = 0; i < mHalls.size(); i++)
		{
			mHalls.at(i).CalculateFitness();
		}
	}
	void Initialize_Generation(int populationSize, std::vector<PERSON> PeopleToBeAssigned)
	{
		mSize = populationSize;
		int i;
		for (i = 0; i < mSize; i++)
		{
			THEATRE Hall = THEATRE(i + 1);
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
		int rand = std::rand() % totalSum;	// CARE rand gives really bad numbers, really smalle in values...
		int partialSum = 0;
		for (i = 0; i < mHalls.size(); i++)
		{
			partialSum += mHalls.at(i).Fitness();	// Care
			if (partialSum >= rand)
			{
				return mHalls.at(i);
			}
		}
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

int main()
{
	std::cout << "[ SPYC Musical Seat Assigning Algorithm ]\n";
	std::vector<PERSON> PeopleToBeAssigned;
	int i, j;
	int a = 0;
	int GroupNo = 1;
	int nPeopleInGroup;
	srand((unsigned)time(0));
	int AudienceInGroups = 400;
	for (i = 0; i < AudienceInGroups; i++)
	{
		nPeopleInGroup = rand() % 9 + 2; // 2 to 10 people per a group
		i = i + nPeopleInGroup - 1;
		for (j = 0; j < nPeopleInGroup; j++)
		{
			PERSON Audience(a, GroupNo);
			PeopleToBeAssigned.push_back(Audience);
			a++;
		}
		GroupNo++;
	}
	PeopleToBeAssigned.resize(400);

	for (i = 0; i < 100; i++)
	{
		PERSON Audience(i + 400, -1);
		PeopleToBeAssigned.push_back(Audience);
	}

	std::cout << "============================================================" << std::endl;
	GENERATION firstGen = GENERATION();
	firstGen.Initialize_Generation(100, PeopleToBeAssigned);
	firstGen.CalculateTheatreFitness();
	PrintTheatres(firstGen.GetTheatres());

	std::vector<THEATRE> fittest = firstGen.SelectTheFittest_GENERIC(10);
	std::cout << "Best 10 From First Generation [ GENERIC ]: " << std::endl;
	PrintTheatres(fittest);

	
	THEATRE selected;
	std::cout << "Best 50 From First Generation [ Roulette Wheel Selection ]: " << std::endl;
	for (i=0; i < 50; i++)
	{
		selected = firstGen.RouletteWheelSelection();
		PrintTheatre(selected);
	}

}
