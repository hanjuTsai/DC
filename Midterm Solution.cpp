#include <iostream>
#include <cmath>
#include <map>
#include <cstdlib>
using namespace std;
class Point
{
	private:
		int x;
		int y;
	public:
		Point();
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

class Logistics: public Building
{
	private:
		int capacity;
		map <int, Store*> possibleStores;
		int unsold;
	public:
		Logistics();
		Logistics(int id, Point position, int cost, int capacity);
		int getLogisticsCapacity()
		{
			return capacity;
		}
		int getLogisticsUnsold()
		{
			return unsold;
		}
		int send(Store to, int units);
};

class Store:public Building//subclass
{
	private:
		int demand;
		map <int, Logistics*> possibleLogistics;
		int price;
		int unsatisfied;
	public:
		Store();
		Store(int id, Point position, int cost, int demand, int price);
		int getDemand()
		{
			return demand;
		}
		int getPrice()
		{
			return price;
		}
		int getUnsatisfied()
		{
			return unsatisfied;
		}
		int receive(Logistics from, int units);
};

class Distribution
{
	private:
		Logistics& from;
		Store& to;
		int price;
		int unitCost;
		int units;
	public:
		Distribution();
		Distribution(Logistics from, Store to);
		int getUnitNet()
		{
			return price-unitCost;
		}
		int getNet()
		{
			return units*(price-unitCost);
		}
		int getPrice()
		{
			return price;
		}
		int getUnitCost()
		{
			return unitCost;
		}
		int getUnits()
		{
			return units;
		}

};

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
	Plan(Logistics* ls, int lNum, Store* ss, int sNum);
	// Functions
	int getNet() const;
	string toString() const;
	bool remove(Building building);
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
// Plan
Plan::Plan()
{
	exit(1);
}

Plan::Plan(Logistics* ls, int lNum, Store* ss, int sNum)
{
	exit(1);
}

Plan::~Plan()
{
	exit(1);
}

int Plan::getNet() const
{
	exit(1);
}

string Plan::toString() const
{
	exit(1);
}

bool Plan::remove(Building building)
{
	exit(1);
}

void Plan::update()
{
	exit(1);
}

// Getters
map<int,Logistics&>& Plan::getLogistics()
{
	exit(1);
}

map<int,Store&>& Plan::getStores()
{
	exit(1);
}

int Plan::getRevenue()
{
	exit(1);
}

int Plan::getExpense()
{
	exit(1);
}

map<int,Logistics&>& Plan::getUnsold()
{
	exit(1);
}

map<int,Store&>& Plan::getUnsatisfied()
{
	exit(1);
}


/** JasonBaby end */
int Point:: manhattonDistance(Point to)
{
	int manhattonDistance = 0;
	manhattonDistance = abs(x - to.x) + abs(y - to.y);
	return manhattonDistance;
}
