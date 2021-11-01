#include<iostream>   
using namespace std; 

/**
 * @author Natalia Solar
 * CSD 415
 * Assignment 6: Memory Allocation
 * This class implements boundary tag memory allocation algorithm that allocates blocks 
 * of memory, frees blocks blocks of memory, and displays blocks in the memory pool.
 */  

int *LwtAllocate(int size);
void LwtFree(int *address);
void LwtDisplayPool();   

static const int ALLOCATED = -1;
static const int FREE = 0;
static const int maxSize = 256; //Max size of the memory pool
int memPool[maxSize];           //Initializing globalMemoryPoolArray of size maxSize

//Allocate method that places a block into the memory pool
int *LwtAllocate(int size){
  int i;
  for (i = 1; i < maxSize; i++ ){
    if (memPool[i] == FREE && memPool[i + 1] > size) {
      int leftoverSize = memPool[i + 1] - size - 4;

      memPool[i] = ALLOCATED;                //set prefix flag to allocated
      memPool[i + 1] = size;                 //set prefix size
      memPool[i + size + 2] = size;          //set suffix size
      memPool[i + size + 3] = ALLOCATED;     //set suffix flag to allocated

      memPool[i + size + 4] = FREE;           //set leftoverPrefix flag to free
      memPool[i + size + 5] =  leftoverSize;  //set leftoverPrefix size
      memPool[i + size + 6 + leftoverSize] = leftoverSize;   //set leftoverSuffix flag to free
      memPool[i + size + 7 + leftoverSize] = FREE;           //set leftoverSuffix size
      break;

    } else {  //jump to the next block
      i += memPool[i+ 1] + 3; 
    }
  }

  return memPool+i; //returns address of element i in memPool (using pointer syntex)
}


void LwtFree(int *address){
  int size = *(address+1);        //size is the block size
  int locatorSize = *(address+1); //same as above, but used for locating

  *(address) = 0;                 //set prefixFlag to FREE
  *(address + size + 3) = FREE;   //set suffixFlag to FREE

  if(*(address-1) == FREE ){      //check block to the left, if it's free - join them together
    int predecessorSize = *(address-2);
    int leftJointSize = predecessorSize + size + 4;

    *(address - predecessorSize - 3) = leftJointSize;   //update prefixSize of the newly joint block to jointSize
    *(address-2) = FREE;                                //remove predecessor's suffixSize
    *(address+1) = FREE;                                //remove current block's prefixSize
    *(address + size + 2) = leftJointSize;              //update suffixSize of the newly joint block to jointSize                  
    size = leftJointSize;                               //update size
  } 

  if(*(address + locatorSize + 4) == FREE){  //check the block to the right, if it's free - join them together
    int successorSize = *(address + locatorSize + 5);
    int rightJointSize = successorSize + size + 4;

    *(address + locatorSize - size + 1) = rightJointSize;           //updating current prefixSize
    *(address + locatorSize + 2) = FREE;                            //removing current suffixSize
    *(address + locatorSize + 5) = FREE;                            //removing successor prefixSize
    *(address + successorSize + locatorSize + 6) = rightJointSize;  //update successor suffixSize               
  }
}

//Display function that displays Block#, flag, start offset, end: offset, and size
void LwtDisplayPool(){
  int block = 1, i, size;     //initilizing block#, loopCounter, and blockSize
  for (i = 1; i < maxSize; i++ ){
    if((memPool[i] == ALLOCATED) && memPool[i + 1] > 0) {  //if the block is allocated
      size = memPool[i+1];

      cout <<  "Block #" <<  block << ": " << "Allocated" << "\n";
      cout <<  "Starts at: " <<  i + 2 << "\n";
      cout << "Ends at: " << i + size + 1 << "\n";
      cout << "Size: " << size << "\n" << "\n";

      block++;
      i = i + size + 3;   //Jump to the next block

    } else if( memPool[i] == FREE && memPool[i + 1] > 0 ){ //if the block is FREE
      size = memPool[i+1];
  
      cout <<  "Block #" <<  block << ": " << "Free" << "\n";
      cout <<  "Starts at: " <<  i + 2 << "\n";
      cout << "Ends at: " << i + size + 1 << "\n";
      cout << "Size: " << size << "\n" << "\n";

      block++;
      i = i + size + 3;   //Jump to the next block
    }  
  }
}


int main() { 
  for ( int i = 0; i < maxSize ; i++ ){  //Initiallize array with all indexes set to FREE
    memPool[i] = FREE;
  }

  memPool[0] = ALLOCATED;       
  memPool[1] = FREE;                     //Setting the initial arrayPrefix to FREE
  memPool[2] = maxSize - 6;

  memPool[maxSize-3] = maxSize - 6;
  memPool[maxSize-2] = FREE;             //Setting the initial arraySuffix to FREE
  memPool[maxSize-1] = ALLOCATED;

  int *addressStorage[maxSize];          //array to store addresses of added blocks
  
  //Add block test-cases
  addressStorage[1] = LwtAllocate(4);
  addressStorage[2] = LwtAllocate(17);
  addressStorage[3] = LwtAllocate(5);
  addressStorage[4] = LwtAllocate(2);

  cout << "We've allocated the blocks: " << "\n";
  cout << "---------------------------" << "\n";
  LwtDisplayPool(); 

  //Remove-block test cases
  LwtFree(addressStorage[2]);
  LwtFree(addressStorage[4]);
  LwtFree(addressStorage[3]);
  LwtFree(addressStorage[1]);

  cout << "We've freed the blocks: " << "\n";
  cout << "-----------------------" << "\n";
  LwtDisplayPool(); 
  return 0;
} 