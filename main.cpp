/**
 * Author: Kenny Zhang
 * Github: https://github.com/KZhang2002
 *
 * Project: Solve the Sorting Mystery
 * Repository: https://github.com/SMUCSE2341/22s-pa03-KZhang2002
 *
 * Class: SMU CS 2341 (4649)
 *
 * Date Released: 3/19/2022
 * Last Updated: 3/19/2022
 */

#include "include/sorting_library.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <vector>
#include <algorithm>

using std::cout;
using std::endl;
using std::vector;
using std::string;

/**
 * asc = ascending, desc = descending, ran = random
 */
enum arrayOrder {asc, desc, ran};

/**
 * Converts enum arrayOrder to a string of the enum name.
 * @param order
 * @return orderName
 */
string enumToStr (enum arrayOrder ord) {
    switch (ord) {
        case asc:
            return "asc";
        case desc:
            return "desc";
        case ran:
            return "ran";
        default:
            return "NULL";
    }
}

/**
 * Stores the time data of one algorithm on one size
 */
struct algoTest {
    vector <long double> trials; //vector should be formatted: trial 1,trial 2,trial 3, ...etc.
    long double mean; //mean of trials
    int mysteryNum;
};

/**
 * Stores all the tests done for a certain array size,
 */
struct sizeData {
    vector <algoTest> dataset; //vector should be formatted: mystery 1,mystery 2,...,mystery 5
    int size;
    enum arrayOrder order; //array to be sorted's order
};

/**
 * Sorts a data set using a mystery function.
 * @param sortNum - Number of mystery function
 * @param data - Array being sorted
 * @param size - Size of array
 * @return Time in seconds for the function to run
 */
long double sort(int sortNum, int *data, int size) {
    //create a start and end time_point object
    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;

    //get the current time, run the algo, then get the current time again
    start = std::chrono::high_resolution_clock::now();

    switch(sortNum) {
        case 1:
            mystery01(data, size);
            break;
        case 2:
            mystery02(data, size);
            break;
        case 3:
            mystery03(data, size);
            break;
        case 4:
            mystery04(data, size);
            break;
        case 5:
            mystery05(data, size);
            break;
        default:
            break;
    }

    end = std::chrono::high_resolution_clock::now();

    //calculate the duration between start and end and print to the terminal
    std::chrono::duration<double> time_in_seconds = end - start;
    std::cout << std::fixed << time_in_seconds.count() << ", ";
    return time_in_seconds.count();
}

/**
 * Runs all mystery algorithms for chosen array multiple times. Data from trials is put into sizeTrials.
 * @param data - array being sorted
 * @param size - size of array
 * @param sizeTrials - data destination
 */
void runAllAlgos(const int *data, int size, sizeData& sizeTrials) {
    cout << "Creating dataset for size: " << size << endl;
    int trials = 7; //change value for amount of trials you want to do
    for (int i = 1; i <= 5; i++) {
        algoTest temp;
        temp.mysteryNum = i;

        long double sum = 0;

        for (int j = 0; j < trials; j++) {
            int *deepCopy = new int[size];

            for (size_t k = 0; k < size; k++)
                deepCopy[k] = data[k]; // copy the allocated memory
            cout << "Running mystery0" << i << endl;

            long double duration = sort(i, deepCopy, size);
            temp.trials.push_back(duration);
            sum += duration;
            cout << sum << endl;
        }

        temp.mean = sum/trials;
        cout << endl << "mean: " << sum/trials << endl;
        sizeTrials.dataset.push_back(temp);
    }
}

/**
 * Tests the mystery algorithms with an array whose order is determined by arrOrd.
 * @param dataSetSizes - sizes of arrays to sort
 * @param arrSize - size of dataSetSizes
 * @param arrOrd - order of the array (asc, desc, ran)
 * @param dataVect - data destination
 */
void testDataset(const int* dataSetSizes, int arrSize, enum arrayOrder arrOrd, vector<sizeData>& dataVect) {
    //make some data to sort
    for (int j = 0; j < arrSize; j++) {
        sizeData temp;
        temp.size = dataSetSizes[j];
        temp.order = arrOrd;

        const int DATA_SIZE = dataSetSizes[j];
        int data[DATA_SIZE];
        int val = DATA_SIZE;
        switch (arrOrd) {
            case asc:
                for (int i = DATA_SIZE - 1; i >= 0; i--)
                    data[i] = i;
                break;

            case desc:
                for (int i = 0; i < DATA_SIZE; i++) {
                    data[i] = val;
                    val--;
                }
                break;

            case ran:
                for (int i = DATA_SIZE - 1; i >= 0; i--)
                    data[i] = i;
                std::random_shuffle(data, data + DATA_SIZE);
                break;
        }

        if (arrOrd == ran) {
            int randTrials = 7;
            sizeData temptemp;
            vector <long double> tempVect(5, 0);

            for (int i = 0; i < randTrials; i++) {
                std::random_shuffle(data, data + DATA_SIZE);
                runAllAlgos(data, DATA_SIZE, temptemp);
                for (int k = 0; k < 5; k++) {
                    tempVect[k] += temptemp.dataset[k].mean;
                }
            }

            for (int k = 0; k < 5; k++) {
                algoTest empty;
                temp.dataset.push_back(empty);
                temp.dataset[k].mean = tempVect[k]/randTrials;
            }
        } else {
            runAllAlgos(data, DATA_SIZE, temp);
        }
        dataVect.push_back(temp);
    }
}

/**
 * Prints data to file dataset_[INSERT ORDER HERE].csv
 * @param dataVect - data to be printed
 */
void printData(vector<sizeData> dataVect) {
    string order;
    //replace with enumToStr
    order = enumToStr(dataVect[0].order);
    if (order == "NULL") {
        cout << "Invalid array order selected. Ending process." << endl; return;
    }

    string title = "dataset_" + order + ".csv";
    std::ofstream output(title);
    output << std::fixed << std::showpoint;
    output.precision(9);

    output << "Dataset Size,Mystery 1,Mystery 2,Mystery 3,Mystery 4,Mystery 5\n";

    int testNum = dataVect[0].dataset.size();
    for (int i = 0; i < dataVect.size(); i++) {
        output << dataVect[i].size << ",";
        for (int j = 0; j < testNum; j++) {
            output << dataVect[i].dataset[j].mean;
            if (j != testNum - 1) {
                output << ",";
            }
        }
        output << "\n";
    }

    output.close();
}

//Driver function
int main() {
    std::cout << std::fixed << std::showpoint;
    std::cout.precision(9);
    print_version_number();

    int dataset[8] = {10, 100, 1000, 10000, 25000, 50000, 75000, 100000};

    vector<sizeData> dataVectDesc;
    testDataset(dataset, 8, desc, dataVectDesc);
    printData(dataVectDesc);

    vector<sizeData> dataVectAsc;
    testDataset(dataset, 8, asc, dataVectAsc);
    printData(dataVectAsc);

    vector<sizeData> dataVectRan;
    testDataset(dataset, 8, ran, dataVectRan);
    printData(dataVectRan);

    return 0;
}


