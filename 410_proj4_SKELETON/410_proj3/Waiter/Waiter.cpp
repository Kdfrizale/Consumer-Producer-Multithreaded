//Kyle Frizzell--50%
//Ricardo Flores--50%
#include <string>
#include "..\includes\externs.h"
#include "stdlib.h"

#include "..\includes\Waiter.h"

using namespace std;

Waiter::Waiter(int id,std::string filename):id(id),myIO(filename){

}

Waiter::~Waiter()
{
}

//gets next Order(s) from file_IO
int Waiter::getNext(ORDER &anOrder){
	return myIO.getNext(anOrder);
}

//get orders from file, addto order in queue
//notify bakers when new order is added
//notify all bakers when finished and set b_waiterIsFinished = true
void Waiter::beWaiter() {
	ORDER anOrder;
	while (getNext(anOrder) != NO_ORDERS) {
		unique_lock<mutex> mylock(mutex_order_inQ);//lock for ready queue
		order_in_Q.push(anOrder); // access to ready queue
		mylock.unlock();
		cv_order_inQ.notify_one();//notify one baker that an order is ready
	}
	b_WaiterIsFinished = true;
	cv_order_inQ.notify_all();//notify all bakers that the waiter is done
}

