#include <iostream>
#include <cmath>
#include <map>
#include <vector>
#include <algorithm>
#include <sstream>

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
        //getter
        int getRevenue() const;
        int getExpense() const;
        int manhattonDistance(const Building& to);
        int getNet()const;
        static int compareNet(const Building& b1,const Building& b2);
        double getOER()const;//OperatingExpenseRatio
        static int compareOER(const Building& b1,const Building& b2);//OperatingExpenseRatio
        static int send(Logistics& from, Store& to, int units);
};
int Building::costPerKM;

/** Zhen start */
class Logistics: public Building //subclass
{
	private:
		const int capacity;
		map<int,Store*> possibleStores;
	public:
		int unsold;
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

class Store: public Building//subclass
{
	private:
		const int demand;
		map<int,Logistics*> possibleLogistics;
		const int price;
	public:
		int unsatisfied;
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
		Logistics& from;
		Store& to;
	public:
		// Variables
		const int price;
		const int unitCost;
		int units;
		// Constructors
		Distribution(Logistics& from, Store& to);
		// Functions
		int getUnitNet();
		int getNet();
		// Accessors
		Logistics& getFrom();
		Store& getTo();
};
/** Zhen end */


/** JasonBaby start */
class Plan
{
public:
    map<int,Logistics*> logistics;
    map<int,Store*> stores;
    int revenue;
    int expense;
    map<int,Logistics*> unsold;
    map<int,Store*> unsatisfied;
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
int Plan::numLogistics;
int Plan::numStores;
/** JasonBaby end */

bool moreNet(Building*, Building*);
bool lessNet(Building*, Building*);
bool moreOER(Building*, Building*);
bool lessOER(Building*, Building*);

int main()
{
	int storeNum = 0;
	int logisticsNum = 0;
	int costPerkm = 0;
	cin >> logisticsNum >> storeNum >> costPerkm;
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

	for(int i = 0; i < logisticsNum; i++)
	{
		int x, y;
		cin >> x >> y;
		Point* p = new Point(x, y);
		logistic[i] = p;
	}
	for(int i = 0; i < storeNum; i++)
	{
		int x, y;
		cin >> x >> y;
		Point* p = new Point(x, y);
		store[i] = p;
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
		int id = i + 1;
		Point* position = store[i];
		int cost = storeCost[i];
		int demand2 = demand[i];
		int price2 = price[i];
		Store* storeS = new Store(id, *position, cost, demand2, price2);
		stores[i] = storeS;
	}
	Logistics** logistics = new Logistics*[logisticsNum];
	for(int i = 0; i < logisticsNum; i++)
	{
		int id = i + 1;
		Point* position = logistic[i];
		int cost = logisticCost[i];
		int capacity2 = capacity[i];
		Logistics* logisticS = new Logistics(id, *position, cost, capacity2);
		logistics[i] = logisticS;
	}

	Plan original = Plan(logistics, logisticsNum, stores, storeNum);
	Plan current = original;
	string result = original.toString();
	int bestNet = original.getNet();

	vector<Building*> allBuildings;
	for (auto lit = current.logistics.begin(); lit != current.logistics.end();
		lit++)
	{
        allBuildings.push_back(lit->second);
	}
	for (auto sit = current.stores.begin(); sit != current.stores.end();
		sit++)
	{
        allBuildings.push_back(sit->second);
	}

	while(allBuildings.size() > 0)
	{
		current.update();
		if(current.getNet() > bestNet)
		{
			bestNet = current.getNet();
			result = current.toString();
		}
		sort(allBuildings.begin(),allBuildings.end(), moreNet);
		Building* worst = allBuildings[allBuildings.size() - 1];
		current.remove(worst);
		allBuildings.pop_back();
	}
	current.update();
	if(current.getNet() > bestNet)
	{
		bestNet = current.getNet();
		result = current.toString();
	}

	/** !!!DEBUG!!! */
    cout << result;// << endl << endl << bestNet;

	return 0;
}

bool moreNet(Building *b1, Building *b2)
{
	return Building::compareNet(*b1,*b2) > 0;
}
bool lessNet(Building *b1, Building *b2)
{
	return Building::compareNet(*b1,*b2) < 0;
}
bool moreOER(Building *b1, Building *b2)
{
	return Building::compareOER(*b1,*b2) > 0;
}
bool lessOER(Building *b1, Building *b2)
{
	return Building::compareOER(*b1,*b2) < 0;
}

/** JasonBaby start */

// Plan
// Constructors
Plan::Plan(Logistics**& ls, int lNum, Store**& ss, int sNum)
{
    revenue = 0;
    expense = 0;

    for (int i = 0; i < lNum; i++)
	{
		Logistics* l = ls[i];
        logistics[l->id] = l;
        unsold[l->id] = l;
        expense += l->cost;
	}

    for (int i = 0; i < sNum; i++)
	{
		Store* s = ss[i];
        stores[s->id] = s;
        unsatisfied[s->id] = s;
        expense += s->cost;
	}

    for (int i = 0; i < lNum; i++)
	{
		ls[i]->include(ss, sNum);
	}
	for (int i = 0; i < sNum; i++)
	{
		ss[i]->include(ls, lNum);
	}
}

Plan::Plan(const Plan& p)
{
	*this = p;
}

Plan& Plan::operator=(const Plan& p)
{
    revenue = p.revenue;
    expense = p.expense;

    for (map<int,Logistics*>::const_iterator it = p.logistics.begin();
		it != p.logistics.end(); it++)
	{
		// Has problem??? does it get deleted twice???
		Logistics* l = new Logistics(*it->second);
		logistics[it->first] = l;
		unsold[it->first] = l;
	}
    for (map<int,Store*>::const_iterator it = p.stores.begin();
		it != p.stores.end(); it++)
	{
		Store* s = new Store(*it->second);
		stores[it->first] = s;
		unsatisfied[it->first] = s;
	}

    return *this;
}

int Plan::getNet() const
{
	return revenue - expense;
}

string Plan::toString() const
{
	stringstream result;

	result << logistics.size();
	for(auto it = logistics.begin();
		it != logistics.end(); it++)
	{
		int id = it->first;
		result << " " << id;
	}

	result << endl;
	result << stores.size();
	for(auto it = stores.begin();
		it != stores.end(); it++)
	{
		int id = it->first;
		result << " " << id;
	}

	for(int i = 1; i <= numStores; i++)
	{
		result << endl;
		if(stores.find(i) == stores.end())
		{
			if (numLogistics >= 1)
			{
				result << 0;
			}
			for (int j = 2; j <= numLogistics; j++)
			{
				result << " 0";
			}
			continue;
		}

		Store* s = stores.find(i)->second;
		map<int,Distribution*>& ds = s->distribution;
		for(int j = 1; j <= numLogistics; j++)
		{
			if (j != 1)
			{
				result << " ";
			}
			auto dit = ds.find(j);
            if (dit == ds.end())
			{
				result << 0;
			}
			else
			{
				Distribution* d = dit->second;
				result << d->units;
			}
		}
	}

	return result.str();
}

bool Plan::remove(Building* building)
{
	if (Store* s = dynamic_cast<Store*> (building))
	{
        int id = s->id;
		stores.erase(id);
		revenue -= s->revenue;
		expense -= s->expense;
		for (map<int,Distribution*>::iterator it = s->distribution.begin();
			it != s->distribution.end(); it++)
		{
			Distribution*& d = it->second;
			Logistics& l = d->getFrom();
			unsold[l.id] = &l;

			int revenue = d->price * d->units;
			int expense = d->unitCost * d->units;
			l.revenue -= revenue;
			l.expense -= expense;
			l.unsold += d->units;

            l.getPossibleStores().erase(id);
            l.distribution.erase(id);
			delete d;
		}
		unsatisfied.erase(id);
		delete s;
	}
	else
	{
		Logistics* l = dynamic_cast<Logistics*> (building);
        int id = l->id;
		logistics.erase(id);
		revenue -= l->revenue;
		expense -= l->expense;
		for (map<int,Distribution*>::iterator it = l->distribution.begin();
			it != l->distribution.end(); it++)
		{
			Distribution*& d = it->second;
			Store& s = d->getTo();
			unsatisfied[s.id] = &s;

			int revenue = d->price * d->units;
			int expense = d->unitCost * d->units;
			s.revenue -= revenue;
			s.expense -= expense;
			s.unsatisfied += d->units;

            s.getPossibleLogistics().erase(id);
            s.distribution.erase(id);
            delete d;
		}
		unsold.erase(id);
		delete l;
	}
	return true;
}

void Plan::update()
{
	for (map<int,Logistics*>::iterator lit = unsold.begin();
		lit != unsold.end(); lit++)
	{
        for (map<int,Store*>::iterator sit = unsatisfied.begin();
			sit != unsatisfied.end(); sit++)
		{
            lit->second->include(*sit->second);
            sit->second->include(*lit->second);
		}
	}

	bool done = false;

	while (!done)
	{
		Distribution* bestD = nullptr;
		for (map<int,Logistics*>::iterator lit = unsold.begin();
			lit != unsold.end(); lit++)
		{
			Logistics* l = lit->second;
			map<int,Store*>& possible = l->getPossibleStores();
			for (map<int,Store*>::iterator sit = possible.begin();
				sit != possible.end(); sit++)
			{
				Store* s = sit->second;
				Distribution* d = new Distribution(*l, *s);
				d->units++;

				if (bestD == nullptr || d->getNet() > bestD->getNet())
				{
					delete bestD;
					bestD = d;
				}
			}
		}

		if (bestD == nullptr)
		{
            done = true;
		}
		else
		{
			Logistics& l = bestD->getFrom();
			Store& s = bestD->getTo();
			int units = min(l.getUnsold(), s.getUnsatisfied());
			Building::send(l, s, units);

			int price = bestD->price;
			int unitCost = bestD->unitCost;
			revenue += price * units;
			expense += unitCost * units;

			if (l.getUnsold() == 0)
			{
                unsold.erase(l.id);
			}
			if (s.getUnsatisfied() == 0)
			{
				unsatisfied.erase(s.id);
			}
		}
	}
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

int Plan::getRevenue() const
{
	return revenue;
}

int Plan::getExpense() const
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

/** HanjuTsai start*/
Point:: Point(int x,int y):x(x),y(y)
{
}
int Point:: manhattonDistance(const Point& to)const
{
    int manhattonDistance = 0;
    manhattonDistance = abs(x - to.x) + abs(y - to.y);
    return manhattonDistance;
}
Building::Building(int id,Point& position,int cost): id(id), position(position), cost(cost)
{
	revenue = 0;
	expense = cost;
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
    return this->position.manhattonDistance(to.position);
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
    return static_cast<double>(expense)/revenue;
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
int Building::send(Logistics& from, Store& to, int units)
{
	if (from.distribution.find(to.id) == from.distribution.end())
	{
		Distribution* d = new Distribution(from, to);
		d->units = units;
		from.distribution[to.id] = d;
		to.distribution[from.id] = d;
	}
    else
	{
		from.distribution[to.id]->units += units;
	}
    from.send(to,units);
    to.receive(from,units);
    return units;
}
Building:: ~Building()
{
    for (auto it = distribution.begin(); it != distribution.end(); it++)
	{
        delete it->second;
	}
}
/** HanjuTsai end */

/** Zhen start */
// Logistics
// Logistics-Constructors
Logistics::Logistics(int id, Point& position, int cost, int capacity)
	: Building(id, position, cost), capacity(capacity)
{
	unsold = capacity;
}
// Logistics-Functions
int Logistics::send(Store& to, int units)
{
	this->unsold -= units;
	Distribution dis(*this, to);
	revenue += to.getPrice() * units;
	expense += dis.unitCost * units;
	if(this->unsold == 0)
	{
		to.getPossibleLogistics().erase(this->id);
	}
	return units;
}
void Logistics::include(Store& s)
{
	if (possibleStores.find(s.id) != possibleStores.end())
	{
		return;
	}
	if(s.getDemand() == 0)
	{
		return;
	}
	if(Building::costPerKM * this->manhattonDistance(s) < s.getPrice())
	{
		possibleStores[s.id] = &s;
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
Store::	Store(int id, Point& position, int cost, int demand, int price)
	: Building(id, position, cost), demand(demand), price(price)
{
	unsatisfied = demand;
}
// Store-Functions
int Store::receive(Logistics& from, int units)
{
	this->unsatisfied -= units;
	Distribution dis(from, *this);
	revenue += this->price * units;
	expense += dis.unitCost * units;
	if(this->unsatisfied==0)
	  from.getPossibleStores().erase(this->id);
	return units;
}
void Store::include(Logistics& l)
{
	if(possibleLogistics.find(l.id) != possibleLogistics.end())
	{
		return;
	}
	if(l.getCapacity() == 0)
	{
		return;
	}
	if(Building::costPerKM * this->manhattonDistance(l) < price)
	{
		possibleLogistics[l.id] = &l;
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
	, units(0)
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
Logistics& Distribution::getFrom()
{
	return from;
}
Store& Distribution::getTo()
{
	return to;
}


/** Zhen end */
