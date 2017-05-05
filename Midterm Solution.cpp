#include <iostream>
#include <cmath>
#include <map>
#include <algorithm>

using namespace std;

class Point;
class Building;
class Distribution;
class Logistics;
class Store;

class Point
{
	public:
		const int x;
		const int y;
		Point(int x,int y);
		int manhattonDistance(Point to);
};

class Building
{
	protected:
		Building(int id, Point position,int cost);
		virtual ~Building() ;
	public:
		int revenue;
		int expense;
		map<int,Distribution*> distribution;
		static int costPerKM;
		const int id;
		const Point& position;
		const int cost;
		int getCost() const
		{
			return cost;
		}
		int getRevenue() const
		{
			return revenue;
		}
		int getExpense() const
		{
			return expense;
		}
		int manhattonDistance(Building& to);
		int getNet();
		int compareNet(Building b1, Building b2);
		double getOER();//OperatingExpenseRatio
		int copareOER(Building b1, Building b2);//OperatingExpenseRatio
		int send(Logistics from, Store to, int units);
};

/** Zhen start */
class Logistics: public Building //subclass
{
	private:
		const int capacity;
		map<int,Store*> possibleStores;
		int unsold;
	public:
		// Constructors
		Logistics(int id, Point position, int cost, int capacity);
		// Functions
		int send(Store to, int units);
		void include(Store s);
		void include(Store* ss, int sNum);
		void include(Store** ss, int sNum);
		// Accessors
		int getCapacity() const;
		map<int,Store*>& getPossibleStores();
		int getUnsold() const;
};

class Store: public Building//subclass
{
	private:
		const int demand;
		map<int,Logistics*> possibleLogistics;
		const int price;
		int unsatisfied;
	public:
		// Constructors
		Store(int id, Point position, int cost, int demand, int price);
		// Functions
		int receive(Logistics from, int units);
		void include(Logistics l);
		void include(Logistics* ls, int lNum);
		void include(Logistics** ls, int lNum);
		// Accessors
		int getDemand() const;
		map<int,Logistics*>& getPossibleLogistics();
		int getPrice() const;
		int getUnsatisfied() const;
};

class Distribution
{
	private:
		const Logistics& from;
		const Store& to;
	public:
		// Variables
		const int price;
		const int unitCost;
		const int units;
		// Constructors
		Distribution(Logistics& from, Store& to);
		// Functions
		int getUnitNet();
		int getNet();
		// Accessors
		const Logistics& getFrom() const;
		const Store& getTo() const;
};
/** Zhen end */


/** JasonBaby start */
class Plan
{
private:
    map<int,Logistics*> logistics;
    map<int,Store*> stores;
    int revenue;
    int expense;
    map<int,Logistics*> unsold;
    map<int,Store*> unsatisfied;
public:
	// Static fields
	static int numLogistics;
	static int numStores;
	// Constructors
	Plan(const Plan& p);
	Plan& operator=(const Plan& p);
	Plan(Logistics**& ls, int lNum, Store**& ss, int sNum);
	// Functions
	int getNet() const;
	string toString() const;
	bool remove(Building* building);
	void update();
	// Accessors
    map<int,Logistics*>& getLogistics();
    map<int,Store*>& getStores();
    int getRevenue() const;
    int getExpense() const;
    map<int,Logistics*>& getUnsold();
    map<int,Store*>& getUnsatisfied();
};
/** JasonBaby end */

int main()
{

	return 0;
}

/** JasonBaby start */
// For Plan


/** JasonBaby end */

Point:: Point(int x,int y):x(x),y(y)
{

}

int Point:: manhattonDistance(Point to)
{
	int manhattonDistance = 0;
	manhattonDistance = abs(x - to.x) + abs(y - to.y);
	return manhattonDistance;
}

/** Zhen start */
// Logistics
// Logistics-Constructors
Logistics::Logistics(int id, Point position, int cost, int capacity)
	: Building(id, position, cost), capacity(capacity)
{
	unsold = capacity;
}
// Logistics-Functions
int Logistics::send(Store to, int units)
{
	this->unsold -= units;
	Distribution dis(*this, to);
	revenue += to.getPrice() * units;
	expense += dis.unitCost * units;
	if(this->capacity == 0)
	{
		to.getPossibleLogistics().erase(this->id);
	}
	return units;
}
void Logistics::include(Store s)
{
	for(int i=0; i<possibleStores.size(); i++)
	{
		if( s.id == (*possibleStores[i]).id)
		  return;
	}
	if(s.getDemand()!=0)
	{
		if(Building::costPerKM * this->manhattonDistance(s) < s.getPrice())
		{
			possibleStores[s.id]=&s;
		}
	}
}
void Logistics::include(Store* ss, int sNum)
{
	for(int i=0; i<sNum; i++)
	  this->include(ss[i]);
}
void Logistics::include(Store** ss, int sNum)
{
	for(int i=0; i<sNum; i++)
	  this->include(*ss[i]);
} 
// Logistics-Getters
int Logistics::getCapacity() const
{
	return capacity;
}
map<int, Store*>& Logistics::getPossibleStores()
{
	return possibleStores;
}
int Logistics::getUnsold() const
{
	return unsold;
}

// Store
// Store-Constructors
Store::	Store(int id, Point position, int cost, int demand, int price)
	: Building(id, position, cost), demand(demand), price(price)
{
	unsatisfied = demand;
}
// Store-Functions
int Store::receive(Logistics from, int units)
{
	this->unsatisfied -= units;
	Distribution dis(from, *this);
	revenue += this->price * units;
	expense += dis.unitCost * units;
	if(this->unsatisfied==0)
	  from.getPossibleStores().erase(this->id);
	return units;		
}
void Store::include(Logistics l)
{
	for(int i=0; i<possibleLogistics.size(); i++)
	{
		if(l.id == (*possibleLogistics[i]).id)
		  return;
	}		
	if(l.getCapacity()!=0)
	{
		if(Building::costPerKM * this->manhattonDistance(l) < price)
		{
			possibleLogistics[l.id]=&l;
		}
	}
}
void Store::include(Logistics* ls, int lNum)
{
	for(int i=0; i<lNum; i++)
	{
		this->include(ls[i]);
	}
}
void Store::include(Logistics** ls, int lNum)
{
	for(int i=0; i<lNum; i++)
	{
		this->include(*ls[i]);
	}
} 
// Store-Getters
int Store::getDemand() const
{
	return demand;
}
map<int, Logistics*>& Store::getPossibleLogistics()
{
	return possibleLogistics;
}
int Store::getPrice() const
{
	return price;
}
int Store::getUnsatisfied() const
{
	return unsatisfied;
}


// Distribution
// Distribution-Constructors
Distribution::Distribution(Logistics& from, Store& to)
	: from(from)
	, to(to)
	, price(to.getPrice())
	, unitCost(Building::costPerKM * from.manhattonDistance(to))
	, units(min(from.getCapacity(), to.getDemand()))
{}


// Distribution-Functios
int Distribution::getUnitNet()
{
	int getUnitNet = price-unitCost;
	return getUnitNet;
}
int Distribution::getNet()
{
	int getNet = units * this->getUnitNet();
	return getNet;
}
// Distribution-Getters
const Logistics& Distribution::getFrom() const
{
	return from;
}
const Store& Distribution::getTo() const
{
	return to;
}

/** Zhen end */
