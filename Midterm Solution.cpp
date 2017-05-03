#include <iostream>
#include <cmath>
#include <map>
#include <cstdlib>
#include <exception>

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
		Building(const Building& b);
		Building& operator=(const Building& b);
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

class Logistics: public Building
{
	private:
		int capacity;
		map <int, Store*> possibleStores;
		int unsold;
	public:
		Logistics();
		Logistics(int id, Point position, int cost, int capacity);
		Logistics(const Logistics& l);
		Logistics& operator=(const Logistics& l);
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
		Store(const Store& s);
		Store& operator=(const Store& s);
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
	Plan(const Logistics* ls, int lNum, const Store* ss, int sNum);
	Plan(const Plan& p);
	Plan& operator=(const Plan& p);
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

class NotImplemented : public std::logic_error
{
public:
    NotImplemented() : std::logic_error("Not implemented."){};
    NotImplemented(string err) : std::logic_error(err){};
};

// Plan
// Constructors
Plan::Plan(const Logistics* ls, int lNum, const Store* ss, int sNum)
{
    revenue = 0;
    expense = 0;

    for (int i = 0; i < lNum; i++)
	{
		Logistics l = ls[i];
        logistics.insert(pair<int,Logistics*>(l.id, &l));
        unsold.insert(pair<int,Logistics*>(l.id, &l));
        revenue += l.revenue;
        expense += l.expense;
	}

    for (int i = 0; i < sNum; i++)
	{
		Store s = ss[i];
        stores.insert(pair<int,Store*>(s.id, &s));
        unsatisfied.insert(pair<int,Store*>(s.id, &s));
        revenue += s.revenue;
        expense += s.expense;
	}
}

Plan::Plan(const Plan& p)
{
	*this = p;
}

Plan& Plan::operator=(const Plan& p)
{
    logistics = p.logistics;
    stores = p.stores;
    revenue = p.revenue;
    expense = p.expense;
    unsold = p.unsold;
    unsatisfied = p.unsatisfied;

    return *this;
}

int Plan::getNet() const
{
	return revenue - expense;
}

string Plan::toString() const
{
	throw new NotImplemented();
}

bool Plan::remove(Building* building)
{
	if (Store* s = dynamic_cast<Store*> (building))
	{

	}
	else
	{
		Logistics* l = dynamic_cast<Logistics*> (building);
	}
	throw new NotImplemented();
}

void Plan::update()
{
	throw new NotImplemented();
}

// Accessors
map<int,Logistics*>& Plan::getLogistics()
{
	return logistics;
}

map<int,Store*>& Plan::getStores()
{
	return stores;
}

int Plan::getRevenue()
{
	return revenue;
}

int Plan::getExpense()
{
	return expense;
}

map<int,Logistics*>& Plan::getUnsold()
{
	return unsold;
}

map<int,Store*>& Plan::getUnsatisfied()
{
	return unsatisfied;
}


/** JasonBaby end */
int Point::manhattonDistance(Point to)
{
	int manhattonDistance = 0;
	manhattonDistance = abs(x - to.x) + abs(y - to.y);
	return manhattonDistance;
}
