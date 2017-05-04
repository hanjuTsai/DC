#include <iostream>
#include <cmath>
#include <map>
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
		Point();
		Point(int x,int y);
		int manhattonDistance(Point to);
};
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
		double getOER();//OperatingExpenseRatio
		int copareOER(Building b1, Building b2);//OperatingExpenseRatio
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
		int getCapacity()
		{
			return capacity;
		}
		int getUnsold()
		{
			return unsold;
		}
		Logistics(const Logistics& l);
		Logistics& operator=(const Logistics& l);
		// Functions
		int send(Store to, int units);
		void include(Store s);
		void include(Store* ss, int sNum);
		//Getters
		int getCapacity();
		map<int,Store*> getPossibleStores();
		int getUnsold();
};

class Store:public Building//subclass
{
	private:
		const int demand;
		map<int,Logistics*> possibleLogistics;
		const int price;
		int unsatisfied;
	public:
		// Constructors
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
		Store(const Store& s);
		Store& operator=(const Store& s);
		// Functions
		int receive(Logistics from, int units);
		void include(Logistics l);
		void include(Logistics* ls, int lNum);
		// Getters
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
		// Functios
		int getUnitNet();
		int getNet();
		// Getters
		Logistics& getFrom();
		Store& getTo();
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
// For Plan


/** JasonBaby end */
Point:: Point():x(0),y(0)
{
}	
Point:: Point(int x,int y):x(x),y(y)
{
	
}
int Point:: manhattonDistance(Point to)
{
	int manhattonDistance = 0;
	manhattonDistance = abs(x - to.x) + abs(y - to.y);
	return manhattonDistance;
}
