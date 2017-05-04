#include <iostream>
#include <cmath>
#include <map>
#include <vector>

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
	Plan(Logistics** ls, int lNum, Store** ss, int sNum);
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

bool lessNet(Building, Building);
bool moreOER(Building,Building);

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

	for(int i = 0; i < storeNum; i++){
		int x, y;
		cin >> x >> y;
		Point p =  Point(x, y);
		store[i] = &p;
	}
	for(int i = 0; i < logisticsNum; i++){
		int x, y;
		cin >> x >> y;
		Point p =  Point(x, y);
		logistic[i] = &p;
	}
	for(int i = 0; i < storeNum; i++){
		cin >> demand[i];
	}
	for(int i = 0; i < storeNum; i++){
		cin >> storeCost[i];
	}
	for(int i = 0; i < storeNum; i++){
		cin >> price[i];
	}
	for(int i = 0; i < logisticsNum; i++){
		cin >> logisticCost[i];
	}
	for(int i = 0; i < logisticsNum; i++){
		cin >> capacity[i];
	}
	Store** stores = new Store*[storeNum];
	for(int i = 0; i < storeNum; i++){
		int id = i;
		Point position = *store[i];
		int cost = storeCost[i];
		int demand2 = demand[i];
		int capacity2 = capacity[i];
		Store storeS = Store(id, position, cost, demand2, capacity2);
		stores[i] = &storeS;
	}
	Logistics** logistics = new Logistics*[logisticsNum];
	for(int i = 0; i < logisticsNum; i++){
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
	Plan bestPlan = originalPlan;

	vector<Building*> allBuildings;
	for(int i = 0; i < logisticsNum+storeNum; i++){
		if(i < logisticsNum){
			allBuildings[i] = &logistics[i];
		}
		if(i >= logisticsNum){
			allBuildings[i] = &stores[i - logisticsNum];
		}

	}

	vector<Building*> allBuildings1;
	for(int i = 0; i < logisticsNum+storeNum; i++){
		if(i < logisticsNum){
			allBuildings1[i] = &logistics[i];
		}
		if(i >= logisticsNum){
			allBuildings1[i] = &stores[i - logisticsNum];
		}

	}


	while(allBuildings.size() > 0)
	{
		plan1.update();
		sort(allBuildings.begin(),allBuildings.end(), lessNet);
		plan1.remove(allBuildings[0]);
		allBuildings.erase(allBuildings[0]);
		if(plan1.getNet() > bestPlan.getNet())
		{
			bestPlan = plan1;
		}


	}
	while(allBuildings1.size() > 0)
	{
		plan2.update();
		sort(allBuildings1.begin(),allBuildings1.end(), moreOER);
		plan2.remove(allBuildings1[0]);
		allBuildings1.erase(allBuildings1[0]);
		if(plan2.getNet > bestPlan.getNet())
		{
			bestPlan = plan2;
		}

	}


	return 0;
}

/** JasonBaby start */
// For Plan


/** JasonBaby end */


bool lessNet(Building b1, Building b2)
{
	return Building::compareNet(b1,b2) < 0;

}
bool moreOER(Building b1, Building b2)
{
	return Building::compareOER(b1,b2) > 0;
}


int Point:: manhattonDistance(Point to)
{
	int manhattonDistance = 0;
	manhattonDistance = abs(x - to.x) + abs(y - to.y);
	return manhattonDistance;
}
