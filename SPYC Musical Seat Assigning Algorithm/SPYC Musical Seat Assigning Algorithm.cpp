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
	static int mGlobalId;
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
	THEATRE(std::vector<PERSON> people)
	{
		if (people.size() != 500)
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
				p = people.at(i);
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
public:
	void SelectParentsForInternal()
	{
		std::cout << "Best 50 From First Generation [ Roulette Wheel Selection ]: " << std::endl;
		for (int i = 0; i < 50; i++)
		{
			THEATRE selected;
			selected = RouletteWheelSelection();
			mHalls_Parent.push_back(selected);
			std::cout << "Theatre ID: " << selected.GetId() << " Fitness: " << selected.Fitness() << std::endl;
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
	GENERATION BreedNextGeneration()
	{
		if (mHalls_Parent.size() != mHalls.size() / 2)
		{
			std::cout << "NOT ENOUGH PARENTS TO BREED" << std::endl;
		}
		std::cout << "START BREEDING WITH PARENTS x " << mHalls_Parent.size() << std::endl;
		
		std::vector<PERSON> Seats1;
		std::vector<PERSON> Seats2;
		int i;
		int j;
		for (j = 0; j < mHalls_Parent.size(); j += 2)
		{
			Seats1 = mHalls_Parent.at(j).GetRaw1DSeats();
			Seats2 = mHalls_Parent.at(j+1).GetRaw1DSeats();

			int pos1 = 250;
			int pos2 = 333;
			std::vector<PERSON> offspringSeats1;
			std::vector<PERSON> offspringSeats2;

			for (i = 0; i < pos1; i++)
			{
				offspringSeats1.push_back(Seats1.at(i));
			}
			for (i = pos1; i < 500; i++)
			{
				offspringSeats1.push_back(Seats2.at(i));
			}
			for (i = 0; i < pos1; i++)
			{
				offspringSeats2.push_back(Seats2.at(i));
			}
			for (i = pos1; i < 500; i++)
			{
				offspringSeats2.push_back(Seats1.at(i));
			}
			/*
			for (i = 0; i < offspringSeats1.size(); i++)
			{
				std::cout << " [" << offspringSeats1[i].GetGroupId() << "]" << "\t";
			}
			std::cout << std::endl;
			std::cout << std::endl;
			for (i = 0; i < offspringSeats2.size(); i++)
			{
				std::cout << " [" << offspringSeats2[i].GetGroupId() << "]" << "\t";
			}
			*/
			THEATRE offspringTheatre1(offspringSeats1);
			THEATRE offspringTheatre2(offspringSeats2);
			offspringTheatre1.AssignID();
			offspringTheatre2.AssignID();

			offspringTheatre1.CalculateFitness();
			offspringTheatre2.CalculateFitness();

			mHalls_Offspring.push_back(offspringTheatre1);
			mHalls_Offspring.push_back(offspringTheatre2);
			mHalls_Offspring.push_back(mHalls_Parent.at(j));
			mHalls_Offspring.push_back(mHalls_Parent.at(j + 1));			
		}

		int iLargest = 0;
		for (i = 1; i < mHalls_Offspring.size(); i++)
		{
			if (mHalls_Offspring.at(iLargest).Fitness() > mHalls_Offspring.at(iLargest).Fitness())
			{
				iLargest = i;
			}
		}

		std::cout << std::endl;
		std::cout << std::endl;
		std::cout << "GENERATION: " << mGenerationId << std::endl;
		std::cout << "BEST FROM GENERATION: " << std::endl;
		std::cout << "THEATRE ID: " << mHalls_Offspring.at(iLargest).GetId() << " Fitness: " << mHalls_Offspring.at(iLargest).Fitness() << std::endl;

		std::cout << std::endl;
		for (i = 0; i < mHalls_Offspring.at(iLargest).GetRaw1DSeats().size(); i++)
		{
			std::cout << " [" << mHalls_Offspring.at(iLargest).GetRaw1DSeats()[i].GetGroupId() << "]" << "\t";
		}
		std::cout << std::endl;

		GENERATION NextGen( mGenerationId + 1, mHalls_Offspring );
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
	firstGen.InitializeFirstGeneration(100, PeopleToBeAssigned);
	// // // // //
	firstGen.CalculateTheatreFitness();
	PrintTheatres(firstGen.GetTheatres());

	std::vector<GENERATION> Gens;
	firstGen.SelectParentsForInternal();
	GENERATION currGen = firstGen.BreedNextGeneration();
	Gens.push_back(currGen);
	
	for (int i=1;i<100;i++)
	{
		std::cout << "Generation: " << i+1 << std::endl;
		Gens.at(i - 1).CalculateTheatreFitness();
		Gens.at(i - 1).SelectParentsForInternal();
		currGen = Gens.at(i-1).BreedNextGeneration();
		Gens.push_back(currGen);
	}
	

}
