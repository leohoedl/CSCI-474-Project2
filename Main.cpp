/*Project 2
  CSCI 474
  11/25/2019
  Leo Hoedl, Jacob Bellemare, Isaac Flage*/

#include <iostream>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <iostream>
#include <functional>
#include <array>
#include <algorithm>
#include "TextTable.h"

using namespace std;

int diskSize;
TextTable table('-', '|', '+');

//FIFO algorithm
void FIFO(int tracks[], int head, double size) {
	int headTrack = head;
	int seekCount = 0;
	int distance, currentTrack;
	double averageSeek;

	for (int i = 0; i < size; i++) {
		currentTrack = tracks[i];

		//calculate what the distance between current track and head is
		distance = abs(currentTrack - headTrack);
		//Increase the total seek count
		seekCount += distance;
		//current track is now the new head track
		headTrack = currentTrack;
	}

	averageSeek = seekCount / size;

	std::string headStr = std::to_string(head);
	std::string seekStr = std::to_string(seekCount);
	std::string avgStr = std::to_string(averageSeek);
	table.add(headStr);
	table.add(seekStr);
	table.add(avgStr);
	table.endOfRow();
}

//LIFO algorithm
void LIFO(int tracks[], int head, double size) {
	int headTrack = head;
	int seekCount = 0;
	int distance, currentTrack;
	double averageSeek;


	for (int i = size - 1; i >= 0; i--) {
		currentTrack = tracks[i];

		//Calculate distance between two tracks
		distance = abs(currentTrack - headTrack);
		//Increase the total seek count
		seekCount += distance;
		//current track is now the new head
		headTrack = currentTrack;
	}

	averageSeek = seekCount / size;

	std::string headStr = std::to_string(head);
	std::string seekStr = std::to_string(seekCount);
	std::string avgStr = std::to_string(averageSeek);

	table.add(headStr);
	table.add(seekStr);
	table.add(avgStr);
	table.endOfRow();
}

//SSTF
void SSTF(int tracks[], int head, double size) {
	int headTrack = head;
	int seekCount = 0;
	int distance, currentTrack;
	double averageSeek;
	bool done[1000];

	//Initializes done array
	//done array exists to determine if track request has been completed yet.
	//If index value is false, track has not been accessed.
	for (int j = 0; j < size; j++) {
		done[j] = false;
	}

	//Calculation to determine what the shortest access is.
	for (int i = 0; i < size; i++) {
		int index = headTrack, shortest = 1000; // <- large value that will never be topped
		for (int j = 0; j < size; j++) {
			if ((abs(headTrack - tracks[j]) < shortest) && !done[j]) {
				index = j;
				shortest = abs(headTrack - tracks[j]);
			}
		}
		done[index] = true;
		seekCount += shortest;
		headTrack = tracks[index];
	}

	averageSeek = seekCount / size;

	std::string headStr = std::to_string(head);
	std::string seekStr = std::to_string(seekCount);
	std::string avgStr = std::to_string(averageSeek);

	table.add(headStr);
	table.add(seekStr);
	table.add(avgStr);
	table.endOfRow();
}

//SCAN algorithm
//Adapted from: https://www.geeksforgeeks.org/scan-elevator-disk-scheduling-algorithms/
//Added in changes to allow for average seek distance calculation
void SCAN(int tracks[], int head, double size, string direction) {
	int seekCount = 0, min = INT8_MAX, max = INT8_MIN; //Guarantees that values will be smaller or bigger
	int prevHead = head;
	double averageSeek = 0;
	int distance, currtrack;
	vector<int> left, right;

	//Finding minimum and maximum values.
	for (int i = 0; i < size; i++) {
		if (tracks[i] < min) {
			min = tracks[i];
		}
		if (tracks[i] > max) {
			max = tracks[i];
		}
	}

	if (direction == "left")
		left.push_back(min);
	else if (direction == "right")
		right.push_back(max);

	//Pushes values onto vector depending if less than head or greater than head.
	//If greater, push onto right. If less, push onto left.
	for (int i = 0; i < size; i++) {
		if (tracks[i] < head)
			left.push_back(tracks[i]);
		if (tracks[i] > head)
			right.push_back(tracks[i]);
	}

	//Sorts vectors into order for easy seeking
	std::sort(left.begin(), left.end());
	std::sort(right.begin(), right.end());

	//Only want to run twice. Once left and once right.
	int run = 2;
	while (run--) {
		if (direction == "left") {
			for (int i = left.size() - 1; i >= 0; i--) {
				currtrack = left[i];

				// calculate distance between current track and current head
				distance = abs(currtrack - head);

				// increase total seek count
				seekCount += distance;

				// current track is now the new head.
				head = currtrack;
			}
			//change direction when done.
			direction = "right";
		}
		else if (direction == "right") {
			for (int i = 0; i < right.size(); i++) {
				currtrack = right[i];

				// calculate distance between current track and current head
				distance = abs(currtrack - head);

				// increase total seek count.
				seekCount += distance;

				// current track is now the new head.
				head = currtrack;
			}
			direction = "left";
		}
	}

	averageSeek = seekCount / size;

	std::string headStr = std::to_string(prevHead);
	std::string seekStr = std::to_string(seekCount);
	std::string avgStr = std::to_string(averageSeek);

	table.add(headStr);
	table.add(seekStr);
	table.add(avgStr);
	table.endOfRow();
}

//CSCAN algorithm
//Adapted from previous SCAN algorithm due to similarity
//Changed so that when a direction is finished it goes to min and finishes the calculation
void CSCAN(int tracks[], int head, double size) {
	int seekCount = 0, min = INT8_MAX, max = INT8_MIN;
	int distance, currtrack;
	int prevHead = head;
	double averageSeek = 0;
	vector<int> left, right;

	//Finding minimum and maximum values.
	for (int i = 0; i < size; i++) {
		if (tracks[i] < min) {
			min = tracks[i];
		}
		if (tracks[i] > max) {
			max = tracks[i];
		}
	}

	left.push_back(min);
	right.push_back(max);

	//Pushes values onto vector depending if less than head or greater than head.
	//If greater, push onto right. If less, push onto left.
	for (int i = 0; i < size; i++) {
			if (tracks[i] < head)
					left.push_back(tracks[i]);
			if (tracks[i] > head)
					right.push_back(tracks[i]);
	}

	//Sorts vectors into order for easy seeking
	std::sort(left.begin(), left.end());
	std::sort(right.begin(), right.end());

	 for (int i = 0; i < right.size(); i++) {
			 currtrack = right[i];

			 // calculate distance between current track and the head
			 distance = abs(currtrack - head);

			 // increase the total seek count
			 seekCount += distance;

			 // current track is now the new head
			 head = currtrack;
	 }
	 // once reached the right, jump to the beginning of the left vector
	 head = max;

	 for (int i = 0; i < left.size(); i++) {
			 currtrack = left[i];

			 // calculate distance between current track and the head
			 distance = abs(currtrack - head);

			 // increase the total seek count
			 seekCount += distance;

			 // current track is now the new head
			 head = currtrack;
	 }


	averageSeek = seekCount / size;

	std::string headStr = std::to_string(prevHead);
	std::string seekStr = std::to_string(seekCount);
	std::string avgStr = std::to_string(averageSeek);

	table.add(headStr);
	table.add(seekStr);
	table.add(avgStr);
	table.endOfRow();
}

//N-STEP-SCAN
void NSTEPScan(int tracks[], int head, double size, int n, string direction) {
	//if n = 1, FIFO
	int seekCount = 0, min = INT8_MAX, max = INT8_MIN;
	int prevHead = head;
	double averageSeek = 0;
	int distance, currtrack;
	vector<int> left, right;

	int tracksNum = ( size / n);
	int buffer[tracksNum][n];

	for (int x = 0; x < tracksNum; x++) {

		for (int y = 0; y < n; y++)
		{
			buffer[x][y] = tracks[x];
		}

	}

	for (int j = 0; j < tracksNum; j++) {
		for (int i = 0; i < n; i++) {
			if (buffer[j][i] < min) {
				min = buffer[j][i];
			}
			if (buffer[j][i] > max) {
				max = buffer[j][i];
			}
		}
	}

	if (direction == "left")
		left.push_back(min);
	else if (direction == "right")
		right.push_back(max);

	for (int j = 0; j < tracksNum; j++) {
		for (int i = 0; i < n; i++) {
			if (buffer[j][i] < head)
				left.push_back(buffer[j][i]);
			if (buffer[j][i] > head)
				right.push_back(buffer[j][i]);
		}
	}

	//Sort such that seeking is easier.
	std::sort(left.begin(), left.end());
	std::sort(right.begin(), right.end());

	int run = 2;
	while (run--) {
		if (direction == "left") {
			for (int i = left.size() - 1; i >= 0; i--) {
				currtrack = left[i];

				// calculate distance between current track and the head.
				distance = abs(currtrack - head);

				// increase the total seek count.
				seekCount += distance;

				// current track is now the new head.
				head = currtrack;
			}
			direction = "right";
		}
		else if (direction == "right") {
			for (int i = 0; i < right.size(); i++) {
				currtrack = right[i];

				// calculate distance between current track and the head.
				distance = abs(currtrack - head);

				// increase the total seek count.
				seekCount += distance;

				// current track is now the new head.
				head = currtrack;
			}
			direction = "left";
		}
	}

	averageSeek = seekCount / size;

	std::string headStr = std::to_string(prevHead);
	std::string seekStr = std::to_string(seekCount);
	std::string avgStr = std::to_string(averageSeek);

	table.add(headStr);
	table.add(seekStr);
	table.add(avgStr);
	table.endOfRow();
}

//FSCAN
void FSCAN(int tracks[], int head, double size, string direction){//test file for building algorithm.
  int seekCount = 0, min = INT8_MAX, max = INT8_MIN;
  int distance, currtrack;
  int prevHead = head;
  double averageSeek = 0;
  int newSize = int(size);
  vector<int> left, right;

  if ((newSize % 2) > 0) {
  //handle splitting odd number queueSize
  int half = newSize / 2;
  int biggerHalf = half + 1;
  int smallerHalf = half;
  int queue1[biggerHalf];
  int queue2[biggerHalf];
  int queue[2][biggerHalf];
  for(int i = 0; i< smallerHalf; i++){
    queue[0][i] = tracks[i];
  }
  for(int i = 0; i< biggerHalf; i++){
    queue[1][i] = tracks[i+half];
  }
  //finding min and max of both queues
  for(int n = 0; n < 2; n++){
    for(int i = 0; i< half; i++){
      if(queue[n][i] < min){
        min = queue[n][i];
      }
      else if(queue[n][i] > max){
        max = queue[n][i];
      }
    }
  }
  //end of finding min and max.
  //start of determining which tracts should go to the left vector or the right vector.

  if (direction == "left") {
    left.push_back(min);
  }
  else {
    right.push_back(max);
  }
  for (int n = 0; n < 2; n++) {
    for (int i = 0; i < half; i++) {
      if (queue[n][i] < head) {
        left.push_back(queue[n][i]);
      }
      else {
        right.push_back(queue[n][i]);
      }
    }
  }

  std::sort(left.begin(), left.end());
  std::sort(right.begin(), right.end());


  int run = 2;
   while (run--) {
     if (direction == "left") {

       for (int i = left.size() - 1; i >= 0; i--) {
         currtrack = left[i];

         // calculate absolute distance
         distance = abs(currtrack - head);

         // increase the total count
         seekCount += distance;

         // accessed track is now the new head
         head = currtrack;
       }
       direction = "right";
     }
     else if (direction == "right") {
       for (int i = 0; i < right.size(); i++) {
         currtrack = right[i];

         // calculate absolute distance
         distance = abs(currtrack - head);

         // increase the total count
         seekCount += distance;

         // accessed track is now new head
         head = currtrack;
       }
       direction = "left";
     }
   }

   averageSeek = seekCount / size;
   std::string headStr = std::to_string(prevHead);
   std::string seekStr = std::to_string(seekCount);
	 std::string avgStr = std::to_string(averageSeek);

	 table.add(headStr);
	 table.add(seekStr);
	 table.add(avgStr);
	 table.endOfRow();


  }//end of odd array input.
  else{
    //handle spliting of even queueSize
    int half = newSize/2;
    int queue1[half];
    int queue2[half];
    int queue[2][half];
    for(int i = 0; i< half; i++){
      queue[0][i] = tracks[i];
    }
    for(int i = 0; i< half; i++){
      queue[1][i] = tracks[i + half];
    }
    //finding min and max of both queues
    for(int n = 0; n < 2; n++){
      for(int i = 0; i< half; i++){
        if(queue[n][i] < min){
          min = queue[n][i];
        }
        else if(queue[n][i] > max){
          max = queue[n][i];
        }
      }
    }
    //end of finding min and max.
    //start of determining which tracts should go to the left vector or the right vector.
    if(direction == "left"){
      left.push_back(min);
    }
    else{
      right.push_back(max);
    }
    for(int n = 0; n < 2; n++){
      for(int i = 0; i < half; i++){
        if(queue[n][i] < head){
          left.push_back(queue[n][i]);
        }
        else{
          right.push_back(queue[n][i]);
        }
      }
    }

    std::sort(left.begin(), left.end());
  	std::sort(right.begin(), right.end());

  	int run = 2;
  	while (run--) {
  		if (direction == "left") {

  			for (int i = left.size() - 1; i >= 0; i--) {
  				currtrack = left[i];

  				// calculate absolute distance
  				distance = abs(currtrack - head);

  				// increase the total count
  				seekCount += distance;

  				// accessed track is now the new head
  				head = currtrack;
  			}
  			direction = "right";
  		}
  		else if (direction == "right") {
  			for (int i = 0; i < right.size(); i++) {
  				currtrack = right[i];

  				// calculate absolute distance
  				distance = abs(currtrack - head);

  				// increase the total count
  				seekCount += distance;

  				// accessed track is now new head
  				head = currtrack;
  			}
  			direction = "left";
  		}
  	}

  	averageSeek = seekCount / size;
    std::string headStr = std::to_string(prevHead);
	  std::string seekStr = std::to_string(seekCount);
	  std::string avgStr = std::to_string(averageSeek);

	  table.add(headStr);
	  table.add(seekStr);
	  table.add(avgStr);
	  table.endOfRow();
}
}

/*-------------------------------------------------------------------------------
------------------------------Start of Main Program------------------------------
-------------------------------------------------------------------------------*/


//Main Program
int main() {

	double size1 = 9.0;
	double size2 = 1000.0;
	int tracks1[9] = { 55, 58, 39, 18, 90, 160, 150, 38, 184 };
	int tracks2[1000];
	int tracks3[1000];

	//Creating tracksay for 1000 random integers
	for (int i = 0; i < 1000; i++) {
		tracks2[i] = rand() % 200;
	}

	//Creating probability tracksay
	for (int i = 0; i < 1000; i++) {
		int prob = rand() % 100 + 1;

		if (i == 0) {
			tracks3[i] = rand() % 200;
		}
		else {
			if (prob >= 1 && prob < 11) {
				tracks3[i] = tracks3[i - 1];
			}
			if (prob == 98) {
				tracks3[i] = 0;
			}
			if (prob == 99) {
				tracks3[i] = 199;
			}
			if (prob >= 11 && prob < 16) {
				tracks3[i] = rand() % 10;
			}
			if (prob >= 16 & prob < 30) {
				tracks3[i] = rand() % (50 - 10 + 1) + 10;
			}
			if (prob >= 30 && prob < 55) {
				tracks3[i] = rand() % (100 - 50 + 1) + 50;
			}
			if (prob >= 55 && prob < 80) {
				tracks3[i] = rand() % (150 - 100 + 1) + 100;
			}
			if (prob >= 80 && prob < 94) {
				tracks3[i] = rand() % (190 - 150 + 1) + 150;
			}
			if (prob >= 94 && prob <= 100) {
				tracks3[i] = rand() % (200 - 190 + 1) + 190;
			}
		}
	}

	//Simulations
	{
		//First Simulation

		table.add("First Simulation");
		table.add("");
		table.add("");
		table.add("");
		table.endOfRow();

		table.add("");
		table.add("Head Track");
		table.add("Total tracks traversed");
		table.add("Average seek length");
		table.endOfRow();

		//Head = 0
		int head = 0;
		table.add("FIFO 11.2");
		FIFO(tracks1, head, size1);
		table.add("LIFO 11.2");
		LIFO(tracks1, head, size1);
		table.add("SSTF 11.2");
		SSTF(tracks1, head, size1);
		table.add("SCAN 11.2");
		SCAN(tracks1, head, size1, "right");
		table.add("CSCAN 11.2");
		CSCAN(tracks1, head, size1);
		table.add("N-Step-SCAN 11.2");
		NSTEPScan(tracks1, head, size1, 5, "right");
		table.add("FSCAN 11.2");
    FSCAN(tracks1,head,size1,"right");



		//Head = 100
		head = 100;
		table.add("FIFO 11.2");
		FIFO(tracks1, head, size1);
		table.add("LIFO 11.2");
		LIFO(tracks1, head, size1);
		table.add("SSTF 11.2");
		SSTF(tracks1, head, size1);
		table.add("SCAN 11.2");
		SCAN(tracks1, head, size1, "right");
		table.add("CSCAN 11.2");
		CSCAN(tracks1, head, size1);
		table.add("N-Step-SCAN 11.2");
		NSTEPScan(tracks1, head, size1, 5, "right");
		table.add("FSCAN 11.2");
    FSCAN(tracks1,head,size1,"right");


		//Head = 199
		head = 199;
		table.add("FIFO 11.2");
		FIFO(tracks1, head, size1);
		table.add("LIFO 11.2");
		LIFO(tracks1, head, size1);
		table.add("SSTF 11.2");
		SSTF(tracks1, head, size1);
		table.add("SCAN 11.2");
		SCAN(tracks1, head, size1, "right");
		table.add("CSCAN 11.2");
		CSCAN(tracks1, head, size1);
		table.add("N-Step-SCAN 11.2");
		NSTEPScan(tracks1, head, size1, 5, "right");
		table.add("FSCAN 11.2");
    FSCAN(tracks1,head,size1,"right");



		//Second Simulation

		table.add("Second Simulation");
		table.add("");
		table.add("");
		table.add("");
		table.endOfRow();

		table.add("");
		table.add("Head Track");
		table.add("Total tracks traversed");
		table.add("Average seek length");
		table.endOfRow();

		//Head = 0
		head = 0;
		table.add("FIFO 1000 Random");
		FIFO(tracks2, head, size2);
		table.add("LIFO 1000 Random");
		LIFO(tracks2, head, size2);
		table.add("SSTF 1000 Random");
		SSTF(tracks2, head, size2);
		table.add("SCAN 1000 Random");
		SCAN(tracks2, head, size2, "right");
		table.add("CSCAN 1000 Random");
		CSCAN(tracks2, head, size2);
		table.add("N-Step-SCAN 1000 Random");
		NSTEPScan(tracks2, head, size2, 10, "right");
		table.add("FSCAN 1000 Random");
    FSCAN(tracks2,head,size2,"right");



		//Head = 100
		head = 100;
		table.add("FIFO 1000 Random");
		FIFO(tracks2, head, size2);
		table.add("LIFO 1000 Random");
		LIFO(tracks2, head, size2);
		table.add("SSTF 1000 Random");
		SSTF(tracks2, head, size2);
		table.add("SCAN 1000 Random");
		SCAN(tracks2, head, size2, "right");
		table.add("CSCAN 1000 Random");
		CSCAN(tracks2, head, size2);
		table.add("N-Step-SCAN 1000 Random");
		NSTEPScan(tracks2, head, size2, 10, "right");
		table.add("FSCAN 1000 Random");
    FSCAN(tracks2,head,size2,"right");


		//Head = 199
		head = 199;
		table.add("FIFO 1000 Random");
		FIFO(tracks2, head, size2);
		table.add("LIFO 1000 Random");
		LIFO(tracks2, head, size2);
		table.add("SSTF 1000 Random");
		SSTF(tracks2, head, size2);
		table.add("SCAN 1000 Random");
		SCAN(tracks2, head, size2, "right");
		table.add("CSCAN 1000 Random");
		CSCAN(tracks2, head, size2);
		table.add("N-Step-SCAN 1000 Random");
		NSTEPScan(tracks2, head, size2, 10, "right");
		table.add("FSCAN 1000 Random");
    FSCAN(tracks2,head,size2,"right");



		//Third Simulation

		table.add("Third Simulation");
		table.add("");
		table.add("");
		table.add("");
		table.endOfRow();

		table.add("");
		table.add("Head Track");
		table.add("Total tracks traversed");
		table.add("Average seek length");
		table.endOfRow();

		//Head = 0
		head = 0;
		table.add("FIFO Probability array");
		FIFO(tracks3, head, size2);
		table.add("LIFO Probability array");
		LIFO(tracks3, head, size2);
		table.add("SSTF Probability array");
		SSTF(tracks3, head, size2);
		table.add("SCAN Probability array");
		SCAN(tracks3, head, size2, "right");
		table.add("CSCAN Probability array");
		CSCAN(tracks3, head, size2);
		table.add("N-Step-SCAN Probability array");
		NSTEPScan(tracks3, head, size2, 10, "right");
		table.add("FSCAN Probability array");
    FSCAN(tracks3,head,size2,"right");


		//Head = 100
		head = 100;
		table.add("FIFO Probability array");
		FIFO(tracks3, head, size2);
		table.add("LIFO Probability array");
		LIFO(tracks3, head, size2);
		table.add("SSTF Probability array");
		SSTF(tracks3, head, size2);
		table.add("SCAN Probability array");
		SCAN(tracks3, head, size2, "right");
		table.add("CSCAN Probability array");
		CSCAN(tracks3, head, size2);
		table.add("N-Step-SCAN Probability array");
		NSTEPScan(tracks3, head, size2, 10, "right");
		table.add("FSCAN Probability arrsay");
    FSCAN(tracks3,head,size2,"right");


		//Head = 199
		head = 199;
		table.add("FIFO Probability array");
		FIFO(tracks3, head, size2);
		table.add("LIFO Probability array");
		LIFO(tracks3, head, size2);
		table.add("SSTF Probability array");
		SSTF(tracks3, head, size2);
		table.add("SCAN Probability array");
		SCAN(tracks3, head, size2, "right");
		table.add("CSCAN Probability array");
		CSCAN(tracks3, head, size2);
		table.add("N-Step-SCAN Probability array");
		NSTEPScan(tracks3, head, size2, 10, "right");
		table.add("FSCAN Probability array");
    FSCAN(tracks3,head,size2,"right");
	}

	table.setAlignment(3, TextTable::Alignment::LEFT);
	cout << table;

	return 0;
}
