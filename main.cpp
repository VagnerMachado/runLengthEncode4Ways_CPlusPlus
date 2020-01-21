/**********************************************************
 * main.cpp
 **********************************************************
 *  Created on: Mar 20, 2019
 *      Author: Vagner Machado
 *      CSC 323 - Professor Phillips
 *      Spring 2019 - Queens College
 ***********************************************************
 */

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <sstream>

using namespace std;

class RunLength
{
private:
	int row;
	int col;
	int picRows;
	int picColumns;
	int minVal;
	int maxVal;
	int numRuns;
	string nameEncodeFile;
	string nameDecodeFile;
	int whichMethod;

public:
	RunLength();
	void encodeMethod1(ifstream & infile,ofstream & outfile); //yes zero, no wrap
	void encodeMethod2(ifstream & infile, ofstream & outfile); //no zero, no wrap
	void encodeMethod3(ifstream & infile,ofstream & outfile); // yes zero, yes wrap
	void encodeMethod4(ifstream & infile,ofstream & outfile); //no zero, yes wrap
	void decodeMethod1(ifstream & infile,ofstream & outfile); //yes zero, no wrap
	void decodeMethod2(ifstream & infile, ofstream & outfile); //no zero, no wrap
	void decodeMethod3(ifstream & infile,ofstream & outfile); // yes zero, yes wrap
	void decodeMethod4(ifstream & infile,ofstream & outfile); //no zero, yes wrap
	int getNotZero(ifstream & infile); //get not zero input
	void getOriginalHeader(ifstream & infile, ofstream & outfile);
	void getEncodedHeader(ifstream & infile, ofstream & outfile);
	void runLengthEncode(ifstream & infile, ofstream & outfile, int method);
	void runLengthDecode(ifstream & infile, ofstream & outfile);
	void writeMetaData(ofstream & outfile); //writes the header data to file
};

int main(int argc, char ** argv)
{
	ifstream infile;
	ofstream outfile;
	infile.open(argv[1]);
	outfile.open(argv[2]);

	RunLength code;
	char yesNo = 'y';
	string answer;
	cout << "\n\n*** Welcome to Vagner's Run Length Encode and Decode Program *** \n\n";

	cout << "\nDo you want to encode the image passed as argument? ( Y / N )\n\nChoice: ";
	cin >> answer;
	if(answer.length() == 1)
		yesNo = answer[0];
	else
		yesNo = '#';
	while ((yesNo!= 'y' && yesNo!= 'Y' && yesNo!= 'n' && yesNo!= 'N') || cin.fail())
	{
		cin.clear();
		cin.ignore(256,'\n');
		cout << "\n\n *** Invalid choice, enter: ***\nY if YES, Let's Encode.\nN if NO, Exit Program.\n\n";
		cout << "\n\nDo you want to encode the image? ( Y / N )\n\nChoice: ";
		cin >> answer;
		if(answer.length() == 1)
			yesNo = answer[0];
		else
			yesNo = '#';
	}
	if (yesNo == 'n' || yesNo == 'N')
	{
		infile.close();
		outfile.close();
		cout << "\n\n*** GOOD BYE ***\n\n";
		return 0;
	}
	while (yesNo == 'y' || yesNo!= 'Y')
	{
		cout << "\nWhich method do you want to used to encode the image?\nEnter an integer from 1 to 4. Choices:\n";
		cout << "Method 1) With zero and not wrap-around.\n"
				<< "Method 2) Without zero and not wrap-around.\n"
				<< "Method 3) With zero and wrap-around.\n"
				<< "Method 4) Without zero and wrap-around.\n\nChoice: ";
		int method;
		cin >> method;
		while (method   < 1 || method > 4 || cin.fail())
		{
			cin.clear();
			cin.ignore(256,'\n');
			cout << "\n\n *** Invalid method choice, enter a method from 1 to 4 *** \n\n";
			cout << "Which method do you want to used to encode the image?\nEnter an integer from 1 to 4. Choices:\n";
			cout << "Method 1) With zero and not wrap-around.\n"
					<< "Method 2) Without zero and not wrap-around.\n"
					<< "Method 3) With zero and wrap-around.\n"
					<< "Method 4) Without zero and wrap-around.\n\nChoice: ";
			cin >> method;
		}

		// encode area
		stringstream  nameEncodeFile;
		//make encoded name
		string temp = argv[1];
		temp = temp.substr(0,temp.find('.'));
		nameEncodeFile << temp << "_EncodeMethod" << method << ".txt";
		ofstream encodedStreamOut;
		encodedStreamOut.open(nameEncodeFile.str().c_str());

		code.getOriginalHeader(infile, encodedStreamOut);
		encodedStreamOut << method << "\n";
		code.runLengthEncode(infile, encodedStreamOut, method);
		code.writeMetaData(outfile);
		encodedStreamOut.close(); //close encode file
		infile.close(); //close input image
		infile.open(argv[1]); //open case of loop
		encodedStreamOut.close();

		//decode area
		stringstream nameDecodeFile;
		temp = nameEncodeFile.str();
		temp = temp.substr(0,temp.find('.'));
		nameDecodeFile << temp << "_Decoded.txt";
		ofstream decodedStreamOut;
		ifstream encodedStreamIn;
		encodedStreamIn.open(nameEncodeFile.str().c_str());
		decodedStreamOut.open(nameDecodeFile.str().c_str());
		code.runLengthDecode(encodedStreamIn, decodedStreamOut);
		decodedStreamOut.close();
		encodedStreamIn.close();

		//loop inquiry
		cout << "\n\nDo you Want to Encode Again? ( Y / N )\n\nChoice: ";
		cin >> answer;
		if(answer.length() == 1)
			yesNo = answer[0];
		else
			yesNo = '#';
		while ((yesNo!= 'y' && yesNo!= 'Y' && yesNo!= 'n' && yesNo!= 'N') || cin.fail())
		{
			cin.clear();
			cin.ignore(256,'\n');
			cout << "\n\n *** Invalid choice, enter: ***\nY if YES, Encode Again.\nN if NO, Exit Program.\n\n";
			cout << "\n\nDo You Want to Encode Again? ( Y / N )\n\nChoice: ";
			cin >> answer;
			if(answer.length() == 1)
				yesNo = answer[0];
			else
				yesNo = '#';
		}
		if (yesNo == 'n' || yesNo == 'N')
		{
			infile.close();
			outfile.close();
			cout << "\n\n*** GOOD BYE ***\n\n";
			return 0;
		}
	}
	infile.close();
	outfile.close();
	return 0;
}

RunLength::RunLength()
{
//variables initialized inmethod as needed
}

void RunLength::writeMetaData(ofstream & outfile)
{
	outfile << "\nMethod " << whichMethod << " was used to compress the input image.\n"
			<< numRuns << " runs were produced in Run-Length encoding.\n"
			<< numRuns * 16 << " bytes were used to encode not counting the image header.\n";
}

void RunLength::runLengthEncode(ifstream & infile, ofstream & outfile, int method)
{
	whichMethod = method; //initialize the variable
	switch(method)
	{
	case 1:
	{
		encodeMethod1(infile, outfile);
	}
	break;
	case 2:
	{
		encodeMethod2(infile, outfile);
	}
	break;
	case 3:
	{
		encodeMethod3(infile, outfile);
	}
	break;
	case 4:
	{
		encodeMethod4(infile, outfile);
	}
	break;
	default:
	{
		cout << "Switch error in RunLength::runLengthEncode";
		exit(1);
	}
	}

	//the outfile needed for this part is not in scope so i called the
	//method from main to write the metadata.
}

void RunLength::runLengthDecode(ifstream & infile, ofstream & outfile)
{
	getEncodedHeader(infile, outfile); //get header and which method

	switch(whichMethod)
	{
	case 1:
	{

		decodeMethod1(infile, outfile);
	}
	break;
	case 2:
	{
		decodeMethod2(infile, outfile);
	}
	break;
	case 3:
	{
		decodeMethod3(infile, outfile);
	}
	break;
	case 4:
	{
		decodeMethod4(infile, outfile);
	}
	break;
	default:
	{
		cout << "Switch error in RunLength::runLengthDecode.";
		exit(1);
	}
	}
}

int RunLength::getNotZero(ifstream & infile)
{
	int temp;
	infile >> temp;
	while (temp == 0)
	{
		col++;
		infile >>temp;
		if( col == picColumns -1 && temp == 0)
		{
			col = -1;
			row ++;
			if (row == picRows)
				return -1;
		}
	}
	return temp;
}

void RunLength::getOriginalHeader(ifstream & infile, ofstream & outfile)
{
	infile >> picRows;
	infile >> picColumns;
	infile >>minVal;
	infile >> maxVal;
	outfile << picRows << " " << picColumns << " " << minVal << " " << maxVal << "\n";
}

void RunLength::getEncodedHeader(ifstream & infile, ofstream & outfile)
{

	infile >> picRows;
	infile >> picColumns;
	infile >>minVal;
	infile >> maxVal;
	infile >> whichMethod;
	outfile << picRows << " " << picColumns << " " << minVal << " " << maxVal;
}
void RunLength::encodeMethod1(ifstream & infile, ofstream & outfile) //yes zero, no wrap
{
	row = 0;
	col = 0;
	int nextGray = 0;
	int curGray = 0;
	int runLength = 0;
	numRuns = 0;

	while (row < picRows)
	{
		col = 0;
		runLength = 0;
		infile >> curGray;
		runLength++;
		outfile <<  row << " " <<  col << " " << curGray << " ";

		while (col < picColumns -1)
		{
			col++;
			infile >> nextGray;
			if (curGray == nextGray) //a match
				runLength++;
			else //not a match
			{
				outfile << runLength << endl;
				numRuns++;
				runLength = 1;
				curGray = nextGray;
				outfile <<  row << " " <<  col << " " << curGray << " ";
			}

		}
		outfile << runLength;
		if(row != picRows - 1)
			outfile << endl;
		numRuns++;
		row++;
	}
}

void RunLength::encodeMethod2(ifstream & infile, ofstream & outfile) // no zero,  no wrap
{
	row = 0;
	col = 0;
	int nextGray = 0;
	int curGray = 0;
	int runLength = 0;
	numRuns = 0;

	while (row < picRows)
	{
		col = 0;
		runLength = 0;
		curGray = this->getNotZero(infile);
		if(curGray == -1) //no non zero left
		{
			if (runLength != 0)
			{
				outfile << runLength;
				numRuns++;
			}
			return;
		}

		runLength++;
		outfile <<  row << " " <<  col << " " << curGray << " ";

		while (col < picColumns -1)
		{
			int colBefore = col;
			col++;
			nextGray = this->getNotZero(infile);
			if(nextGray == -1)
			{
				if (runLength != 0)
				{
					outfile << runLength;
					numRuns++;
				}
				return;
			}

			if (curGray == nextGray && colBefore == col - 1) //if there is a match and they are neighbors
				runLength++;
			else
			{
				outfile << runLength << endl;
				numRuns++;
				runLength = 1;
				curGray = nextGray;
				outfile <<  row << " " <<  col << " " << curGray << " ";

			}

		}
		outfile << runLength << endl;
		numRuns++;
		row++;
	}
}

void RunLength::encodeMethod3(ifstream & infile, ofstream & outfile) // yes zero, yes wrap
{
	row = 0;
	col = 0;
	int nextGray = -1;
	int curGray = 0;
	int runLength = 0;
	numRuns = 0;

	while (row < picRows)
	{
		col = 0;
		infile >> curGray;
		if(nextGray != -1 && nextGray != curGray)
		{
			outfile << runLength << endl;
			numRuns++;
			runLength = 1;
			outfile <<  row << " " <<  col << " " << curGray << " ";
		}
		else
			runLength++;
		if (row == 0 && col == 0)
			outfile <<  row << " " <<  col << " " << curGray << " ";

		while (col < picColumns -1)
		{
			col++;
			infile >> nextGray;
			if (curGray == nextGray)
				runLength++;
			else
			{
				outfile << runLength << endl;
				numRuns++;
				runLength = 1;
				curGray = nextGray;
				outfile <<  row << " " <<  col << " " << curGray << " ";
			}

		}
		row++;
	}
	numRuns++;
	outfile << runLength;
}

void RunLength::encodeMethod4(ifstream & infile, ofstream & outfile) // no zero, yes  wrap
{
	row = 0;
	col = 0;
	int nextGray = -1;
	int curGray = 0;
	int runLength = 0;
	numRuns = 0;

	while (row < picRows)
	{
		col = 0;
		curGray = this->getNotZero(infile);
		if(curGray == -1)
		{
			if (runLength != 0)
			{
				outfile << runLength;
				numRuns++;
			}

			return;
		}

		if(nextGray != -1 && nextGray != curGray) //if not first run
		{
			outfile << runLength << endl;
			numRuns++;
			runLength = 1;
			outfile <<  row << " " <<  col << " " << curGray << " ";
		}
		else
			runLength++;
		if (nextGray == -1) // if first run
			outfile <<  row << " " <<  col << " " << curGray << " ";

		while (col < picColumns -1)
		{
			int colBefore = col;
			col++;
			nextGray = this->getNotZero(infile);
			if(nextGray == -1)
			{
				if (runLength != 0)
				{
					outfile << runLength;
					numRuns++;
				}
				return;
			}
			if (curGray == nextGray && colBefore == col - 1 ) //match and neighbors
				runLength++;
			else
			{
				outfile << runLength << endl;
				numRuns++;
				runLength = 1;
				curGray = nextGray;
				outfile <<  row << " " <<  col << " " << curGray << " ";
			}
		}
		row++;
	}
	numRuns++;
	cout << runLength << "\n";
}

void RunLength::decodeMethod1(ifstream & infile,ofstream & outfile) //yes zero, no wrap
{
	int curRow, curCol, curGray, curLength, counter, nextRow;
	curRow = curCol= curGray= curLength= counter= nextRow = 0;
	outfile << "\n";
	while(counter++ <= numRuns - 1)
	{
		nextRow = curRow;
		infile >> curRow;
		infile >> curCol;
		infile >> curGray;
		infile >> curLength;
		if (curRow != nextRow)
			outfile << "\n";

		for(int i = 0; i < curLength; i++)
		{
			outfile << curGray << " ";
		}
	}
	infile.close();
	outfile.close();

}
void RunLength::decodeMethod2(ifstream & infile, ofstream & outfile) //no zero, no wrap
{
	outfile << "\n";
	int curRow, curCol, curGray, curLength, counter, colNow, prevRow, prevCol;
	curRow = curCol = curGray = curLength = counter = colNow = prevRow = prevCol = 0;

	while(counter++ <= numRuns - 1)
	{
		prevCol = curCol;
		prevRow = curRow;
		infile >> curRow;
		infile >> curCol;
		infile >> curGray;

		int pad = 0;

		if(curRow != prevRow) // custom zero pad for different rows
			pad = picColumns - (prevCol + curLength) + curCol + ((curRow - prevRow - 1) * picColumns);
		else
			pad = curCol - (prevCol + curLength); //custom zero pad for gap in same row

		//add the zeroes
		for (int i = 0; i < pad; i++)
		{
			outfile << "0" << " ";
			colNow++;
			if(colNow == picColumns)
			{
				colNow = 0;
				outfile << "\n";
			}
		}
		infile >> curLength;
		//insert the non zero gray code
		for(int i = 0; i < curLength; i++)
		{
			outfile << curGray << " ";
			colNow++;
			if(colNow == picColumns)
			{
				outfile << endl;
				colNow = 0;
			}
		}
	}
	//fill the rest with zeros based on columns and rows
	for( ; curRow < picRows; curRow++)
	{
		for(; colNow < picColumns; curCol++)
		{
			outfile << "0" << " ";
			colNow++;
			if(colNow == picColumns)
			{
				colNow = 0;
				if(curRow != picRows - 1)
					outfile << "\n";
				break;
			}
		}
	}
}

void RunLength::decodeMethod3(ifstream & infile,ofstream & outfile) // yes zero, yes wrap
{
	outfile << "\n";
	int curRow, curCol, curGray, curLength, counter, colNow, tracker;
	curRow = curCol = curGray = curLength = counter = colNow = tracker = 0;

	while(counter++ <= numRuns - 1)
	{
		infile >> curRow;
		infile >> curCol;
		infile >> curGray;
		infile >> curLength;


		for(int i = 0; i < curLength; i++)
		{
			outfile << curGray << " ";
			colNow++;
			tracker++;
			if(colNow == picColumns && tracker < picColumns * picRows) //tracks last \n for cursor
			{
				outfile << "\n";
				colNow = 0;
			}
		}
	}
	infile.close();
	outfile.close();
}
void RunLength::decodeMethod4(ifstream & infile,ofstream & outfile) //no zero, yes wrap
{
	outfile << "\n";
	int curRow, curCol, curGray, curLength, counter, colNow, prevRow, prevCol;
	curRow = curCol = curGray = curLength = counter = colNow = prevRow = prevCol = 0;

	while(counter++ <= numRuns - 1)
	{
		prevCol = curCol;
		prevRow = curRow;
		infile >> curRow;
		infile >> curCol;
		infile >> curGray;

		int pad = 0;

		if(curRow != prevRow) // custom zero pad for different rows
			pad = picColumns - (prevCol + curLength) + curCol + ((curRow - prevRow - 1) * picColumns);
		else
			pad = curCol - (prevCol + curLength); //custom zero pad for gap in same row

		//add the zeroes
		for (int i = 0; i < pad; i++)
		{
			outfile << "0" << " ";
			colNow++;
			if(colNow == picColumns)
			{
				colNow = 0;
				outfile << "\n";
			}
		}
		infile >> curLength;
		//insert the non zero gray code
		for(int i = 0; i < curLength; i++)
		{
			outfile << curGray << " ";
			colNow++;
			if(colNow == picColumns)
			{
				outfile << endl;
				colNow = 0;
			}
		}
	}
	//fill the rest with zeros based on columns and rows
	for( ; curRow < picRows - 1; curRow++)
	{
		for(; colNow < picColumns; curCol++)
		{
			outfile << "0" << " ";
			colNow++;
			if(colNow == picColumns)
			{
				colNow = 0;
				if(curRow != picRows - 2)
					outfile << "\n";
				break;
			}
		}
	}
}

