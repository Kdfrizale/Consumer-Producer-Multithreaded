//Kyle Frizzell--50%
//Ricardo Flores--50%
#include <mutex>

#include "..\includes\externs.h"
#include "..\includes\Baker.h"
using namespace std;

Baker::Baker(int id):id(id)
{
}

Baker::~Baker()
{
}

//bake, box and append to anOrder.boxes vector
//if order has 13 donuts there should be 2 boxes
//1 with 12 donuts, 1 with 1 donut
void Baker::bake_and_box(ORDER &anOrder) {
	DONUT aDonut;
	int numberofDoughnuts = anOrder.number_donuts;
	while (numberofDoughnuts > 0) {
		Box aBox;
		//while there are donuts left, and space in the box; add donut to box
		while (numberofDoughnuts > 0 && aBox.addDonut(aDonut)){
			numberofDoughnuts--;
		}
		//add box to out vector when box if full or no donuts left
		anOrder.boxes.push_back(aBox);
	}
}


//Waits for waiter to put an order on the ready queue
//removes the order from the ready queue, and bakes_and_box the order
//exits when waiter is done and ready queue is empty
void Baker::beBaker() {
	do {
		if (!b_WaiterIsFinished) {//only wait for waiter to put order on queue, if waiter is still working
			unique_lock<mutex> orderInLock(mutex_order_inQ);
			cv_order_inQ.wait(orderInLock);
		}

		bool b_Qempty = false;
		while (!b_Qempty) {
			//Bake 
			unique_lock<mutex> myInlock(mutex_order_inQ);//lock for ready queue
			b_Qempty = order_in_Q.empty();//access to ready queue
			if (b_Qempty) {
				break;
			}
			ORDER tempOrder = order_in_Q.front();//get order and bake
			order_in_Q.pop();
			myInlock.unlock();
			bake_and_box(tempOrder);

			unique_lock<mutex> myOutlock(mutex_order_outQ);//lock for out vector
			order_out_Vector.push_back(tempOrder);//access to out vector
			myOutlock.unlock();
		}
	} while (!b_WaiterIsFinished);
}
