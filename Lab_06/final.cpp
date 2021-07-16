#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <math.h>
#include <fstream>
#include<assert.h>
using namespace std;

/*
TEAM MEMBERS :
    cs19b007 - Aswin Ramesh
    cs18b053 - B Sai Chaitanya
    cs18b022 - K V Pradeep Reddy
*/

struct CacheBlock{
    int tag;
    int validBit;
    int dirtyBit;
    CacheBlock* next;
};

//converts a string storing the binary form of a number to the decimal value
int BinToInt(string s){
    int result = 0;
    for(int i=0;i<s.size();i++){
        result = result*2 + (int)s[i] - 48;
    }
    return result ;
}

class Cache{
    private:
        vector<CacheBlock*> cache;
        int cacheSize;                          //Cache size in bytes                      
        int blockSize;                          //Block size in bytes
        int cacheType;                          //Set associativity of cache
        int rPolicy;                            //Replacement policy used
        int cacheAccesses;                      //Total number of cache accesses
        int readAccesses;                       //Total number of read accesses
        int writeAccesses;                      //Total number of write accesses
        int cacheMisses;                        //Total number of caches misses
        int coldMisses;                         //Total number of compulsary/cold misses
        int capacityMisses;                     //Total number of capacity misses
        int conflictMisses;                     //Total number of conflict misses
        int readMisses;                         //Total number of read misses
        int writeMisses;                        //Total number of write misses
        int dirtyBlocksEvicted;                 //Total number of dirty blocks evicted

        int numBlocks;                          //Number of blocks present in cache
        int numSets;                            //Number of sets present in cache
        int numWays;                            //Number of blocks per set

        int tagSize;                            //Number of bits used to represent tag of a block
        int indexSize;                          //Number of bits used to represent the set in which block is present
        int offsetSize;                         //Number of bits used to represent the offset in the block

        vector<int> sizes;                      //Vector storing the size of each set
        vector<vector<int> > validTags;         //Vector of vector storing the valid tags of the blocks for each set
        vector<vector<int> > plru_tags;         
        vector <int> plru_indexes;              
    public:
	 void printing(){
        printf("%d\n%d\n",cacheSize,blockSize);
	    if(cacheType == 0){
            printf("fully associative cache\n");
	    }else if (cacheType == 1){
            printf("Direct-mapped cache\n");
	    }else if ((cacheType == 2)||(cacheType == 4)||(cacheType == 8)||(cacheType == 16)||(cacheType == 32)){
		    printf("Set-associative cache\n");
	    }
	    if(rPolicy == 0){
            printf("Random Replacement\n");
        }else if(rPolicy == 1){
            printf("LRU Replacement\n");
        }else if(rPolicy == 2){
            printf("Pseudo-LRU Replacement\n");
        }
	    printf("%d\n%d\n%d\n%d\n",cacheAccesses,readAccesses,writeAccesses,cacheMisses);
	    printf("%d\n%d\n%d\n%d\n",coldMisses,capacityMisses,conflictMisses,readMisses);
        printf("%d\n%d\n",writeMisses,dirtyBlocksEvicted);
	 }
        void getCache(int cSize, int bSize, int cType, int repPolicy){
            cacheSize = cSize;
            blockSize = bSize;
            cacheType = cType;
            rPolicy = repPolicy;
            cacheAccesses = 0;
            readAccesses = 0;
            writeAccesses = 0;
            cacheMisses = 0;
            coldMisses = 0;
            capacityMisses = 0;
            conflictMisses = 0;
            readMisses = 0;
            writeMisses = 0;
            dirtyBlocksEvicted = 0;

            numBlocks = cacheSize/blockSize;
            if(cType == 0){
                numSets = 1;
                numWays = numBlocks;
            }else if(cType == 1){
                numSets = numBlocks;
                numWays = 1;
            }else{
                numWays = cType;
                numSets = numBlocks/numWays;
            } 
            if(rPolicy == 2){
            	for(int ii=0;ii<numSets;++ii){
            		vector <int> temp_node_set;
            		for(int jj=0;jj<numWays-1;++jj){
            			temp_node_set.push_back(0);
            		}
            		plru_tags.push_back(temp_node_set);
            	}
            }
	    
	    	indexSize = (int)log2(numSets);
          	offsetSize = (int)log2(bSize);
            tagSize = 32 - indexSize - offsetSize;

            for(int i=0;i<numSets;i++){
                CacheBlock* tempBlock = new CacheBlock;
                cache.push_back(tempBlock);
                vector<int> tempVector;
                validTags.push_back(tempVector);
                sizes.push_back(0);//why always pushing 0
	    	}
        }

        //returns the block pointer that points to the block preceding the block in the end of the set 
        CacheBlock* prevTail(int set){
            CacheBlock* tempNode = cache[set];
            while(tempNode->next->next != NULL) tempNode = tempNode->next;//if only 1 element in list ?
            return tempNode;
        }
        void read(string s){
        	cacheAccesses++;
            readAccesses++;
            string tag,index;
            int count = 0;
            for(int i=0;i<tagSize;i++){
                tag += s[i];
            }
            for(int i=0;i<indexSize;i++){
                index += s[i+tagSize];
            }
            int blockTag = BinToInt(tag);
            int blockIndex = BinToInt(index);
            int set = blockIndex;

            //If the set is not empty
            if(sizes[set] != 0){
                CacheBlock* tempBlock = cache[set];
                int flag = 0;
                //check if the block is present or not
                while(tempBlock->next != NULL){
                    if(tempBlock->tag == blockTag){
                        flag = 1;
                        break;
                    }
                    count++;
                    tempBlock = tempBlock->next;
                }
                if(tempBlock->tag == blockTag) flag = 1;

                //if the block is not present
                if(flag == 0){
                    //check if the block had already been called in the set
                    for(int i=0;i<validTags[set].size();i++){
                        if(validTags[set][i] == blockTag){
                            flag = 1;
                            break;
                        }
                    }
                    //first time block is being called in set
                    if(flag == 0){
                        coldMisses++;
                        validTags[set].push_back(blockTag);
                    }else{
                        //check if all sets are full
                        for(int i=0;i<numSets;i++){
                            if(sizes[i] != numWays){
                                flag = 0;
                                break;
                            }
                        }
                        if(flag == 0) conflictMisses++;
                        else capacityMisses++;
                    }
                    CacheBlock* newBlock = new CacheBlock;
                    newBlock->tag = blockTag;
                    newBlock->dirtyBit = 0;                
                    readMisses++;
                    cacheMisses++;
                    if(sizes[set] == numWays) replacement(set);
                    addBlock(set,newBlock);
                }else{
                    //SUCCESSFUL HIT
                    if(rPolicy == 1){
                        if(tempBlock != cache[set]) moveStart(set, tempBlock);
                    //changes the values of the nodes in the tree
                    }else if(rPolicy == 2){
                    	int coun = plru_indexes[count];
                    	coun = coun+numWays-1;
                    	for(;coun>0;){
                    		if(coun%2){//left node
                    			coun=(coun-1)/2;
                    			plru_tags[set][coun] = 1;
                    		}else{//right node
                    			coun = (coun-1)/2;
                    			plru_tags[set][coun] = 0;
                    		}
                    	}
                    }
                }
            }else{
                //first block to enter the set
                CacheBlock* newBlock = new CacheBlock;
                newBlock->tag = blockTag;
                newBlock->dirtyBit = 0;  
                coldMisses++;
                readMisses++;
                cacheMisses++;
                validTags[set].push_back(blockTag);
                addBlock(set,newBlock);
            }
        }
        void write(string s){
            cacheAccesses++;
            writeAccesses++;
            string tag,index;
            for(int i=0;i<tagSize;i++){
                tag += s[i];
            }
            for(int i=0;i<indexSize;i++){
                index += s[i+tagSize];
            }
            int blockTag = BinToInt(tag);
            int blockIndex = BinToInt(index);
            int set = blockIndex;
            int count = 0;

            //If the set is not empty
            if(sizes[set] != 0){
                CacheBlock* tempBlock = cache[set];
                int flag = 0;
                //check if the block is present or not
                while(tempBlock->next != NULL){
                    if(tempBlock->tag == blockTag){
                        break;
                    }
                    count++;
                    tempBlock = tempBlock->next;
                }
                if(tempBlock->tag == blockTag){
                    flag = 1;
                    tempBlock->dirtyBit = 1;
                }

                //if the block is not present
                if(flag == 0){
                    //check if the block had already been called in the set
                    for(int i=0;i<validTags[set].size();i++){
                        if(validTags[set][i] == blockTag){
                            flag = 1;
                            break;
                        }
                    }
                    //first time block is being called in set
                    if(flag == 0){
                        coldMisses++;
                        validTags[set].push_back(blockTag);
                    }else{
                        //check if all sets are full
                        for(int i=0;i<numSets;i++){
                            if(sizes[i] != numWays){
                                flag = 0;
                                break;
                            }
                        }
                        if(flag == 0) conflictMisses++;
                        else capacityMisses++;
                    }
                    CacheBlock* newBlock = new CacheBlock;
                    newBlock->tag = blockTag;
                    newBlock->dirtyBit = 1;
                    cacheMisses++;
                    writeMisses++;
                    if(sizes[set] == numWays) replacement(set);
                    addBlock(set,newBlock);
                }else{
                    tempBlock->dirtyBit = 1;
                    //SUCCESSFUL HIT
                    if(rPolicy == 1){
                        if(tempBlock != cache[set]) moveStart(set, tempBlock);
                    }else if(rPolicy == 2){
                    //changes the values of the nodes in the tree
                    	int coun = plru_indexes[count];
                    	coun = coun+numWays-1;
                    	for(;coun>0;){
                    		if(coun%2){//left node
                    			coun=(coun-1)/2;
                    			plru_tags[set][coun] = 1;
                    		}else{//right node
                    			coun = (coun-1)/2;
                    			plru_tags[set][coun] = 0;
                    		}
                    	}
                    }
                }
            }else{
                //first block to enter the set
                CacheBlock* newBlock = new CacheBlock;
                newBlock->tag = blockTag;
                newBlock->dirtyBit = 1;  
                coldMisses++;
                writeMisses++;
                cacheMisses++;
                validTags[set].push_back(blockTag);
                addBlock(set,newBlock);
            }
        }
        void replacement(int set){
            sizes[set]--;
            if(rPolicy == 0) evictRandomBlock(set);
            else if(rPolicy == 1) evictLastBlock(set);
            else pLRU(set);
        }
        void evictRandomBlock(int set){
            int blockNum = rand()%numWays;                  //block index to be evicted
            CacheBlock* tempBlock = cache[set];
            //evicting the first block in set
            if(blockNum == 0){
                if(cache[set]->dirtyBit == 1) dirtyBlocksEvicted++;
                if(tempBlock->next != NULL){
                    cache[set] = tempBlock->next;
                    tempBlock->next = NULL;
                }else{
                    cache[set]->tag = 0;
                    cache[set]->dirtyBit = 0;
                }
            }else{
                for(int i=0;i<blockNum-1;i++){
                    tempBlock = tempBlock->next;
                }
                if(tempBlock->next->next != NULL){
                    if(tempBlock->next->dirtyBit == 1) dirtyBlocksEvicted++;
                    tempBlock->next = tempBlock->next->next;

                }else{
                    if(tempBlock->next->dirtyBit == 1) dirtyBlocksEvicted++;
                    tempBlock->next = NULL;
                }
            }
        }
        void evictLastBlock(int set){
            if(sizes[set] > 1){
                CacheBlock* tempBlock = prevTail(set);
                if(tempBlock->next->dirtyBit == 1) dirtyBlocksEvicted++;
                tempBlock->next = NULL;
            }else{
                //removes the first block since size = 1
                if(cache[set]->dirtyBit == 1) dirtyBlocksEvicted++;
                cache[set]->tag = 0;
                cache[set]->dirtyBit = 0;
                cache[set]->next = NULL;
            }
        }
        void pLRU(int set){//
            int kk=0;
            for(kk=0;kk<numWays-1;){
            	if(plru_tags[set][kk] == 0){
            		kk = 2*kk+1;
            	}
            	else{
            		kk = 2*kk+2;
            	}
            }
            kk = kk+1-numWays;
            int ii=0;
            for(ii=0;ii<plru_indexes.size();++ii){
            	if(plru_indexes[ii] > kk){
            		break;
            	}
            }
            plru_indexes.erase(plru_indexes.begin()+ii);
            CacheBlock* tempBlock = cache[set];
            if(ii == 0){
                if(cache[set]->dirtyBit == 1) dirtyBlocksEvicted++;
                cache[set] = tempBlock->next;
                tempBlock->next = NULL;
            }
            else{
                for(int i=0;i<ii-1;i++){
                    tempBlock = tempBlock->next;
                }
                if(tempBlock->next->next != NULL){
                    if(tempBlock->next->dirtyBit == 1) dirtyBlocksEvicted++;
                    tempBlock->next = tempBlock->next->next;

                }else{
                    if(tempBlock->next->dirtyBit == 1) dirtyBlocksEvicted++;
                    tempBlock->next = NULL;
                }
            }
            
        }
        void addBlock(int set, CacheBlock* block){
            if(rPolicy <= 1){
            	addStart(set, block);
            // else
            }else {//
            	int kk=0;
            	for(kk=0;kk<numWays-1;){
            		if(plru_tags[set][kk] == 0){
            			plru_tags[set][kk] = 1;
            			kk = 2*kk+1;
            		}
            		else{
            			plru_tags[set][kk] = 0;
            			kk = 2*kk+2;
            		}
            	}
            	kk = kk+1-numWays;
            	int ii=0;
            	for(ii=0;ii<plru_indexes.size();++ii){
            		if(plru_indexes[ii] > kk){
            			break;
            		}
            	}
            	plru_indexes.insert(plru_indexes.begin()+ii,kk);
            	if(ii==0){
            		addStart(set,block);
            	}
            	else{
            		CacheBlock* tempBlock = cache[set];
            		for(;ii>1;--ii){
            			tempBlock = tempBlock->next;
            		}
            		block->next = tempBlock->next;
            		tempBlock->next = block;
            	}
            }
        }
        //adds the block to the start of the set
        void addStart(int set, CacheBlock* block){
            if(sizes[set] != 0){
                CacheBlock* tempBlock = cache[set];
                block->next = tempBlock;
                cache[set] = block;
            }else if(sizes[set] == 0){
                cache[set]->tag = block->tag;
                cache[set]->dirtyBit = block->dirtyBit;
                cache[set]->next = NULL;
            }
            sizes[set]++;
        }
        //moves the block to the start of the set
        void moveStart(int set, CacheBlock* block){
            CacheBlock* tempBlock = cache[set];
            while(tempBlock->next != block){
                tempBlock = tempBlock->next;
            }
            if(tempBlock->next->next == NULL){
                tempBlock->next = NULL;
            }else{
                tempBlock->next = tempBlock->next->next;
            }
            block->next = cache[set];
            cache[set] = block;
        }
	

};


int main(){
    int cache_size,block_size;
    string input_name;
    int associativity,rep_policy;
    cin>>cache_size>>block_size;
    cin>>associativity>>rep_policy;
    struct Cache cachee;
    cachee.getCache(cache_size,block_size,associativity,rep_policy);
    cin>> input_name;
    int n=input_name.length();
    n++;
    char* arr = new char[n];
    strcpy(arr,input_name.c_str());
    FILE *fptr = fopen(arr,"r");
    char ch;
    for(;;){
        string s = "11111111111111111111111111111111";
        ch = fgetc(fptr);
        ch = fgetc(fptr);
        for(int i=0;i<8;++i){
            ch = fgetc(fptr);
            int kk = ch-48;
            if(kk>9){
        	    kk = ch - 87;
        	}
       	    s[4*i] = (kk/8)+48;
       	    kk = kk%8;
       	    s[4*i+1] = (kk/4)+48;
       	    kk = kk%4;
       	    s[4*i+2] = (kk/2)+48;
       	    kk = kk%2;
       	    s[4*i+3] = kk+48;
		}
		ch = fgetc(fptr);
		ch = fgetc(fptr);
        if(ch == 'w'){
            cachee.write(s);
        }
        else if(ch == 'r'){
            cachee.read(s);
        }
        char c = fgetc(fptr);
        c = fgetc(fptr);
        fseek(fptr,-1,SEEK_CUR);
        if (c == EOF){
            break;
        }
    }
    cachee.printing();
    
}
