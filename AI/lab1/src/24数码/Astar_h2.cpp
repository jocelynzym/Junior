#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include <sstream>
#include <time.h>
#include <iomanip>

using namespace std;
#define ROWSIZE 5
#define COLUMNSIZE 5


const int distance[25][25]={
	{0,1,2,3,4, 1,2,3,4,4, 2,3,4,4,3, 3,4,4,5,4, 4,4,3,4,5},
	{1,0,1,2,3, 2,1,2,3,4, 3,2,3,4,4, 4,3,3,4,5, 4,3,2,3,4},
	{2,1,0,1,2, 3,2,1,2,3, 4,3,2,3,4, 4,3,2,3,4, 3,2,1,2,3},	
	{3,2,1,0,1, 4,3,2,1,2, 4,4,3,2,3, 5,4,3,3,4, 4,3,2,3,4},
	{4,3,2,1,0, 4,4,3,2,1, 3,4,4,3,2, 4,5,4,4,3, 5,4,3,4,4},

	{1,2,3,4,4, 0,1,2,3,3, 1,2,3,3,2, 2,3,4,4,3, 3,4,4,5,4},
	{2,1,2,3,4, 1,0,1,2,3, 2,1,2,3,3, 3,2,3,4,4, 4,3,3,4,5},
	{3,2,1,2,3, 2,1,0,1,2, 3,2,1,2,3, 4,3,2,3,4, 4,3,2,3,4},
	{4,3,2,1,2, 3,2,1,0,1, 3,3,2,1,2, 4,4,3,2,3, 5,4,3,3,4},
	{4,4,3,2,1, 3,3,2,1,0, 2,3,3,2,1, 3,4,4,3,2, 4,5,4,4,3},

	{2,3,4,4,3, 1,2,3,3,2, 0,1,2,2,3, 1,2,3,3,2, 2,3,4,4,3},
	{3,2,3,4,4, 2,1,2,3,3, 1,0,1,2,2, 2,1,2,3,3, 3,2,3,4,4},
	{4,3,2,3,4, 3,2,1,2,3, 2,1,0,1,2, 3,2,1,2,3, 4,3,2,3,4},
	{4,4,3,2,3, 3,3,2,1,2, 2,2,1,0,1, 3,3,2,1,2, 4,4,3,2,3},
	{3,4,4,3,2, 2,3,3,2,1, 3,2,2,1,0, 2,3,3,2,1, 3,4,4,3,2},

	{3,4,4,5,4, 2,3,4,4,3, 1,2,3,3,2, 0,1,2,3,3, 1,2,3,4,4},
	{4,3,3,4,5, 3,2,3,4,4, 2,1,2,3,3, 1,0,1,2,3, 2,1,2,3,4},
	{4,3,2,3,4, 4,3,2,3,4, 3,2,1,2,3, 2,1,0,1,2, 3,2,1,2,3},
	{5,4,3,3,4, 4,4,3,2,3, 3,3,2,1,2, 3,2,1,0,1, 4,3,2,1,2},
	{4,5,4,4,3, 3,4,4,3,2, 2,3,3,2,1, 3,3,2,1,0, 4,4,3,2,1},

	{4,4,3,4,5, 3,4,4,5,4, 2,3,4,4,3, 1,2,3,4,4, 0,1,2,3,4},
	{4,3,2,3,4, 4,3,3,4,5, 3,2,3,4,4, 2,1,2,3,4, 1,0,1,2,3},
	{3,2,1,2,3, 4,3,2,3,4, 4,3,2,3,4, 3,2,1,2,3, 2,1,0,1,2},
	{4,3,2,3,4, 5,4,3,3,4, 4,4,3,2,3, 4,3,2,1,2, 3,2,1,0,1},
	{5,4,3,4,4, 4,5,4,4,3, 3,4,4,3,2, 4,4,3,2,1, 4,3,2,1,0}	
};



class Picture {
private:

    int **pictureArray = NULL;
    int hValue;
    int gValue;
    int fValue;
public:
    Picture *parente=NULL;
	char parent;
    int zeroRow, zeroColumn;
    static int rowSize, columnSize;

    static void setSize(int row, int column) {
        rowSize = row;
        columnSize = column;
    }

    Picture(int content[][COLUMNSIZE]) {
        if (rowSize == 0 || columnSize == 0)
            throw "no init";
        pictureArray = new int *[rowSize];
        for (int i = 0; i < rowSize; ++i) {
            pictureArray[i] = new int[columnSize];
        }
        hValue = 0;
        gValue = 0;
        fValue = 0;
        setArray(content);
    }

    Picture(const Picture &sourse) {
        zeroRow = sourse.getZeroRow();
        zeroColumn = sourse.getZeroColumn();
        parente = sourse.parente;
		parent=0;
        pictureArray = new int *[rowSize];
        for (int i = 0; i < rowSize; ++i) {
            pictureArray[i] = new int[columnSize];
        }
        for (int i = 0; i < rowSize; ++i)
            for (int j = 0; j < columnSize; ++j) {
                pictureArray[i][j] = sourse.getPicturePoint()[i][j];
            }
    }

    ~Picture() {
        delete[] pictureArray;
    }

    bool operator==(const Picture &source) const {
        for (int i = 0; i < rowSize; ++i)
            for (int j = 0; j < columnSize; ++j) {
                if (pictureArray[i][j] != source.getPicturePoint()[i][j])
                    return false;
            }
        return true;
    }

    void setArray(int content[][COLUMNSIZE]) {
        for (int i = 0; i < rowSize; ++i) {
            for (int j = 0; j < columnSize; ++j) {
                pictureArray[i][j]=content[i][j];
                if (pictureArray[i][j] == 0) {
                    zeroRow = i;
                    zeroColumn = j;
                }
            }
        }
	
    }

    
    void setHValue(const Picture &endPicture) {
        int result = 0;
        int **tempPicture = endPicture.getPicturePoint();
        for (int i = 0; i < rowSize; ++i)
            for (int j = 0; j < columnSize; ++j) {
                if (pictureArray[i][j] != tempPicture[i][j]){
                	int cur_no=i*COLUMNSIZE+j;
                	int tar_no=pictureArray[i][j]-1;
                	result=result+ ::distance[cur_no][tar_no];
				}
            }
        hValue = result;
    }

    void updateFvalue() {
        fValue = hValue + gValue;
    }

    void setGvalue(int value) {
        gValue = value;
    }

    int **getPicturePoint() const {
        return pictureArray;
    }

    int getFValue() const {
        return fValue;
    }

    int getGvalue() const {
        return gValue;
    }

    int getZeroRow() const {
        return zeroRow;
    }

    int getZeroColumn() const {
        return zeroColumn;
    }

    void showPicture() const {
        for (int i = 0; i < rowSize; ++i) {
            for (int j = 0; j < columnSize - 1; ++j) {
                cout << pictureArray[i][j] << ' ';
            }
            cout << pictureArray[i][columnSize - 1] << endl;
        }
        cout<<endl;
    }
	
	void showParent() const {
		if(parent=='D'||parent=='U'||parent=='L'||parent=='R')
        cout << parent;
    }
	
	
	
	
};


int inVector(vector<Picture *> const &theVector, Picture const &element) {
    for (int i = 0; i < theVector.size(); ++i)
        if (element == *theVector[i])
            return i;
    return -1;
}

void deleteElement(vector<Picture *> &theVector, const Picture *element) {
    for (int i = 0; i < theVector.size(); ++i) {
        if (element == theVector[i]) {
            theVector.erase(theVector.begin() + i);
            return;
        }
    }
}

struct cmp {
    bool operator()(const Picture *a, const Picture *b) {
        return a->getFValue() > b->getFValue();
    }
};

//A*
bool Astart(Picture *&beginPicture, Picture *&endPicture) {
    priority_queue<Picture *, vector<Picture *>, cmp> openQueue;
    vector<Picture *> openTable, closeTable;
    beginPicture->setHValue(*endPicture);
    beginPicture->updateFvalue();
    
    openQueue.push(beginPicture);
    openTable.push_back(beginPicture);
    int move[4][2] = { {-1, 0},
                      {1,  0},
                      {0,  -1},
                      {0,  1}};
   int counter=0;
    while (!openQueue.empty()) {
    	if(counter==5000){
    		cout<<"5000"<<endl;
    		counter=0;
		}
    	counter++;
        Picture *bestPicture = openQueue.top();
            
        if (*bestPicture == *endPicture) {
        	cout<<"find the result"<<endl;
            delete endPicture;
            endPicture = bestPicture;
            return true;
        }
        
        closeTable.push_back(bestPicture);
        openQueue.pop();
        deleteElement(openTable, bestPicture);
        //4 directions
        for (int i = 0; i < 4; ++i){
            int row = bestPicture->zeroRow + move[i][0];
            int column = bestPicture->zeroColumn + move[i][1];
			
			if(row==2 && column==-1)	{row=2; column=Picture::columnSize-1;}
			if(row==2 && column == Picture::columnSize )	{row=2; column=0;}
			if(row==-1 && column==2)	{row=Picture::rowSize-1; column==2;}
			if(row==Picture::rowSize && column==2)	{row=0; column==2;}
			
            if (row >= 0 && row < Picture::rowSize && column >= 0 && column < Picture::columnSize)
			{
                Picture *successor = new Picture(*bestPicture);
                int **theArray = successor->getPicturePoint();
				
					theArray[successor->zeroRow][successor->zeroColumn] = theArray[row][column];
					theArray[row][column] = 0;
					successor->zeroRow = row;
					successor->zeroColumn = column;
					successor->parente = bestPicture;
				
				switch(i)
				{
					case 0: successor->parent='U'; break;
					case 1:	successor->parent='D'; break;
					case 2:	successor->parent='L'; break;
					case 3:	successor->parent='R'; break;
				}
				
                successor->setGvalue(bestPicture->getGvalue() + 1);
                int flag = inVector(openTable, *successor);
                if (flag >= 0) {
                    if (successor->getGvalue() < openTable[flag]->getGvalue()) {
                        openTable[flag]->setGvalue(successor->getGvalue());
                        openTable[flag]->parente = bestPicture;
						openTable[flag]->parent = successor->parent;
                        openTable[flag]->updateFvalue();
                        delete successor;
                    }
                }
                flag = inVector(closeTable, *successor);
                if (flag >= 0) {
                    if (successor->getGvalue() < closeTable[flag]->getGvalue()) {
                        closeTable[flag]->setGvalue(successor->getGvalue());
                        closeTable[flag]->parente = bestPicture;
						closeTable[flag]->parent = successor->parent;
                        closeTable[flag]->updateFvalue();
                        delete successor;
                        openQueue.push(closeTable[flag]);
                        openTable.push_back(closeTable[flag]);
                        closeTable.erase(closeTable.begin() + flag);
                    }
                } else {
					//cout<<"here"<<endl;
                    successor->setHValue(*endPicture);
                    successor->updateFvalue();
                    openQueue.push(successor);
                    openTable.push_back(successor);
                }
            }
			
        }
    }
	
    return false;
}

int Picture::rowSize = 0;
int Picture::columnSize = 0;

void showResult(const Picture *endPicture) {
    if (endPicture != NULL) {
        showResult(endPicture->parente);
        endPicture->showPicture();
    }
}


void showPath(const Picture *endPicture) {
    if (endPicture != NULL) {
        showPath(endPicture->parente);
        endPicture->showParent();
    }
}




//read file
int ** readFile(string filename){
	int **content ;
	char line[50]={0};
	string trans;
	ifstream ifs(filename.c_str());
	if (ifs.fail())  
    {  
       cout<<"fail to open file"<<endl;
		return NULL;
    }  

	for (int i = 0; i < ROWSIZE; ++i) {
        content[i] = new int[COLUMNSIZE];
		ifs.getline(line,sizeof(line));
		cout<<line<<endl;
		stringstream word(line);
		for(int j=0;j<COLUMNSIZE;++j){
			word>>trans;
			content[i][j]=atoi(trans.c_str());
		}
    }
    
    ifs.close();
 
	return content;
} 




int main() {
    
    Picture::setSize(ROWSIZE, COLUMNSIZE);
	
	int input_array[ROWSIZE][COLUMNSIZE];
	int target_array[ROWSIZE][COLUMNSIZE];
	
	
	char line1[50]={0};
	string trans1;
	ifstream ifs1("input.txt");

	if (ifs1.fail())  
    {  
       cout<<"fail to open file"<<endl;
	   return -1;
    }  
    	
	for (int i = 0; i < ROWSIZE; ++i) {
		ifs1.getline(line1,sizeof(line1));
		cout<<line1<<endl;
		stringstream word1(line1);
		for(int j=0;j<COLUMNSIZE;++j){
			word1>>trans1;
			input_array[i][j]=atoi(trans1.c_str());
		}
    }
    
    ifs1.close();
	
	char line2[50]={0};
	string trans2;
	ifstream ifs2("target.txt");
	if (ifs2.fail())  
    {  
       cout<<"fail to open file"<<endl;
		return -1;
    }  
	for (int i = 0; i < ROWSIZE; ++i) {
		ifs2.getline(line2,sizeof(line2));
		cout<<line2<<endl;
		stringstream word2(line2);
		for(int j=0;j<COLUMNSIZE;++j){
			word2>>trans2;
			target_array[i][j]=atoi(trans2.c_str());
		}
    }
    ifs2.close();
	
	
	Picture *beginPicture = new Picture(input_array);
	Picture *endPicture = new Picture(target_array);
     
   
    clock_t begin,end;
    double cost;    
	begin=clock();
    
    if (Astart(beginPicture, endPicture)) {
    	end=clock();
    	cost=(double)(end-begin)/CLOCKS_PER_SEC;
    	printf("%lfs\n",cost);
    
        //showResult(endPicture);
        showPath(endPicture);
		cout<<endl;
        cout << endPicture->getGvalue() << endl;
    } 
	else{
        cout<<"no answer"<<endl;
    }
	
	return 0;
}


