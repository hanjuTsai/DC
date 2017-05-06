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
        Point(int x,int y);
        int manhattonDistance(const Point& to);
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
        //getter
        int getRevenue() const;
        int getExpense() const;
        int manhattonDistance(const Building& to);
        int getNet()const;
        static int compareNet(const Building& b1,const Building& b2);
        double getOER()const;//OperatingExpenseRatio
        static int compareOER(const Building& b1,const Building& b2);//OperatingExpenseRatio
        int send(Logistics& from, Store& to, int units);
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
        void include(Store** ss, int sNum);
        // Accessors
        int getCapacity() const;
        map<int,Store*> getPossibleStores();
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
        void include(Logistics** ls, int lNum);
        // Accessors
        int getDemand() const;
        map<int,Logistics*> getPossibleLogistics();
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
/** HanjuTsai start*/
Point:: Point(int x,int y):x(x),y(y)
{

}
int Point:: manhattonDistance(const Point& to)
{
    int manhattonDistance = 0;
    manhattonDistance = abs(x - to.x) + abs(y - to.y);
    return manhattonDistance;
}
Building::Building(int id,Point& position,int cost): id(id), position(position), cost(cost)
{
}
int Building::getRevenue()const
{
    return revenue;
}
int Building::getExpense()const
{
    return expense;
}
int Building::getNet()const
{
    return revenue - expense;
}
int Building::manhattonDistance(const Building& to)//because the point reference is a const
{
    int manhattonDistance = 0;
    manhattonDistance = (this->position.x - to.position.x)+(this->position.y - to.position.y);//not quite sure
    return manhattonDistance;
}
int Building ::compareNet(const Building& b1,const Building& b2)
{
    if(b1.getNet() < b2.getNet())
    {
        return -1; 
    }
    else if(b1.getNet() == b2.getNet())
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
double Building::getOER()const
{
    double OER = 0;
    OER = static_cast<float>(revenue)/expense;
    return OER; 
}
int Building:: compareOER(const Building& b1,const Building& b2)
{
    if(b1.getOER() < b2.getOER())
    {
        return -1;
    }
    else if(b1.getOER() == b2.getOER())
    {
        return 0;
    }
    else 
    {
        return 1;
    }
}
int Building:: send(Logistics& from, Store& to, int units)
{
	for(int i = 0; i < distribution.size(); i++)
	{
		units += distribution[i]->units;
	}
	from.send(to,units);
	to.receive(from,units);
	return units;
}
Building:: ~Building()
{
	distribution.clear();
}
/** HanjuTsai end */