#include <iostream>
#include <cmath>
#include <map>

using namespace std;

class Point
{
	private:
		int x;
		int y;
	public:
		int getX()
		{
			return x;
		}
		int getY()
		{
			return y;
		}
		Point(int x,int y);
		int manhattonDistance(Point to);
};
class Distribution;
class Logistics;
class Store;
class Building
{
	public:
		int id;
		Point& position;
		int cost;
		int revenue;
		int expense;
		map<int,Distribution*> distribution;
		int costPerKM;
	protected:
		Building();
		Building(int id, Point position,int cost);
		virtual ~Building();
	public:
		int getCost()
		{
			return cost;
		}
		int getRevenue()
		{
			return revenue;
		}
		int getExpense()
		{
			return expense;
		}
		int manhattonDistance(Building to);
		int getNet();
		int compareNet(Building b1, Building b2);
		double getOperatingExpenseRatio();
		int copareOER(Building b1, Building b2);
		int send(Logistics from, Store to, int units);
};

/**Zhen start*/
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
		int getCapacity();
		map<int,Store*> getPossibleStores();
		int getUnsold();
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
		int getDemand();
		map<int,Logistics*> getPossibleLogistics();
		int getPrice();
		int getUnsatisfied();
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
		Distribution(Logistics from, Store to);
		// Functions
		int getUnitNet();
		int getNet();
		// Accessors
		Logistics& getFrom() const;
		Store& getTo() const;
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
    int getRevenue();
    int getExpense();
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
Logistics::Logistics(const Logistics& l)// copy constructor
{

}
Logistics& Logistics::operator=(const Logistics& l)
{

}
// Logistics-Functions
int Logistics::send(Store to, int units)
{
	int send = Building::send(*this, to, units);
	this->unsold -=units;
	return send;
}
void Logistics::include(Store s)
{
	if(s.getDemand()!=0)
	{
		if(Building::costPerKM * s.position.manhattonDistance(this->position) < s.price)
		{
			possibleStores[s.id]=&s;
		}
	}
}
void Logistics::include(Store* ss, int sNum)
{
	for(int i=0; i<sNum; i++)
	{
		if(ss[i].getDemand()!=0)
		{
			if(Building::costPerKM * ss[i].position.manhattonDistance(this->position) < ss[i].price)
			{
				possibleStores[i]=&ss[i];
			}
		}
	}
}
// Logistics-Getters
int Logistics::getCapacity()
{
	return capacity;
}
map<int, Store*> Logistics::getPossibleStores()
{
	return possibleStores;
}
int Logistics::getUnsold()
{
	return unsold;
}

// Store
// Store-Constructors
Store::	Store(int id, Point position, int cost, int demand, int price)
	: this->id(id)
	, this->position(position)
	, this->cost(cost)
	, this->demand(demand)
	, this->price(price)
{
	unsatisfied = demand;
}

Store::	Store(const Store& s);
Store& Store::operator=(const Store& s);
// Store-Functions
int Store::receive(Logistics from, int units)
{
	int receive = Building::send(*this, from, units);
	unsatisfied -= units;
	return receive;
}
void Store::include(Logistics l)
{
	if(l.getCapacity()!=0)
	{
		if(Building::costPerKM * position.manhattonDistance(l.position) < price)
		{
			possibleLogistics[l.id]=&l;
		}
	}
}
void Store::include(Logistics* ls, int lNum)
{
	for(int i=0; i<lNum; i++)
	{
		if(ls[i].getCapacity()!=0)
		{
			if(Building::costPerKM * ls[i].position.manhattonDistance(this->position) < price)
			{
				possibleLogistics[i]=&ls[i];
			}
		}
	}
}
// Store-Getters
int Store::getDemand()
{
	return demand;
}
map<int, Logistics*> Store::getPossibleLogistics()
{
	return possibleLogistics;
}
int Store::getPrice()
{
	return price;
}
int Store::getUnsatisfied()
{
	return unsatisfied;
}


// Distribution
// Distribution-Constructors
Distribution::Distribution(Logistics from, Store to)
	: this->from(from)
	, this->to(to)
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
Logistics& Distribution::getFrom() const
{
	return from;
}
Store& Distribution::getTo() const
{
	return to;
}

/** Zhen end */
