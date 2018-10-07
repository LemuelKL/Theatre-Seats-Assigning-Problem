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

class THREATRE	// Chromosome
{
private:
	int mId;
	std::vector<std::vector<PERSON>> mSeats;
	int mFitness;
public:
	int Fitness()
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
		for (auto& row : mSeats) {
			for (auto& p : row) {
				
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

				if (neighborL.GetId()!=-2 && neighborL.IfIsInGroup(p.GetGroupId()))
				{
						PersonHappiness = PersonHappiness + 500;
				}
					
				if (neighborR.GetId() != -2 && neighborR.IfIsInGroup(p.GetGroupId()))
				{
					PersonHappiness = PersonHappiness + 500;
				}
					
				p.SetHappyIndex(PersonHappiness);
				mFitness = mFitness + p.GetHappyIndex();
			}
		}
	}

	THREATRE(int id=-1)
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

	bool operator < (THREATRE &other)
	{
		return Fitness() < other.Fitness();
	}
	
};

class GENERATION	// Generation
{
private:
	int mSize;
	std::vector<THREATRE> mHalls;

public:
	void Initialize_Generation(int populationSize, std::vector<PERSON> PeopleToBeAssigned)
	{
		mSize = populationSize;
		int i;
		for (i = 0; i < mSize; i++)
		{
			THREATRE Hall = THREATRE(i+1);
			Hall.InitFirstPopulation(PeopleToBeAssigned);
			mHalls.push_back(Hall);
		}
	}
	int GetSize()
	{
		return mHalls.size();
	}
	std::vector<THREATRE> SelectTheFittest(int n)
	{
		for (auto hall : mHalls)
		{
			hall.CalculateFitness();
			
		}
		SortByFitness();
		std::vector<THREATRE> ret = mHalls;
		
		ret.resize(n);
		return ret;
	}
	void SortByFitness()
	{
		
	}
	std::vector<THREATRE> GetThreatres()
	{
		return mHalls;
	}

};

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
		i = i + nPeopleInGroup-1;
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
		PERSON Audience(i+400, -1);
		PeopleToBeAssigned.push_back(Audience);
	}

	std::cout << "==================================================" << std::endl;
	GENERATION firstGen = GENERATION();
	firstGen.Initialize_Generation(100, PeopleToBeAssigned);

	for (THREATRE hall : firstGen.GetThreatres())
	{
		hall.CalculateFitness();
		std::cout << "Hall ID: " << hall.GetId() << " Fitness: " << hall.Fitness() << std::endl;
	}

	std::cout << "==================================================" << std::endl;
	std::vector<THREATRE> best10 = firstGen.SelectTheFittest(10);
	std::cout << "Best 10 From First Generation: " << std::endl;
	for (THREATRE hall : best10)
	{
		hall.CalculateFitness();
		std::cout << "Hall ID: " << hall.GetId() << " Fitness: " << hall.Fitness() << std::endl;
	}


}
