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
		Point position;
		int cost;
		int revenue;
		int expense;
		map <Distribution, Distribution> distrubution;
		int costPerKM;
	private:
		Building();
		Building(int id, Point position,int cost);
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
		map <int, Store> possibleStores;
		int unsold;
	public:
		Logistics();
		Logistics(int id, Point position, int cost, int capacity);
		int getLogisticsCapacity(){
			return capacity;
		}
		int getLogisticsUnsold(){
			return unsold;
		}
		int send(Store to, int units);
		~Logistics();
};

class Store:public Building//subclass
{
	private:
		int demand;
		map <int, Logistics> possibleLogistics;
		int price;
		int unsatisfied;
	public:
		Store();
		Store(int id, Point position, int cost, int demand, int price);
		int getDemand(){
			return demand;
		}
		int getPrice(){
			return price;
		}
		int getUnsatisfied(){
			return unsatisfied;
		}
		int receive(Logistics from, int units);
		~Store();
};

class Distribution
{
	private:
		Logistics from;
		Store to;
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
		~Distribution();

};

/** JasonBaby start */
class Plan
{
private:
    map<int,Logistics> logistics;
    map<int,Store> stores;
    int revenue;
    int expense;
    map<int,Logistics> unsold;
    map<int,Store> unsatisfied;
public:
	Plan();
	Plan(Logistics* ls, int lNum, Store* ss, int sNum);
	~Plan();
	int getNet() const;
	string toString() const;
	bool remove(Building building);
	void update();
	// Getters
    vector<Logistics> getLogistics();
    vector<Store> getStores();
    int getRevenue();
    int getExpense();
    vector<Logistics> getUnsold();
    vector<Store> getUnsatisfied();
};
/** JasonBaby end */

int main()
{

	return 0;
}

/** JasonBaby start */
// For Plan


/** JasonBaby end */
