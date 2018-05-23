/**
*  Programming exercise - Calendar tree
*/
#define _CRT_SECURE_NO_WARNINGS
//#include "stdafx.h"  (this will not run on Windows)
#include <stdio.h>
#include <stdlib.h>
#include <climits>
#include <iostream>
#include <string>
#include <time.h>
#include <climits>
using namespace std;

#include "CalendarTree.h"

/**
*  Programming exercise - Calendar tree
*  main program for testing the class
*
*  INPUT:  the program reads from the standard input lines of the form
*		<instruction> <instruction parameters>
*        where <instruction> can be I(nsert), D(elete), F(ind), A(fter), N(umBefore), P(rintSorted), Q(uit)
*		 and it is follows by appropriate parameters (e.g., time for Find)
*        as this is a test program, there is little care for unexpected inputs.
*  OUTPUT:  the results of the operations are printed to stdout in readable form.
*/
int main(int argc, char* argv[])
{
	CalendarTree myCalendar;
	char inst; // instruction, can be I(nsert), D(elete), F(ind), A(fter), N(umBefore), P(rintSorted), Q(uit), B(ig test)
	time_t t, d;  //time, duration
	string desc;
	CalendarEvent *ev;
	int n;
	const int BIG_TEST = 1000;
	bool munchLine;

	while ((inst = cin.get()) != 'Q')
	{
		munchLine = true;
		switch (inst) {
		case 'D':
			ev = myCalendar.deleteFirst();
			if (ev) {
				cout << "Deleted: "; ev->print();
				delete ev;
			}
			else {
				cout << "Calendar empty.\n";
			};
			break;
		case 'F':
			cin >> t;
			ev = myCalendar.eventAt(t);
			if (ev) {
				cout << "Found: "; ev->print();
			}
			else {
				cout << "Not found.\n";
			};
			break;
		case 'A':
			cin >> t;
			ev = myCalendar.eventAfter(t);
			if (ev) {
				cout << "Found: "; ev->print();
			}
			else {
				cout << "Not found.\n";
			};
			break;
		case 'N':
			cin >> t;
			n = myCalendar.numBefore(t);
			cout << n << '\n';
			break;
		case 'I':
			cin >> t;
			cin >> d;
			getline(cin, desc); munchLine = false;
			ev = myCalendar.insert(new CalendarEvent(t, d, desc));
			if (ev) {
				cout << "Inserted: "; ev->print();
			}
			else {
				cout << "Cannot insert: (" << t << ", " << d << ")\n";
			};
			break;
		case 'P':
			myCalendar.printSorted();
			break;
		case 'B': {
			// insert a big number of random events, find how much time it takes
			int i;
			clock_t start, finish;
			double  duration;
			start = clock();
			for (i = 0; i < BIG_TEST; i++) {
				myCalendar.insert(new CalendarEvent(rand(), 2, "test"));
			};
			finish = clock();
			duration = (double)(finish - start) / CLOCKS_PER_SEC;
			cout << "This took ";
			printf("%2.3f seconds\n", duration);
		};
				  break;
		default:
			// wrong line? skip it
			cout << "what?\n";
			scanf("*[^\n]");
		};
		if (munchLine) cin.ignore(INT_MAX, '\n');
		ev = NULL; // to be clean, since ev is only temporary.
	}

	return 0;
}

