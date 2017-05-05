#include <iostream>
#include <cmath>
#include <map>
#include <vector>
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
		int manhattonDistance(const Point& to) const;
};

class Building
{
	protected:
		Building(int id, Point& position,int cost);
		virtual ~Building();
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
		int manhattonDistance(const Building& to) const;
		int getNet();
		static int compareNet(const Building& b1, const Building& b2);
		double getOER();//OperatingExpenseRatio
		static int compareOER(const Building& b1, const Building& b2);//OperatingExpenseRatio
		static int send(Logistics& from, Store& to, int units);
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
		Logistics(int id, Point& position, int cost, int capacity);
		// Functions
		int send(Store& to, int units);
		void include(Store& s);
		void include(Store* ss, int sNum);
		void include(Store** ss, int sNum);
		// Accessors
		int getCapacity() const;
		map<int,Store*>& getPossibleStores();
		int getUnsold() const;
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
		Store(int id, Point& position, int cost, int demand, int price);
		// Functions
		int receive(Logistics& from, int units);
		void include(Logistics& l);
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
    int getRevenue() const;
    int getExpense() const;
    map<int,Logistics*>& getUnsold();
    map<int,Store*>& getUnsatisfied();
};
/** JasonBaby end */

bool moreNet(Building, Building);
bool lessNet(Building, Building);
bool lessOER(Building,Building);

int main()
{

	int storeNum = 0;
	int logisticsNum = 0;
	int costPerkm = 0;
	cin >> storeNum >> logisticsNum >> costPerkm;
	Point** store = new Point*[storeNum];
	Point** logistic = new Point*[logisticsNum];
	int* demand = new int[storeNum];
	int* storeCost = new int[storeNum];
	int* price = new int[storeNum];
	int* logisticCost = new int[logisticsNum];
	int* capacity = new int[logisticsNum];
	Building::costPerKM = costPerkm;
	Plan::numLogistics = logisticsNum;
	Plan::numStores = storeNum;

	for(int i = 0; i < storeNum; i++)
	{
		int x, y;
		cin >> x >> y;
		Point p =  Point(x, y);
		store[i] = &p;
	}
	for(int i = 0; i < logisticsNum; i++)
	{
		int x, y;
		cin >> x >> y;
		Point p =  Point(x, y);
		logistic[i] = &p;
	}
	for(int i = 0; i < storeNum; i++)
	{
		cin >> demand[i];
	}
	for(int i = 0; i < storeNum; i++)
	{
		cin >> storeCost[i];
	}
	for(int i = 0; i < storeNum; i++)
	{
		cin >> price[i];
	}
	for(int i = 0; i < logisticsNum; i++)
	{
		cin >> logisticCost[i];
	}
	for(int i = 0; i < logisticsNum; i++)
	{
		cin >> capacity[i];
	}
	Store** stores = new Store*[storeNum];
	for(int i = 0; i < storeNum; i++)
	{
		int id = i;
		Point position = *store[i];
		int cost = storeCost[i];
		int demand2 = demand[i];
		int capacity2 = capacity[i];
		Store storeS = Store(id, position, cost, demand2, capacity2);
		stores[i] = &storeS;
	}
	Logistics** logistics = new Logistics*[logisticsNum];
	for(int i = 0; i < logisticsNum; i++)
	{
		int id = i+1;
		Point position = *logistic[i];
		int cost = logisticCost[i];
		int capacity2 = capacity[i];
		Logistics logisticS = Logistics(id, position, cost, capacity2);
		logistics[i] = &logisticS;
	}
	Plan originalPlan = Plan(logistics, logisticsNum, stores, storeNum );
	Plan plan1 = originalPlan;
	Plan plan2 = originalPlan;
	Plan bestPlan1 = originalPlan;
	Plan bestPlan2 = originalPlan;

	vector<Building*> allBuildings;
	for(int i = 0; i < logisticsNum+storeNum; i++)
	{
		if(i < logisticsNum)
		{
			allBuildings[i] = logistics[i];
		}
		if(i >= logisticsNum)
		{
			allBuildings[i] = stores[i - logisticsNum];
		}

	}

	vector<Building*> allBuildings1;
	for(int i = 0; i < logisticsNum+storeNum; i++){
		if(i < logisticsNum)
		{
			allBuildings1[i] = logistics[i];
		}
		if(i >= logisticsNum)
		{
			allBuildings1[i] = stores[i - logisticsNum];
		}

	}


	while(allBuildings.size() > 0)
	{
		plan1.update();
		sort(&allBuildings[0],&allBuildings[-1], moreNet);
		plan1.remove(allBuildings[-1]);
		allBuildings.erase(allBuildings.end());
		if(plan1.getNet() > bestPlan1.getNet())
		{
			bestPlan1 = plan1;
		}


	}
	while(allBuildings1.size() > 0)
	{
		plan2.update();
		sort(&allBuildings1[0],&allBuildings1[-1], lessOER);
		plan2.remove(allBuildings1[-1]);
		allBuildings1.erase(allBuildings1.end());
		if(plan2.getNet() > bestPlan2.getNet())
		{
			bestPlan2 = plan2;
		}

	}

	if(bestPlan1.getNet() < bestPlan2.getNet())
	{
		bestPlan1 = bestPlan2;
	}
	string bestplan = bestPlan1.toString();



	return 0;
}

bool moreNet(Building b1, Building b2)
{
	return Building::compareNet(b1,b2) > 0;
}

bool lessNet(Building b1, Building b2)
{
	return Building::compareNet(b1,b2) < 0;
}
bool lessOER(Building b1, Building b2)
{
	return Building::compareOER(b1,b2) < 0;
}




/** JasonBaby start */
// For Plan


/** JasonBaby end */

Point:: Point(int x,int y):x(x),y(y)
{

}

int Point:: manhattonDistance(const Point& to) const
{
	int manhattonDistance = 0;
	manhattonDistance = abs(x - to.x) + abs(y - to.y);
	return manhattonDistance;
}
